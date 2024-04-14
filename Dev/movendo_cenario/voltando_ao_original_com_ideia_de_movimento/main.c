// Para rodar pela linha de comando: gcc nome_do_arquivo.c -lglut -lGLU -lGL -lm
#include <GL/glut.h>


// Para testes
#include <stdio.h>
#include <stdbool.h>

int frameNumber = 0; // Frame number geral 

int frameNumberCenario = 0; // Frame number para cenário

bool cenario_direita=false;
bool cenario_esquerda=false;



float dark_green[3] = {1.0/255.0, 50.0/255.0, 32.0/255.0};
float light_blue[3] = {173.0/255.0, 216.0/255.0, 230.0/255.0};

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


void display() {

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);

  // Parte verde
  glColor3fv(dark_green); 
  glBegin(GL_POLYGON);
  	glVertex3f(-10.f, 4.0f, 0.0f);
	glVertex3f(10.0f, 4.0f, 0.0f);
	glVertex3f(10.0f, -4.0f, 0.0f);
	glVertex3f(-10.0f, -4.0f, 0.0f);
  glEnd();

  // Parte Azul
  glColor3fv(light_blue); 
  glBegin(GL_POLYGON);
  	glVertex3f(10.f, -4.0f, 0.0f); // parte superior esquerda
	glVertex3f(20.0f, -4.0f, 0.0f); // parte superior direita
	glVertex3f(20.0f, -12.0f, 0.0f); // parte inferior direita
	glVertex3f(10.0f, -12.0f, 0.0f); // parte inferior esquerda
  glEnd();

  // Parte verde denovo, irá dar a impressão de continuidade
  glPushMatrix();
  glColor3fv(dark_green);
  glTranslated(30, 0, 0);
  glBegin(GL_POLYGON);
  	glVertex3f(-10.f, 4.0f, 0.0f);
	glVertex3f(10.0f, 4.0f, 0.0f);
	glVertex3f(10.0f, -4.0f, 0.0f);
	glVertex3f(-10.0f, -4.0f, 0.0f);
  glEnd();
  glPopMatrix();

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
		if (frameNumberCenario >= 30) {
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

		// TODO: Teste que irá possivelmente resetar algum valor para retornamos ao "começo"
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

