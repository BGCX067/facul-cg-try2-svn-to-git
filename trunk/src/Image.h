/* 
 * File:   Image.h
 * Author: filiagees
 *
 * Created on March 29, 2013, 2:09 PM
 */

/**
 * All source code here is based on Nate Robins's work (1997). He deserves credits and
 * blame ;) check specifically the file 'maiden.c'
 */


#ifndef IMAGE_H
#define	IMAGE_H

#include "config.h"

#include <cstdio>

class Image {
public:
    Image();
    Image(const Image& orig);
    virtual ~Image();
    
    static void textures(void);
    static void display(void);
    
private:
    static int imgHeight;
    static int imgWidth;
    static unsigned char* ppmRead(char* filename, int* width, int* height);

};

#endif	/* IMAGE_H */

