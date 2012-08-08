#!/bin/sh
g++ svm_unit_test.cc hog.o util.o svm.o svm_wrapper.o `pkg-config opencv --cflags --libs` -o svm_unit_test -ggdb -std=c++0x
