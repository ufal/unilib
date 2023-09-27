#!/usr/bin/env python3
import argparse
import re

parser = argparse.ArgumentParser()
parser.add_argument("unicode_data_path", type=str, help="Path to UnicodeData file")
args = parser.parse_args()

with open(args.unicode_data_path, "r", encoding="utf-8") as unicode_data_file:
    for line in unicode_data_file:
        line = line.rstrip("\n")
        line = re.sub(r"\s*[#@].*$", "", line)
        line = re.sub(r"\s*;$", "", line)
        if line:
            print(line)
