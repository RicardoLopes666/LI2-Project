# Nome do executável principal
EXEC = programa

# Nome do executável de testes
TEST_EXEC = testesparte1

# Ficheiros fonte
SRC = parte1.c main.c
TEST_SRC = testesparte1.c parte1.c

# Ficheiros de cabeçalho
HEADERS = parte1.h

# Flags do compilador
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lcunit

# Alvo padrão (compila tudo)
all: $(EXEC) $(TEST_EXEC)

# Compila o programa principal
$(EXEC): main.c parte1.c $(HEADERS)
	@gcc $(CFLAGS) -o $(EXEC) main.c parte1.c

# Compila o programa de testes
$(TEST_EXEC): testesparte1.c parte1.c $(HEADERS)
	@gcc $(CFLAGS) -o $(TEST_EXEC) testesparte1.c parte1.c $(LDFLAGS)

# Executa os testes
test: $(TEST_EXEC)
	@./$(TEST_EXEC)

# Limpa os ficheiros gerados
clean:
	@rm -f $(EXEC) $(TEST_EXEC)

# Limpa tudo, incluindo ficheiros temporários
clean-all: clean
	@rm -f *.o