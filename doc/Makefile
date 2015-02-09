include t2t_docsys/Makefile.include

.PHONY: all doc install
all: doc
doc: manual.txt manual.html readme.txt readme_md.md
install: doc
	cp manual.txt ../MANUAL
	cp manual.html ../MANUAL.html
	cp readme.txt ../README
	cp readme_md.md ../README.md

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
	@rm -f $(T2T_DOCSYS_OUTPUT_WILDCARDS)
