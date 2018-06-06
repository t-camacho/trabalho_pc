FILES = DList.cpp Benchmark.cpp BenchMain.cpp
CFLAGS = -fgnu-tm
LFLAGS = -lpthread -lrt
TARGET = build/app
RESULT = main

all: compile
	@echo "Success"

compile: $(FILES) DList.hpp Benchmark.hpp RWPathology.hpp
	g++ $(CFLAGS) $(FILES) -o $(RESULT) $(LFLAGS)
	mkdir -p $(TARGET)
	mv $(RESULT) $(TARGET)

clean: $(TARGET)/$(RESULT)
	rm -r $(TARGET)

