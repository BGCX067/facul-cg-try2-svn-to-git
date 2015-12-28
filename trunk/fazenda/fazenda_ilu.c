/**
 *  Conferir porque os objetos ficam todos prateados/avermelhados
 * quando o uso de iluminacao esta ativado.
 *  Deve ser porque TODO objeto criado deve ter bloco com 3 
 * 'glMaterialfv()' para definir a sua iluminacao. Entao, talvez caso
 * algum esteja sem, deve renderizar dessa forma...
 * 
 */
#include<stdlib.h>
#include<GL/glut.h>
#include<math.h>

#include "image.h"

#define COORD_TEXTURA_PLANO 1.0
#define COR_DO_PLANO 0.52,0.52,0.78,1.0
#define TEXTURA_DO_PLANO "montanhas.rgb"

GLfloat ctp[4][2]={
  {-COORD_TEXTURA_PLANO,-COORD_TEXTURA_PLANO},
  {+COORD_TEXTURA_PLANO,-COORD_TEXTURA_PLANO},
  {+COORD_TEXTURA_PLANO,+COORD_TEXTURA_PLANO},
  {-COORD_TEXTURA_PLANO,+COORD_TEXTURA_PLANO}
};

GLuint  textura_plano;

#define USAR_LUZ

GLint fatias=30;
GLint pilhas=30;
GLint raioEsfera=1.5;

GLfloat chao_difusa[]    = { 0.0, 0.0, 1.0, 1.0 };
GLfloat chao_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat chao_brilho[]    = { 50.0 };

GLfloat arm_difusa[]    = { 1.0, 0.0, 1.0, 1.0 };
GLfloat arm_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat arm_brilho[]    = { 50.0 };

GLfloat porta_difusa[]    = { 1.0, 1.0, 0.0, 1.0 };
GLfloat porta_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat porta_brilho[]    = { 50.0 };

GLfloat macaneta_difusa[]    = { 1.0, 0.0, 0.0, 1.0 };
GLfloat macaneta_especular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat macaneta_brilho[]    = { 50.0 };

GLfloat posicao_luz[]    = { 0.0, 4.0, 0.0, 1.0};
GLfloat cor_luz[]        = { 1.0, 1.0, 1.0, 1.0};
GLfloat cor_luz_amb[]    = { 0.3, 0.3, 0.3, 1.0};

static int shoulder = 0, elbow = 0, elbow2 = 0;

GLint gouraud=0;


/** se precisar de PI, use o M_PI (da math.h) */

static int eixoy = 0;
static int eixox = 0;
static int eixoz = 0;
static int anguloPasMoinho = 0;	//< [0~90]

static GLUquadricObj *quadobj;
static GLUquadricObj *arvore1_copa, *arvore1_tronco;
static GLUquadricObj *moinho, *ovelha;

void init();
void init_arvore();
void init_moinho();
inline void gerarChao();
inline void gerarTodasArvores();
inline void rotacionarCamera();
inline void gerarTodasOvelhas();
inline void gerarCercaCompleta();
inline void gerarTodasCenouras();

inline void set_Color_and_Materialfv(float r, float g, float b){
	GLfloat brilho[]    = { 50 };
	GLfloat cores[4] = {r, g, b, 1};
	//GLfloat cores[4] = {1, 1, 1, 1};
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cores);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cores);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, brilho);
	
	glColor3f(r, g, b);
}

void carregar_texturas(void){
  IMAGE *img;
  GLenum gluerr;

  /* textura do plano */
  glGenTextures(1, &textura_plano);
  glBindTexture(GL_TEXTURE_2D, textura_plano);
  
  if(!(img=ImageLoad(TEXTURA_DO_PLANO))) {
    fprintf(stderr,"Error reading a texture.\n");
    exit(-1);
  }

  gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 
			   img->sizeX, img->sizeY, 
			   GL_RGB, GL_UNSIGNED_BYTE, 
			   (GLvoid *)(img->data));
  if(gluerr){
    fprintf(stderr,"GLULib%s\n",gluErrorString(gluerr));
    exit(-1);
  }

  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

}


