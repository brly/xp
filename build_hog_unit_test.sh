#!/bin/sh
g++ hog_unit_test.cc `pkg-config opencv --cflags --libs` -o hog_unit_test -ggdb
