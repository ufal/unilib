TARGETS=Makefile.include unicode.cpp unicode.h uninorms.cpp uninorms.h unistrip.cpp unistrip.h utf8.cpp utf8.h utf16.cpp utf16.h version.h
UNILIB=3.0.1-devel
UNICODE=7.0.0

all: generate.pl Makefile data/UnicodeData-$(UNICODE).txt.xz data/CompositionExclusions-$(UNICODE).txt.xz $(TARGETS:%=template/%)
	./generate.pl $(UNILIB) $(UNICODE) data/UnicodeData-$(UNICODE).txt.xz data/CompositionExclusions-$(UNICODE).txt.xz $(foreach t,$(TARGETS),template/$(t) ../unilib/$(t))
