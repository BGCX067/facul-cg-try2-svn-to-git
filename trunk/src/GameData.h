/* 
 * File:   GameData.h
 * Author: Padula
 *
 * Created on March 19, 2013, 22:40 PM
 */
 
#ifndef GAME_DATA_H
#define	GAME_DATA_H

#include "config.h"
#include "Figura_grafica.h"
#include "Utils.h"

#include <math.h>

class GameData
{
public:    
	//usar algum limitante abaixo:
	static int tempoRestante;
	static int vidas;
	static int totalClicks;
	static int pontos;
        static int estado;
        static bool fig_invisiveis;
        static string tarefaAtual;
        static int numTempoZerado;
        	
	static vector<Figura_grafica> vetFiguras;
        
        /** INDICES das coordenadas ocupadas por quadrados 
         * No fomato: 0 = primeira valida = (x=0 , y=0)
         *            1 = (x=50 , y=0) <- considerando espacamento de 50px
         */
        /// ESTRUTURA ATUALMENTE NAO USADA
        static vector<int> posUsadasIndex; 
        
        static vector<int> coordXusadas;
        static vector<int> coordYusadas;
	
	GameData();
	GameData(const GameData& orig);
        virtual ~GameData();
	
        //static void reset();
        static void reset()
        {
            GameData::vidas = FULL_LIFE; //colocar na config.h
            GameData::tempoRestante = FULL_TIME; //colocar na config.h
            GameData::tarefaAtual = "Memorize os objetos!";
            GameData::totalClicks = 0;
            GameData::pontos = 0;
            GameData::estado = EST_INIT;
            GameData::numTempoZerado = 0;
        };
        
        static void decrementa_tempo(int CBdata){
            if( 0 < GameData::tempoRestante ){
                GameData::tempoRestante--;
            }
            
            if( GameData::tempoRestante == 0 && GameData::numTempoZerado < 2){
                GameData::numTempoZerado++;
                GameData::fig_invisiveis = true;
                GameData::tempoRestante = 2*FULL_TIME;
                GameData::estado = EST_GAME_RUNNING;
                GameData::tarefaAtual = "Clique nas figuras!";
            }
            
            if( GameData::numTempoZerado == 2 ){
                GameData::fig_invisiveis = false;
                GameData::tempoRestante = 0;
                GameData::estado = EST_LOSE_SCREEN;
                GameData::tarefaAtual = "Jogo encerrado!";
            }
            //cerr << "GameData::tempoRestante=" << GameData::tempoRestante << endl;
            glutTimerFunc(1000, GameData::decrementa_tempo, 0);
            glutPostRedisplay();
        };
        
        static void clicou_figura(){
            GameData::pontos++;
        };
        
        /** Estar no vetor significa nao estar usada.
         * Muito util para randomizar posicoes validas 
         */
        static void set_nenhuma_pos_usadas(){
            int i, width, height;
            int numCols, numLins;
            /// pode ser maior que o real, se menor, havera sobreposicao 
            int ladoQuadrado = 50; 
            
            Utils::get_window_size(&width, &height);
            numCols = width / ladoQuadrado; ///< ~ 19 e 9, para janela de 950 x 500
            numLins = height / ladoQuadrado;
                    
            //cerr << "numCols=" << numCols << " | numLins=" << numLins << endl;
            for(i=0; i<(numCols*numLins); i++){
                GameData::posUsadasIndex.push_back( i );
            }
        };
        
        /**
         * Retorna coordenadas X,Y para um quadrado, dentro dum grid para nao
         * se sobreporem. 
         * Checa vector GameData::posUsadasIndex para nao colocar dois na mesma
         * posicao.
         * @param x Coordenada x do objeto
         * @param y Coordenada y do objeto
         */
        
        /** @WARNING! Ainda nao esta funcionando perfeitamente!! */
        static void get_randCoord_fromIndex(int *x, int *y){
            static bool foiInicializado=false;
            int posSorteada, numSorteado;
            int width, height;
            int numCols, numLins, ladoQuadrado=50;
            /// numeros inteiros, 0 e 0 = primeiro, etc.. Sera multiplicado pelo espacamento
            int posX, posY; 
            
            if( foiInicializado == false ){
                GameData::set_nenhuma_pos_usadas();
                foiInicializado = true;
            }
            
            /// escolhe um elem do vector randomicamente 
            //cout << "GameData::posUsadasIndex.size()-1=" << GameData::posUsadasIndex.size()-1 << endl;
            posSorteada = Utils::rand_num( 0, GameData::posUsadasIndex.size()-1 );
            numSorteado = GameData::posUsadasIndex.at(posSorteada);
                    
            /// remove do vector para nao mais ser selecionado o mesmo
            GameData::posUsadasIndex.erase(GameData::posUsadasIndex.begin() +posSorteada);
            
            /// calcula a posicao correspondente            
            Utils::get_window_size(&width, &height);
            numCols = width / ladoQuadrado; // grid 19 x 9, para janela de 950 x 500
            numLins = (height / ladoQuadrado) -1; //'-1' faz nao chegarem ao topo
            
            posX = ceil(numSorteado/numLins);
            posY = numSorteado % numLins;
            
            /// retorna
            (*x) = 50 * (posX+1);   //< a coordenada é para ponto Inf. Dir.
            (*y) = 50 * (posY+1);
            
            /// debug
            static int i=1;
            cerr << i++ <<": ("<< *x <<","<< *y <<")" <<endl;
        }
};

/* Insira o trecho abaixo no inicio do '.cpp', e SÓ LÁ. Caso contrário, surgirá
 * erros 'undefined reference'. Parece ser porque o compilador não sabe quanto espaço
 * precisa reservar, ou algo similar.
 * Maiores informações consulte:
 *     http://stackoverflow.com/questions/272900/c-undefined-reference-to-static-class-member
 ******
  "
       C++ I love U, but somethimes... :\ 
  "
*/

/* 
int GameData::tempoRestante = FULL_TIME;
int GameData::vidas = FULL_LIFE; 
int GameData::totalClicks = 0;
int GameData::pontos = 0;
*/


#endif