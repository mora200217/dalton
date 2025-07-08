from migen import *
from litex.soc.interconnect.csr import *
import os
import math

"""
Generic quadrature decoder. HDL wrapper
"""
class QuadratureDecoder(Module, AutoCSR):
    def __init__(self, platform):
        hdl_src = os.path.join(os.path.dirname(os.path.abspath(__file__)),
            'rtl/quadrature_decoder/quadrature_decoder.v')
        platform.add_source(hdl_src)
        PPR = 960 # base resolution of the encoding wheel
        UPDATE_RATE = int(0.5*1/platform.default_clk_period) # in clock cycles

        self.a = CSRStorage(1)
        self.b = CSRStorage(1)
        self.p_cnt = CSRStatus(math.ceil(math.log2(PPR)))

        self.specials += [
            Instance('quadrature_decoder',
                p_PPR=PPR,
                p_UPDATE_RATE=UPDATE_RATE,
                i_clk=ClockSignal('sys'),
                i_rst=ResetSignal('sys'),
                i_a=self.a.storage,
                i_b=self.b.storage,
                o_p_cnt=self.p_cnt.status
            )
        ]

