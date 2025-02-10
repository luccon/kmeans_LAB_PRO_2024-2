/**********************************************************************/
/*Aluno: Lucas dos Santos Barboza                                     */
/*Matricula: 20192045050420                                           */
/*Avaliação 04: Trabalho Final                                        */
/*04.505.23 - 2024.2 - Prof. Daniel Ferreira                          */
/*Compilador:GCC                                                      */
/**********************************************************************/

#include <dirent.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "./lib/prosimages.h"
#include "./lib/kmeans.h"


#define MAX_FILENAME 512
#define INPUT_DIR "./images/"
#define OUTPUT_DIR "./images_modificadas/"

int main(){
    struct pgm img;
    int k = 3;

	clock_t begin, end;
	double time_total=0;

	// Criar diretório de saída "images_modificadas", se não existir
    mkdir(OUTPUT_DIR, 0777);

	begin = clock();
    // Inicio da medição do tempo
    
	
	DIR *d;
    struct dirent *dir;
    d = opendir(INPUT_DIR);
    if(!d){
        printf("Erro ao abrir diretório de entrada");
        exit(1);
    }

    if (d){
		
        while ((dir = readdir(d)) != NULL){
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
				continue;  // Pula arquivos especiais "." e ".."
			}
            
            char filepath[MAX_FILENAME];
        	snprintf(filepath, sizeof(filepath), "./images/%s", dir->d_name);

			printf("Processando: %s\n", filepath);
			
			readPGMImage(&img,filepath);

            viewPGMImage(&img);

            kmeans(&img);

			writePGMImage(&img,dir->d_name);
				
			free(img.pData);
        }
		
        closedir(d);
    }
    // Fim da medição do tempo           
    end = clock();

	time_total = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("Tempo Total: %lf\n",time_total);
	return 0;
}