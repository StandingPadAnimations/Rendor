CXX       := g++
CXX_FLAGS := -std=c++17 -Wall -Wfatal-errors 

BIN     := bin
SRC     := src
INCLUDE := include
BOOST := "C:/C++ Libaries/boost_1_77_0/"

LIBRARIES   := "C:/C++ Libaries/boost_1_77_0/stage/lib/"
EXECUTABLE  := Rendor

BOOSTFILE := boost_filesystem
BOOSTSYS := boost_system

all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(BOOST) $^ -o $@ -L$(LIBRARIES) -l$(BOOSTFILE) -l$(BOOSTSYS)

clean:
	-rm $(BIN)/*