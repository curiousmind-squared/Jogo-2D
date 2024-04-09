// Para rodar pela linha de comando: gcc nome_do_arquivo.c -lglut -lGLU -lGL
#include <GL/glut.h>


float dark_green[3] = {1.0/255.0, 50.0/255.0, 32.0/255.0};

void init(void);
void display(void);

void init(void)
{
  glClearColor(1.0, 1.0, 1.0, 1.0);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho (-10, 10, -10, 10, -10, 10); // TODO: Resolve como projetar a tela da forma correta, sem deixar espaços
}


void display() {

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);

  // Montanha verde
  glColor3fv(dark_green); 
  glBegin(GL_POLYGON);
  	glVertex3f(-10.f, 4.0f, 0.0f);
	glVertex3f(-8.0f, 4.0f, 0.0f);
	glVertex3f(-7.0f, 4.0f, 0.0f);
	glVertex3f(-1.0f, 7.0f, 0.0f);
	glVertex3f(6.0f, 4.0f, 0.0f);
	glVertex3f(8.0f, 5.0f, 0.0f);
	glVertex3f(10.0f, 4.0f, 0.0f);
	glVertex3f(10.0f, -4.0f, 0.0f);
	glVertex3f(-10.0f, -4.0f, 0.0f);
  glEnd();

  glFlush();
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

  glutInitWindowSize (800, 800);
  glutInitWindowPosition (200, 200);

  glutCreateWindow ("Carro na estrada");

  init();

  glutDisplayFunc(display);

  glutMainLoop();

  return 0;

}
