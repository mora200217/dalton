from litex.build.generic_platform import *
from litex.build.lattice import LatticeECP5Platform
from litex.build.lattice.programmer import OpenOCDJTAGProgrammer
# IOs ----------------------------------------------------------------------------------------------
_io = [
    # Clk
    ("clk33", 0, Pins("P6"), IOStandard("LVCMOS33")),

    # Serial ecb_T8
#    ("serial", 0,
#        Subsignal("tx", Pins("142")), # 27 on H4 Must be changed in next HW version to 144
#        Subsignal("rx", Pins("144")), # 28 on H4 Must be changed in next HW version to 142
#        IOStandard("LVCMOS33") , Misc("WEAK_PULLUP")
#    ),
    # Serial sbRIO
    ("serial", 0,
        Subsignal("tx", Pins("C4")), # 27 on H4 Must be changed in next HW version to 144
        Subsignal("rx", Pins("D4")), # 28 on H4 Must be changed in next HW version to 142
        IOStandard("LVCMOS33")
    ),
    ("serial", 1,
        Subsignal("tx", Pins("E3")), # 27 on H4 Must be changed in next HW version to 144
        Subsignal("rx", Pins("N4")), # 28 on H4 Must be changed in next HW version to 142
        IOStandard("LVCMOS33")
    ),
    
    # Leds
    ("user_led", 0, Pins("T6"), IOStandard("LVCMOS33")),
    #("user_led", 1, Pins("76"), IOStandard("LVCMOS33"), Misc("DRIVE_STRENGTH=3")),
    # Buttons
    ("user_btn_n", 0, Pins("R7"), IOStandard("LVCMOS33")),

    # SPIFlash
#    ("spiflash", 0,
#        Subsignal("cs_n", Pins("31")),
#        Subsignal("clk",  Pins("30")),
#        Subsignal("mosi", Pins("29")),
#        Subsignal("miso", Pins("28")),
#        IOStandard("LVCMOS33")
#s    ),

    # SPIRAM
    # SPIFlash
    ("i2c", 0,
     Subsignal("sda", Pins("E4")),
     Subsignal("scl", Pins("D3")),
     IOStandard("LVCMOS33"))


    # RMII Ethernet

]



_bank_info = [
]

# Connectors ---------------------------------------------------------------------------------------
_connectors = [
    ("j1",  "C4  D4  E4  - D3  E3  F4  N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j2",  "F3  F5  G3  - G4  H3  H4  N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j3",  "G5  H5  J5  - J4  B1  C2  N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j4",  "C1  D1  E2  - E1  F2  F1  N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j5",  "G2  G1  H2  - K5  K4  L3  N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j6",  "L4  L5  P2  - R2  T2  R3  N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j7",  "T3  R4  M5  - P5  N6  N7  N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j8",  "P7  M7  P8  - R8  M8  M9  N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j9",  "P11 N11 M11 - T13 R12 R13 N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j10", "R14 T14 D16 - C15 C16 B16 N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j11", "B15 C14 T15 - P15 R15 P12 N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j12", "P13 N12 N13 - M12 P14 N14 N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j13", "H15 H14 G16 - F16 G15 F15 N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j14", "E15 E16 L12 - L13 M14 L14 N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j15", "J13 K13 J12 - H13 H12 G12 N4 N5 N3 P3 P4 M3 N1 M4 -"),
    ("j16", "G14 G13 F12 - F13 F14 E14 N4 N5 N3 P3 P4 M3 N1 M4 -"),
]

# Platform -----------------------------------------------------------------------------------------

class Platform(LatticeECP5Platform):
    default_clk_name   = "clk33"
    default_clk_freq   = 25.000e6
    default_clk_period = 1e9/25.000e6
		
    def __init__(self, toolchain="trellis"):
        LatticeECP5Platform.__init__(self, "LFE5U-25F-6BG256C",_io, _connectors, toolchain=toolchain)

    def do_finalize(self, fragment):
        LatticeECP5Platform.do_finalize(self, fragment)
        self.add_period_constraint(self.lookup_request("clk33", loose=True), 1e9/25.000e6)

    def create_programmer(self):
        return OpenOCDJTAGProgrammer("openocd_colorlight_5a_75b.cfg")
