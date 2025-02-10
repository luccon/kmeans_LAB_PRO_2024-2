# Regra padrão
all: TF.o prosimages.o kmeans.o
	gcc TF.o prosimages.o kmeans.o -o programa

# Compilar TF.o
TF.o: TF.c 
	gcc -c TF.c -o TF.o

# Compilar prosimages.o
prosimages.o: prosimages.c 
	gcc -c prosimages.c -o prosimages.o

kmeans.o: kmeans.c
	gcc -c kmeans.c -o kmeans.o

# Limpeza de arquivos objetos e o executável
clean:
	rm -f TF.o prosimages.o programaclear