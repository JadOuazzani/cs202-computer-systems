CC = gcc
CFLAGS = -std=c99 -I cg -I cg_nl
LDLIBS = -lm

hand: main.o hpwl.o ourCG.o ourPlacer.o parser.o quadraticPlacer.o random_gp.o tetris_lg.o writeResults.o cg.o
	$(CC) $^ -o $@ $(LDLIBS)

hpwl.o: hpwl.c parser.h hpwl.h
main.o: main.c parser.h random_gp.h tetris_lg.h hpwl.h writeResults.h \
 quadraticPlacer.h ourPlacer.h cg_nl/cg_descent.h
ourCG.o: ourCG.c parser.h ourPlacer.h cg_nl/cg_descent.h ourCG.h
ourPlacer.o: ourPlacer.c parser.h quadraticPlacer.h ourPlacer.h \
 cg_nl/cg_descent.h ourCG.h
parser.o: parser.c parser.h
quadraticPlacer.o: quadraticPlacer.c parser.h quadraticPlacer.h \
 ourPlacer.h cg_nl/cg_descent.h cg/cg.h
random_gp.o: random_gp.c parser.h random_gp.h
tetris_lg.o: tetris_lg.c parser.h tetris_lg.h
writeResults.o: writeResults.c parser.h writeResults.h
cg.o: cg/cg.c cg/cg.h