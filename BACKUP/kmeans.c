#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct pgm{
	int tipo;
	int c;
	int r;
	int mv;
	unsigned char *pData;
};

// Função para inicializar os centróides aleatoriamente
void inicializarCentroides(unsigned char *pixels, unsigned char *centroides, int k, int n_pixels) {
    srand(time(NULL));

    for (int i = 0; i < k; i++) {
        int indice = rand() % n_pixels;
        centroides[i] = pixels[indice];
    }
}
// Função para calcular a distância (diferença de valores de pixel)
unsigned char distancia(unsigned char p1, unsigned char p2) {
    return abs(p1 - p2);
}

// Função para realizar o algoritmo K-means
void kmeans(struct pgm *pio, int k, unsigned char *centroides) {
    int n_pixels = pio->r * pio->c;
    int *labels = (int*) malloc(n_pixels * sizeof(int));

    unsigned char *pixels = pio->pData;
    unsigned char *novos_centroides = (unsigned char*) malloc(k * sizeof(unsigned char));
    int *cluster_sizes = (int*) malloc(k * sizeof(int));

    // Inicializar centroides aleatoriamente
    inicializarCentroides(pixels, centroides, k, n_pixels);

    int max_iteracoes = 10;

    for (int iter = 0; iter < max_iteracoes; iter++) {
        // Atribuir cada pixel ao centróide mais próximo
        for (int i = 0; i < n_pixels; i++) {
            unsigned char pixel = pixels[i];
            int melhor_cluster = 0;
            unsigned char melhor_distancia = distancia(pixel, centroides[0]);

            for (int j = 1; j < k; j++) {
                unsigned char dist = distancia(pixel, centroides[j]);
                if (dist < melhor_distancia) {
                    melhor_distancia = dist;
                    melhor_cluster = j;
                }
            }
            labels[i] = melhor_cluster;
        }

        // Recalcular os centroides
        // Recalcular os centroides
    for (int i = 0; i < k; i++) {
        cluster_sizes[i] = 0; // Zerar o contador de elementos no cluster
        novos_centroides[i] = 0; // Zerar a soma dos pixels para o cálculo do centróide
    }

    // Somar os valores dos pixels para cada cluster
    for (int i = 0; i < n_pixels; i++) {
        int cluster = labels[i];
        novos_centroides[cluster] += pixels[i]; // Somar o valor do pixel no cluster correspondente
        cluster_sizes[cluster]++; // Incrementar o número de pixels no cluster
    }

    // Atualizar os centroides
    for (int i = 0; i < k; i++) {
        if (cluster_sizes[i] > 0) {
            centroides[i] = novos_centroides[i] / cluster_sizes[i]; // Calcular o novo centróide como a média
        } else {
            // Caso um cluster não tenha pixels atribuídos (o que pode ocorrer), definir o centróide para 0 (ou outra lógica)
            centroides[i] = 0;
        }
    }

    }

    // Atualizar a imagem com os novos valores dos centroides
    for (int i = 0; i < n_pixels; i++) {
        int cluster = labels[i];
        pixels[i] = centroides[cluster];
    }

    free(labels);
    free(novos_centroides);
    free(cluster_sizes);
}