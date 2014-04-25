all: UnicodeData-6.3.0.txt.xz

UnicodeData-%.txt.xz:
	curl http://www.unicode.org/Public/$*/ucd/UnicodeData.txt | xz -c9v >$@
