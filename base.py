from migen import *
from board import ecb_t8_t113
from litex.build.generic_platform import *
from litex.build.lattice import LatticePlatform
from litex.soc.integration.soc_core import *
from litex.soc.integration.builder import *
from litex.soc.cores import dna
from litex.soc.cores.led import LedChaser

from litex.soc.cores.bitbang import I2CMaster as I2C
from litex.soc.cores.uart import UART, UARTPHY
from litex.soc.cores.spi_flash import ECP5SPIFlash 

from cores.modules.my_uart import Verilog_UART


# import all the custom cores
from cores.modules.cordic import VerilogCORDIC

# _custom_serial = [
#     ("custom_serial", 1,
#         Subsignal("uart_txd", Pins("E3")),  # J1.3
#         Subsignal("uart_rxd", Pins("F4")),  # J1.5
#         IOStandard("LVCMOS33")
#     ),
# ]



# Define SoC
class BaseSoC(SoCCore):
    def __init__(self, platform, **kwargs):
        sys_clk_freq = int(25.000e6)

        # platform.add_extension(_custom_serial)
        # platform.add_source("cores/rtl/my_uart.v")

        ## Add verilogs
        # platform.add_source("rtl/sum.v")
        # platform.add_source("rtl/sum.v")
        # 

        # SoC with CPU
        SoCCore.__init__(self, platform,
            cpu_type="vexriscv",
            clk_freq=sys_clk_freq,
            ident="LiteX CPU Test on ecbt813", ident_version=True,
            integrated_rom_size=0x8000,
            # with_uart=True, 
            
            integrated_main_ram_size=0x4000,
            **kwargs
        )

        # Clock Reset Generation
        self.submodules.crg = CRG(
            platform.request("clk33"),
            ~platform.request("user_btn_n")
        )

        # Add I2C 
        self.submodules.i2c = I2C(platform.request("i2c", 0))
        
       # Add custom uart 
        # self.add_uart(name="uart_lidar", uart_name="uart_lidar_phy", uart_pads=platform.request("serial", 1), baudrate=115200)
        # SoCCore.add_csr(self,"uart_ver")
        # self.submodules.uart_ver = Verilog_UART(platform.request("custom_serial",1))
        # self.add_csr("uart_lidar") 
        # self.submodules.pwm = PWM(platform.request("user_led", 0))

        # HDL wrappers include their own source
        # self.submodules.quadrature_decoder = QuadratureDecoder(platform)
        # self.add_csr('quadrature_decoder')

        # self.submodules.spi_flash = ECP5SPIFlash(platform.request("spiflash"), sys_clk_freq)

        # CORDIC module 
        # platform.add_source("cores/rtl/utils/cordic.v")
        # self.submodules.cordic = VerilogCORDIC(platform)

        # self.add_csr("VerilogCORDIC")


# Build
platform = ecb_t8_t113.Platform()
soc = BaseSoC(platform)
builder = Builder(soc, output_dir="build", csr_csv="csr.csv")
builder.build(build_name="top")
