################################################################################
## basis

LIBS := `pkg-config opencv --cflags --libs`

CXX := g++

CXXFLAGS := -Wall -std=c++0x -ggdb 

OBJS := main.o svm_wrapper.o svm.o visualize_hog.o \
	hog.o simple_method.o assembling_method.o  \
	simple_represent_vector_factory.o linear_combination_method.o \
	search_database.o util.o base_method.o normal_method.o \
	experiment_manager.o c_method.o

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
main.o: base_method.h simple_method.h assembling_method.h \
	linear_combination_method.h timer.h search_database.h normal_method.h \
	experiment_manager.h c_method.h

svm_wrapper.o: svm_wrapper.h hog.h constant.h visualize_hog.h

visualize_hog.o: visualize_hog.h constant.h

hog.o: hog.h

simple_method.o: simple_method.h svm_wrapper.h hog.h svm.h constant.h \
		 random_function.h timer.h

assembling_method.o: assembling_method.h random_function.h hog.h timer.h

simple_represent_vector_factory.o: 	simple_represent_vector_factory.h hog.h \
					constant.h random_function.h

util.o: util.h

linear_combination_method.o: linear_combination_method.h search_database.h \
			     hog.h timer.h

search_database.o: search_database.h hog.h constant.h

base_method.o: constant.h svm_wrapper.h base_method.h hog.h random_function.h \
		util.h timer.h search_database.h

normal_method.o: method.h constant.h normal_method.h hog.h search_database.h

c_method.o: method.h constant.h hog.h random_function.h search_database.h \
		timer.h svm_wrapper.h util.h

experiment_manager.o: experiment_manager.h constant.h random_function.h \
			util.h base_method.h simple_method.h \
			linear_combination_method.h assembling_method.h \
			normal_method.h c_method.h

################################################################################
## Clean
.PHONY: clean
clean:
	$(RM) $(OBJS) $(OUTPUT)
