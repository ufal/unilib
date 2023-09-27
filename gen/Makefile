TARGETS=Makefile.include unicode.cpp unicode.h uninorms.cpp uninorms.h unistrip.cpp unistrip.h utf8.cpp utf8.h utf16.cpp utf16.h version.cpp version.h
UNILIB=3.3.2-dev
UNICODE=15.1.0

all: generate.pl Makefile data/UnicodeData-$(UNICODE).txt data/CompositionExclusions-$(UNICODE).txt $(TARGETS:%=template/%)
	./generate.pl $(UNILIB) $(UNICODE) data/UnicodeData-$(UNICODE).txt data/CompositionExclusions-$(UNICODE).txt $(foreach t,$(TARGETS),template/$(t) ../unilib/$(t))
