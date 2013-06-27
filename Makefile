CPP =  g++
#CPPFLAGS = -O3
CPPFLAGS = -g
LDFLAGS = -lfl
LEX = flex
LEXFLAGS = 
YACC = bison
YACCFLAGS = -d

.PHONY: all
all: om

om: lex.yy.o om.tab.o common.o type.o ast.o node.o hash.o list.o
	$(CPP) $(CPPFLAGS) -o $@ $^

ast.o: ast.cpp ast.h
	$(CPP) $(CPPFLAGS) -c $<

node.o: node.cpp node.h
	$(CPP) $(CPPFLAGS) -c $<

type.o: type.cpp type.h
	$(CPP) $(CPPFLAGS) -c $<

hash.o: hash.cpp hash.h
	$(CPP) $(CPPFLAGS) -c $<

list.o: list.cpp list.h
	$(CPP) $(CPPFLAGS) -c $<

lex.yy.o: lex.yy.c om.tab.h++ 
	$(CPP) $(CPPFLAGS) -c $<

om.tab.o: om.tab.c++ om.tab.h++ 
	$(CPP) $(CPPFLAGS) -c $<

common.o: common.cpp common.h
	$(CPP) $(CPPFLAGS) -c $<

lex.yy.c: om.l om.tab.c++
	$(LEX) $(LEXFLAGS) $<

om.tab.c++: om.y++ 
	$(YACC) $(YACCFLAGS) $<


.PHONY: clean
clean :
	rm -f om core a.out *.o 
	rm -f om.tab.* om.tab.* lex.yy.* 
	rm -f *.exe *.exe.stackdump


test: all
	time for x in test/* ; do echo $$x ; ./om $$x; echo; done