// trans: translate [x,y,z] ,, scale: scale[x,y,z]
void gerarCaixaDagua
  (float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ);
 
void gerarCenoura
  (float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ);

void gerarArvore
  (float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ);
 
void gerarCerca
  (float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ);
  
void gerarCasa
  (float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ);
  
void gerarMoinho
  (float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ);		 
  
void gerarOvelha
  (float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ);
  
void gerarTuboFechado
  (float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ, 
  float rotGraus, float rotX, float rotY, float rotZ, 
  float corR, float corG, float corB, 
  int quantLados);

void girarPasMoinho(int dummyCBparam){
	anguloPasMoinho = (anguloPasMoinho + 5) % 91;
	glutTimerFunc(100, girarPasMoinho, 0);
	glutPostRedisplay();
}

void delete_quadrics(){
	gluDeleteQuadric( quadobj );
	gluDeleteQuadric( arvore1_copa );
	gluDeleteQuadric( arvore1_tronco );
	gluDeleteQuadric( moinho );
	gluDeleteQuadric( ovelha );
}

void adicionarLuz(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glDepthMask(GL_TRUE);
//	glClearColor(1.0,1.0,1.0,1.0);
///	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	///< parece bom, mas nao exibe o resto :\

	if(gouraud){
		glShadeModel(GL_SMOOTH);
	}
	else{
		glShadeModel(GL_FLAT);
	}

/***
	glPushMatrix();
		glRotatef ((GLfloat) shoulder, 0.0, 1.0, 0.0);

		/// propriedades do material do chao 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, chao_difusa);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, chao_especular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, chao_brilho);
		// Chao :
		glPushMatrix();
			glTranslatef (0.0, -3.4, 0.0);
			glScalef (10.0, 0.2, 10.0);
			glutSolidCube (1.0);
		glPopMatrix();

		/// propriedades do material do armario 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, arm_difusa);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, arm_especular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, arm_brilho);
		// armario :
		glColor3f(0.0, 1.0, 1.0);
		glPushMatrix();
			glScalef (4.0, 6.0, 2.0);
			glutSolidCube (1.0);
		glPopMatrix();

		/// propriedades do material do porta 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porta_difusa);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, porta_especular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, porta_brilho);
		// porta 1 :
		glPushMatrix();
			glTranslatef (-2.0, 0.0, 1.1);
			glRotatef ((GLfloat) elbow, 0.0, 1.0, 0.0);
			glTranslatef (2.0, 0.0, -1.1);
			glTranslatef (-1.0, 0.0, 1.0);
			glScalef (2.0, 6.0, 0.2);
			glutSolidCube (1.0);
			
			/// propriedades do material da macaneta 
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, macaneta_difusa);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, macaneta_especular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, macaneta_brilho);
			// desenha macaneta 1 :
			glPushMatrix();    
				glTranslatef (0.4, 0.0, 1.1);
				glScalef (0.2, 0.2, 0.2);
				glutSolidCube (1.0);
			glPopMatrix();
		glPopMatrix();

		/// propriedades do material do porta 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, porta_difusa);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, porta_especular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, porta_brilho);
		// porta 2 :
		glPushMatrix();
			glTranslatef (2.0, 0.0, 1.1);
			glRotatef ((GLfloat) elbow2, 0.0, 1.0, 0.0);
			glTranslatef (-2.0, 0.0, -1.1);
			glTranslatef (1.0, 0.0, 1.0);
			glScalef (2.0, 6.0, 0.2);
			glutSolidCube (1.0);
			/// propriedades do material da macaneta 
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, macaneta_difusa);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, macaneta_especular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, macaneta_brilho);
			// desenha macaneta 2 :
			glPushMatrix();    
				glTranslatef (-0.4, 0.0, 1.1);
				glScalef (0.2, 0.2, 0.2);
				glutSolidCube (1.0);
			glPopMatrix();
		glPopMatrix();

	// origem volta para o sistema de coordenadas original :
	glPopMatrix();
***/	
	glutSwapBuffers();
}


