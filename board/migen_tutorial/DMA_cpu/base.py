#!/usr/bin/env python3
# export LITEX_ENV_EFINITY=/home/carlos/Embedded/efinity/2023.2/
from migen import *
from board import sbRIO
from litex.build.generic_platform import *
from litex.build.efinix import EfinixPlatform
from litex.soc.integration.soc_core import *
from litex.soc.integration.soc import SoCRegion
from litex.soc.integration.builder import *
from litex.soc.cores import dna
from litex.soc.cores.hyperbus import HyperRAM
from liteeth.phy.rmii import LiteEthPHYRMII
from litex.soc.interconnect.csr import *
from litex.soc.cores.dma import WishboneDMAReader
from litex.soc.cores.dma import WishboneDMAWriter
from litex.soc.interconnect import wishbone
from litex.soc.cores.spi import SPIMaster
from litespi.core.master import LiteSPIMaster
from litex.soc.interconnect import stream, wishbone
from migen.genlib.fifo import SyncFIFO


from litex.gen import *

from litex.soc.interconnect.csr import *
from litex.soc.interconnect import stream
from liteeth.common import convert_ip
from liteeth.frontend.stream import LiteEthStream2UDPTX






from pwm import PWM 
platform = sbRIO.Platform()

#!/usr/bin/env python3
# Key fixes applied to original code

class DMAUpload(LiteXModule):
    def __init__(self, bus, udp_port, dst_ip, dst_udp_port):
        # DMA Reader
        # ----------
        self.bus             = bus
        self.dma_reader      = WishboneDMAReader(bus, with_csr=True)
        self.dma_reader_conv = stream.Converter(bus.data_width, 8)

        # UDP Streamer
        # ------------
        udp_streamer   = LiteEthStream2UDPTX(
            ip_address = convert_ip("10.42.0.100"),
            udp_port   = 2000,
            fifo_depth = 1024,
            with_csr   = True
        )


        self.udp_cdc      = stream.ClockDomainCrossing([("data", 8)], "sys", "eth_rx")
        self.udp_streamer = ClockDomainsRenamer("eth_rx")(udp_streamer)

        self.comb += [
            self.dma_reader.source.connect(self.dma_reader_conv.sink),
            self.dma_reader_conv.source.connect(self.udp_cdc.sink),
            self.udp_cdc.source.connect(self.udp_streamer.sink),
            self.udp_streamer.source.connect(udp_port.sink)
        ]



class BaseSoC(SoCCore):
    def __init__(self, platform, **kwargs):
        sys_clk_freq = int(33.333e6)
        SoCCore.__init__(self, platform,
            cpu_type                 = "femtorv",
            cpu_variant              = "gracilis",  #RV32IMCzCSR
            clk_freq                 = 33.333e6,
            ident                    = "LiteX CPU Test SoC", ident_version=True,
            integrated_rom_size      = 0x8000
        )

        # Use HyperRAM generic PHY as SRAM
        size = int((8*1024*1024))
        hr_pads = platform.request("hyperram", 0)
        self.hyperram = HyperRAM(hr_pads, latency=7, sys_clk_freq=sys_clk_freq)
        self.bus.add_slave("main_ram",  self.hyperram.bus, SoCRegion(origin=0x40000000, size=size, mode="rw"))

        # Add DMA buffer region
        self.bus.add_region("dma_buffers", SoCRegion(
            origin=0x30000000, 
            size=0x100000,  # 1MB
            mode="rw"
        ))

        self.submodules.crg = CRG(platform.request("clk33"), ~platform.request("user_btn_n")) 
        
        self.ethphy  = LiteEthPHYRMII(
          clock_pads = self.platform.request("eth_clocks"),
          pads       = self.platform.request("eth"),
          refclk_cd  = None)

        self.add_etherbone(
            phy         = self.ethphy,
            ip_address  = "10.42.0.220"        
        )
        
        bus = wishbone.Interface(
            data_width = self.bus.data_width,
            adr_width  = self.bus.get_address_width(standard="wishbone"),
            addressing = "word",
        )

        # DMA Upload
        self.submodules.dma_upload = DMAUpload(
            bus          = bus,
            udp_port     = self.ethcore_etherbone.udp.crossbar.get_port(2000, dw=8),
            dst_ip       = "10.42.0.100",
            dst_udp_port = 2000
        )
        self.add_csr("dma_upload")
        self.bus.add_master("dma_upload", self.dma_upload.dma_reader.bus)

        self.submodules.pwm = PWM(platform.request("user_led",  0))
        self.add_csr("pwm")


soc = BaseSoC(platform)
# Build --------------------------------------------------------------------------------------------
builder = Builder(soc, output_dir="build/", csr_csv="csr.csv")
builder.build(build_name="top")




'''
https://github.com/efabless/caravel_mgmt_soc_litex/blob/main/verilog/dv/tests-standalone/spi_master/spi_master.c
https://github.com/efabless/caravel_mgmt_soc_litex/blob/main/litex/caravel.py
https://github.com/enjoy-digital/litex/issues/571
Wishbone




sink source 

https://github.com/amaranth-lang/amaranth/issues/317#issuecomment-899407394






#        dma_width = 32 #FIXME read memory width
#        self.submodules.dma_reader = WishboneDMAReader(endianness="big", bus=wishbone.Interface(data_width=dma_width), with_csr=True)
#        self.bus.add_master("dma_reader", self.dma_reader.bus)
#        self.submodules.dma_writer = WishboneDMAWriter(endianness="big", bus=wishbone.Interface(data_width=dma_width), with_csr=True)
#        self.bus.add_master("dma_writer", self.dma_writer.bus)
#        self.comb += self.dma_reader.source.connect(self.dma_writer.sink) #Connect Reader to Writer







#        self.add_ethernet(phy=self.ethphy)



***************************************************
**************************************************
***************************************************
***************************************************
***************************************************
How to Use the DMA From Software:
Configure Transfer:

python
# Set source address (must be word-aligned)
soc.dma_upload.dma_reader.base.write(0x30000000)

# Set length in bytes (must be multiple of 4)
soc.dma_upload.dma_reader.length.write(1024) 

# Start transfer (write 1 to control register)
soc.dma_upload.dma_reader.control.write(1)
Monitor Status:

python
while not soc.dma_upload.done.status:
    print(f"Bytes sent: {soc.dma_upload.bytes_sent.status}")
    time.sleep(0.1)
Reset Counters:

python
soc.dma_upload.control.fields.reset = 1




#   SOFTWARE


def test_dma_transfer(soc):
    # Prepare test data
    test_data = b"Hello World!" * 1000
    soc.write_mem(0x30000000, test_data)
    
    # Configure DMA
    soc.dma_upload.dma_reader.base.write(0x30000000)
    soc.dma_upload.dma_reader.length.write(len(test_data))
    
    # Start transfer
    soc.dma_upload.dma_reader.control.write(1)
    
    # Monitor progress
    while not soc.dma_upload.done.status:
        print(f"Sent: {soc.dma_upload.bytes_sent.status} bytes")
        print(f"Throughput: {soc.dma_upload.throughput.status} B/s")
        time.sleep(0.1)
    
    if soc.dma_upload.error.status:
        print(f"Error occurred: {bin(soc.dma_upload.error.status)}")
    print(f"Completed. {soc.dma_upload.packets_sent.status} packets sent")

    

    

    

'''