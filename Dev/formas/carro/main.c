#include <GL/gl.h>
#include <math.h>
#include <GL/glut.h>

// Apenas para fins de testes
#include <stdio.h> 
#include <stdbool.h>

#define DOUBLE_PI 2*M_PI

bool sentido_carro = false;

int frameNumber=0;
int carroNumber=0;

int circle_points = 100;

float squareX = 0.0f; // Square's X position
float speedX = 0.01f; // Speed of movement
int sentido = 1.0;

float dark_green[3] = {1.0/255.0, 50.0/255.0, 32.0/255.0};
float light_blue[3] = {173.0/255.0, 216.0/255.0, 230.0/255.0};
float light_red[3] = {255.0/255.0, 71.0/255.0, 76.0/255.0};
float light_black[3] = {39.0/255.0, 39.0/255.0, 39.0/255.0};
float white[3] = {1.0, 1.0, 1.0};

void init(void);
void display(void);
void doFrame(int v);
void keyboard(unsigned char key, int x, int y);

void circulo(int nPonto);
void circulo_fechado(int nPonto);
void carro(void);
void roda(void);
void quadrado(void);



void init(void)
{
  // define a cor de background da janela
  glClearColor(light_blue[0], light_blue[1], light_blue[2], 1.0);

  // define o sistema de visualização - tipo de projeção
  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();
  glOrtho (-10, 10, -4, 10, -10, 10);
  //glOrtho (-50, 50, -4, 10, -10, 10); // Apenas para fins de testes -> Vemos uma imagem maior assim e podemos diagnosticar que o carro estava realmente saindo dos limites da visualização 

}

void circulo(int nPonto) {
	int raio = 1;
	glBegin(GL_LINE_LOOP);
		for (int i=0; i<nPonto;i++) {
			double angle = DOUBLE_PI * i / nPonto;
			glVertex3f(raio * cos(angle),
				   raio * sin(angle),
			   	   0);
		}
	glEnd();
}

void circulo_fechado(int nPonto) {
	int raio = 1;
	glBegin(GL_POLYGON);
		for (int i=0; i<nPonto;i++) {
			double angle = DOUBLE_PI * i / nPonto;
			glVertex3f(raio * cos(angle),
				   raio * sin(angle),
			   	   0);
		}
	glEnd();
}

void roda() {
	
	int num_of_lines_wheel = 12;

	glColor3f(0.4, 0.4, 0.4);
	circulo_fechado(circle_points);

	glColor3f(0, 0, 0);
	glLineWidth(3);
	circulo(circle_points);

	glBegin(GL_LINES);
	for (int i=0; i<num_of_lines_wheel; i++) {
		double angle = DOUBLE_PI * i / num_of_lines_wheel;
		glVertex3f(0, 0, 0);
		glVertex3f(cos(angle),sin(angle), 0);
	}
	glEnd();
}

void quadrado()
{
	  glBegin(GL_POLYGON);
	    glVertex3f(-1, -1, 0);
	    glVertex3f(1, -1, 0);
	    glVertex3f(1, 1, 0);
	    glVertex3f(-1, 1, 0);
	  glEnd();
}

void carro() {

 // roda da frente
  glPushMatrix();
  glTranslated(-1.5, -0.4, 0);
  glScaled(0.5, 0.5, 1);
  glRotated(carroNumber*-1, 0, 0, 1);
  roda();
  glPopMatrix();

  // roda de trás
  glPushMatrix();
  glTranslated(-3.5, -0.4, 0);
  glScaled(0.5, 0.5, 1);
  glRotated(carroNumber*-1, 0, 0, 1);
  roda();
  glPopMatrix();

  // parte maior do carro
  glColor3f(1, 0, 0);
  glPushMatrix();
  glTranslated(-2.5, 0.4, 0);
  glScaled(1.5, 0.5, 1);
  quadrado();
  glPopMatrix();

  // parte menor do carro
  glColor3f(1, 0, 0);
  glPushMatrix();
  glTranslated(-3.0, 1.0, 0);
  glScaled(0.5, 0.25, 1);
  quadrado();
  glPopMatrix();


}

void display() {

  // Limpa a janela, colocando na tela a cor definida pela função glClearColor
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);

  // Animação e posicionamento do carro
  glPushMatrix();
  glTranslated(-5.8 + (float) carroNumber/20, -1, 0);
  carro();
  glPopMatrix();

  glutSwapBuffers();
  //glFlush();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'a':
			printf("a foi pressionada\nMovimento para a esquerda\n");
			sentido_carro = true;
			break;
		case 'd':
			printf("d foi pressionada\nMovimento para a direita\n");
			sentido_carro= false;
			break;
	}
	display();
}

void doFrame(int v) {
	frameNumber++;
	if (sentido_carro) {
		carroNumber--;
	} else {
		carroNumber++;
	}
	glutPostRedisplay();
	glutTimerFunc(20, doFrame, 0);
}

int main(int argc, char** argv)
{

  glutInit(&argc, argv);

  glutInitDisplayMode (GLUT_DOUBLE);

  glutInitWindowSize (800, 800);
  glutInitWindowPosition (200, 200);

  glutCreateWindow ("Carro na estrada");

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(20, doFrame, 0);

  glutMainLoop();

  return 0;

}

