/* 
 * File:   HorizViewport.h
 * Author: filiagees
 *
 * Created on March 16, 2013, 5:25 PM
 */

#ifndef HORIZVIEWPORT_H
#define	HORIZVIEWPORT_H

#include "config.h"
#include "Utils.h"

class HorizViewport {
public:
    HorizViewport(float percIni, float percFim);
    HorizViewport(const HorizViewport& orig);
    virtual ~HorizViewport();
    
    void set_color(float r, float g, float b, float a);
    void get_color(float *r, float *g, float *b, float *a);
    void set_desl_x(int desX);
    void set_desl_y(int desY);
    void draw();
    void print_strings();
    
    static int xdesl, ydesl;       //< estetica: desenha a borda um pouco para dentro
    
#ifdef HAVE_OPENGL    
    void set_tipo_objeto(GLenum tipo);
#endif
    
private:
    float percIni, percFim;
    float cor[4];
    
#ifdef HAVE_OPENGL
    GLenum objTipo;
#endif
};

#endif	/* HORIZVIEWPORT_H */

