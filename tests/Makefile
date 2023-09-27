# This file is part of UniLib <http://github.com/ufal/unilib/>.
#
# Copyright 2014-2023 Institute of Formal and Applied Linguistics, Faculty
# of Mathematics and Physics, Charles University in Prague, Czech Republic.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

include Makefile.builtem
include ../unilib/Makefile.include

CPP_STANDARD = c++17
TESTS = $(foreach test,compile conversion normalization strip ucd,test_$(test))

.PHONY: all compile test
all: compile
compile: $(call exe,$(TESTS))
test: $(addprefix run_,$(TESTS))

run_test_compile: $(call exe,test_compile)
run_test_conversion: $(call exe,test_conversion)
	python3 test_conversion_input.py | $(call platform_name,./$<)
run_test_normalization: $(call exe,test_normalization)
	python3 test_normalization_input.py ../gen/data/NormalizationTest.txt | $(call platform_name,./$<)
run_test_strip: $(call exe,test_strip)
	$(call platform_name,./$<)
run_test_ucd: $(call exe,test_ucd)
	python3 test_ucd_input.py ../gen/data/UnicodeData.txt | $(call platform_name,./$<)

C_FLAGS += $(call include_dir,../unilib)
$(call exe,test_compile): $(call obj,$(addprefix ../unilib/,$(UNILIB_OBJECTS)))
$(call exe,test_conversion): $(call obj,$(addprefix ../unilib/,unicode utf8 utf16))
$(call exe,test_normalization): $(call obj,$(addprefix ../unilib/,uninorms))
$(call exe,test_strip): $(call obj,$(addprefix ../unilib/,unicode uninorms unistrip))
$(call exe,test_ucd): $(call obj,$(addprefix ../unilib/,unicode))
$(call exe,$(TESTS)):$(call exe,%): $$(call obj,%)
	$(call link_exe,$@,$^,$(call win_subsystem,console))

.PHONY: clean
clean:
	$(call rm,.build $(call all_exe,$(TESTS)))
