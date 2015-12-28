/* 
 * File:   Controle_gui.cpp
 * Author: filiagees
 * 
 * Created on March 9, 2013, 11:30 PM
 */

#include "Controle_gui.h"
#include "Utils.h"
#include "Quadrado.h"
#include "GameData.h"

Controle_gui::Controle_gui() {
    this->setTitulo("Projeto 1 - Jogo do Clique - Enrique & Lucas (ESC para sair)");
    this->new_hVp( -1 , 1.0);   //< estabelece altura default
}

Controle_gui::Controle_gui(const Controle_gui& orig) {
}

Controle_gui::~Controle_gui() {
    int i;
    for(i=0; i<this->hVp.size(); i++){
        delete this->hVp.at(i);
    }
}

void Controle_gui::setTitulo(string Titulo){
    this->titulo = Titulo;
}
string Controle_gui::getTitulo(){
    return this->titulo;
}

void Controle_gui::init(int argc, char** argv, void(*callback)(void) ){
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(950, 500);
    
    glutCreateWindow( this->getTitulo().c_str() );
    
    glutDisplayFunc( callback );
}

//static 
void Controle_gui::desenha_menu(){
    int width, height;
    
    Utils::get_window_size(&width, &height);
    
    /// desenha borda do menu inferior
    glColor3ub(255, 0, 0);
    glBegin(GL_LINE_LOOP);
      short xdesl=5, ydesl=5; //< diminui retangulo para nao ter lados invisiveis
      glVertex2i(0+xdesl , (height/4)-ydesl);
      glVertex2i(width-xdesl , (height/4)-ydesl);
      glVertex2i(width-xdesl, 0+ydesl);
      glVertex2i(0+xdesl, 0+ydesl);
    glEnd();
};

/// veja uso no demo 'area.c' de Nate Robins, 1997
void Controle_gui::reshape(int width, int height){

};

void Controle_gui::mainLoop(){
    glutMainLoop();  
}; 
/**
 * Criar nova Viewport horizontal. Cuidado para nao sobrepo-las.
 * Exemplo: se params=(0.0 , 0.5) sera criada no metada inicial da janela.
 * @param percIni Posicao de inicio da Viewport
 * @param percFim Posicao de fim da Viewport
 */
void Controle_gui::new_hVp(float percIni, float percFim){
    HorizViewport *horViewp = new HorizViewport(percIni, percFim);
    this->hVp.push_back( horViewp );
};

//static
void Controle_gui::draw_all_hvp(){
    int i;
    for(i=0; i<this->hVp.size(); i++){
        this->hVp.at(i)->draw();
    }
};

/** Por hora cria uma so, depois, criar varias */
void Controle_gui::criar_figuras(){
    int i;
    int numFigs=10; //< talvez valor relacionado a alguma fase
    Quadrado *qAux;
    
    for(i=0; i<numFigs; i++){
        qAux = new Quadrado();
        /** da para usar uma ESTRUTURA DE DADOS melhor aqui! **/
        this->figuras.push_back( qAux );
    }
};

/**
 * @warning Essa funcao retornará UM objeto apagado (só o primeiro) caso varias
 * figuras estejam se sobrepondo em um mesmo ponto. Cuidado! 
 *  Para corrigir isso, recomenda-se mudar a função: Utils::rand_coord()
 */
bool Controle_gui::try_erase_figure_at(int x, int y){
    int i;
    Figura_grafica *figAux;
    bool apagouFig = false; 
    
    /** da para usar uma ESTRUTURA DE DADOS melhor aqui! **/
    for(i=0; i<this->figuras.size(); i++ ){
        if( this->figuras.at(i)->is_point_inside(x,y) == true ){
            /* e' a melhor forma de fazer a remocao?? */
            this->figuras.at(i)->was_clicked();
            
            /** descomente as linhas abaixo, para as fig NAO reaparecerem */ 
              /*
            figAux = this->figuras.at(i);
            this->figuras.erase(this->figuras.begin()+i);
            delete figAux;      //< afinal, foi criado dinamicamente
              */
            
            apagouFig = true;
            /* Movido para Figura_grafica::was_clicked() - para nao contabilizar
            mesmo clique varias vezes */
            //GameData::clicou_figura();
        }     
    }
    
    return apagouFig; ///< default: nao apagou nada   
};

void Controle_gui::draw_figures(){
    int i;
    for(i=0; i<this->figuras.size(); i++ ){
        this->figuras.at(i)->draw();
    }
};

//static
void Controle_gui::exibe_alertas(){
    int width, height;

    if( GameData::estado == EST_LOSE_SCREEN ){
        Utils::get_window_size(&width, &height);
        /// desenha borda do menu inferior
        glColor3ub(255, 0, 0);
        glBegin(GL_LINE_LOOP);
          short xdesl=5, ydesl=5; //< diminui retangulo para nao ter lados invisiveis
          /* rocket science below ;) */
          glVertex2i( (width/4)+xdesl , (height/4)-ydesl);
          glVertex2i( 3*(width/4)-xdesl , (height/4)-ydesl);
          glVertex2i( 3*(width/4)-xdesl, 3*(height/4)+ydesl);
          glVertex2i( (width/4)+xdesl, 3*(height/4)+ydesl );
        glEnd();

        /* ..and keeps here ;) */
        Utils::printTxt( (((width/4)+xdesl) + (width/2))/2 , height/2, "FIM DE JOGO!", 1.3);
        Utils::printTxt( (((width/4)+xdesl) + (width/2))/2 , height/2-100 , "precione 'r' para novo jogo", 1);
    }
};

int Controle_gui::get_numElem(){
    return this->figuras.size();
}

void Controle_gui::delete_figures(){
    while( !this->figuras.empty() ){
        this->figuras.pop_back();
    }    
    
    /// MUITO importante! Liberar a lista de coordenadas ocupadas! X e Y
    while( !GameData::coordXusadas.empty() ){
        GameData::coordXusadas.pop_back();
    }    
    while( !GameData::coordYusadas.empty() ){
        GameData::coordYusadas.pop_back();
    }
};