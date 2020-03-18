/***************************************************
    To read a bmp image file and download
    it as a texture map for openGL
****************************************************/

#include <stdio.h>
#include <GL/glut.h>
#include "LoadBMP.h"


GLuint Load_BMP(char* FileName, int *out_width, int *out_height)
{
    int width, height, i, j, w3_n, w3;
    int pixel_offset;
    unsigned long k, kk, data_size;
    unsigned char *jdata;
    unsigned char temp;
    unsigned short int planes;          // number of planes in image (must be 1)
    unsigned short int bpp;             // number of bits per pixel (must be 24)

    unsigned int texture_id;

    FILE *fp = fopen(FileName, "rb");

    //if the bmp file doesn't load
    if (!fp) {
        fprintf(stderr, "[Load_BMP] Error reading BMP file %s!\n", FileName);
        return 0;
    }
    if (strcasecmp(".bmp",strrchr(FileName,'.')) != 0) {      // Sortie directement si l'extension est incorrecte
        fprintf(stderr, "[Load_BMP] Error reading BMP file %s : has not a .bmp extension !\n", FileName);
        fclose(fp);
        return 0;
    }
    // seek through the bmp header, up to the offset of pixel array
    fseek(fp, 10, SEEK_CUR);
    fread(&pixel_offset, 4, 1, fp);
    // seek through the bmp header, up to the width/height:
    fseek(fp, 4, SEEK_CUR);


    // read the width
    if ((i = fread(&width, 4, 1, fp)) != 1) {
        fprintf(stderr,"[Load_BMP] Error reading width from %s.\n", FileName);
        fclose(fp);
        return 0;
    }
//    printf("Width of %s: %lu\n", FileName, width);

    // read the height
    if ((i = fread(&height, 4, 1, fp)) != 1) {
        fprintf(stderr,"[Load_BMP] Error reading height from %s.\n", FileName);
        fclose(fp);
        return 0;
    }

    // read the planes
    if ((fread(&planes, 2, 1, fp)) != 1) {
        fprintf(stderr,"[Load_BMP] Error reading planes from %s.\n", FileName);
        fclose(fp);
        return 0;
    }
    if (planes != 1) {
        fprintf(stderr,"[Load_BMP] Planes from %s is not 1: %u\n", FileName, planes);
        fclose(fp);
        return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, fp)) != 1) {
        fprintf(stderr,"[Load_BMP] Error reading bpp from %s.\n", FileName);
        fclose(fp);
        return 0;
    }
    if ((bpp != 24) && (bpp != 8)) {    // Doit valoir 24 ou 8
        fprintf(stderr,"[Load_BMP] Bpp from %s is not 24 nor 8: %u\n", FileName, bpp);
        fclose(fp);
        return 0;
    }
    bpp /= 8;   // Vaut 3 pour un fichier bmp en RGB et 1 en N&B

    int width_original = width;
    if ((width % 4) != 0) {         // rustine pour le cas où la largeur n'est pas un multiple de 4 (pas sûr que ce soit la bonne solution).
        width += (4- (width % 4));  // <=> augmenter la taille au multiple de 4 supérieur à la taille réelle
    }
    data_size = (unsigned long)width*height*bpp;

    // seek past the rest of the bitmap header.

    fseek(fp, pixel_offset, SEEK_SET);  // Pixels Bitmap

    // read the data.

    jdata = (unsigned char *)malloc(data_size);
    if (jdata == NULL) {
        fprintf(stderr,"[Load_BMP] Error allocating memory for color-corrected image data");
        fclose(fp);
        return 0;
    }

    if (width == width_original) {
        if ((i = fread(jdata, data_size, 1, fp)) != 1) {    // Lit tout d'un coup. Mais décalages si width n'est pas un multiple de 4
            fprintf(stderr,"[Load_BMP] Error reading image data from %s.\n", FileName);
            fclose(fp);
            return 0;
        }
    } else {
        unsigned char * rows_ptr;
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);    // Ne change rien ???

        w3   = width_original*bpp;
        w3_n = width*bpp - (width_original % 4); // plutôt étrange mais ça semble fonctionner !

        for (j=0; j<height ; j++) { // Lecture ligne par ligne
            rows_ptr = (unsigned char *)jdata + j*w3_n;
            if (( i = fread(rows_ptr, w3, 1, fp )) != 1) {
                fprintf(stderr,"[Load_BMP] Error reading image data from %s.\n", FileName);
                fclose(fp);
                return 0;
            }
        }
    }

    // Rows invertion
    w3 = width*bpp;
    for (j = 0; j <height/2; j++) { // reverse all the lines (top to bottom)
        k  = j*w3;
        kk =(height - j -1)*w3 ;
        for(i = 0; i < w3; i++) {
             temp        = jdata[k];
             jdata[k++]  = jdata[kk];
             jdata[kk++] = temp;
        }
    }

    //---- create OpenGL tex map (omit if not needed) ----------------------------------------------------------
    glGenTextures(1,&texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    if (bpp == 3)
        glTexImage2D (GL_TEXTURE_2D,  0, 3, width, height, 0, GL_BGR_EXT,   GL_UNSIGNED_BYTE, jdata);   // 3 Couleurs dans le sens BGR
//        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,    GL_BGR_EXT,   GL_UNSIGNED_BYTE, jdata);   // 3 Couleurs dans le sens BGR
    else
//        glTexImage2D (GL_TEXTURE_2D,  0, 1, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, jdata);   // Niveaux de gris sur 1 octet
        gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height,    GL_LUMINANCE, GL_UNSIGNED_BYTE, jdata);   // Niveaux de gris sur 1 octet

    fclose(fp);
    free(jdata);

    *out_width  = width_original;
    *out_height = height;

    return texture_id;    // for OpenGL tex maps
}
