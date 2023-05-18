# This Makefile is designed to be simple and readable.  It does not
# aim at portability.  It requires GNU Make.

BISON = bison
CXX = g++
FLEX = flex
XSLTPROC = xsltproc

all: build move clean
build: bison flex

bison:
	$(BISON) $(BISONFLAGS) --xml --graph=parser.gv -o parser.cc parser.yy

flex:
	$(FLEX) $(FLEXFLAGS) -o scanner.cc scanner.ll

html: parser.html
%.html: %.xml
	$(XSLTPROC) $(XSLTPROCFLAGS) -o $@ $$($(BISON) --print-datadir)/xslt/xml2xhtml.xsl $<

CLEANFILES =										\
  parser.hh parser.cc parser.output parser.xml parser.html parser.gv location.hh	\
  scanner.cc
clean:
	rm -f $(CLEANFILES)

DESTFOLDER = "../simulator/lib"
MOVEFILES = scanner.cc location.hh parser.hh parser.cc

move:
	mv -t $(DESTFOLDER) $(MOVEFILES)
