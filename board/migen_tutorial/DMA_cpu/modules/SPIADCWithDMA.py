from migen import *
from litex.soc.interconnect.csr import *
from litex.soc.interconnect import stream
from litex.soc.cores.spi import SPIMaster
from litex.soc.cores.dma import WishboneDMAReader
from litex.soc.cores.dma import WishboneDMAWriter
from litex.soc.interconnect import wishbone

class SPIADCWithDMA(Module, AutoCSR):
    def __init__(self, platform,):
        # Registros CSR (accesibles desde software)
        self.start = CSRStorage()  # Bit de inicio
        self.done = CSRStatus()    # Bit de fin
        
        # Configuración SPI
        self.submodules.spi = SPIMaster(
            pads=platform.request("spi_adc"),
            data_width=8,
            sys_clk_freq=100e6,
            spi_clk_freq=1e6
        )
        
        # 2. Crear bus Wishbone para DMA
        self.wb_bus = wishbone.Interface()
        
        # 3. Instanciar DMA Writer para transferencias ADC->Memoria
        self.submodules.dma_writer = WishboneDMAWriter(self.wb_bus)
        
        # 4. Conectar SPI al DMA
        # Convertir señales SPI a interfaz de stream
        self.submodules.spi_stream = AsyncFIFO(
            width=16,
            depth=16,
            buffered=True
        )
        
        # Conexiones
        self.comb += [
            # Conectar datos del SPI al FIFO
            self.spi_stream.sink.valid.eq(self.spi.source.valid),
            self.spi_stream.sink.data.eq(self.spi.source.data),
            self.spi.source.ready.eq(self.spi_stream.sink.ready),
            
            # Conectar FIFO al DMA Writer
            self.dma_writer.sink.valid.eq(self.spi_stream.source.valid),
            self.dma_writer.sink.data.eq(self.spi_stream.source.data),
            self.spi_stream.source.ready.eq(self.dma_writer.sink.ready)
        ]
        
        # 5. Registros de control
        self.dma_start = Signal()
        self.dma_addr = Signal(32)  # Dirección de memoria destino
        self.dma_count = Signal(32) # Número de muestras a capturar
        
        # 6. Máquina de estados para controlar la captura
        self.submodules.fsm = FSM(reset_state="IDLE")
        self.fsm.act(
            "IDLE",
            If(self.dma_start,
                NextState("START_DMA")
            )
        )
        self.fsm.act(
            "START_DMA",
            NextValue(self.dma_writer.sink.address, self.dma_addr),
            NextValue(self.dma_writer.sink.length, self.dma_count),
            NextValue(self.dma_writer.sink.start, 1),
            NextState("RUNNING")
        )
        self.fsm.act(
            "RUNNING",
            If(self.dma_writer.done,
                NextState("IDLE")
            )
        )
