/* 
 * File:   Utils.cpp
 * Author: filiagees
 * 
 * Created on March 10, 2013, 1:24 AM
 */

#include "config.h"
#include "Utils.h"
#include "GameData.h"

//static 
/** Calcula uma cor randomica */
void Utils::rand_color(float *r, float *g, float *b, float *a){
    (*r) = rand_num(0,255)/255.0;
    (*g) = rand_num(0,255)/255.0;
    (*b) = rand_num(0,255)/255.0;
    (*a) = rand_num(0,255)/255.0;
};

//static 
/**
 * Retorna um inteiro randomico dentro do intervalo min~max, inclusivo.
 * Espera que srand() ja tenha sido chamada.
 */
int Utils::rand_num(int min, int max){
    int randNum;
    randNum = rand() % (max-min+1);     //< '+1' torna o intervalo inclusivo
    return min+randNum;
};

//static 
/** 
 * Gera coordenada randomica dentro do tamanho da janela OpenGL.
 * @warning 
 *      Talvez se torne um problema, uma figura grande ser desenhada muito à
 * direita, pois a janela vai cortá-la.
 */
void Utils::rand_coord(int *x, int *y){
///*  /// Bloco para posicionamento aleatorio
    int ogl_width, ogl_height;
    //int borda=60;       //< fica NO MAX 'a BORDA px da borda
    int xAux, yAux;
    
    Utils::get_window_size(&ogl_width, &ogl_height);
    
    /** Enquanto posicao randomizada ja tiver sido ocupada, re-randomize
     * ATENCAO! Cuidade com a ineficiencia, e loop infinitos, se adicionar muitos elementos
     * ou nao apagar os antigos.
     */ 
    do{
        /// referencia no ponto Inf Dir.
        xAux = rand_num(0+50, ogl_width);
        xAux = xAux - (xAux % 50);
        yAux = rand_num(0+50, ogl_height-50); //< '-50' é menu superior
        yAux = yAux - (yAux % 50);
    }while( Utils::contains_value(GameData::coordXusadas, xAux)==true 
         && Utils::contains_value(GameData::coordYusadas, yAux)==true
    ); /**> o ideal é usar uma E.D. que use as coord X Y como chave...  */
    
    /// marca como ja ocupado
    GameData::coordXusadas.push_back(xAux);
    GameData::coordYusadas.push_back(yAux);
    
    /// retorna
    (*x) = xAux;
    (*y) = yAux;
    
    /// debug
    //static int i=1;
    //cerr << i++ <<": ("<< *x <<","<< *y <<")" <<endl;
    
//*/    
    /* Ainda NAO esta perfeita!
     GameData::get_randCoord_fromIndex(x,y); */
};

bool Utils::contains_value(vector<int> v, int valor){
    if( std::find(v.begin(), v.end(), valor) != v.end() ){
        return true;
    }else{
        return false;
    }
}

#ifdef HAVE_OPENGL    
void Utils::printTxt(GLfloat x, GLfloat y, const char *string, float size){
  int len, i;
  glPushMatrix();
       glTranslatef(x, y, 0);
       glScalef(0.1f*size, 0.1f*size, 0.1f*size);
       // Extraindo comprimento da string
       len = (int)strlen(string);
       for (i=0; i<len; i++)
       glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, string[i]);
  glPopMatrix();
};
#endif

//static 
void Utils::get_window_size(int *width, int *height){
#ifdef HAVE_OPENGL    
    (*width) = glutGet(GLUT_WINDOW_WIDTH);
    (*height) = glutGet(GLUT_WINDOW_HEIGHT);
#endif
};

//static 
unsigned int get_mseconds(){
    struct timespec ts;
    int ret;
    if( (ret=clock_gettime(CLOCK_MONOTONIC,&ts)) != 0) {
        cerr << "Erro! @" << __FILE__ << ":" << __LINE__ << " clock_gettime() retornou: " << ret;
    }
};

//static
string Utils::num2str(int number){
   /** original FROM: http://www.cplusplus.com/forum/beginner/7777/ */
   stringstream ss;//create a stringstream
   
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}