FLAGS=-O3
LIBS=-lm -fopenmp

CC=gcc

RM=rm -f

EXEC=tsp

# Regra padrão
all: $(EXEC)

# Compilação e linkagem
$(EXEC):
	$(CC) $(FLAGS) $(EXEC).c -c -o $(EXEC).o $(LIBS)
	$(CC) $(FLAGS) $(EXEC).o -o $(EXEC) $(LIBS)

# Rodar o programa
run:
	./$(EXEC)

# Limpar arquivos gerados
clean:
	$(RM) $(EXEC).o $(EXEC)
