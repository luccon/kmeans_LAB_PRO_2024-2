/*************************************************************************/
/*Alunos:Lucas dos Santos Barboza, Thales Lucas Lima e Gomes, Felipe     */
/*Matheus Vasconcelos Oliveira e Francisco Cassimiro de Sousa Albuquerque*/
/*Matricula: 20192045050420                                              */
/*Avaliação 04: Trabalho Final                                           */
/*04.505.23 - 2024.2 - Prof. Daniel Ferreira                             */
/*Compilador:GCC                                                         */
/*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct pgm {
    int tipo;
    int c;
    int r;
    int mv;
    unsigned char *pData;
};
// Struct para armazenar a frequencia das intensidades dos bits das imagens
struct IntensidadeFrequencia {
    unsigned char intensidade;
    int frequencia;
};
// Função para definir os primeiros valores dos Clusters
void inicializarCentroides(struct pgm *pio, unsigned char *centroides, int k) {
    unsigned char *vx = (unsigned char*) malloc(pio->r * pio->c * sizeof(unsigned char));
    struct IntensidadeFrequencia *vf = (struct IntensidadeFrequencia*) malloc(256 * sizeof(struct IntensidadeFrequencia));
    
    // Copiar os valores dos pixels
    for (int i = 0; i < pio->r * pio->c; i++) {
        vx[i] = pio->pData[i];
    }
    
    // Inicializar o vetor de frequências
    for (int i = 0; i < 256; i++) {
        vf[i].intensidade = i;
        vf[i].frequencia = 0;
    }
    
    // Contar a frequência de cada intensidade
    for (int i = 0; i < pio->r * pio->c; i++) {
        vf[vx[i]].frequencia++;
    }
    
    // Ordenar as intensidades por frequência
    for (int i = 0; i < 255; i++) {
        for (int j = 0; j < 255 - i; j++) {
            if (vf[j].frequencia < vf[j + 1].frequencia) {
                struct IntensidadeFrequencia temp = vf[j];
                *(vf + j) = *(vf + j + 1);
                *(vf + j + 1) = temp;
            }
        }
    }
    
    // Selecionar os k valores mais frequentes como centroides
    for (int i = 0; i < k; i++) {
        *(centroides+i) = vf[i].intensidade;
    }
    
    free(vx);
    free(vf);
}


// Função para calcular a distância entre dois pixels
unsigned char distancia(unsigned char p1, unsigned char p2) {
    return abs(p1 - p2);
}
// Função para ver as mudanças dos centroides
void printCentroides(unsigned char *centroides, int k) {
    printf("Centroides atuais: ");
    for (int i = 0; i < k; i++) {
        printf("%d ", *(centroides+i));
    }
    printf("\n");
}


// Função para realizar o algoritmo K-means
void kmeans(struct pgm *img, int k, unsigned char *centroides) {
    int n_pixels = img->r * img->c;
    int *i_clusters = (int *) malloc(n_pixels * sizeof(int));
    int *soma_centroides = (int *) malloc(k * sizeof(int)); 
    int *tamanho_clusters = (int *) malloc(k * sizeof(int));

    inicializarCentroides(img,centroides,k);

    int max_iteracoes = 10; 
    for (int iter = 0; iter < max_iteracoes; iter++) {
        // Atribuir cada pixel ao centróide mais próximo
        for (int i = 0; i < n_pixels; i++) {
            unsigned char pixel = img->pData[i];
            int melhor_cluster = 0;
            int menor_distancia = abs(pixel - *centroides);

            for (int j = 1; j < k; j++) {
                int dist = abs(pixel - *(centroides+j));
                if (dist < menor_distancia) {
                    menor_distancia = dist;
                    melhor_cluster = j;
                }
            }
            *(i_clusters+i) = melhor_cluster;
        }

        // Reinicializar somas e contadores
        for (int i = 0; i < k; i++) {
            *(soma_centroides+i) = 0;
            *(tamanho_clusters+i) = 0;
        }

        // Somar valores dos pixels para cada cluster
        for (int i = 0; i < n_pixels; i++) {
            int cluster = *(i_clusters+i);
            soma_centroides[cluster] += img->pData[i];
            tamanho_clusters[cluster]++;
        }

        // Atualizar os centroides
        for (int i = 0; i < k; i++) {
            if (*(tamanho_clusters+i) > 0) {
                *(centroides+i) = *(soma_centroides+i) / *(tamanho_clusters+i);  // Média dos pixels
            } else {
                *(centroides+i) = rand() % 256; // Evitar centroides mortos
            }
        }

        printCentroides(centroides, k); // Checar os centroides
    }

    // Atualizar a imagem com os novos valores dos clusters
    for (int i = 0; i < n_pixels; i++) {
        int cluster = *(i_clusters+i);
        img->pData[i] = centroides[cluster];
    }

    free(i_clusters);
    free(soma_centroides);
    free(tamanho_clusters);
}
