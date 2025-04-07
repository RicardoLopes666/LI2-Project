# Nome do executável principal
EXEC = jogo

# Nome do executável de testes
TEST_EXEC = testes

# Ficheiros fonte
SRC = main.c parte1/parte1.c
TEST_SRC = parte1/testesparte1.c parte1/parte1.c

# Ficheiros de cabeçalho
HEADERS = parte1/parte1.h

# Flags do compilador
CFLAGS = -Wall -Wextra -pedantic -O1 -fsanitize=address -fno-omit-frame-pointer -g 
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -lcunit -fsanitize=address
TEST_LDFLAGS = $(LDFLAGS) -lgcov

.PHONY: all clean clean-all testar coverage

all: $(EXEC) $(TEST_EXEC)

# Compila o jogo principal
$(EXEC): $(SRC) $(HEADERS)
	@gcc $(CFLAGS) $(SRC) -o $(EXEC)

# Compila os testes com cobertura
$(TEST_EXEC): $(TEST_SRC) $(HEADERS)
	@gcc $(CFLAGS) $(COVERAGE_FLAGS) $(TEST_SRC) -o $(TEST_EXEC) $(TEST_LDFLAGS)

# Executa os testes
testar: $(TEST_EXEC)
	@./$(TEST_EXEC)

# Informação sobre cobertura com gcov
coverage: testar
	@gcov parte1.c

# Limpa os ficheiros gerados
clean:
	@rm -f $(EXEC) $(TEST_EXEC)

# Limpa tudo, incluindo ficheiros temporários e cobertura
clean-all: clean
	@rm -f *.o *.gcno *.gcda *.gcov
