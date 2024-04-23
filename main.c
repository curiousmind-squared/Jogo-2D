// Para rodar pela linha de comando: gcc nome_do_arquivo.c -lglut -lGLU -lGL -lm
#include <GL/glut.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

// Para testes
#include <stdio.h>

#define DOUBLE_PI 2*M_PI


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
float view_desloc_x_begin = -120;
float view_desloc_x_end   = -100;

int vidas = 3;

bool perdi_vida_pedra1 = true;
bool perdi_vida_pedra2 = true;
bool perdi_vida_pedra3 = true;

bool ganhou_vida_vida1 = true;

bool poder_pular = true;
bool permite_pular_infinito = false;
time_t end_count_poder;
time_t begin_count_poder;


// Váriaveis relacionas ao carro

// As variáveis "Point" guardam as posições do carro depois de alteradas (Por um scale por exemplo)
// Isso é útil pois utilizamos para detectar colisões. Só precisamos de alguns pontos
Point carr_sup_dir = {1, 1};
Point carr_inf_esq = {-1, -1};
// Por isso não colocamos elas aqui em baixo (Fora que o C também não deixa)
Quadrado quad_carro = {{-1, 1}, {1, 1}, {1, -1}, {-1, -1}};

float desloc=0; // Deslocamento horizontal do carro

int circle_points = 100;

float carr_scale_x;
float carr_scale_y;
float carr_y_pos = -4.0;
float carr_y_vel = 0.0f;
float gravidade = -0.0070;


int pedra1_x_pos;
int pedra2_x_pos;
int pedra3_x_pos;

int vida1_x_pos;

int poder1_x_pos;


// Variáveis relacionadas ao Frame
int frameNumber        = 0; // Frame number geral 

// Variáveis booleanas de sistema
bool change_superuser = false; // Váriavel que troca para controle do jogo
bool cenario_direita  = false;
bool cenario_esquerda = false;
bool jump = false;
bool gerar_estruturas = false;

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
void quarado_carro();
void circulo(int nPonto);
void circulo_fechado(int nPonto);

void quadrado_pedra();
void quadrado_vida();
void quadrado_poder();

// Colisao
void checar_colisao();

// Funções do OpenGl
void init(void);
void display(void);
void doFrame(int v);
void keyboard(unsigned char key, int x, int y);