void display(void){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	rotacionarCamera();			

	gerarChao();	
 	gerarCaixaDagua(5.0, 0.0, -4.0 , 1.0, 1.0, 1.0);
	gerarTodasCenouras();	
	gerarCasa(0.0, 0.0, -1.5 , 1.0, 1.0, 1.0);
	gerarMoinho(-5,-2,3 , 1,1,1);	//< -2 eh a altura do chao
	gerarTodasArvores();
	gerarTodasOvelhas();
	gerarCercaCompleta();

/** ATENCAO! resultado estranho se invocado ANTES do bloco anterior! */
#ifdef USAR_LUZ 
	adicionarLuz();
#endif

	glPopMatrix();
	glutSwapBuffers();
}

void reshape (int w, int h){
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 120.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef (0.0, 0.0, -13.0);
}

void keyboard (unsigned char key, int x, int y){
	switch (key) {
	case 'a':
		eixoy = (eixoy + 5) % 360;
		glutPostRedisplay();
		break;
	case 'd':
		eixoy = (eixoy - 5) % 360;
		glutPostRedisplay();
		break;
	case 'q':
		eixoz = (eixoz - 5) % 360;
		glutPostRedisplay();
		break;
	case 'e':
		eixoz = (eixoz + 5) % 360;
		glutPostRedisplay();
		break;
	case 's':
		eixox = (eixox - 5) % 360;
		glutPostRedisplay();
		break;
	case 'w':
		eixox = (eixox + 5) % 360;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

int main(int argc, char** argv){
	quadobj = gluNewQuadric();
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500); 
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display); 
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(100, girarPasMoinho, 0);
	glutMainLoop();
	
	delete_quadrics();
	return 0;
}

void gerarCaixaDagua
	(float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ)
{
	//GLUquadricObj *quadobj;
	//quadobj = gluNewQuadric();
	
	glPushMatrix();
	
		glTranslatef (transX, transY-.2, transZ);
		glScalef (scaleX, scaleY, scaleZ);
		
		//perna1
    	glPushMatrix();
        	glTranslatef (0.4, -0.2, .4);
        	glScalef (0.2, 3.0, 0.2);
        	set_Color_and_Materialfv(.5, 0.5, 0.0);
        	glutSolidCube (1.0);
    	glPopMatrix();	
		
		//perna2
    	glPushMatrix();
        	glTranslatef (0.4, -0.2, -0.4);
        	glScalef (0.2, 3.0, 0.2);
        	set_Color_and_Materialfv(.5, 0.5, 0.0);
        	glutSolidCube (1.0);
    	glPopMatrix();
    	
		//perna3
    	glPushMatrix();
        	glTranslatef (-.4, -0.2, 0.4);
        	glScalef (0.2, 3.0, 0.2);
        	set_Color_and_Materialfv(.5, 0.5, 0.0);
        	glutSolidCube (1.0);
    	glPopMatrix();
    
		//perna4
    	glPushMatrix();
        	glTranslatef (-.4, -0.2, -0.4);
        	glScalef (0.2, 3.0, 0.2);
        	set_Color_and_Materialfv(.5, 0.5, 0.0);
        	glutSolidCube (1.0);
    	glPopMatrix();
    	
		//cilindro da caixa dagua
    	glPushMatrix();
            glTranslatef ( 0.0, 2, 0.0);
        	glRotatef ((GLfloat) 90, 1.0, 0.0, 0.0);
        	set_Color_and_Materialfv(.5, 0.0, 0.7);
        	gluCylinder( quadobj , 1.2 , 1.2 , 1.0, 10 , 10 );
    	glPopMatrix();	
    	
		//tampa de cima
    	glPushMatrix();
            glTranslatef ( 0.0, 2.0, 0.0);
        	glRotatef ((GLfloat) 90, 1.0, 0.0, 0.0);
        	set_Color_and_Materialfv(.5, 0.0, 0.7);
        	gluDisk( quadobj, 0.0 , 1.2 , 10 , 10 );
    	glPopMatrix();	
		
		//tampa de baixo
    	glPushMatrix();
            glTranslatef ( 0.0, 1.0, 0.0);
        	glRotatef ((GLfloat) 90, 1.0, 0.0, 0.0);
        	set_Color_and_Materialfv(.5, 0.0, 0.7);
        	gluDisk( quadobj, 0.0 , 1.2 , 10 , 10 );
    	glPopMatrix();	
    	
	glPopMatrix();
}

