// Para rodar pela linha de comando: gcc nome_do_arquivo.c -lglut -lGLU -lGL
#include <GL/glut.h>


// Para testes
#include <stdio.h>
#include <stdbool.h>

int frameNumber = 0;
int frameNumberGame = 0;

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
  glOrtho(-10, 10, -10, 10, -10, 10); // Assim não aparece
  //glOrtho (-30, 30, -10, 10, -10, 10); // Assim aparece o quadrado azul 
}


void display() {

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);

  // Montanha verde
  glColor3fv(dark_green); 
  glBegin(GL_POLYGON);
  	glVertex3f(-10.f, 4.0f, 0.0f);
	glVertex3f(10.0f, 4.0f, 0.0f);
	glVertex3f(10.0f, -4.0f, 0.0f);
	glVertex3f(-10.0f, -4.0f, 0.0f);
  glEnd();

  glColor3fv(light_blue); 
  glBegin(GL_POLYGON);
  	glVertex3f(10.f, -4.0f, 0.0f); // parte superior esquerda
	glVertex3f(20.0f, -4.0f, 0.0f); // parte superior direita
	glVertex3f(20.0f, -12.0f, 0.0f); // parte inferior direita
	glVertex3f(10.0f, -12.0f, 0.0f); // parte inferior esquerda
  glEnd();


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

	}
	display();
}

void doFrame(int v) {
	frameNumber++;
	if (cenario_direita) {
		frameNumberGame += 5;
		glMatrixMode (GL_PROJECTION);
 		glLoadIdentity();
		glOrtho(-10+(frameNumberGame), 10+frameNumberGame, -10, 10, -10, 10);
		cenario_direita=false;
	}
	if (cenario_esquerda) {
		frameNumberGame -= 5;
		glMatrixMode (GL_PROJECTION);
 		glLoadIdentity();
		glOrtho(-10+frameNumberGame,10+frameNumberGame, -10, 10, -10, 10);
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
