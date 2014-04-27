all: UnicodeData-6.3.0.txt.xz CompositionExclusions-6.3.0.txt.xz

UnicodeData-%.txt.xz:
	curl http://www.unicode.org/Public/$*/ucd/UnicodeData.txt | xz -c9v >$@

CompositionExclusions-%.txt.xz:
	curl http://www.unicode.org/Public/$*/ucd/CompositionExclusions.txt | xz -c9v >$@