void gerarCenoura
	(float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ)
{
	glPushMatrix();
	
		glTranslatef (transX, transY, transZ);
		glScalef (scaleX, scaleY, scaleZ);
	
		//folha da cenoura
		glPushMatrix();
            glTranslatef ( 0.0, 2.3, 0.0);
        	glRotatef ((GLfloat) 90, 1.0, 0.0, 0.0);
        	set_Color_and_Materialfv(0.0, 0.7, 0.0);
        	gluCylinder( quadobj , 0.2 , 0.02 , 0.44, 4 , 4 );
    	glPopMatrix();
	
		//corpo da cenoura
    	glPushMatrix();
            glTranslatef ( 0.0, 2, 0.0);
        	glRotatef ((GLfloat) 90, 1.0, 0.0, 0.0);
        	set_Color_and_Materialfv(.8, 0.4, 0.0);
        	gluCylinder( quadobj , 0.1 , .03 , 0.3, 10 , 10 );
    	glPopMatrix();	
	
	glPopMatrix();
}

void gerarArvore
	(float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ)
{
	const float numDivs=30, alturaTronco=1, raioTronco=1;
	const float raioCopa=1, alturaCopa=2;

  glPushMatrix(); 
   glTranslatef (transX, transY, transZ);
   glScalef (scaleX, scaleY, scaleZ);
		
	/** copa da arvore */
   glPushMatrix();
    glTranslatef(0.0, alturaTronco, 0.0);
    glRotatef(270.0, 1.0, 0.0, 0.0);
    //glScalef(1,1,1);
    set_Color_and_Materialfv(0.0, 0.80392, 0.40000); //< verde diferente
    glShadeModel (GL_SMOOTH);
    gluCylinder(arvore1_copa, raioCopa, 0.0, alturaCopa, numDivs,numDivs);
   glPopMatrix();

   /** tronco */    
   glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef(270.0, 1.0, 0.0, 0.0);
    //glScalef(1,1,1);
    set_Color_and_Materialfv(0.54510, 0.27059, 0.074510); //marrom 139-69-19
    glShadeModel (GL_SMOOTH);
    gluCylinder(arvore1_copa, raioTronco/2, raioTronco/2, alturaTronco, numDivs,numDivs);
   glPopMatrix();   
   
  glPopMatrix();
    
  glFlush();
}

void gerarCerca
	(float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ)
{
	glPushMatrix();

	glTranslatef (transX, transY, transZ);
	glScalef (scaleX, scaleY, scaleZ);

	/* estaca 1 */
	glPushMatrix();
	  glTranslatef (-4.5, -1.3, 4.5);
	  glScalef (0.2, 1.2, 0.2);
	  glutSolidCube (1.0);
	glPopMatrix();

	/* estaca 2 */
	glPushMatrix();
	  glTranslatef (4.5, -1.3, 4.5);
	  glScalef (0.2, 1.2, 0.2);
	  glutSolidCube (1.0);
	glPopMatrix();

	/* cerca 1 */
	glPushMatrix();
	  glTranslatef (0.0, -1.0, 4.7);
	  glScalef (9.4, 0.2, 0.2);
	  glutSolidCube (1.0);
	glPopMatrix();

	/* cerca 2 */
	glPushMatrix();
	  glTranslatef (0.0, -1.3, 4.7);
	  glScalef (9.4, 0.2, 0.2);
	  glutSolidCube (1.0);
	glPopMatrix();
	
	glPopMatrix();
}

