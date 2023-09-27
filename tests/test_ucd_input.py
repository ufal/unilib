#!/usr/bin/env python3
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("unicode_data_path", type=str, help="Path to UnicodeData file")
args = parser.parse_args()

with open(args.unicode_data_path, "r", encoding="ascii") as unicode_data_file:
    print(unicode_data_file.read(), end="")
