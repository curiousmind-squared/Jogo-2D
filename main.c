// Para rodar pela linha de comando: gcc nome_do_arquivo.c -lglut -lGLU -lGL -lm
#include <GL/glut.h>
#include <stdbool.h>

// Para testes
#include <stdio.h>

// Váriaveis relacionadas às dimensões da tela e do mundo
int view_desloc_x_begin = -10;
int view_desloc_x_end   =  10;


// Variáveis relacionadas ao Frame
int frameNumber        = 0; // Frame number geral 

// Variáveis booleanas de sistema
bool change_superuser = false; // Váriavel que troca para controle do jogo
bool cenario_direita  = false;
bool cenario_esquerda = false;

// Cores
float dark_green[3]  = {1.0/255.0, 50.0/255.0, 32.0/255.0};
float light_blue[3]  = {173.0/255.0, 216.0/255.0, 230.0/255.0};
float light_black[3] = {39.0/255.0, 39.0/255.0, 39.0/255.0};
float white[3]       = {1.0, 1.0, 1.0};

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
  
  glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10); // Assim vemos o cenário do usuário

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
		case 'p':
			if (change_superuser) {
				printf("Você mudou para modo usuário\n");
				change_superuser=false;
			} else {
				printf("Você mudou para modo superusuário\n");
				change_superuser=true;
			}
			break;
		case 'l':
		    if (change_superuser) {
				printf("Você apertou l\tCenário para a direita\n");
				cenario_direita=true;
				break;
			} else {
				printf("Você é apenas um usário comum, não pode se mover pelo cenário\n");
				break;
			}
		case 'j':
			if (change_superuser) {
				printf("Você apertou j\tCenário para a esquerda\n");
				cenario_esquerda=true;
				break;
			} else {
				printf("Você é apenas um usário comum, não pode se mover pelo cenário\n");
				break;
			}
		case 'q':
		    if (change_superuser) {
				printf("Você apertou q.\tEstá vendo o cenário todo.\tAlterando variáveis de visualização do mundo\n");

				view_desloc_x_begin = -100;
				view_desloc_x_end   =  100;

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);

				break;
			} else {
				printf("Você é apenas um usuário comum, não pode ver o cenário todo\n");
				break;
			}

	}
	display();
}

void doFrame(int v) {
	frameNumber++;
	if (change_superuser) {
		if (cenario_direita) {
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity();

			view_desloc_x_begin += 5;
			view_desloc_x_end   += 5;
			
			printf("Tamanho do cenário em x: %d até %d\n", view_desloc_x_begin, view_desloc_x_end);
			
			if (view_desloc_x_end >= 100) {
				printf("Passamos do limite de visualização do usuário, devemos dar a ideia de voltar ao começo para termos ilusão de cenário infinito\n");
				view_desloc_x_begin = -100;
				view_desloc_x_end   =  -80;

				glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);			
				
			} else {
				glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);
			}

			cenario_direita=false;
		}
		if (cenario_esquerda) {
			//frameNumberCenario -= 5;
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity();

			view_desloc_x_begin -= 5;
			view_desloc_x_end   -= 5;		

			printf("Tamanho do cenário em x: %d até %d\n", view_desloc_x_begin, view_desloc_x_end);
	
			
			glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);


			cenario_esquerda=false;
			
		}
	} else {
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();

		view_desloc_x_begin += 5;
		view_desloc_x_end   += 5;
		
		printf("Tamanho do cenário em x: %d até %d\n", view_desloc_x_begin, view_desloc_x_end);

		if (view_desloc_x_end >= 100) {
			printf("Passamos do limite de visualização do usuário, devemos dar a ideia de voltar ao começo para termos ilusão de cenário infinito\n");
			view_desloc_x_begin = -100;
			view_desloc_x_end   =  -80;
			glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);			
			
		} else {
			glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);
		}
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

