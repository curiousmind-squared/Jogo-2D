// Para rodar pela linha de comando: gcc nome_do_arquivo.c -lglut -lGLU -lGL -lm
#include <GL/glut.h>
#include <stdbool.h>

// Para testes
#include <stdio.h>

// Váriaveis relacionadas às dimensões da tela e do mundo
int view_desloc_x_begin = -10;
int view_desloc_x_end   =  10;

// Váriaveis relacionas ao carro
float carr_y_pos = -4.0;
float carr_y_vel = 0.0f;
float gravidade = -0.0070;

// Variáveis relacionadas ao Frame
int frameNumber        = 0; // Frame number geral 

// Variáveis booleanas de sistema
bool change_superuser = false; // Váriavel que troca para controle do jogo
bool cenario_direita  = false;
bool cenario_esquerda = false;
bool jump = false;

// Cores
float dark_green[3]  = {1.0/255.0  , 50.0/255.0 , 32.0/255.0 };
float light_blue[3]  = {173.0/255.0, 216.0/255.0, 230.0/255.0};
float light_red[3]   = {255.0/255.0, 71.0/255.0 , 76.0/255.0 };
float light_black[3] = {39.0/255.0 , 39.0/255.0 , 39.0/255.0 };
float brown[3]       = {205.0/255.0, 127.0/255.0, 50.0/255.0 };
float white[3]       = {1.0        , 1.0        , 1.0        };

// Funções de desenho de formas
void quadrado();
void asfalto(int desloc_x);
void placas_de_asfalto();
void carro();


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
}

void quadrado()
{
/*
 * Unidade básica de um quadrado
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

void carro() 
{
	int car_x_pos = (view_desloc_x_end + view_desloc_x_begin) / 2; // Carro no meio da tela

	glColor3fv(light_red);
	glPushMatrix();
	  glTranslated(car_x_pos, carr_y_pos, 0);
	  glScaled(1.5, 0.5, 1);
	  quadrado();
	glPopMatrix();
}

void display() {

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);

  placas_de_asfalto();
 
  carro();
  
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':
			jump=true;
			break;
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
		
		printf("Tamanho do cenário em x: %d até %d\t", view_desloc_x_begin, view_desloc_x_end);


		if (view_desloc_x_end >= 100) {
			printf("Passamos do limite de visualização do usuário, devemos dar a ideia de voltar ao começo para termos ilusão de cenário infinito\n");
			view_desloc_x_begin = -100;
			view_desloc_x_end   =  -80;
			glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);			
			
		} else {
			glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);
		}
	}

	if (jump && carr_y_vel == 0) { 
		carr_y_vel = 0.2;
		jump=false;
	} else if (jump && carr_y_vel != 0) { // Se existir velocidade em y, não permitimos múltiplos pulos (por enquanto, isso irá mudar no futuro)
		jump=false;
	}

	carr_y_pos += carr_y_vel;
	carr_y_vel += gravidade;
	
	printf("Y pos carro: %f\n", carr_y_pos);
	if (carr_y_pos <= -4.0) {
		carr_y_vel = 0;
		carr_y_pos = -4.0;
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

