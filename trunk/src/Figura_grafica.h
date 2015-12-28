/* 
 * File:   Figura_grafica.h
 * Author: filiagees
 *
 * Created on March 10, 2013, 12:31 AM
 */

#ifndef FIGURA_GRAFICA_H
#define	FIGURA_GRAFICA_H

#include <GL/glut.h>

class Figura_grafica {
public:
    Figura_grafica();
    Figura_grafica(const Figura_grafica& orig);
    virtual ~Figura_grafica();
    
    /**
     * Classes derivadas devem implementá-la, de diferentes maneiras. ;)
     * @param x Coordenada x
     * @param y Coordenada y
     * @return Se a coordenada está dentro da figura
     */
    virtual bool is_point_inside(const unsigned int x, const unsigned int y) = 0;
    
    /**
     * O tamanho min/max corresponde ao menor e maior retangulo que pode conter
     * a figura geometrica.
     * O objetivo é gerar retangulos de tamanhos variados, sem nenhum ser 
     * demasiadamente grande/pequeno. Assim como facilitar o cálculo se o objeto
     * foi clicado.
     */
    void set_tam_min(int larg, int alt);
    void set_tam_max(int larg, int alt);
    void get_tam_min(int *larg, int *alt);
    void get_tam_max(int *larg, int *alt);
    
    /**
     * Deve-se randomizar o tamanho? Será limitado pelos valores das 'set_tam_*()'
     * @param b Opção desejada
     */
    void use_random_pos(bool b);
    bool is_random_pos();
    
    /** Forca o desenho da figura */
    virtual void draw() = 0;
    virtual void print_pos() = 0;
    
    bool foi_clicado;
    void was_clicked();
    
private:
    bool randomize_pos; //< novas figuras devem auto-calcular posiçoẽs?
    bool randomize_tam;
    int minLargura;
    int maxLargura;
    int minAltura;
    int maxAltura;
};

#endif	/* FIGURA_GRAFICA_H */

