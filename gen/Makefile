# This file is part of UniLib <http://github.com/ufal/unilib/>.
#
# Copyright 2014-2024 Institute of Formal and Applied Linguistics, Faculty
# of Mathematics and Physics, Charles University in Prague, Czech Republic.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

TARGETS=Makefile.include unicode.cpp unicode.h uninorms.cpp uninorms.h unistrip.cpp unistrip.h utf.h version.h
UNILIB=4.1.3-dev
UNICODE=17.0.0

all: generate.pl Makefile $(wildcard data/*.txt) $(TARGETS:%=template/%)
	./generate.pl $(UNILIB) $(UNICODE) data $(foreach t,$(TARGETS),template/$(t) ../unilib/$(t))
	make -C ../doc
