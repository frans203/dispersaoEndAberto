# Compilador utilizado
COMP = gcc

# Linker utilizado
LINKER = gcc

# Op��es de compila��o
OPCOES_COMP = -c -std=c99 -Wall -g

# Op��es de liga��o
OPCOES_LINK = -lm

# Arquivos-objeto
OBJETOS = Leitura.o Registros.o TabelaCuco.o main.o

# Nome do arquivo execut�vel
EXEC = Secao07_05_03.exe

$(EXEC): $(OBJETOS)
	$(LINKER) $(OPCOES_LINK) $(OBJETOS) -o $(EXEC)

Leitura.o: Leitura.c Leitura.h
	$(COMP) $(OPCOES_COMP) Leitura.c -o Leitura.o

Registros.o: Registros.c Registros.h Macros.h Tipos.h
	$(COMP) $(OPCOES_COMP) Registros.c -o Registros.o

TabelaCuco.o: TabelaCuco.c TabelaCuco.h Macros.h Tipos.h
	$(COMP) $(OPCOES_COMP) TabelaCuco.c -o TabelaCuco.o

main.o: main.c Registros.h Macros.h Tipos.h
	$(COMP) $(OPCOES_COMP) main.c -o main.o
