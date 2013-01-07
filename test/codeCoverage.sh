#!/bin/bash

make clean
make
./testsRunner
gcov -o ../src/source/ Graphe.c 
gcov -o ../src/source/ TPersonne.c
gcov -o ../src/source/ FacebookLib.c
gcov -o ../src/source/ ParseFile.c
gcov -o ../src/source/ ExportGraph.c
rm -rf ./*.gcno ./*.gcda ../*.gcno ../*.gcda ../src/source/*.gcno ../src/source/*.gcda
make clean