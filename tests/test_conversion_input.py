#!/usr/bin/env python3
import array
import unicodedata

u32, u16, u8 = [], [], []

for i in range(1, 0x110000):
    c = chr(i)
    if unicodedata.category(c) != "Cs":
        u32.append(i)
        u16.extend(array.array("H", c.encode("utf-16"))[1:])
        u8.extend(c.encode("utf-8"))

for u in [u32, u16, u8]:
    print(" ".join(map(str, u)))
