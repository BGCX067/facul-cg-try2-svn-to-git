/* 
 * File:   Quadrado.cpp
 * Author: filiagees
 * 
 * Created on March 10, 2013, 12:46 AM
 */

#include "config.h"

#include "Quadrado.h"
#include "Figura_grafica.h"
#include "Utils.h"
#include "GameData.h"

/** Se o construtor for chamado, mas glMainLoop nao, o programa deve encerrar
 * com exit-code 1 (run FAILED)
 */
Quadrado::Quadrado() {
    int lado, minLado;
    
    this->foi_clicado = false;
    
#if 1
    /// Randomiza posicao
    //lado = Utils::rand_num(50, 150); 
    lado = 50;
    Utils::rand_coord( &this->xInfDir, &this->yInfDir);        
    this->xSupEsq = this->xInfDir - lado;
    this->ySupEsq = this->yInfDir - lado;
#else
    // Posicao estatica (testes)
    xSupEsq = 0;
    ySupEsq = 50;
    xInfDir = 50;
    yInfDir = 0;
#endif
    
    Utils::rand_color(&this->cor[0], &this->cor[1], &this->cor[2], &this->cor[3]);
    this->draw();
}

Quadrado::Quadrado(const Quadrado& orig) {
}

Quadrado::~Quadrado() {
}

bool Quadrado::is_point_inside(const unsigned int x, const unsigned int y){
    if( (this->xSupEsq < x) && (x < this->xInfDir)  //< horizontal
    &&  (this->ySupEsq < y) && (y < this->yInfDir)  //< vertical
    ){
        return true;
    }else{
        return false;
    }
}

void Quadrado::draw(){
#ifdef HAVE_OPENGL
    if( GameData::fig_invisiveis == false || this->foi_clicado == true ){
        glColor3f(this->cor[0], this->cor[1], this->cor[2]);
    }else{
        glColor3f(0, 0, 0);     ///< suposta cor default do fundo.
    }
    glBegin(GL_POLYGON); // Start drawing a point primitive  
        glVertex2i(this->xSupEsq, this->ySupEsq); // superior esquerda
        glVertex2i(this->xInfDir, this->ySupEsq); // sup dir
        glVertex2i(this->xInfDir, this->yInfDir); // inferior direita
        glVertex2i(this->xSupEsq, this->yInfDir); // inf esq
    glEnd();
#else
    cout << "Warning! Undefined: '__GLUT_H__'";
#endif
};

void Quadrado::print_pos(){
    cout << "\nxSupEsq=" << this->xSupEsq
         << " xInfDir=" << this->xInfDir
         << " | ySupEsq=" << this->ySupEsq 
         << " yInfDir=" << this->yInfDir;
};
