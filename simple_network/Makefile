CXX := g++
CXXFLAGS := -Wall -std=c++11
TARGET := main.exe
SOURCES := main.cpp ServerGame.cpp ClientGame.cpp ServerNetwork.cpp ClientNetwork.cpp NetworkServices.cpp
OBJECTS := $(SOURCES:.cpp=.o)
INCLUDES := -I.
LDFLAGS := -lws2_32


all: $(TARGET)


$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)


%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


.PHONY: clean
clean:
	del /Q *.o *.exe 2>nul || true
