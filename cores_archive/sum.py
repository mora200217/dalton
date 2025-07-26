from migen import *
from migen.genlib.cdc import MultiReg
from litex.soc.interconnect.csr import *
from litex.soc.interconnect.csr_eventmanager import *

class Verilog_SUM(Module, AutoCSR):
    def __init__(self):
        # self.clk = ClockSignal()
        # self.rst = ResetSignal

        # Inputs regisers in CSR bus
        self.a = CSRStorage()
        self.b = CSRStorage()


        # Outpus
        self.c = CSRStatus()

        ## intanciamos el verilog
        self.specials += Instance("sum",
                                  i_a = self.a.storage,
                                  i_b = self.b.storage,
                                  o_c = self.c.status)

        self.submodules.ev = EventManager()
        self.ev.ok = EventSourceProcess()
        self.ev.finalize()