void gerarChao(){
	
	/// INICIO TEXTURA!
   /* habilita/desabilita uso de texturas*/
  glEnable(GL_TEXTURE_2D);

	  glColor4f(COR_DO_PLANO);
	  glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
	  glBindTexture(GL_TEXTURE_2D,textura_plano);
	   
	  glBegin(GL_QUADS);
		  glTexCoord2fv(ctp[0]);  glVertex3f(-6.5,-1.8, 6.5);
		  glTexCoord2fv(ctp[1]);  glVertex3f(6.5, -1.8, 6.5);
		  glTexCoord2fv(ctp[2]);  glVertex3f(6.5, -1.8, -6.5);
		  glTexCoord2fv(ctp[3]);  glVertex3f(-6.5,-1.8, -6.5);
	  glEnd();

  glDisable(GL_TEXTURE_2D);
///FIM TEXTURA!

	
	glPushMatrix();
	  glTranslatef (0.0, -2.0, 0.0);
	  glScalef (13.0, 0.2, 13.0);
	  set_Color_and_Materialfv(0.0, 1.0, 0.0);
	  glutSolidCube (1.0);
	glPopMatrix();	
}

void gerarTodasArvores(){
	gerarArvore(-5.0, -2.0, -6.0 , 1,2.2,1);
	gerarArvore(-3.0, -2.0, -6.0 , 1,2.3,1);  
	gerarArvore(-1.0, -2.0, -6.0 , 1,2.0,1);  
	gerarArvore(+1.0, -2.0, -6.0 , 1,2.1,1);  
	gerarArvore(+3.0, -2.0, -6.0 , 1,2.4,1);  
	gerarArvore(+5.0, -2.0, -6.0 , 1,2.0,1); 
}

void init_iluminacao(){
  gouraud=1;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz);
  glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz);
  glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz_amb);
  glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz);

  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);


  glEnable(GL_AUTO_NORMAL);
  glEnable(GL_NORMALIZE);
}

void init_moinho(){
	moinho = gluNewQuadric();
	gluQuadricDrawStyle(moinho, GLU_FILL);
	gluQuadricNormals(moinho, GLU_SMOOTH);	
}

void init_ovelha(){
	ovelha = gluNewQuadric();
	gluQuadricDrawStyle(ovelha, GLU_FILL);
	gluQuadricNormals(ovelha, GLU_SMOOTH);		
}

void init_arvore(){
	arvore1_copa = gluNewQuadric();
	gluQuadricDrawStyle(arvore1_copa, GLU_FILL); /* preenchido */
	gluQuadricNormals(arvore1_copa, GLU_SMOOTH);

	arvore1_tronco = gluNewQuadric();
	gluQuadricDrawStyle(arvore1_tronco, GLU_FILL); 
	gluQuadricNormals(arvore1_tronco, GLU_SMOOTH);
}

void init(void){
	glClearColor (0.0, 0.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_TEXTURE_2D);
	carregar_texturas();
	
#ifdef USAR_LUZ 
	init_iluminacao();
#endif
	init_arvore();
	init_moinho();
	init_ovelha();
}

void rotacionarCamera(){
	glRotatef ((GLfloat) eixoy, 0.0, 1.0, 0.0);
	glRotatef ((GLfloat) eixox, 1.0, 0.0, 0.0);
	glRotatef ((GLfloat) eixoz, 0.0, 0.0, 1.0);	
}

