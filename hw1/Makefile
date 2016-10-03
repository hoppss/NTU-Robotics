CC = gcc
CXX = g++
CFLAGS = -Wall -g -fno-exceptions -fPIC
LDFLAGS = -lAria -pthread -ldl -lArNetworking -lm
INCS = -I/usr/local/Aria/include -I/usr/local/Aria/ArNetworking/include
LIBS = -L/usr/local/Aria/lib
EXEC = example part_c part_e bonus 
OBJS = example.o part_c.o part_e.o bonus.o

all: $(EXEC)

run_c: part_c
	./part_c

run_e: part_e
	./part_e

run_bonus: bonus
	./bonus

%: %.cpp
	$(CXX) -o $@ $< $(CFLAGS) $(LDFLAGS) $(INCS) $(LIBS)

.PHONY: clean
clean: 
	$(RM) -rf $(EXEC) $(OBJS)
