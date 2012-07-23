#!/bin/sh
g++ hog_unit_test.cc hog.cc `pkg-config opencv --cflags --libs` -o hog_unit_test -ggdb -std=c++0x