void gerarTuboFechado(float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ, float rotGraus, float rotX, float rotY, float rotZ, float corR, float corG, float corB, int quantLados)
{	
		glPushMatrix();
		
			glTranslatef (transX, transY, transZ);
			//glScalef (scaleX, scaleY, scaleZ);
			glRotatef ((GLfloat) rotGraus, rotX, rotY, rotZ);
			
			//glTranslatef (transX, transY, transZ);
			set_Color_and_Materialfv(corR, corG, corB);
			
			//corpo
			glPushMatrix();
				glTranslatef (0, 0.0, 0);
				glScalef (scaleX, scaleY, scaleZ);
				gluCylinder( quadobj , 1.0 , 1.0 , 2.0, quantLados , 10 );
			glPopMatrix();
			
			//tampa A
			glPushMatrix();
				glTranslatef (0.0, 0.0, 0);
				glScalef (scaleX, 1.0, scaleZ/2);			
				gluDisk( quadobj, 0.0 , 1.0 , quantLados , 10 );
			glPopMatrix();
			
			//tampa B
			glPushMatrix();
				glTranslatef (0.0, 0.0, scaleZ);
				glScalef (scaleX, 1.0, scaleZ/2);	
				gluDisk( quadobj, 0.0 , 1.0 , quantLados , 10 );
			glPopMatrix();
			
		glPopMatrix();
}

void gerarCasa(float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ)
{
	
	glPushMatrix();
		
		glTranslatef (transX, transY, transZ);
		glScalef (scaleX, scaleY, scaleZ);		
		
		//bloco maior da casa
		glPushMatrix();
            glTranslatef ( 0.0, -0.5, 0.0);
			glScalef (6.0, 3.0, 3.0);
        	set_Color_and_Materialfv(0.2, 0.2, 0.7);
        	glutSolidCube (1.0);
    	glPopMatrix();
		
		//bloco menor da frente da casa
		glPushMatrix();
            glTranslatef (2.0, -0.5, 2.0);
			glScalef (2.0, 3.0, 1.0);
        	glutSolidCube (1.0);
    	glPopMatrix();		

        //telhado grande
		gerarTuboFechado( -3.0, 1.51, 0, 
						1.75, 1.0, 3.0, 
						90, 0, 1.0, 0, 
						1.0, 0.0, .0, 
						3);
		
        //telhado pekeno			
		gerarTuboFechado( 1.92, 1.51, 0.0, 
						1.25, 1.0, 1.25, 
						0, 0, 0, 0, 
						1.0, 0.0, .0, 
						3);						
		
		//pórrrrrta
		glPushMatrix();
            glTranslatef (2.4, -1.5, 2.501);
			glScalef (0.6, 1.0, 0.02);
        	set_Color_and_Materialfv(0.0, 0.0, 0.0);
        	glutSolidCube (1.0);
    	glPopMatrix();
	
	glPopMatrix();
	
}

