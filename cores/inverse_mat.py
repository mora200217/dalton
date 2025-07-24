from migen.genlib.cdc import MultiReg
from litex.soc.interconnect.csr import *
from litex.soc.interconnect.csr_eventmanager import *

class VerilogInverse(Module, AutoCSR):
    def __init__(self, data):
        # Interfaz
        self.clk = ClockSignal()
        self.rst = ResetSignal()

        # Datos directos 
        # self.uarttxd = data.uarttxd
        # self.uartrxd = data.uartrxd
        self.angle = CSRStorage(32)
        self.enable = CSRStorage() 

        self.sin_out = CSRStatus(32)
        self.cos_out = CSRStatus(32)

        self.opt = CSRStorage(2)         
        self.done = CSRStatus() 

        # Instanciación del módulo Verilog
        self.specials += Instance("cordic",
            i_clk=self.clk,
            i_reset=self.rst,
            
            o_sin_out = self.sin_out.status, 
            o_cos_out = self.cos_out.status, 
            i_angle = self.angle.storage,
            i_enable = self.enable.storage,

            i_opt = self.opt.storage, 
            o_done = self.done.status, 
 
        )

        self.submodules.ev = EventManager()
        self.ev.ok = EventSourceProcess()
        self.ev.finalize()
