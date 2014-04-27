TARGETS=unicode.cpp unicode.h utf8.cpp utf8.h utf16.cpp utf16.h
UNILIB=0.9.9
UNICODE=6.3.0

all: $(foreach target,$(TARGETS),../unilib/$(target))

../unilib/%: template/% generate.pl data/UnicodeData-$(UNICODE).txt.xz
	./generate.pl $(UNILIB) $(UNICODE) data/UnicodeData-$(UNICODE).txt.xz <$< >$@
