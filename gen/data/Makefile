# This file is part of UniLib <http://github.com/ufal/unilib/>.
#
# Copyright 2014-2023 Institute of Formal and Applied Linguistics, Faculty
# of Mathematics and Physics, Charles University in Prague, Czech Republic.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

all: $(foreach data,UnicodeData CompositionExclusions NormalizationTest,$(data)-15.1.0.txt)

UnicodeData-%.txt:
	curl http://www.unicode.org/Public/$*/ucd/UnicodeData.txt >$@

CompositionExclusions-%.txt:
	curl http://www.unicode.org/Public/$*/ucd/CompositionExclusions.txt >$@

NormalizationTest-%.txt:
	curl http://www.unicode.org/Public/$*/ucd/NormalizationTest.txt >$@
