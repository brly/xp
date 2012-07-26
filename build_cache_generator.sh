#!/bin/sh
g++ util.cc cache_generator.cc hog.cc svm_wrapper.cc svm.cc visualize_hog.cc `pkg-config opencv --cflags --libs` -O2 -o cache_generator -ggdb -std=c++0x