void gerarMoinho(float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ)
{
	const float alturaMoinho=4, raioMoinho=0.7;
	const int comprimPa=5, espessuraPa=0.1, numDivs=30;
	
	glPushMatrix();
		glTranslatef (transX, transY, transZ);
		glScalef (scaleX, scaleY, scaleZ);		
		
		/// poste
		glPushMatrix();
		  glTranslatef ( 0.0, 0, 0.0);
		  glRotatef ((GLfloat) 270, 1.0, 0.0, 0.0);
		  set_Color_and_Materialfv(1, 1, 0.8);
		  gluCylinder( moinho , raioMoinho,raioMoinho , alturaMoinho, 10,10);
		glPopMatrix();
		
		/// pa 1
		glPushMatrix();
		  glTranslatef ( -raioMoinho, alturaMoinho, 0);
		  glRotatef ((GLfloat) anguloPasMoinho, 1.0, 0.0, 0.0);
		  glScalef (espessuraPa, (0.3*alturaMoinho), comprimPa);
          set_Color_and_Materialfv(1, 1, 1);
          glutSolidCube (1.0);
		glPopMatrix();
				
		/// pa 2
		glPushMatrix();
		  glTranslatef ( -raioMoinho, alturaMoinho, 0);
		  glRotatef ((GLfloat) anguloPasMoinho+90, 1.0, 0.0, 0.0);
		  glScalef (espessuraPa, (0.3*alturaMoinho), comprimPa);
          set_Color_and_Materialfv(1, 1, 1);
          glutSolidCube (1.0);
		glPopMatrix();
	
		/// cobertura do moinho
		glPushMatrix ();
		  glTranslatef (0.0, alturaMoinho-0.1, 0.0);
		  //glRotatef (0.0, 0.0, 0.0, 0.0); 
		  glScalef (1.0, 1.0, 1.0);
		  set_Color_and_Materialfv(1, 1, 0.8);
		  glutSolidSphere(raioMoinho, numDivs,numDivs);
		glPopMatrix ();

	
	glPopMatrix();
}

void gerarOvelha(float transX, float transY, float transZ, float scaleX, float scaleY, float scaleZ)
{
	const float alturaOvelha=0.6, raioOvelha=0.6, numDivs=10;
	const float espessPata=0.05;
	const float deformX=0.3, deformY=0.3, deformZ=0.6;
	float raioCabeca, posZCabeca;
	
	glPushMatrix();
		glTranslatef (transX, transY, transZ);
		glScalef (scaleX, scaleY, scaleZ);
		
		/// corpo
		glPushMatrix();
		  glTranslatef ( 0.0, alturaOvelha, 0.0);
		  glRotatef ((GLfloat) 0, 1.0, 0.0, 0.0);
		  set_Color_and_Materialfv(1, 1, 1);
		  glScalef (deformX, deformY, deformZ);
		  glutSolidSphere(raioOvelha, numDivs,numDivs);
		glPopMatrix();
		
		/// pata "frente" 1
		glPushMatrix();
		  glTranslatef ( (raioOvelha/2)*deformX, 0, (raioOvelha/2)*deformZ);
		  glRotatef ((GLfloat) 270, 1.0, 0.0, 0.0);
		  //glRotatef ((GLfloat) 90, 0.0, 1.0, 0.0); //< dar uma naturalidade pras patinhas ;)
		  glScalef (1, 1, 1);
          set_Color_and_Materialfv(1, 1, 0.6);
          gluCylinder( ovelha , espessPata,espessPata , alturaOvelha, 10,10);
		glPopMatrix();
		
		/// pata "frente" 2
		glPushMatrix();
		  glTranslatef ( -((raioOvelha/2)*deformX), 0, (raioOvelha/2)*deformZ);
		  glRotatef ((GLfloat) 270, 1.0, 0.0, 0.0);
		  //glRotatef ((GLfloat) 90, 0.0, 1.0, 0.0);
		  glScalef (1, 1, 1);
          set_Color_and_Materialfv(1, 1, 0.6);
          gluCylinder( ovelha , espessPata,espessPata , alturaOvelha, 10,10);
		glPopMatrix();
		
		/// pata "tras" 1
		glPushMatrix();
		  glTranslatef ( (raioOvelha/2)*deformX, 0, -((raioOvelha/2)*deformZ));
		  glRotatef ((GLfloat) 270, 1.0, 0.0, 0.0);
		  //glRotatef ((GLfloat) 90, 0.0, 1.0, 0.0); 
		  glScalef (1, 1, 1);
          set_Color_and_Materialfv(1, 1, 0.6);
          gluCylinder( ovelha , espessPata,espessPata , alturaOvelha, 10,10);
		glPopMatrix();
		
		/// pata "tras" 2
		glPushMatrix();
		  glTranslatef ( -((raioOvelha/2)*deformX), 0, -((raioOvelha/2)*deformZ));
		  glRotatef ((GLfloat) 270, 1.0, 0.0, 0.0);
		  //glRotatef ((GLfloat) 90, 0.0, 1.0, 0.0);
		  glScalef (1, 1, 1);
          set_Color_and_Materialfv(1, 1, 0.6);
          gluCylinder( ovelha , espessPata,espessPata , alturaOvelha, 10,10);
		glPopMatrix();
		///--------------------- fim das patas --------
		
		/// cabeça
		// Atencao! O certo eh levar em conta a distorcao nos demais eixos, 
		// mas enfim. Vai de X mesmo...
		raioCabeca = raioOvelha * deformX;
		// Ultimo termo eh a posicao das patas da frente
		posZCabeca = (raioCabeca / 2) + (((raioOvelha/2)*deformZ)); 
		glPushMatrix();
		  glTranslatef ( 0, alturaOvelha, posZCabeca);
		  glScalef (1, 1, 1);
          set_Color_and_Materialfv(1, 1, 0.6);
          glutSolidSphere(raioCabeca, 10,10); 
		glPopMatrix();

	glPopMatrix();
}

