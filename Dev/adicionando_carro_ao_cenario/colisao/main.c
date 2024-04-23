// Para rodar pela linha de comando: gcc nome_do_arquivo.c -lglut -lGLU -lGL -lm
#include <GL/glut.h>
#include <stdbool.h>

// Para testes
#include <stdio.h>

typedef struct 
{
	float x;
	float y;
} Point;

typedef struct 
{
	Point sup_esq;
	Point sup_dir;
	Point inf_dir;
	Point inf_esq;
} Quadrado;


// Váriaveis relacionadas às dimensões da tela e do mundo
float view_desloc_x_begin = -10;
float view_desloc_x_end   =  10;

// Váriaveis relacionas ao carro

// As variáveis "Point" guardam as posições do carro depois de alteradas (Por um scale por exemplo)
// Isso é útil pois utilizamos para detectar colisões. Só precisamos de alguns pontos
Point carr_sup_dir = {1, 1};
Point carr_inf_esq = {-1, -1};
// Por isso não colocamos elas aqui em baixo (Fora que o C também não deixa)
Quadrado quad_carro = {{-1, 1}, {1, 1}, {1, -1}, {-1, -1}};

float carr_scale_x;
float carr_scale_y;
//float carr_translate_x; // Não precisa pois as vars de baixo fazem o serviço
//float carr_translate_y; 
float carr_y_pos = -4.0;
float carr_y_vel = 0.0f;
float gravidade = -0.0070;

float pedra_x_pos = 10.0;
float pedra_y_pos = -4.0;

// Variáveis relacionadas ao Frame
int frameNumber        = 0; // Frame number geral 

// Variáveis booleanas de sistema
bool change_superuser = true; // Váriavel que troca para controle do jogo
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
void quarado_carro();
void asfalto(int desloc_x);
void placas_de_asfalto();
void carro();

void checar_colisao();

// Funções do OpenGl
void init(void);
void display(void);
void doFrame(int v);
void keyboard(unsigned char key, int x, int y);

// Constantes do quadrado


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
 * Unidade básica de um quadrado -> Para formas que não interagem com o usuário
 */
  glBegin(GL_POLYGON);
    glVertex3f(-1, -1, 0);
    glVertex3f(1, -1, 0);
    glVertex3f(1, 1, 0);
    glVertex3f(-1, 1, 0);
  glEnd();
}

void quadrado_carro() 
{
 glBegin(GL_LINE_LOOP);
	glVertex3f(quad_carro.sup_esq.x, quad_carro.sup_esq.y, 0);
	glVertex3f(quad_carro.sup_dir.x, quad_carro.sup_dir.y, 0);
	glVertex3f(quad_carro.inf_dir.x, quad_carro.inf_dir.y, 0);
	glVertex3f(quad_carro.inf_esq.x, quad_carro.inf_esq.y, 0);
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
	float car_x_pos = (view_desloc_x_end + view_desloc_x_begin) / 2; // Carro no meio da tela

	carr_scale_x = 1.5;
	carr_scale_y = 0.5;

	glColor3fv(light_red);
	glPushMatrix();
	  glTranslated(car_x_pos, carr_y_pos, 0);
	  glScaled(carr_scale_x, carr_scale_y, 1);
	  quadrado_carro();
	glPopMatrix();
}

void display() {

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);

  placas_de_asfalto();
 
  carro();

  glPushMatrix();
    glTranslated(pedra_x_pos , pedra_y_pos, 0);	
    quadrado();
  glPopMatrix();
  
  glutSwapBuffers();
}

void checar_colisao() 
{
	float carr_x_pos = (view_desloc_x_end + view_desloc_x_begin) / 2;


	carr_inf_esq.x = carr_x_pos - carr_scale_x;
	carr_inf_esq.y = carr_y_pos - carr_scale_y;

	carr_sup_dir.x = carr_x_pos + carr_scale_x;
	carr_sup_dir.y = carr_y_pos + carr_scale_y;

	Point pedra_inf_esq = {pedra_x_pos-1,pedra_y_pos-1};
	Point pedra_sup_dir = {pedra_x_pos+1,pedra_y_pos+1};
	
	printf("\n\nColisão\n\n");
	printf("Carro em x: %f ; em y: %f\n", carr_x_pos, carr_y_pos);
	printf("Ponto inferior esquerdo do carro:  em x: %f ; em y: %f\n", carr_inf_esq.x, carr_inf_esq.y);
   	printf("Ponto superior direito do carro :  em x: %f ; em y: %f\n", carr_sup_dir.x, carr_sup_dir.y);
    printf("\n");
	printf("Pedra em x: %f ; em y: %f\n", pedra_x_pos, pedra_y_pos);
	printf("Ponto inferior esquerdo da pedra:  em x: %f ; em y: %f\n", pedra_inf_esq.x, pedra_inf_esq.y);
   	printf("Ponto superior direito da pedra :  em x: %f ; em y: %f\n", pedra_sup_dir.x, pedra_sup_dir.y);

	if (carr_inf_esq.x < pedra_sup_dir.x &&
	    carr_sup_dir.x > pedra_inf_esq.x &&
		carr_inf_esq.y < pedra_sup_dir.y &&
		carr_sup_dir.y > pedra_inf_esq.y) {
			printf("------------------------------------------------------- COLISAO -------------------------------------------------------\n");
		}
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

	// Primeira coisa que fazemos é alterar o cenário
	if (change_superuser) {
		if (cenario_direita) {
			glMatrixMode (GL_PROJECTION);
			glLoadIdentity();

			view_desloc_x_begin += 1;
			view_desloc_x_end   += 1;
			
			//printf("Tamanho do cenário em x: %f até %f\n", view_desloc_x_begin, view_desloc_x_end);
			
			if (view_desloc_x_end >= 100) {
				//printf("Passamos do limite de visualização do usuário, devemos dar a ideia de voltar ao começo para termos ilusão de cenário infinito\n");
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

			view_desloc_x_begin -= 1;
			view_desloc_x_end   -= 1;		

			//printf("Tamanho do cenário em x: %f até %f\n", view_desloc_x_begin, view_desloc_x_end);
	
			
			glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);


			cenario_esquerda=false;
			
		}
	} else {
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();

		view_desloc_x_begin += 0.5;
		view_desloc_x_end   += 0.5;
		
		//printf("Tamanho do cenário em x: %f até %f\t", view_desloc_x_begin, view_desloc_x_end);


		if (view_desloc_x_end >= 100) {
			printf("Passamos do limite de visualização do usuário, devemos dar a ideia de voltar ao começo para termos ilusão de cenário infinito\n");
			view_desloc_x_begin = -100;
			view_desloc_x_end   =  -80;
			glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);			
			
		} else {
			glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);
		}
	}
	
	checar_colisao();

	if (jump && carr_y_vel == 0) { 
		carr_y_vel = 0.2;
		jump=false;
	} else if (jump && carr_y_vel != 0) { // Se existir velocidade em y, não permitimos múltiplos pulos (por enquanto, isso irá mudar no futuro)
		jump=false;
	}

	carr_y_pos += carr_y_vel;
	carr_y_vel += gravidade;
	
	//printf("Y pos carro: %f\n", carr_y_pos);
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

