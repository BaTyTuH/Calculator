SHELL=/bin/sh
CC=g++
CFLAGS=-Wall -Werror -Wextra
OS=$(shell uname)
ifeq ($(OS), Linux)
    CHECK_FLAGS=-lgtest -pthread -lrt -lm -lsubunit
else
    CHECK_FLAGS=-lgtest
endif
BUILD=calculator
HOME=calc_plus
TEST_COMPILE=$(CC) $(CFLAGS) test.cpp $(CHECK_FLAGS)

all: clean install test

install: uninstall
	mkdir $(BUILD)
	cd $(HOME); qmake calc_plus.pro
	make -C $(HOME)
	cd $(HOME); mv moc_* ui_* calc_plus.app Makefile ../$(BUILD)
	cd $(HOME); mv *.o ../$(BUILD)

uninstall:
	rm -rf $(BUILD)

start:
	open $(BUILD)/calc_plus.app

test:
	$(TEST_COMPILE)
	./a.out
	make clean

dvi:
	makeinfo --html doc.texi
	open doc/index*

dist:
	rm -rf ../smart_calc.tar.gz
	tar -czvf smart_calc.tar.gz Makefile calc_plus test.cpp

linter: 
	cp ../materials/linters/CPPLINT.cfg ./
	python3 ../materials/linters/cpplint.py --extensions=cpp calc_plus/g* \
						calc_plus/c* calc_plus/ma* calc_plus/s* calc_plus/mo*p
	rm CPPLINT.cfg

clean:
	rm -rf doc *.g* *.info *.out build*
