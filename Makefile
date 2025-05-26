EXEC = a.out

CC = gcc
CFLAGS = -Wall
SRC = tema.c fisier_comun.h fisier_task1.h fisier_task2.h fisier_task3.h fisier_task4.h 

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXEC)

clean:
	rm -f $(EXEC)