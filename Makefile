all: TF.o prosimages.o kmeans.o
	gcc TF.o prosimages.o kmeans.o -o programa

TF.o: TF.c 
	gcc -c TF.c -o TF.o

prosimages.o: prosimages.c 
	gcc -c prosimages.c -o prosimages.o

kmeans.o: kmeans.c
	gcc -c kmeans.c -o kmeans.o

clean:
	rm -f TF.o prosimages.o programaclear