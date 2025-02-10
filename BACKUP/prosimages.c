/**********************************************************************/
/*Aluno: Lucas dos Santos Barboza                                     */
/*Matricula: 20192045050420                                           */
/*Avaliação 04: Trabalho Final                                        */
/*04.505.23 - 2024.2 - Prof. Daniel Ferreira                          */
/*Compilador:GCC                                                      */
/**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#define INPUT_DIR "./images/"
#define OUTPUT_DIR "./images_modificadas/"
struct pgm{
	int tipo;
	int c;
	int r;
	int mv;
	unsigned char *pData;
};

void readPGMImage(struct pgm *pio, char *filename){

	FILE *fp;
	char ch;

	if (!(fp = fopen(filename,"rb"))){
		perror("Erro.");
		exit(1);
	}

	if ( (ch = getc(fp))!='P'){
		puts("A imagem fornecida não está no formato pgm");
		exit(2);
	}
	
	pio->tipo = getc(fp) - '0'; 
	
	fseek(fp,1, SEEK_CUR);

	while((ch=getc(fp))=='#'){
		while( (ch=getc(fp))!='\n');
	}

	fseek(fp,-1, SEEK_CUR);

	fscanf(fp, "%d %d",&pio->c,&pio->r);
	if (ferror(fp)){ 
		perror(NULL);
		exit(3);
	}	
	fscanf(fp, "%d",&pio->mv);
	fseek(fp,1, SEEK_CUR);

	pio->pData = (unsigned char*) malloc(pio->r * pio->c * sizeof(unsigned char));

	switch(pio->tipo){
		case 2:
			puts("Lendo imagem PGM (dados em texto)");
			for (int k=0; k < (pio->r * pio->c); k++){
				fscanf(fp, "%hhu", pio->pData+k);
			}
		break;	
		case 5:
			puts("Lendo imagem PGM (dados em binário)");
			fread(pio->pData,sizeof(unsigned char),pio->r * pio->c, fp);
		break;
		default:
			puts("Não está implementado");
			exit(5);
	}
	
	fclose(fp);

}

void writePGMImage(struct pgm *pio, char *filename){
	FILE *fp;
	char newFilename[256];  // Novo nome do arquivo

    // Gera um novo nome automaticamente, adicionando "_modificado.pgm"
    snprintf(newFilename, sizeof(newFilename), "%s/%s_modificado.pgm", OUTPUT_DIR, filename);

    // Abre o arquivo para escrita binária
    if (!(fp = fopen(newFilename, "wb"))) {
        perror("Erro ao abrir o arquivo para escrita.");
        exit(1);
    }

	fprintf(fp, "%s\n","P5");
	fprintf(fp, "%d %d\n",pio->c, pio->r);
	fprintf(fp, "%d\n", 255);

	fwrite(pio->pData, sizeof(unsigned char),pio->c * pio->r, fp);

	fclose(fp);
	printf("Imagem salva como: %s\n", newFilename);
}


void viewPGMImage(struct pgm *pio){
	printf("Tipo: P%d\n", pio->tipo);
    printf("Dimensões: [%d x %d]\n", pio->c, pio->r);
    printf("Max: %d\n", pio->mv);
}