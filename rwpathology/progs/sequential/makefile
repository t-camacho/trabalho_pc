FILES = DList.cpp Benchmark.cpp BenchMain.cpp
#FLAGS = -lpthread
TARGET = build/app
RESULT = main

all: compile
	@echo "Success"

compile: $(FILES) DList.hpp Benchmark.hpp RWPathology.hpp
	g++ $(FILES) -o $(RESULT)
	mkdir -p $(TARGET)
	mv $(RESULT) $(TARGET)

clean: $(TARGET)/$(RESULT)
	rm -r $(TARGET)
