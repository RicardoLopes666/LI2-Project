# Nome do executável principal
EXEC = jogo
# Nome do executável de testes
TEST_EXEC = testes

# Ficheiros fonte
SRC = main.c parte1/parte1.c parte2/parte2.c parte3/parte3.c parte4/parte4.c
TEST_SRC = testesMain.c parte1/testesparte1.c parte2/testesparte2.c parte3/testesparte3.c parte4/testesparte4.c 
LIB_SRC = parte1/parte1.c parte2/parte2.c parte3/parte3.c parte4/parte4.c

# Headers
HEADERS = parte1/parte1.h parte2/parte2.h parte3/parte3.h parte4/parte4.h colors.h
TEST_HEADERS = parte1/testesparte1.h parte2/testesparte2.h parte3/testesparte3.h parte4/testesparte4.h

# Flags de compilação e ligação
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g
COVERAGE_FLAGS = --coverage
LDFLAGS = -fsanitize=address -lcunit 
TEST_LDFLAGS = -lgcov

.PHONY: all clean clean-all testar coverage

all: $(EXEC) $(TEST_EXEC)

# Rodar o jogo
$(EXEC): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

# Obter os paths para os objetos dos testes
TEST_OBJ = $(TEST_SRC:.c=.o)
LIB_OBJ = $(LIB_SRC:.c=.o)

# Regra para compilar os arquivos de teste com cobertura
%.o: %.c
	$(CC) $(CFLAGS) $(COVERAGE_FLAGS) -c $< -o $@ 

# Gera binário de testes
$(TEST_EXEC): $(TEST_OBJ) $(LIB_OBJ)
	$(CC) $^ -o $@ $(LDFLAGS) $(TEST_LDFLAGS)

# Executa os testes
testar: $(TEST_EXEC)
	./$(TEST_EXEC)

# Testar a cobertura do código
coverage: testar
	gcov -o parte1 parte1/parte1.c
	gcov -o parte2 parte2/parte2.c
	gcov -o parte3 parte3/parte3.c
	gcov -o parte4 parte4/parte4.c

# Limpeza básica
clean:
	rm -rf $(EXEC) $(TEST_EXEC) $(TEST_OBJ) $(LIB_OBJ)

# Limpeza total (inclui coverage)
clean-all: clean
	find . -type f \( -name '*.gcda' -o -name '*.gcno' -o -name '*.gcov' \) -delete