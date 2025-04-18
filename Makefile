# Nome do executável principal
EXEC = jogo

# Nome do executável de testes
TEST_EXEC = testes

# Ficheiros fonte
SRC = main.c parte1/parte1.c parte2/parte2.c
TEST_SRC = testesMain.c parte1/testesparte1.c parte2/testesparte2.c
LIB_SRC = parte1/parte1.c parte2/parte2.c #sem o main.c para não existirem dois main nos testes


# Ficheiros de cabeçalho
HEADERS = parte1/parte1.h parte2/parte2.h
TEST_HEADERS = parte1/testesparte1.h parte2/testesparte2.h 

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
$(TEST_EXEC): $(TEST_SRC)  $(LIB_SRC) $(HEADERS) $(TEST_HEADERS)
	@gcc $(CFLAGS) $(COVERAGE_FLAGS) $(TEST_SRC) $(LIB_SRC) -o $(TEST_EXEC) $(TEST_LDFLAGS)

# Executa os testes
testar: $(TEST_EXEC)
	@./$(TEST_EXEC)

# Informação sobre cobertura com gcov
coverage: testar
	@gcov -o parte1/  parte1/parte1.c

# Limpa os ficheiros gerados
clean:
	@rm -f $(EXEC) $(TEST_EXEC)

# Limpa tudo, incluindo ficheiros temporários e cobertura
clean-all: clean
	@rm -f *.o *.gcno *.gcda *.gcov
