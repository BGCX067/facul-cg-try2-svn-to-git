/* 
 * File:   Utils.h
 * Author: filiagees
 *
 * Created on March 10, 2013, 1:24 AM
 * 
 * Essa classe calcula automaticamente valores randômicos. 
 * Seja para uma coordenada adequada para a janela, ou uma cor RGBA num range 
 * correto.
 * As funções estão como 'static' para não precisar um objeto para usá-las.
 */

#ifndef UTILS_H
#define	UTILS_H

#include "config.h"
#include <algorithm>    // std::find

#define EST_INIT 1
#define EST_HUD_DRAW 2
#define EST_START_BUTTON 3
#define EST_SHOWING 4
#define EST_GAME_RUNNING 5
#define EST_WIN_SCREEN 6
#define EST_LOSE_SCREEN 7
 
class Utils {
public:
    Utils();
    Utils(const Utils& orig);
    virtual ~Utils();
    
    static void rand_color(float *r, float *g, float *b, float *a);
    static int rand_num(int min, int max);
    static void rand_coord(int *x, int *y);
    static unsigned int get_mseconds();

    static bool contains_value(vector<int> v, int valor);
    
#ifdef HAVE_OPENGL    
    static void printTxt(GLfloat x, GLfloat y, const char *string, float size);
#endif
    static void get_window_size(int *width, int *height);
    static string num2str(int number);
private:

};

#endif	/* UTILS_H */

