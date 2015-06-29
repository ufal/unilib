all: $(foreach data,UnicodeData CompositionExclusions NormalizationTest,$(data)-8.0.0.txt.xz)

UnicodeData-%.txt.xz:
	curl http://www.unicode.org/Public/$*/ucd/UnicodeData.txt | xz -c9v >$@

CompositionExclusions-%.txt.xz:
	curl http://www.unicode.org/Public/$*/ucd/CompositionExclusions.txt | xz -c9v >$@

NormalizationTest-%.txt.xz:
	curl http://www.unicode.org/Public/$*/ucd/NormalizationTest.txt | xz -c9v >$@
