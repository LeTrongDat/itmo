.PHONY: build run

BUILDDIR = build
SRCDIR = src
INCDIR = include
TESTDIR = tests

build: $(BUILDDIR)/myparser

$(BUILDDIR)/myparser: $(SRCDIR)/parser/lexer.l $(SRCDIR)/parser/parser.y $(SRCDIR)/types.c
	@mkdir -p $(BUILDDIR)
	bison -d $(SRCDIR)/parser/parser.y -o $(BUILDDIR)/parser.c
	lex -o $(BUILDDIR)/lexer.c $(SRCDIR)/parser/lexer.l
	gcc -I$(INCDIR) -o $(BUILDDIR)/myparser $(BUILDDIR)/lexer.c $(BUILDDIR)/parser.c $(SRCDIR)/types.c

run:
	@./$(BUILDDIR)/myparser $(TESTDIR)/create_table.sql
	@./$(BUILDDIR)/myparser $(TESTDIR)/insert.sql
	@./$(BUILDDIR)/myparser $(TESTDIR)/select.sql
	@./$(BUILDDIR)/myparser $(TESTDIR)/update.sql
	@./$(BUILDDIR)/myparser $(TESTDIR)/delete.sql


debug: build run
