CC=g++ -g
BOOST_INSTALL_PATH=
BOOST_LIB=-lboost_serialization

EXE=\
	spdfp1

all: $(EXE)

spdfp1: spdfp1.cpp
	$(CC) -I$(BOOST_INSTALL_PATH)/include -L$(BOOST_INSTALL_PATH)/lib $< -o $@ $(BOOST_LIB)

clean:
	rm -rf $(EXE)