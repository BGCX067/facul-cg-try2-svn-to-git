/* 
 * File:   Figura_grafica.cpp
 * Author: filiagees
 * 
 * Created on March 10, 2013, 12:31 AM
 */

#include "Figura_grafica.h"
#include "GameData.h"

Figura_grafica::Figura_grafica() {
}

Figura_grafica::Figura_grafica(const Figura_grafica& orig) {
}

Figura_grafica::~Figura_grafica() {
}

void Figura_grafica::use_random_pos(bool b){
    this->randomize_pos = b;
};

bool Figura_grafica::is_random_pos(){
    return this->randomize_pos;
};

void Figura_grafica::set_tam_min(int larg, int alt){
    this->minLargura = larg;
    this->minAltura = alt;
};

void Figura_grafica::set_tam_max(int larg, int alt){
    this->maxLargura = larg;
    this->maxAltura = alt;
};

void Figura_grafica::was_clicked(){
    if( GameData::estado != EST_INIT && this->foi_clicado == false){
        this->foi_clicado = true;
        GameData::clicou_figura();
    }
};