void init(void)
{
  // define a cor de background da janela
  glClearColor(light_blue[0], light_blue[1], light_blue[2], 1.0);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity();

  gerar_estruturas=true;
  
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

void quadrado_pedra() {
	glBegin(GL_POLYGON);
		glVertex3f(-0.5, -0.5, 0);
		glVertex3f(0.5, -0.5, 0);
		glVertex3f(0.5, 0.5, 0);
		glVertex3f(-0.5, 0.5, 0);
  	glEnd();
}

void quadrado_vida() {
	glBegin(GL_LINE_LOOP);
		glVertex3f(-0.5, -0.5, 0);
		glVertex3f(0.5, -0.5, 0);
		glVertex3f(0.5, 0.5, 0);
		glVertex3f(-0.5, 0.5, 0);
  	glEnd();
}

void quadrado_poder() {
	glBegin(GL_POLYGON);
		glVertex3f(-0.5, -0.5, 0);
		glVertex3f(0.5, -0.5, 0);
		glVertex3f(0.5, 0.5, 0);
		glVertex3f(-0.5, 0.5, 0);
  	glEnd();
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
  int num = 14; // Número de peças de asfalto que teremos
  for (int i=0; i<num; i++) {
     if (i == 0) {
	asfalto(0);
     } else {
	asfalto(i*10);
	asfalto(i*10*-1);
     }
  }
}

void montanha(int desloc_x) {

	glColor3fv(dark_green);
	glBegin(GL_POLYGON);
		glVertex3f(desloc_x-10,0, 0);
		glVertex3f(desloc_x, 5.0, 0);
		glVertex3f(desloc_x+10,0, 0);
	glEnd();

}

void placas_de_montanha() {
 	int num = 14; // Número de peças de asfalto que teremos
	for (int i=0; i<num; i++) {
		if (i == 0) {
			montanha(0);
		} else {
			montanha(i*10);
			montanha(i*10*-1);
		}
	}
}

void carro() 
{
	float car_x_pos = (view_desloc_x_end + view_desloc_x_begin+desloc) / 2; // Carro no meio da tela

	carr_scale_x = 1.5;
	carr_scale_y = 0.5;

	glColor3fv(light_red);
	glPushMatrix();
	  glTranslated(car_x_pos, carr_y_pos, 0);
	  glScaled(carr_scale_x, carr_scale_y, 1);
	  
	    // roda da frente
		glPushMatrix();
			glTranslated(0.75, -0.6, 0);
			glScaled(0.25, 0.25, 1);
			glRotated(frameNumber*-1, 0, 0, 1);
			roda();
		glPopMatrix();

		// roda de trás
		glPushMatrix();
			glTranslated(-0.75, -0.6, 0);
			glScaled(0.25, 0.25, 1);
			glRotated(frameNumber*-1, 0, 0, 1);
			roda();
		glPopMatrix();

		// parte maior do carro
		glColor3f(1, 0, 0);
		glPushMatrix();
			glScaled(1.0, 0.5, 1);
			quadrado();
		glPopMatrix();

		// parte menor do carro
		glColor3f(1, 0, 0);
		glPushMatrix();
			glTranslated(-0.5, 0.75, 0);
			glScaled(0.5, 0.25, 1);
			quadrado();
		glPopMatrix();

		
		//quadrado_carro();
	  

	glPopMatrix();
}

void chao_verde() {
	// Parte preta do asfalto
	glColor3fv(dark_green);
	glBegin(GL_POLYGON);
		glVertex3f(-120, 0, 0);
		glVertex3f(120,  0, 0);
		glVertex3f(120,  -10, 0);
		glVertex3f(-120, -10, 0);
	glEnd();
}

void sol() {
	int num_of_lines = 12;
	int circle_points = 100;  

	glColor3f(1, 0.75, 0);
	glBegin(GL_LINES);
		for (int i=0; i<num_of_lines; i++) {
			double angle = DOUBLE_PI * i / num_of_lines;
			glVertex3f(0, 0, 0);
			glVertex3f(1.5 * cos(angle) ,
				   1.5 * sin(angle) ,
				   0);
		}
	glEnd();
	glColor3f(1, 1, 0);
	circulo_fechado(circle_points);
	
	glColor3f(1, 0.75, 0);			
	circulo(circle_points);
}

void posicao_sol() {
	// Animação e posicionamento do Sol
	float sol_x_pos = (view_desloc_x_end + view_desloc_x_begin+15) / 2;

	glPushMatrix();
	glTranslated(sol_x_pos, 8.2, 0);
	glRotated(frameNumber, 0, 0, 1);
	sol();
	glPopMatrix();
}

void helice() {
	  glBegin(GL_POLYGON);
	    glVertex3f(0, 0, 0);
	    glVertex3f(0.3, 0.2, 0);
	    glVertex3f(1, 0, 0);
	    glVertex3f(0.3, -0.2, 0);
	  glEnd();
}

void catavento() {

		// Retangulo
		glPushMatrix();
			glTranslated(0, 0.5, 0);
			glScaled(0.1, 0.5, 1);
			quadrado();
		glPopMatrix();

		glColor3fv(light_red);	

		// pá mais à direita
		glPushMatrix();
			glTranslated(0, 1, 0);
			glScaled(0.5, 0.5, 1);
			glRotated((float)frameNumber*2.0, 0, 0, 1);
			helice();
		glPopMatrix();

		// pá ao centro
		glPushMatrix();
			glTranslated(0, 1, 0);
			glRotated(120+(float)frameNumber*2.0, 0, 0, 1);
			glScaled(0.5, 0.5, 1);
			helice();
		glPopMatrix();

		// pá mais à esquerda
		glPushMatrix();
			glTranslated(0, 1, 0);
			glRotated(240+(float)frameNumber*2.0, 0, 0, 1);
			glScaled(0.5, 0.5, 1);
			helice();
		glPopMatrix();

}

void catavento_desenha(int desloc_x) {
	glColor3f(0.4, 0.4, 0.4);
	glPushMatrix();
		glTranslated(desloc_x, 2.5, 0);
		glScaled(2, 2,0);
		catavento();
	glPopMatrix();

	
}

void cataventos() {
	int num = 14; // Número de peças de asfalto que teremos
	for (int i=0; i<num; i+=2) {
		if (i == 0) {
			catavento_desenha(0);
		} else {
			catavento_desenha(i*10);
			catavento_desenha(i*10*-1);
		}
	}

}

void display() {

  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);

  chao_verde();

  posicao_sol();

  placas_de_montanha();

  cataventos();

  placas_de_asfalto();

  carro();

  if (gerar_estruturas) {
	srand(time(NULL));
	 // Número entre -80 and 90

	// PEDRAS
    pedra1_x_pos = rand() % 171 - 80;
	
    do {
        pedra2_x_pos = rand() % 171 - 80;
    } while (abs(pedra2_x_pos - pedra1_x_pos) < 30);


    do {
        pedra3_x_pos = rand() % 171 - 80;
    } while (abs(pedra3_x_pos - pedra1_x_pos) < 30 || abs(pedra3_x_pos - pedra2_x_pos) < 30);


	// VIDA
	vida1_x_pos = rand() % 171 - 80;

	// PODER
	poder1_x_pos = rand() % 171 - 80;
	 do {
        poder1_x_pos = rand() % 171 - 80;
    } while (abs(poder1_x_pos - vida1_x_pos) < 30);
	
	gerar_estruturas=false;
  }

  // PEDRAS -> Tiram uma vida
 glColor3fv(brown);
  glPushMatrix();
    glTranslated(pedra1_x_pos , -4.0, 0);	
    quadrado_pedra();
  glPopMatrix();
  
  glPushMatrix();
    glTranslated(pedra2_x_pos , -4.0, 0);	
    quadrado_pedra();
  glPopMatrix();
  
  glPushMatrix();
    glTranslated(pedra3_x_pos , -4.0, 0);	
    quadrado_pedra();
  glPopMatrix();
  
  // VIDA	
  glColor3fv(light_red);
  glPushMatrix();
    glTranslated(vida1_x_pos , -1.5, 0);
	glPushMatrix();
	glScaled(0.035, 0.035, 0);
		glBegin(GL_POLYGON); // Begin drawing the heart shape with filled color
		for (double t = 0; t < 2 * M_PI; t += 0.001) {
			double x = 16 * pow(sin(t), 3);
			double y = 13 * cos(t) - 5 * cos(2*t) - 2 * cos(3*t) - cos(4*t);
			glVertex2d(x, y);
		}
		glEnd();
	glPopMatrix();
    //quadrado_vida();
  glPopMatrix();

  // PODER 
  glColor3fv(white);
  glPushMatrix();
    glTranslated(poder1_x_pos , -1.5, 0);	
    quadrado_poder();
  glPopMatrix();

  glutSwapBuffers();
}

