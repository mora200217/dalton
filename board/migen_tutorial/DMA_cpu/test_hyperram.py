#!/usr/bin/env python3
#litex_server --udp --udp-ip=10.42.0.220
import time
import random

from litex import RemoteClient

wb = RemoteClient()
wb.open()

base_addr=0x40000000
size=1024

print("=== HyperRAM Test ===")

# Write pattern
pattern = [0xAA55AA55, 0x55AA55AA, 0xDEADBEEF, 0xCAFEBABE]
for i, val in enumerate(pattern):
    wb.write(base_addr + i*4, val)

# Read back
errors = 0
for i, expected in enumerate(pattern):
    actual = wb.read(base_addr + i*4)
    if actual != expected:
        print(f"Error at 0x{(base_addr + i*4):08X}: {actual:08X} != {expected:08X}")
        errors += 1
        
print(f"HyperRAM Test: {errors} errors in {len(pattern)} words")
wb.close()