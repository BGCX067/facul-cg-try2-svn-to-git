/* 
 * File:   MainGameLoop.h
 * Author: Padula
 *
 * Created on March 19, 2013, 21:17 PM
 */
 
#ifndef MAIN_GAME_LOOP_H
#define	MAIN_GAME_LOOP_H


class MainGameLoop
{

	static unsigned int state;
	


public:

	MainGameLoop();
	MainGameLoop(const MainGameLoop& orig);
    virtual ~MainGameLoop();
	
	/* static void runState (); */
        void runState(){
            switch (state)
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

	
	static void runInit();
	static void runHudDraw();
 	static void runStartButton();
	static void runShowing();
	static void runGameRunning();
	static void runWinScreen();
	static void runLoseScreen();
	
	/*
	* static int getState();
	* static void setState(int);
        */
        int getState()
        { return MainGameLoop::state; };

        void setState(int est)
        { MainGameLoop::state = est; }

	
};



#endif