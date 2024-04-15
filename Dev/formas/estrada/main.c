// Para rodar pela linha de comando: gcc nome_do_arquivo.c -lglut -lGLU -lGL -lm
#include <GL/glut.h>
#include <stdbool.h>


// Para testes
#include <stdio.h>


// Váriaveis relacionadas ao Frame
int frameNumber = 0; // Frame number geral 
int frameNumberCenario = 0; // Frame number para cenário

// Váriaveis booleanas de sistema
bool cenario_direita=false;
bool cenario_esquerda=false;


// Cores
float dark_green[3] = {1.0/255.0, 50.0/255.0, 32.0/255.0};
float light_blue[3] = {173.0/255.0, 216.0/255.0, 230.0/255.0};
float light_black[3] = {39.0/255.0, 39.0/255.0, 39.0/255.0};
float white[3] = {1.0, 1.0, 1.0};

// Funções de desenho de formas
void quadrado();
void asfalto(int desloc_x);
void placas_de_asfalto();

// Funções do OpenGl
void init(void);
void display(void);
void doFrame(int v);
void keyboard(unsigned char key, int x, int y);

void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  
  glOrtho(-10, 10, -10, 10, -10, 10); // Assim vemos o cenário do usuário

  //glOrtho (-100, 100, -10, 10, -10, 10); // Assim vemos o cenário todo do jogo 
}

void quadrado()
{
/*
 * Unidade básica de um quadrao
 */
  glBegin(GL_POLYGON);
    glVertex3f(-1, -1, 0);
    glVertex3f(1, -1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(-1, 1, 0);
  glEnd();
}

void asfalto(int desloc_x) 
{
  /*
   * Desenhamos uma placa de asfalto
   */

  // Parte preta do asfalto
  glColor3fv(light_black);
  glPushMatrix();
  glTranslated(desloc_x, -3.75, 0);
  glScaled(10, 1.5, 1);
  quadrado();	
  glPopMatrix();

  // linha branca do asfalto
  glColor3fv(white);
  glPushMatrix();
  glTranslated(desloc_x, -3.5, 0);
  glScaled(10, 0.2, 1);
  quadrado();
  glPopMatrix();


}

void placas_de_asfalto() 
{
  /* 
   * Desenhamos as placas de asfalto ao longo do cenário 
   */
  int num = 11; // Número de peças de asfalto que teremos
  for (int i=0; i<num; i++) {
     if (i == 0) {
	asfalto(0);
     } else {
	asfalto(i*10);
	asfalto(i*10*-1);
     }
  }
  
	
}

void display() {

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);

  placas_de_asfalto();
  
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'd':
			printf("Você apertou d\tCenário para a direita\n");
			cenario_direita=true;
			break;
		case 'a':
			printf("Você apertou a\tCenário para a esquerda\n");
			cenario_esquerda=true;
			break;
		case 'q':
			printf("Você aprtou q\tEstá vendo o cenário todo\n");
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-100, 100, -10, 10, -10, 10);
			break;

	}
	display();
}

void doFrame(int v) {
	frameNumber++;
	if (cenario_direita) {
		frameNumberCenario += 5;
		glMatrixMode (GL_PROJECTION);
	 	glLoadIdentity();
		
		printf("%d\n", frameNumberCenario);
		if (frameNumberCenario >= 100) {
			printf("Passamos do limite de visualização do usuário, devemos dar a ideia de voltar ao começo para termos ilusão de cenário infinito\n");
			glOrtho(-10, 10, -10, 10, -10, 10);			
			frameNumberCenario = 0;
			
		} else {
			glOrtho(-10+(frameNumberCenario), 10+frameNumberCenario, -10, 10, -10, 10);
		}

		cenario_direita=false;
	}
	if (cenario_esquerda) {
		frameNumberCenario -= 5;

		
		//printf("%d\n", frameNumberCenario);
		
		glMatrixMode (GL_PROJECTION);
 		glLoadIdentity();
		glOrtho(-10+frameNumberCenario,10+frameNumberCenario, -10, 10, -10, 10);

		//printf("FrameNumber e Tamanho do cenário em x: %d\t %d até %d\n",frameNumberCenario, -10+frameNumberCenario, 10+frameNumberCenario);

		cenario_esquerda=false;
		
	}

	glutPostRedisplay();
	glutTimerFunc(20, doFrame, 0);
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

  glutInitWindowSize (800, 800);
  glutInitWindowPosition (200, 200);

  glutCreateWindow ("Testando visualização do cenário");

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(20, doFrame, 0);

  glutMainLoop();

  return 0;

}

