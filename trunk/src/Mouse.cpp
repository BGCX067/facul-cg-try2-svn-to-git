/* 
 * File:   Mouse.cpp
 * Author: Padula
 *
 * Created on March 16, 2013, 14:03 PM
 */

 
 
#include "Mouse.h"
 
 
void Mouse::mainLoop(int button, int state, int x, int y)
{
	
	
	
	switch(estado)
	{
		
			
		case EST_START_BUTTON:
			this->clickAtStartButton(button,  state,  x,  y);
			break;
		
		case EST_GAME_RUNNING:
			this->clickAtGameRunning(button,  state,  x,  y);
			break;
		
		case EST_WIN_SCREEN:
			this->clickAtWinScreen(button,  state,  x,  y);
			break;
			
		case EST_LOSE_SCREEN:
			this->clickAtLoseScreen(button,  state,  x,  y);
			break;
	}
	
};


void Mouse::setEstado(unsigned int est)
{
	this->estado = est;
};

void Mouse::clickAtStartButton (int button, int state, int x, int y)
{
	
	
};

void Mouse::clickAtGameRunning (int button, int state, int x, int y)
{

};

void Mouse::clickAtWinScreen (int button, int state, int x, int y)
{

};

void Mouse::clickAtLoseScreen (int button, int state, int x, int y)
{
	this->clickAtWinScreen ( button,  state,  x,  y);
};
