#!/bin/sh
g++ fileopen_unit_test.cc -o fileopen_unit_test -ggdb `pkg-config opencv --cflags --libs` 
