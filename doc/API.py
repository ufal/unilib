#!/usr/bin/env python3

# This file is part of UniLib <http://github.com/ufal/unilib/>.
#
# Copyright 2014-2023 Institute of Formal and Applied Linguistics, Faculty
# of Mathematics and Physics, Charles University in Prague, Czech Republic.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

import argparse
import os
import re

parser = argparse.ArgumentParser()
parser.add_argument("path", type=str, help="Path to the API.md file")
args = parser.parse_args()

# Load the current API.md
with open(args.path, "r", encoding="utf-8") as api_file:
    lines = [line.rstrip("\r\n") for line in api_file.readlines()]

# Copy the current version from version.h
version = None
with open(os.path.join("..", "unilib", "version.h"), "r") as version_file:
    for line in version_file:
        if (match := re.match(r"// UniLib version: ([-0-9.a-z]+)$", line)):
            version = match.group(1)
assert version is not None
assert re.match(r"# UniLib [-0-9.a-z]+$", lines[0])
lines[0] = "# UniLib {}".format(version)

# Copy the information about the following classes
for class_name in [line.removeprefix("## Class `").removesuffix("`")
                   for line in lines if line.startswith("## Class `")]:
    with open(os.path.join("..", "unilib", "{}.h".format(class_name)), "r") as class_file:
        source = class_file.read()

    # Keep only the class itself
    source = re.sub(fr"^.*\n\n(?=(?:(?:[^\n]|\n[^\n])*\n)?class {class_name})", "", source, flags=re.S)
    source = re.sub(r"(?<=\n};)\n.*$", "", source, flags=re.S)

    # Drop the private parts
    source = re.sub(fr"(?:(?<=class {class_name} {{)\n|\n\s*private:\n)(?:(?!public).)*(?=\n\s*public:|\n}})", "", source, flags=re.S)

    # Special rules for the unicode class
    source = re.sub(r" = 1 << _[A-Z][a-z]", "", source)
    source = re.sub(r"(?<=Lt,|Pi,)\n\s*", " ", source)

    # Paste the source to the documentation
    index = lines.index("## Class `{}`".format(class_name))
    lines = lines[:index + 1] + ["", "```cpp", *source.split("\n"), "```"] + lines[lines.index("```", index) + 1:]

# Generate the new API.md
with open(args.path, "w", encoding="utf-8") as api_file:
    print(*lines, file=api_file, sep="\n")
