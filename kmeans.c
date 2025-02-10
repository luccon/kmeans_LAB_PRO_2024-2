#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

struct pgm {
    int tipo;
    int c;
    int r;
    int mv;
    unsigned char *pData;
};

struct IntensidadeFrequencia {
    unsigned char intensidade;
    int frequencia;
};

// Função para comparar intensidades para qsort
int compararFrequencia(const void *a, const void *b) {
    return ((struct IntensidadeFrequencia*)b)->frequencia - ((struct IntensidadeFrequencia*)a)->frequencia;
}

// Inicializar centroides automaticamente
void inicializarCentroides(struct pgm *pio, unsigned char *centroides, int k) {
    struct IntensidadeFrequencia vf[256] = {0};

    // Contar frequência das intensidades
    for (int i = 0; i < pio->r * pio->c; i++) {
        vf[pio->pData[i]].intensidade = pio->pData[i];
        vf[pio->pData[i]].frequencia++;
    }

    // Ordenar por frequência (mais eficiente que Bubble Sort)
    qsort(vf, 256, sizeof(struct IntensidadeFrequencia), compararFrequencia);

    // Selecionar os K valores mais frequentes como centroides
    for (int i = 0; i < k; i++) {
        centroides[i] = vf[i].intensidade;
    }
}

// Função para calcular distância entre pixels
unsigned char distancia(unsigned char p1, unsigned char p2) {
    return abs(p1 - p2);
}

// K-means com detecção de convergência automática
void kmeans(struct pgm *img) {
    int n_pixels = img->r * img->c;
    
    // Definir K automaticamente como sqrt(número de pixels)
    int k = n_pixels / 1000;
    if (k < 2) k = 2;  // Garantir pelo menos 2 clusters

    unsigned char *centroides = (unsigned char *) malloc(k * sizeof(unsigned char));
    inicializarCentroides(img, centroides, k);

    int *labels = (int *) malloc(n_pixels * sizeof(int));
    int *soma_centroides = (int *) calloc(k, sizeof(int));
    int *cluster_sizes = (int *) calloc(k, sizeof(int));

    int max_iteracoes = 50;  // Evita loops infinitos
    bool convergiu = false;

    for (int iter = 0; iter < max_iteracoes && !convergiu; iter++) {
        convergiu = true;  // Assume convergência até provar o contrário

        // Atribuir cada pixel ao centróide mais próximo
        for (int i = 0; i < n_pixels; i++) {
            unsigned char pixel = img->pData[i];
            int melhor_cluster = 0;
            int menor_distancia = distancia(pixel, centroides[0]);

            for (int j = 1; j < k; j++) {
                int dist = distancia(pixel, centroides[j]);
                if (dist < menor_distancia) {
                    menor_distancia = dist;
                    melhor_cluster = j;
                }
            }
            labels[i] = melhor_cluster;
        }

        // Reinicializar somas e contadores
        memset(soma_centroides, 0, k * sizeof(int));
        memset(cluster_sizes, 0, k * sizeof(int));

        // Somar valores dos pixels para cada cluster
        for (int i = 0; i < n_pixels; i++) {
            int cluster = labels[i];
            soma_centroides[cluster] += img->pData[i];
            cluster_sizes[cluster]++;
        }

        // Atualizar os centroides e verificar convergência
        for (int i = 0; i < k; i++) {
            unsigned char novo_centroide = (cluster_sizes[i] > 0) ? soma_centroides[i] / cluster_sizes[i] : centroides[rand() % k];

            if (novo_centroide != centroides[i]) {
                convergiu = false;  // Ainda há mudanças
            }

            centroides[i] = novo_centroide;
        }
    }

    // Atualizar a imagem com os novos valores dos clusters
    for (int i = 0; i < n_pixels; i++) {
        img->pData[i] = centroides[labels[i]];
    }

    free(centroides);
    free(labels);
    free(soma_centroides);
    free(cluster_sizes);
}
