/**********************************************************************/
/*Aluno: Lucas dos Santos Barboza                                     */
/*Matricula: 20192045050420                                           */
/*Avaliação 04: Trabalho Final                                        */
/*04.505.23 - 2024.2 - Prof. Daniel Ferreira                          */
/*Compilador:GCC                                                      */
/**********************************************************************/

#define INPUT_DIR "./images/"
#define OUTPUT_DIR "./images_modificadas/"

struct pgm{
	int tipo;
	int c;
	int r;
	int mv;
	unsigned char *pData;
};

void readPGMImage(struct pgm *, char *);
void viewPGMImage(struct pgm *);
void writePGMImage(struct pgm *, char *);