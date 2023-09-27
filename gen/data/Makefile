# This file is part of UniLib <http://github.com/ufal/unilib/>.
#
# Copyright 2014-2023 Institute of Formal and Applied Linguistics, Faculty
# of Mathematics and Physics, Charles University in Prague, Czech Republic.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

VERSION=15.1.0
UCD=UnicodeData CompositionExclusions NormalizationTest
all: $(UCD:=.txt)

$(UCD:=.txt):
	curl http://www.unicode.org/Public/$(VERSION)/ucd/$@ >$@
