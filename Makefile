# Nome do executável principal
EXEC = jogo

# Nome do executável de testes
TEST_EXEC = testes

# Ficheiros fonte
SRC = main.c parte1.c
TEST_SRC = testesparte1.c parte1.c

# Ficheiros de cabeçalho
HEADERS = parte1.h

# Flags do compilador
CFLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g
LDFLAGS = -lcunit -fsanitize=address

.PHONY: all clean clean-all test

all: $(EXEC) $(TEST_EXEC)

# Compila o jogo principal
$(EXEC): $(SRC) $(HEADERS)
	@gcc $(CFLAGS) $(SRC) -o $(EXEC) 

# Compila os testes
$(TEST_EXEC): $(TEST_SRC) $(HEADERS)
	@gcc $(CFLAGS) $(TEST_SRC) -o $(TEST_EXEC) $(LDFLAGS)

# Executa os testes
testar: $(TEST_EXEC)
	@./$(TEST_EXEC)

# Limpa os ficheiros gerados
clean:
	@rm -f $(EXEC) $(TEST_EXEC)

# Limpa tudo, incluindo ficheiros temporários
clean-all: clean
	@rm -f *.o
