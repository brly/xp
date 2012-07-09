LIBS := `pkg-config opencv --cflags --libs`
CXX := g++
CXXFLAGS := -Wall -std=c++0x -ggdb 
OBJS := main.o svm_wrapper.o svm.o visualize_hog.o hog.o
OUTPUT := application

$(OUTPUT): $(OBJS)
	$(CXX) -g -O2 -o $(OUTPUT) $(OBJS) $(LIBS)

# Suffix
.SUFFIXES: .h .cc. o
.c.o:
	$(CXX) -c $(CXXFLAGS) $(LIBS) $<

.h.cc:
main.o: svm_wrapper.h
svm_wrapper.o: svm_wrapper.h hog.h constant.h
visualize_hog.o: visualize_hog.h constant.h
hog.o: hog.h

# Clean
.PHONY: clean
clean:
	$(RM) $(OBJS) $(OUTPUT)
