TESTS=simple normalization
all: compile
compile: $(foreach test,$(TESTS),test_$(test))
test: $(foreach test,$(TESTS),run_test_$(test))

run_test_simple: test_simple
	./$<

run_test_normalization: test_normalization
	xzcat ../gen/data/NormalizationTest-6.3.0.txt.xz | sed 's/ *[#@].*$$//;s/;$$//;/^$$/d' | ./$<

test_simple: $(foreach src,unicode utf8 utf16,../unilib/$(src).cpp ../unilib/$(src).h)
test_normalization: ../unilib/uninorms.cpp ../unilib/uninorms.h
test_simple test_normalization:%:%.cpp test.h
	g++ $(filter %.cpp,$^) -std=c++11 -I ../unilib -W -Wall -O2 -g -o $@

.PHONY: clean
clean:
	rm -f $(foreach test,$(TESTS),test_$(TEST))
