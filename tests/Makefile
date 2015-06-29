include ../unilib/Makefile.include

TESTS = compile normalization simple strip ucd
UNICODE = 8.0.0

.PHONY: all compile test
all: compile
compile: $(foreach test,$(TESTS),test_$(test))
test: $(foreach test,$(TESTS),run_test_$(test))

run_test_compile: test_compile
run_test_normalization: test_normalization
	xzcat ../gen/data/NormalizationTest-$(UNICODE).txt.xz | sed 's/ *[#@].*$$//;s/;$$//;/^$$/d' | ./$<
run_test_simple: test_simple
	./$<
run_test_strip: test_strip
	./$<
run_test_ucd: test_ucd
	xzcat ../gen/data/UnicodeData-$(UNICODE).txt.xz | ./$<

test_compile: $(foreach src,$(UNILIB_OBJECTS),../unilib/$(src).cpp ../unilib/$(src).h)
test_normalization: ../unilib/uninorms.cpp ../unilib/uninorms.h
test_simple: $(foreach src,unicode utf8 utf16,../unilib/$(src).cpp ../unilib/$(src).h)
test_strip: $(foreach src,unicode uninorms unistrip,../unilib/$(src).cpp ../unilib/$(src).h)
test_ucd: ../unilib/unicode.cpp ../unilib/unicode.h
$(patsubst %,test_%,$(TESTS)):%:%.cpp test.h
	$(CXX) $(filter %.cpp,$^) -std=c++11 -I ../unilib -W -Wall -Werror -O2 -o $@

.PHONY: clean
clean:
	rm -f $(foreach test,$(TESTS),test_$(test))
