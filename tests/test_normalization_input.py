#!/usr/bin/env python3
import argparse
import lzma
import re

parser = argparse.ArgumentParser()
parser.add_argument("unicode_data_path", type=str, help="Path to UnicodeData file")
args = parser.parse_args()

with lzma.open(args.unicode_data_path, "rt", encoding="utf-8") as unicode_data_file:
    for line in unicode_data_file:
        line = line.rstrip("\n")
        line = re.sub(r"\s*[#@].*$", "", line)
        line = re.sub(r"\s*;$", "", line)
        if line:
            print(line)
