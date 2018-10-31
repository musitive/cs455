CC = g++
CC_FLAGS = -g
CC_VERSION = -std=c++17

EXEC = tracer
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

$(EXEC): $(OBJECTS)
	$(CC) $(CC_FLAGS) $(CC_VERSION) $(OBJECTS) -o $@

%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $(CC_VERSION) $< -o $@

clean:
	rm -f $(EXEC) $(OBJECTS)