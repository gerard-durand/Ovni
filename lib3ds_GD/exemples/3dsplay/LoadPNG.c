/***************************************************
    To read a png image file and download
    it as a texture map for openGL
****************************************************/

#include <stdio.h>
#include <GL/glut.h>
#include <png.h>
#include "LoadPNG.h"


GLuint Load_PNG(char* FileName, int *out_width, int *out_height)
{
    png_byte   bit_depth, color_type;
    png_bytep *row_pointers = NULL;
    png_bytep  row, px;
    int width, height;
    unsigned long data_size;
    unsigned char *jdata, *ptr;
    char header[8];                 // 8 is the maximum size that can be checked

    unsigned int texture_id;

    FILE *fp = fopen(FileName, "rb");
    //if the png file doesn't load
    if (!fp) {
        fprintf(stderr, "[Load_PNG] Error reading PNG file %s!\n", FileName);
        return 0;
    }
    if (strcasecmp(".png",strrchr(FileName,'.')) != 0) {      // Sortie directement si l'extension est incorrecte
        fprintf(stderr, "[Load_PNG] Error reading PNG file %s : has not a .png extension !\n", FileName);
        fclose(fp);
        return 0;
    }
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        fprintf(stderr, "[Load_PNG] File %s is not recognized as a PNG file", FileName);
        fclose(fp);
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "[Load_PNG] PNG file %s : cannot create structure png_ptr !\n", FileName);
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "[Load_PNG] PNG file %s : cannot create structure info_ptr !\n", FileName);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "[Load_PNG] PNG file %s : Error in setjmp(png_jmpbuf(png_ptr)) !\n", FileName);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);  // Nécessaire car on a utilisé fread(header,...);

    png_read_info(png_ptr, info_ptr);

    width      = png_get_image_width(png_ptr,  info_ptr);
    height     = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr,   info_ptr);
    bit_depth  = png_get_bit_depth(png_ptr,    info_ptr);

    // Read any color_type into 8bit depth, RGBA format.
    // See http://www.libpng.org/pub/png/libpng-manual.txt

    if(bit_depth == 16)
        png_set_strip_16(png_ptr);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png_ptr);

    // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png_ptr);

    if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    // These color_type don't have an alpha channel then fill it with 0xff.
    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

//    if (row_pointers) abort();

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y=0; y<height; y++)
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

    png_read_image(png_ptr, row_pointers);

    data_size = width*height*4;
    ptr = jdata = (unsigned char *)malloc(data_size);
    for(int y = 0; y < height; y++) {
        row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            px = &(row[x * 4]);
            *ptr++ = px[0]; *ptr++ = px[1]; *ptr++ = px[2]; *ptr++ = px[3]; // Recopie des octets de row_pointers dans jdata
        }
    }
    //---- create OpenGL tex map (omit if not needed) ----------------------------------------------------------
    glGenTextures(1,&texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
//    glTexImage2D (GL_TEXTURE_2D,  0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, jdata);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height,    GL_RGBA, GL_UNSIGNED_BYTE, jdata);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    free(row_pointers);
    free(jdata);

    *out_width  = width;
    *out_height = height;

    return texture_id;    // for OpenGL tex maps
}
