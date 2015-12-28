/* 
 * File:   Mouse.h
 * Author: Padula
 *
 * Created on March 16, 2013, 14:01 PM
 */

#ifndef MOUSE_H
#define	MOUSE_H

#include <GL/glut.h>
#include "Utils.h"



class Mouse {
private:
	unsigned int estado;
	
public:

    Mouse();
	Mouse(const Mouse& orig);
    virtual ~Mouse();
	
	void mainLoop(int button, int state, int x, int y);
	
	void setEstado(unsigned int est);
	
private :
	
	void clickAtStartButton (int button, int state, int x, int y);
	
	void clickAtGameRunning (int button, int state, int x, int y);
	
	void clickAtWinScreen (int button, int state, int x, int y);
	
	void clickAtLoseScreen (int button, int state, int x, int y);
	
};

#endif