void checar_colisao() 
{
	float carr_x_pos = (view_desloc_x_end + view_desloc_x_begin+desloc) / 2;


	carr_inf_esq.x = carr_x_pos - carr_scale_x;
	carr_inf_esq.y = carr_y_pos - carr_scale_y;

	carr_sup_dir.x = carr_x_pos + carr_scale_x;
	carr_sup_dir.y = carr_y_pos + carr_scale_y;

	// Pedras
	Point pedra1_inf_esq = {pedra1_x_pos-0.5,-4.0-0.5};
	Point pedra1_sup_dir = {pedra1_x_pos+0.5,-4.0+0.5};

	Point pedra2_inf_esq = {pedra2_x_pos-0.5,-4.0-0.5};
	Point pedra2_sup_dir = {pedra2_x_pos+0.5,-4.0+0.5};

	Point pedra3_inf_esq = {pedra3_x_pos-0.5,-4.0-0.5};
	Point pedra3_sup_dir = {pedra3_x_pos+0.5,-4.0+0.5};

	// Vida
	Point vida1_inf_esq = {vida1_x_pos-0.5,-1.5-0.5};
	Point vida1_sup_dir = {vida1_x_pos+0.5,-1.5+0.5};

	// Poder
	Point poder1_inf_esq = {poder1_x_pos-0.5,-1.5-0.5};
	Point poder1_sup_dir = {poder1_x_pos+0.5,-1.5+0.5};
	
	// printf("\n\nColisão\n\n");
	// printf("Carro em x: %f ; em y: %f\n", carr_x_pos, carr_y_pos);
	// printf("Ponto inferior esquerdo do carro:  em x: %f ; em y: %f\n", carr_inf_esq.x, carr_inf_esq.y);
   	// printf("Ponto superior direito do carro :  em x: %f ; em y: %f\n", carr_sup_dir.x, carr_sup_dir.y);
    // printf("\n");
	// printf("Pedra em x: %f ; em y: %f\n", pedra_x_pos, pedra_y_pos);
	// printf("Ponto inferior esquerdo da pedra:  em x: %f ; em y: %f\n", pedra_inf_esq.x, pedra_inf_esq.y);
   	// printf("Ponto superior direito da pedra :  em x: %f ; em y: %f\n", pedra_sup_dir.x, pedra_sup_dir.y);


	// COLISÃO COM PEDRAS -> Tiram vidas
	if (carr_inf_esq.x < pedra1_sup_dir.x &&
	    carr_sup_dir.x > pedra1_inf_esq.x &&
		carr_inf_esq.y < pedra1_sup_dir.y &&
		carr_sup_dir.y > pedra1_inf_esq.y) {
			//printf("------------------------------------------------------- COLISAO -------------------------------------------------------\n");
			if (perdi_vida_pedra1) {
				vidas -= 1;
				perdi_vida_pedra1 = false;
				printf("Voce tem %d vidas restantes\n", vidas);
			}
	}
	
	if (carr_inf_esq.x < pedra2_sup_dir.x &&
	    carr_sup_dir.x > pedra2_inf_esq.x &&
		carr_inf_esq.y < pedra2_sup_dir.y &&
		carr_sup_dir.y > pedra2_inf_esq.y) {
			//printf("------------------------------------------------------- COLISAO -------------------------------------------------------\n");
			if(perdi_vida_pedra2) {
				vidas -= 1;
				perdi_vida_pedra2 = false;
				printf("Voce tem %d vidas restantes\n", vidas);
			}
	}

	if (carr_inf_esq.x < pedra3_sup_dir.x &&
	    carr_sup_dir.x > pedra3_inf_esq.x &&
		carr_inf_esq.y < pedra3_sup_dir.y &&
		carr_sup_dir.y > pedra3_inf_esq.y) {
			//printf("------------------------------------------------------- COLISAO -------------------------------------------------------\n");
			if(perdi_vida_pedra3) {
				vidas -= 1;
				perdi_vida_pedra3 = false;
				printf("Voce tem %d vidas restantes\n", vidas);
			}
	}
	
	// COLISÃO COM VIDA -> ADICIONA VIDA
	if (carr_inf_esq.x < vida1_sup_dir.x &&
	    carr_sup_dir.x > vida1_inf_esq.x &&
		carr_inf_esq.y < vida1_sup_dir.y &&
		carr_sup_dir.y > vida1_inf_esq.y) {
			//printf("------------------------------------------------------- COLISAO -------------------------------------------------------\n");
			if(ganhou_vida_vida1) {
				vidas += 1;
				ganhou_vida_vida1 = false;
				printf("Voce ganhou uma vida\n");
				printf("Voce tem %d vidas restantes\n", vidas);
			}
	}

	// COLISÃO COM PODER -> PULOS INFINITOSSS
	if (carr_inf_esq.x < poder1_sup_dir.x &&
	    carr_sup_dir.x > poder1_inf_esq.x &&
		carr_inf_esq.y < poder1_sup_dir.y &&
		carr_sup_dir.y > poder1_inf_esq.y) {
			//printf("------------------------------------------------------- COLISAO -------------------------------------------------------\n");
			if(poder_pular) {
				permite_pular_infinito = true;
				poder_pular = false;
				printf("Voce ganhou o poder de pular!!\n");
				begin_count_poder = time(NULL);
			}
	}

}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'd':
			if (!(desloc >= 17.0)) {
				desloc += 1.0;
			}
			break; 
		case 'a':
			if (!(desloc <= -17.0)){
				desloc -= 1.0;
			}
			break;
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


		if (view_desloc_x_end >= 120) {
			
			gerar_estruturas      = true;
			perdi_vida_pedra1 = true;
			perdi_vida_pedra2 = true;
			perdi_vida_pedra3 = true;
			ganhou_vida_vida1 = true;
			poder_pular = true;


			printf("-------------------- SUAS VIDAS: %d --------------------\n", vidas);
			//printf("Passamos do limite de visualização do usuário, devemos dar a ideia de voltar ao começo para termos ilusão de cenário infinito\n");
			view_desloc_x_begin = -120;
			view_desloc_x_end   = -100;
			glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);			
			
		} else {
			glOrtho(view_desloc_x_begin, view_desloc_x_end, -10, 10, -10, 10);
		}
	}
	
	checar_colisao();

	end_count_poder = time(NULL);
	if (abs(end_count_poder - begin_count_poder) >= 4) {
		permite_pular_infinito = false;
	} else {
       	printf("\nFaltam %d\n para seu poder acabar!\n", 3-abs(end_count_poder - begin_count_poder));
	}

	if (jump && carr_y_vel == 0) { 
		carr_y_vel = 0.2;
		jump=false;
	} else if (jump && carr_y_vel != 0 && carr_y_pos <= 6.0) { 
		if (permite_pular_infinito) {
			carr_y_vel = 0.2;
			jump=false;
		} else {
			jump=false;
		}
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
  glutInitWindowPosition (900, 50);

  glutCreateWindow ("Carro maluco");

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(20, doFrame, 0);

  glutMainLoop();

  return 0;

}

