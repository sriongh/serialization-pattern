CC=g++
BOOST_INSTALL_PATH=/home/sriram/local/boost
BOOST_LIB=-lboost_serialization

EXE=\
	serialize-polymorphic-objects

SRC = serialize-polymorphic-objects.cpp value_object.cpp

OBJS = $(SRC:.cpp=.o)

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) -L$(BOOST_INSTALL_PATH)/lib -o $@ $(OBJS) $(BOOST_LIB)

.c.o:
	$(CC) -g -I$(BOOST_INSTALL_PATH)/include -c $< -o $@


clean:
	rm -rf *.o $(EXE)
