/* 
 * File:   Image.cpp
 * Author: filiagees
 * 
 * Created on March 29, 2013, 2:09 PM
 */

#include "Image.h"
#include "Utils.h"
#include "HorizViewport.h"

/*
#include<unistd.h>
char the_path[256];
*/

/** O netbeans identifica correntamente o endereco da imagem com a instrucao a seguir,
 * mude conforme for necessario.
 */
//string path = "/home/filiagees/faculdade/atual, semestre/Computacao Grafica - ST765 B/f/";
string texnames = "logos/ppm/1_flip.ppm";

int Image::imgHeight;
int Image::imgWidth;

Image::Image() {
}

Image::Image(const Image& orig) {
}

Image::~Image() {
}

/* ppmRead: read a PPM raw (type P6) file.  The PPM file has a header
   that should look something like:

   P6
   # comment
   width height max_value
   rgbrgbrgb...

   where "P6" is the magic cookie which identifies the file type and
   should be the only characters on the first line followed by a
   carriage return.  Any line starting with a # mark will be treated
   as a comment and discarded.  After the magic cookie, three integer
   values are expected: width, height of the image and the maximum
   value for a pixel (max_value must be < 256 for PPM raw files).  The
   data section consists of width*height rgb triplets (one byte each)
   in binary format (i.e., such as that written with fwrite() or
   equivalent).

   The rgb data is returned as an array of unsigned chars (packed
   rgb).  The malloc()'d memory should be free()'d by the caller.  If
   an error occurs, an error message is sent to stderr and NULL is
   returned.

*/
unsigned char* 
Image::ppmRead(char* filename, int* width, int* height)
{
    FILE* fp;
    int i, w, h, d;
    unsigned char* image;
    char head[70];			/* max line <= 70 in PPM (per spec). */

    fp = fopen(filename, "rb");
    if (!fp) {
        perror(filename);
        return NULL;
    }

    /* grab first two chars of the file and make sure that it has the
       correct magic cookie for a raw PPM file. */
    fgets(head, 70, fp);
    if (strncmp(head, "P6", 2)) {
        fprintf(stderr, "%s: Not a raw PPM file\n", filename);
        return NULL;
    }

    /* grab the three elements in the header (width, height, maxval). */
    i = 0;
    while(i < 3) {
        fgets(head, 70, fp);
        if (head[0] == '#')		/* skip comments. */
            continue;
        if (i == 0)
            i += sscanf(head, "%d %d %d", &w, &h, &d);
        else if (i == 1)
            i += sscanf(head, "%d %d", &h, &d);
        else if (i == 2)
            i += sscanf(head, "%d", &d);
    }

    /* grab all the image data in one fell swoop. */
    image = (unsigned char*)malloc(sizeof(unsigned char)*w*h*3);
    fread(image, sizeof(unsigned char), w*h*3, fp);
    fclose(fp);

    *width = w;
    *height = h;
    return image;
}

//static
void
Image::textures(void)
{
    int w, h, i;
    GLubyte* texture;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* XXX - RE bug - must enable texture before bind. */
    glEnable(GL_TEXTURE_2D);

    //for (i = 0; i < 4; i++) {
    for (i = 0; i < 1; i++) { //===========================================
      /*getcwd(the_path, 255);
        cout << "getcwd()=" << the_path << " ("<< __FILE__ ":"<< __LINE__ <<")"; */  
        
        glBindTexture(GL_TEXTURE_2D, i+1);
        texture = (GLubyte*)ppmRead( (char *)texnames.c_str(), &Image::imgWidth, &Image::imgHeight);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Image::imgWidth, Image::imgHeight,
            GL_RGB, GL_UNSIGNED_BYTE, texture);
        free(texture);
    }

    /* XXX - RE bug - must enable texture before bind. */
    glDisable(GL_TEXTURE_2D);
}

//static 
void 
Image::display(void){
    //static int i, start, last, end, step;
    int imgXInfEsq, imgYInfEsq;
    int width, height, bgtexture=1;
    int deslEsq = 4; //< poe a img Npx para a esq

    Utils::get_window_size(&width, &height);

    /// posiciona a img de modo que toque o canto Inf Dir
    /// o '-1' é para parecer dentro do HUD
    imgXInfEsq = width - Image::imgWidth - HorizViewport::xdesl -1;
    imgYInfEsq = height- Image::imgHeight- HorizViewport::ydesl -1;
    
    /// "Desenha" imagem, propriamente.
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bgtexture);
    glColor3ub(255, 255, 255); //< procure deixar como BRANCO
    glBegin(GL_QUADS);
      glTexCoord2i(0, 0);      glVertex2i( imgXInfEsq -deslEsq , imgYInfEsq );
      glTexCoord2i(1, 0);      glVertex2i( imgXInfEsq+Image::imgWidth -deslEsq , imgYInfEsq );
      glTexCoord2i(1, 1);      glVertex2i( imgXInfEsq+Image::imgWidth -deslEsq, imgYInfEsq+Image::imgHeight );
      glTexCoord2i(0, 1);      glVertex2i( imgXInfEsq -deslEsq , imgYInfEsq+Image::imgHeight );
    glEnd();
    glDisable(GL_TEXTURE_2D);

};