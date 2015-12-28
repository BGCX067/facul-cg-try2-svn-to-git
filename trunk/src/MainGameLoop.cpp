/* 
 * File:   MainGameLoop.cpp
 * Author: Padula
 *
 * Created on March 19, 2013, 21:17 PM
 */
 
#include "Utils.h"
#include "MainGameLoop.h"


/*
void MainGameLoop::runState(){

	switch (MainGameLoop::state)
	{
		
		case EST_INIT:
			//incializa o ambiente para openGl

		case EST_HUD_DRAW:
			//desenha a HUD
			break;
		case EST_START_BUTTON:
			//desenha na viewport do jogo um txt + um botao start
			break;
		
		case EST_SHOWING:
			//desenha as figuras a serem mostradas por um tempo
			break;
			
		case EST_GAME_RUNNING:
			//tela preta, recebe clikes, testa condicao de termino
			break;
		
		case EST_WIN_SCREEN:
			//tela de vitoria com txt + um botao replay
			break;
			
		case EST_LOSE_SCREEN:
			//tela de perdedor com + um botao de replay
			break;
		
	}
	
	
};
*/
        
void MainGameLoop::runInit(){

};

void MainGameLoop::runHudDraw(){

};

void MainGameLoop::runStartButton(){

};

void MainGameLoop::runShowing(){

};

void MainGameLoop::runGameRunning(){

};

void MainGameLoop::runWinScreen(){

};

void MainGameLoop::runLoseScreen(){

};

/*
int MainGameLoop::getState()
{ return MainGameLoop::state; };

void MainGameLoop::setState(int est)
{ MainGameLoop::state = est; }
*/

