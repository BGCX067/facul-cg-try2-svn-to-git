/* 
 * File:   Controle_gui.h
 * Author: filiagees
 *
 * Created on March 9, 2013, 11:30 PM
 */

#ifndef CONTROLE_GUI_H
#define	CONTROLE_GUI_H

#include "config.h"
#include "HorizViewport.h"
#include "Figura_grafica.h"

class Controle_gui {
public:
    Controle_gui();
    Controle_gui(const Controle_gui& orig);
    virtual ~Controle_gui();
    void init(int argc, char** argv, void(*callback)(void) );
    static void desenha_menu();
    void reshape(int width, int height);
    void mainLoop(); //< ficara preso aqui, por isso foi tirado do construtor
    
    string getTitulo();
    void setTitulo(string Titulo);
    void new_hVp(float percIni, float percFim);
    //static 
    void draw_all_hvp();
    void criar_figuras();
    bool try_erase_figure_at(int x, int y);
    void draw_figures();
    static void exibe_alertas();
    int get_numElem();
    void delete_figures();
    
private:
    string titulo;
    vector<HorizViewport*> hVp;
    vector<Figura_grafica*> figuras;
};

#endif	/* CONTROLE_GUI_H */

