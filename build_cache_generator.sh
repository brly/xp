#!/bin/sh
g++ cache_generator.cc hog.cc `pkg-config opencv --cflags --libs` -o cache_generator -ggdb -std=c++0x