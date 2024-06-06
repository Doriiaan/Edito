CC= gcc
CFLAGS= -std=c99 -Wall -Wextra -Werror -pedantic-errors
LDFLAGS= -lncurses
SRC= $(wildcard *.c) # wildcard permit use of * in Makefile
OBJ= $(SRC:.c=.o) # replace .c with .o, because .o do not exist at the beginning
EXEC=edito


# first target so do not need to explicit make all
all : $(EXEC) 

$(EXEC) : $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# %.o : %.c decomposose each .o in a target
%.o : %.c
	$(CC) -c $< $(CFLAGS) 

main.o : util.h


.PHONY : clean
clean : 
	rm *.o $(EXEC)
