include t2t_docsys/Makefile.include

all:
	@echo "make [html | txt | pdf | manual]"

html: manual.html
txt: manual.txt
pdf: manual.pdf
manual: manual.html manual.txt readme.txt
	cp manual.txt ../MANUAL
	cp manual.html ../MANUAL.html
	cp readme.txt ../README

%.html: %.t2t $(wildcard *.t2t)
	$(call t2t_docsys_html,$@,$<,manual)

%.txt: %.t2t $(wildcard *.t2t)
	$(call t2t_docsys_txt,$@,$<,manual)

%.md: %.t2t $(wildcard *.t2t)
	$(call t2t_docsys_md,$@,$<,manual)

%.pdf: %.t2t $(wildcard *.t2t)
	$(call t2t_docsys_pdf,$@,$<,manual)

.PHONY: clean
clean:
	@rm -f $(wildcard $(T2T_DOCSYS_OUTPUT_WILDCARDS))
