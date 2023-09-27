all: $(foreach data,UnicodeData CompositionExclusions NormalizationTest,$(data)-15.1.0.txt)

UnicodeData-%.txt:
	curl http://www.unicode.org/Public/$*/ucd/UnicodeData.txt >$@

CompositionExclusions-%.txt:
	curl http://www.unicode.org/Public/$*/ucd/CompositionExclusions.txt >$@

NormalizationTest-%.txt:
	curl http://www.unicode.org/Public/$*/ucd/NormalizationTest.txt >$@
