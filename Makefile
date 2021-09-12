CXX       := g++
CXX_FLAGS := -std=c++17 -Wall 

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   := 
EXECUTABLE  := Rendor


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*