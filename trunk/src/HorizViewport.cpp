/* 
 * File:   HorizViewport.cpp
 * Author: filiagees
 * 
 * Created on March 16, 2013, 5:25 PM
 */

#include "HorizViewport.h"
#include "Utils.h"
#include "GameData.h"

int HorizViewport::xdesl; 
int HorizViewport::ydesl; 

/**
 * Cria um viewport horizontal.
 * @param percIni Valor 0~1 onde INICIA a viewport
 * @param percFim Valor 0~1 onde TERMINA a viewport
 */
HorizViewport::HorizViewport(float percIni, float percFim) {
    this->xdesl = 5;
    this->ydesl = 5; 
    this->percIni = percIni;
    this->percFim = percFim;
    this->set_color(1,1,1, 0); //< default: branco
#ifdef HAVE_OPENGL 
    this->objTipo =  GL_LINE_LOOP; // GL_POLYGON;
#endif    
}

HorizViewport::HorizViewport(const HorizViewport& orig) {
}

HorizViewport::~HorizViewport() {
}

void HorizViewport::set_color(float r, float g, float b, float a){
    this->cor[0] = r;
    this->cor[1] = g;
    this->cor[2] = b;
    this->cor[3] = a;
};

void HorizViewport::get_color(float *r, float *g, float *b, float *a){
    if( GameData::estado == EST_LOSE_SCREEN ){
        //vermelho
        (*r) = 1;
        (*g) = (*b) = (*a) = 0;
    }else{
        (*r) = this->cor[0];
        (*g) = this->cor[1];
        (*b) = this->cor[2];
        (*a) = this->cor[3];                
    }
};

void HorizViewport::set_desl_x(int desX){
    this->xdesl = desX;
};

void HorizViewport::set_desl_y(int desY){
    this->ydesl = desY;
};

/* Essa implementacao atual nao é robusta o bastante para lidar com as "novas
 * coordenadas" da viewport
 */
void HorizViewport::print_strings(){
#ifdef HAVE_OPENGL  
    int width, height;
    int yLinhaSup, yLinhaInf;
    string msg;
        
    Utils::get_window_size(&width, &height);
    yLinhaInf =(int) (this->percIni * (float)height);
    yLinhaSup = (int) (this->percFim * (float)height);
    
    msg = "Pontuacao: "+Utils::num2str( GameData::pontos );            
    msg +=" | Tempo: "+Utils::num2str( GameData::tempoRestante );
    msg +=" | Cliques: "+Utils::num2str( GameData::totalClicks);
    msg +=" | "+GameData::tarefaAtual;
    
    Utils::printTxt( 0 +5*this->xdesl, yLinhaSup -5*this->ydesl , msg.c_str(), 1);
#endif
}

void HorizViewport::draw(){
#ifdef HAVE_OPENGL
    /// inicio = estar mais embaixo (inferior)...
    float r, g, b, a;
    int width, height;
    int yLinhaSup, yLinhaInf;
    int viewpTamX, viewpTamY, viewpPosX, viewpPosY;
       
    Utils::get_window_size(&width, &height);
    yLinhaSup = (int) (this->percFim * (float)height);
    
    if( this->percIni == -1 ){
        yLinhaInf = yLinhaSup - 42;    //< altura ideal para a imagem usada
    }else{
        yLinhaInf =(int) (this->percIni * (float)height);
    }
    
    /// Viewport
    glEnable(GL_SCISSOR_TEST);
    
    viewpPosX = 0 +this->xdesl;
    viewpPosY = yLinhaInf +this->ydesl;
    viewpTamX = (width -this->xdesl)-(0 +this->xdesl);
    viewpTamY = (yLinhaSup -this->ydesl)-(yLinhaInf +this->ydesl);
    
    glViewport( viewpPosX, viewpPosY, viewpTamX, viewpTamY);
    glScissor( viewpPosX, viewpPosY, viewpTamX, viewpTamY);      
    
    this->get_color(&r, &g, &b, &a);
    glColor3f(r, g, b);
    glBegin( this->objTipo );
 /*    
      glVertex2i( 0 +this->xdesl , yLinhaInf +this->ydesl ); //< inf esq
      glVertex2i( width -this->xdesl , yLinhaInf +this->ydesl ); //< inf dir
      glVertex2i( width -this->xdesl, yLinhaSup -this->ydesl ); //< sup dir
      glVertex2i( 0 +this->xdesl, yLinhaSup -this->ydesl ); //< sup esq
 */     
      glVertex2i( 1 , 1 ); //< inf esq
      glVertex2i( width -this->xdesl , 1 ); //< inf dir
      glVertex2i( width -this->xdesl, yLinhaSup -this->ydesl ); //< sup dir
      glVertex2i( 1 , yLinhaSup -this->ydesl ); //< sup esq
    glEnd();
        
    /// Volta para o original
    glViewport( 0, 0, width, height ); 
    glScissor( 0, 0, width, height ); 
    
    /** Nate Robins em 'multiview.c', 1997
     * Escreve texto ANTES de desenhar as viewports, portanto, aqui faco o equivalente */
    HorizViewport::print_strings();
    
    glDisable(GL_SCISSOR_TEST);  
#else
    cout << "Erro! OpenGL undefined";
#endif
}

#ifdef HAVE_OPENGL    
void HorizViewport::set_tipo_objeto(GLenum tipo){
    this->objTipo = tipo;
};
#endif
