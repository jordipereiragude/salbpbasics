OBJ = ../obj/main.o ../obj/hoffmann.o ../obj/io.o ../obj/utils.o ../obj/greedy.o ../obj/ls.o
TARGETS = ../program

all: $(TARGETS)

clean: 
	rm -f $(OBJ) ${TARGETS}

../obj/main.o: main.c
	gcc -c main.c -o ../obj/main.o 

../obj/hoffmann.o: hoffmann.c
	gcc -c hoffmann.c -o ../obj/hoffmann.o 

../obj/io.o: io.c
	gcc -c io.c -o ../obj/io.o 

../obj/utils.o: utils.c
	gcc -c utils.c -o ../obj/utils.o 

../obj/greedy.o: greedy.c
	gcc -c greedy.c -o ../obj/greedy.o 

../obj/ls.o: ls.c
	gcc -c ls.c -o ../obj/ls.o 

../program: $(OBJ)
	gcc $(OBJ) -o ../program