void gerarTodasOvelhas(){
	/// -2 eh a altura do chao
	gerarOvelha(5,-2,5.5, 1,1,1);
	
	glPushMatrix();
	  /* [A] deve ser o INVERSO de [B] e igual (mesmo deslocamento)
	   * que o objeto [C]
	   */
      glTranslatef (-2,-2,6);		// [A]
      glRotatef ((GLfloat) 90, 0.0, 1.0, 0.0);
	  glTranslatef (2,2,-6);		// [B]
      gerarOvelha(-2,-2,6 , 1,1,1);	// [C]
	glPopMatrix();
	
	float x=1, z=5.5;
	glPushMatrix();
      glTranslatef (x,-2,z);	
      glRotatef ((GLfloat) 225, 0.0, 1.0, 0.0);
	  glTranslatef (-x,2,-z);	
      gerarOvelha(x,-2,z , 1,1,1);
	glPopMatrix();
}

void gerarCercaCompleta(){
	const float deslocXCercado=2, deslocZCercado=1.9;
	
	set_Color_and_Materialfv(1, 1, 0);
	
	/// Trechos maiores
	gerarCerca(deslocXCercado,0,0 , 1,1,1);	
	gerarCerca(deslocXCercado,0,deslocZCercado , 1,1,1);	
	
	/** Atencao! os trechos abaixo foram feitos 
	 * baseado no tamanho da cerca original. Ou seja,
	 * cuidado ao mudar valores aparentemente sem significado...
	 */
	/// menor 1, de CIMA
	glPushMatrix();
	  glTranslatef (6.5, -1.0, 5.5);
	  glScalef (0.2, 0.2, 2);
	  glutSolidCube (1.0);
	glPopMatrix();
	/// menor 1, de BAIXO
	glPushMatrix();
	  glTranslatef (6.5, -1.3, 5.5);
	  glScalef (0.2, 0.2, 2);
	  glutSolidCube (1.0);
	glPopMatrix();
	
	/// menor 2, de CIMA
	glPushMatrix();
	  glTranslatef (-2.5, -1.0, 5.5);
	  glScalef (0.2, 0.2, 2);
	  glutSolidCube (1.0);
	glPopMatrix();	
	/// menor 2, de BAIXO
	glPushMatrix();
	  glTranslatef (-2.5, -1.3, 5.5);
	  glScalef (0.2, 0.2, 2);
	  glutSolidCube (1.0);
	glPopMatrix();	
}

void gerarTodasCenouras(){
	short int x, z;
	const int numX=3, numZ=5;
	
	for(x=0; x<numX; x++){
		for(z=0; z<numZ; z++){
			gerarCenoura(4.0+x, -3.1, -2+z , 0.7,0.7,0.7);
		}
	}	
}
