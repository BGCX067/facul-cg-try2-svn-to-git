/* 
 * File:   Quadrado.h
 * Author: filiagees
 *
 * Created on March 10, 2013, 12:47 AM
 */

#ifndef QUADRADO_H
#define	QUADRADO_H

#include "Figura_grafica.h"

/** necessario herdar como PUBLIC, senao a Controle_gui::criar_figuras() nao
 * conseguira' adicionar o quadrado para os vetores de figuras!!
 */
class Quadrado : public Figura_grafica {        
public:
    Quadrado(); 
    Quadrado(const Quadrado& orig);
    virtual ~Quadrado();
    bool is_point_inside(const unsigned int x, const unsigned int y);
    void draw();

    void print_pos();
    
    int xSupEsq, ySupEsq;
    int xInfDir, yInfDir; 
    
private:
    float cor[4];       //< RGBA
};

#endif	/* QUADRADO_H */

