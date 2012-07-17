################################################################################
## basis

LIBS := `pkg-config opencv --cflags --libs`

CXX := g++

CXXFLAGS := -Wall -std=c++0x -ggdb 

OBJS := main.o svm_wrapper.o svm.o visualize_hog.o \
	hog.o simple_method.o assembling_method.o  \
	simple_represent_vector_factory.o linear_combination_method.o \
	search_database.o

OUTPUT := application

$(OUTPUT): $(OBJS)
	$(CXX) -g -O2 -o $(OUTPUT) $(OBJS) $(LIBS)

################################################################################
# Suffix
.SUFFIXES: .h .cc. o
.c.o:
	$(CXX) -c $(CXXFLAGS) $(LIBS) $<

################################################################################
## Header dependency
.h.cc:
main.o: simple_method.h

svm_wrapper.o: svm_wrapper.h hog.h constant.h visualize_hog.h

visualize_hog.o: visualize_hog.h constant.h

hog.o: hog.h

simple_method.o: simple_method.h svm_wrapper.h hog.h svm.h constant.h random_function.h

assembling_method.o: assembling_method.h random_function.h hog.h

simple_represent_vector_factory.o: 	simple_represent_vector_factory.h hog.h \
					constant.h random_function.h

linear_combination_method.o: linear_combination_method.h search_database.h hog.h

search_database.o: search_database.h hog.h constant.h

################################################################################
## Clean
.PHONY: clean
clean:
	$(RM) $(OBJS) $(OUTPUT)
