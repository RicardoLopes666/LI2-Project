FLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g


CC = gcc
RESOURCES = parte1.c  # parte2.c parte3.c parte4.c 
OBJECTS = $(RESOURCES:.c=.o)
TARGET= jogo

$(TARGET) : $(OBJECTS) 
	$(CC) $(FLAGS) -o $@ $^


%.o : %.c
	$(CC) $(FLAGS) -o $@ -c $^ 



