/* 
 * File:   main.cpp
 * Author: filiagees
 *
 * Created on March 9, 2013, 11:21 PM
 */

/**
 * TODO
 * - Randomizar posicoes das figuras geometricas FORA das Viewports!
 * - Fazer a classe HorizViewports de fato usar Viewports..
 * - Centralizar funcoes de drawing, e de widgets... 
 * - Centralizar dado na GameData
 */

#include "src/config.h"

#include "src/Controle_gui.h"
#include "src/Utils.h"
#include "src/Quadrado.h"
#include "src/GameData.h"
#include "src/Image.h"


void displayCallBack();

Controle_gui *gui = new Controle_gui();

void demo_mouseLoop(int button, int state, int x, int y){
    int janWidth, janHeight;
    
    if( state == GLUT_DOWN && GameData::estado != EST_LOSE_SCREEN ){
        /* coordenada do click parece iniciar no canto superior esq da janela,
         * enquanto a dos objetos parecem iniciar no canto inf esquerdo.!!
         */
        Utils::get_window_size(&janWidth, &janHeight);
        y = janHeight - y;      
        
        /// cliques nao tem mais efeito algum!
        if( GameData::estado != EST_INIT ){
            if( gui->try_erase_figure_at(x,y) == true ){
                // apagou algo:
                glutPostRedisplay();
            }
        }
        GameData::totalClicks++;
    }
}

void keyboard(unsigned char key, int x, int y){
    //printf("precionado tecla = %d\n", key);
    switch( key ){
        case 27: 
            exit(0);
        break;
        case 'r':
            if( GameData::estado == EST_LOSE_SCREEN ){
                GameData::reset();     
                gui->delete_figures();     
                gui->criar_figuras(); 
                //glutPostRedisplay();  
            }
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
   
    srand( time(0) );
    
    gui->init(argc, argv, displayCallBack);   
      
    gui->criar_figuras();
    glutMouseFunc(demo_mouseLoop);
    glutKeyboardFunc(keyboard);
    
    GameData::reset();
    Image::textures();
    
    glutTimerFunc(1000, GameData::decrementa_tempo, 0); //< chamada "recursiva"
    gui->mainLoop();    
    return 0;
}

void displayCallBack(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT), -2.0, 100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 20 , 0.0, 0.0, 0.0 , 0.0, 1.0, 0.0);
  
  glColor3f(1,1,1);
  gui->draw_figures();
  
  gui->draw_all_hvp();
  
  /// Funcoes estáticas tem a mais a ver com a estrutura do OpenGL...
  Controle_gui::exibe_alertas();
      
  /// Mostra imagens
//
  Image::display();
  
  glutSwapBuffers();
}
