from migen import *
from litex.soc.interconnect.csr import *

class _PWM(Module, AutoCSR):
    def __init__(self, pwm):
        self.enable = enable = Signal()
        self.width = width = Signal(32)
        self.period = period = Signal(32)
        count = Signal(32)
        self.sync += [
            If(enable,
               If(count < width,
                  pwm.eq(1)
                  ).Else(
                      pwm.eq(0) ),
               If(count == period-1,
                  count.eq(0)
                  ).Else(
                      count.eq(count+1) )
               ).Else(
                   count.eq(0),
                   pwm.eq(0) )
        ]

# PWM --------------------------------------------------
class PWM(Module, AutoCSR):
    def __init__(self, pwm):
        self.enable = CSRStorage()
        self.width = CSRStorage(32)
        self.period = CSRStorage(32)
        _pwm = _PWM(pwm)
        self.submodules += _pwm
        self.comb += [
            _pwm.enable.eq(self.enable.storage),
            _pwm.width.eq(self.width.storage),
            _pwm.period.eq(self.period.storage)
        ]
