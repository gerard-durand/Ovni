/***************************************************
    To read a jpg image file and download
    it as a texture map for openGL
    Derived from Tom Lane's example.c
    -- Obtain & install jpeg stuff from web
    (jpeglib.h, jerror.h jmore.h, jconfig.h,jpeg.lib)
****************************************************/

#include <stdio.h>
#include <GL/glut.h>
#include <jpeglib.h>
#include <jerror.h>
#include "LoadJPEG.h"
//================================
GLuint Load_JPEG(char* FileName, int *width, int *height)
//================================
{
    unsigned long x, y;
    unsigned int texture_id;
    unsigned long data_size;            // length of the file
    int channels;                       //  3 =>RGB   4 =>RGBA
    unsigned int GL_type;
    unsigned char * rowptr[1];          // pointer to an array
    unsigned char * jdata;              // data for the image
    struct jpeg_decompress_struct info; //for our jpeg info
    struct jpeg_error_mgr err;          //the error handler

    FILE* file = fopen(FileName, "rb"); //open the file

    //if the jpeg file doesn't load
    if(!file) {
        fprintf(stderr, "[Load_JPEG] Error reading JPEG file %s!", FileName);
        return 0;
    }
    if (strcasecmp(".jpg",strrchr(FileName,'.')) != 0) {      // Sortie directement si l'extension est incorrecte
        fprintf(stderr, "[Load_JPEG] Error reading JPEG file %s : has not a .jpg extension !", FileName);
        fclose(file);
        return 0;
    }

    info.err = jpeg_std_error(& err);
    jpeg_create_decompress(& info);     //fills info structure

    jpeg_stdio_src(&info, file);
    jpeg_read_header(&info, TRUE);      // read jpeg file header // ATTENTION : si ce n'est pas un fichier jpeg, ça indique un message d'erreur mais ça plante ensuite en interne !

    jpeg_start_decompress(&info);       // decompress the file

    //set width and height
    x = info.output_width;              // Attention : pour certaines largeurs, comme 254 par exemple, on observe une image décalée par ligne (Mieux vaur 256 ici).
    y = info.output_height;
    channels = info.num_components;
    GL_type  = GL_RGB;
    if(channels == 4) GL_type = GL_RGBA;
    if(channels <= 2) GL_type = GL_LUMINANCE;   // Si channels = 2, c'est sûrement autre chose, mais en attendant mieux ...

    if ((x % 4) != 0) {     // rustine pour le cas où la largeur n'est pas un multiple de 4 (pas sûr que ce soit la bonne solution). Pb uniquement avec jpeg ????
        x += (4- (x % 4));  // <=> augmenter la taille au multiple de 4 supérieur à la taille réelle
    }
    data_size = x * y * channels;

    unsigned long offset_location = channels * x;

    //----  ----------------------------------------------------------------------------------------------------
    // read scanlines one at a time & put bytes
    //    in jdata[] array. Assumes an RGB image (or RGBA or LUMINANCE)
    //----  ----------------------------------------------------------------------------------------------------
    jdata = (unsigned char *)malloc(data_size);

    while (info.output_scanline < info.output_height) { // loop
       // Enable jpeg_read_scanlines() to fill our jdata array
        rowptr[0] = (unsigned char *)jdata +  // secret to method
                    offset_location * info.output_scanline ;

        jpeg_read_scanlines(&info, rowptr, 1);  // Ici, on rempli directement jdata via le pointeur rowptr
    }
    //----  ----------------------------------------------------------------------------------------------------

    jpeg_finish_decompress(&info);   //finish decompressing

    //---- create OpenGL tex map (omit if not needed) ----------------------------------------------------------
    glGenTextures(1,&texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    gluBuild2DMipmaps(GL_TEXTURE_2D,    channels, x, y,    GL_type, GL_UNSIGNED_BYTE, jdata);
//    glTexImage2D     (GL_TEXTURE_2D, 0, channels, x, y, 0, GL_type, GL_UNSIGNED_BYTE, jdata);    // Semble donner la même chose que ci-dessus

    jpeg_destroy_decompress(&info);
    fclose(file);                    //close the file
    free(jdata);

    *width  = x;
    *height = y;

    return texture_id;    // for OpenGL tex maps
}
