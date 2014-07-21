TARGETS=unicode.cpp unicode.h uninorms.cpp uninorms.h utf8.cpp utf8.h utf16.cpp utf16.h
UNILIB=1.1
UNICODE=7.0.0

all: $(foreach target,$(TARGETS),../unilib/$(target))

../unilib/%: template/% generate.pl Makefile data/UnicodeData-$(UNICODE).txt.xz data/CompositionExclusions-$(UNICODE).txt.xz
	./generate.pl $(UNILIB) $(UNICODE) data/UnicodeData-$(UNICODE).txt.xz data/CompositionExclusions-$(UNICODE).txt.xz <$< >$@
