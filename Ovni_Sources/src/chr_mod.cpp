
/* Voici une paire de fonctions en C qui fait le transcodage de CP437 a
ISO-8859-1 et retour: */

    /*----  --------------------------------------------------------------------------------------------------*/
    /*
     *   ibm2iso (*c*):  Translates ibm accentuated chars by ISO char.
     *   iso2ibm (*c*):  Translates iso accentuated chars to IBM ones.
     *
     *     The character is modified !!!!  (Pointer )
     *
     *   René Cougnenc 1990    - Public Domain -
     */

/* Ajout Gérard Durand .....
 * les fonctions ibm_To_iso et iso_To_ibm traitent une chaîne de caractères délimitée par \0
 * Elles retournent un pointeur vers la chaîne modifiée en place.
 * Exemple d'appel :    sprintf(Texte,"Chaîne avec caractères accentués") ;
 *                      printf(iso_To_ibm(Texte)) ;
 * ATTENTION : La chaîne de texte ne peut être une constante, il faut d'abord la dupliquer par strdup
 * Exemple faux :       printf(iso_To_ibm("Chaîne avec caractères accentués") ;
 * Exemples OK :        printf(iso_To_ibm(strdup("Chaîne avec caractères accentués")) ;
 *                      printf(iso_To_ibm(strdup("Chaîne avec accents et variables : x= %f y=%f \n")),x,y) ;
 * Déclarer ces fonctions : char * iso_To_ibm(char *) ;
 *
 * Modification : avec wxWidgets, il vaut mieux travailler en UTF8 sous Windows. D'où les fonctions issues de CPPFRANCE et la réécriture en
 *                char * utf8_To_ibm(char *) ;
 */

#define NONE    0xfa    /* Car IBM pour remplacer les inexistants */

/****** pour utf8ToIso8859 ******/

#include "charset.h"
#include <string.h>
#include <stdio.h>

#define ASCII               0x7f    // 7 Bits
#define ISO_8859            0xff    // 8 Bits

#define UTF8_MAX_IN_1       0x7f     //  7 bits (2^7-1)
#define UTF8_MAX_IN_2       0x7ff    // 11 bits (2^11-1)
#define UTF8_MAX_IN_3       0xffff   // 16 bits (2^16-1)

#define UTF8_1ST_OF_1       0     // 0xxx xxxx
#define UTF8_1ST_OF_2       0xc0  // 110x xxxx
#define UTF8_1ST_OF_3       0xe0  // 1110 xxxx
#define UTF8_TRAIL          0x80  // 10xx xxxx
/**********************************/

/**** pour la fonction is_utf8 ****/
#include "utf8.h"
/**********************************/

static char Message_iso[1024];
/* Anciens codes */
static unsigned char IsoTable[256]=
     {
       0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,
       0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,NONE,
       0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,
       0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,
       0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,
       0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,
       0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,
       0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
       0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,
       0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,
       0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,
       0x79,0x7a,0x7b,0x7c,0x7d,0x7e,NONE,NONE,NONE,NONE,NONE,
       NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,NONE,
       NONE,NONE,0x60,0x27,NONE,NONE,NONE,NONE,NONE,NONE,NONE,
       NONE,NONE,NONE,NONE,NONE,NONE,0x20,0xad,0x9b,0x9c,0xfe,
       0x9d,0xb3,0x15,NONE,NONE,0xa6,0xae,0xaa,0x2d,NONE,0xc4,
       0xf8,0xf1,0xfd,NONE,0x27,0xe6,0xe3,NONE,0x2c,NONE,0xb1,
       0xaf,0xac,0xab,NONE,0xa8,0x41,0x41,0x41,0x41,0x8e,0x8f,
       0x92,0x80,0x45,0x90,0x45,0x45,0x49,0x49,0x49,0x49,0x44,
       0xa5,0x4f,0x4f,0x4f,0x4f,0x4f,0x78,0xed,0x55,0x55,0x55,
       0x55,0x59,NONE,0xe1,0x85,0xa0,0x83,0x61,0x84,0x86,0x91,
       0x87,0x8a,0x82,0x88,0x89,0x8d,0xa1,0x8c,0x8b,NONE,0xa4,
       0x95,0xa2,0x93,0x6f,0x94,0xf6,0xed,0x97,0xa3,0x96,0x81,
       0x79,NONE,0x98
     };

#define NOPE    0xb7    /* Car ISO pour remplacer les inexistants */

static unsigned char TableIso[256]=
     {
       0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,
       0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0xa7,
       0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,
       0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,
       0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,
       0x37,0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,0x40,0x41,
       0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,
       0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,
       0x58,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,
       0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6a,0x6b,0x6c,0x6d,
       0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,
       0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x20,0xc7,0xfc,0xe9,0xe2,
       0xe4,0xe0,0xe5,0xe7,0xea,0xeb,0xe8,0xef,0xee,0xec,0xc4,
       0xc5,0xc9,0xe7,0xc6,0xf4,0xf6,0xf2,0xfb,0xf9,0xff,0xd6,
       0xdc,0xa2,0xa3,0xa5,NOPE,NOPE,0xe1,0xed,0xf3,0xfa,0xf1,
       0xd1,0xaa,0xba,0xbf,0xaf,0xac,0xbd,0xbc,0xa1,0xab,0xbb,
       NOPE,NOPE,NOPE,0xa6,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,
       NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,
       NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,
       NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,
       NOPE,NOPE,NOPE,NOPE,NOPE,0xdf,NOPE,0xb6,NOPE,NOPE,0xb5,
       NOPE,NOPE,NOPE,NOPE,NOPE,NOPE,0xf8,NOPE,NOPE,NOPE,0xb1,
       NOPE,NOPE,NOPE,NOPE,0xf7,NOPE,0xb0,0xb7,0xb7,NOPE,NOPE,
       0xb2,0xa4,0x20
     };

void iso2ibm( char *c )
{
    *c = IsoTable[ (short)*c ] ;
}

void ibm2iso( char *c )
{
    *c = TableIso[ (short)*c ] ;
}

char *iso_To_ibm (char *Texte)
{
    char c, *ptr ;
#ifdef WIN32    // Ne concerne que Windows
    ptr = Texte ;
    while ((c=ptr[0]) != '\0') iso2ibm(ptr++) ;
#endif
    return (Texte) ;
}

char *ibm_To_iso (char *Texte)
{
    char c, *ptr ;
#ifdef WIN32    // Ne concerne que Windows
    ptr = Texte ;
    while ((c=ptr[0]) != '\0') iso2ibm(ptr++) ;
#endif
    return (Texte) ;
}

/* Nouveaux et codes modifiés ... */
 char *utf8_To_ibm (char *Texte)
{
/* Fonction invalidée car maintenant la console Windows sort les caractères en utf8 à cause du chcp 65001 > nul ; ajouté en début de OvniApp.cpp
#ifdef WIN32    // Ne concerne que Windows
    unsigned int nb = utf8ToEascii(Texte, strlen(Texte), Message_iso, 1024);
    return (Message_iso);
#endif
*/
    return (Texte) ;
}

/**
 * D'après un code original de Julien Folly sur CPPFrance.com (cppfrance_CHARSET-CONVERTER-LIBRARY___Page.zip)
 * maintenant sur https://codes-sources.commentcamarche.net/source/36876-charset-converter-library
 *
 * Convert from UTF8 to Iso8859
 * Return the number of character, DEST_TOO_SMALL if the destination is too small,
 * or UTF8_BAD_STRING if the UTF8 String is invalid
 * The iDestLen should be equal to iSrcLen
 *
 * capSrc    ->    Pointer to the string to convert
 * iSrcLen   ->    Number of byte in capSrc
 * capDest   ->    Pointer to an allocated memory area for the converted string
 * iDestLen  ->    Size of the allocated memory area
 */
unsigned int utf8ToIso8859(const char * capSrc,unsigned int iSrcLen,char * capDest,unsigned int iDestLen){
//    if(iDestLen == 0) return DEST_TOO_SMALL;
    unsigned int iNbUnicode = 0;
    unsigned int i ;
    wchar wcapDest;

    if(iDestLen < iSrcLen) return DEST_TOO_SMALL;

    for (i = 0; i<iSrcLen; ++i){

        // Found 3 bytes sequence
        if((unsigned char)capSrc[i] >= UTF8_1ST_OF_3){
            if(i+2 >= iSrcLen) return UTF8_BAD_STRING;

            // First 8 bits
            wcapDest  = ((capSrc[  i] << 4)               ) <<8;
            wcapDest |= ((capSrc[++i] >> 2) & 0xf         ) <<8;

            // Last 8 bits
            wcapDest |= (capSrc[  i] & 0x3) << 6;
            wcapDest |=  capSrc[++i] & 0x3F;
        }else

        // Found 2 bytes sequence
        if((unsigned char)capSrc[i] >= UTF8_1ST_OF_2){
            if(i+1 >= iSrcLen) return UTF8_BAD_STRING;
            // First 8 bits
            wcapDest   =  ((capSrc[  i] >> 2 ) & 0x7      ) <<8;

            // Last 8 bits
            wcapDest  |= (capSrc[  i] & 0x3) << 6;
            wcapDest  |=  capSrc[++i] & 0x3F;
        }else

        // Found 1 byte sequence (ASCII)
        /* if((unsigned char)capSrc[i] >= UTF8_1ST_OF_1) */{
            wcapDest = capSrc[i];
        }
        if(wcapDest >> 8 > 0) return INCOMPATIBLE_UNICODE;
        capDest[iNbUnicode++] = wcapDest;
        if(iNbUnicode > iDestLen) return DEST_TOO_SMALL;
    }
    capDest[iNbUnicode] = 0;   // Par précaution
    return iNbUnicode;
}

unsigned int utf8ToEascii(const char * capSrc,unsigned int iSrcLen,char * capDest,unsigned int iDestLen){
/**
 * D'après un code original de Julien Folly sur CPPFrance.com (cppfrance_CHARSET-CONVERTER-LIBRARY___Page.zip)
 * maintenant sur https://codes-sources.commentcamarche.net/source/36876-charset-converter-library
 *
 * Convert from UTF8 to Extended Ascii (pour imprimer des caractères accentués dans une fenêtre DOS, l'entrée étant en UTF8)
 * Return the number of character, DEST_TOO_SMALL if the destination is too small,
 * or UTF8_BAD_STRING if the UTF8 String is invalid
 * The iDestLen should be equal to iSrcLen
 *
 * capSrc    ->    Pointer to the string to convert
 * iSrcLen   ->    Number of byte in capSrc
 * capDest   ->    Pointer to an allocated memory area for the converted string
 * iDestLen  ->    Size of the allocated memory area
 */
//    if(iDestLen == 0) return DEST_TOO_SMALL;
    unsigned int iNbUnicode = 0, ret = 0;
    unsigned int i ;
    wchar wcapDest;
    char capDest_i;

    if(iDestLen < iSrcLen) return DEST_TOO_SMALL;

    for (i = 0; i<iSrcLen; ++i){

        // Found 3 bytes sequence
        if((unsigned char)capSrc[i] >= UTF8_1ST_OF_3){
            if(i+2 >= iSrcLen) return UTF8_BAD_STRING;

            // First 8 bits
            wcapDest  = ((capSrc[  i] << 4)               ) <<8;
            wcapDest |= ((capSrc[++i] >> 2) & 0xf         ) <<8;

            // Last 8 bits
            wcapDest |= (capSrc[  i] & 0x3) << 6;
            wcapDest |=  capSrc[++i] & 0x3F;
        }else

        // Found 2 bytes sequence
        if((unsigned char)capSrc[i] >= UTF8_1ST_OF_2){
            if(i+1 >= iSrcLen) return UTF8_BAD_STRING;
            // First 8 bits
            wcapDest   =  ((capSrc[  i] >> 2 ) & 0x7      ) <<8;

            // Last 8 bits
            wcapDest  |= (capSrc[  i] & 0x3) << 6;
            wcapDest  |=  capSrc[++i] & 0x3F;
        }else

        // Found 1 byte sequence (ASCII)
        /* if((unsigned char)capSrc[i] >= UTF8_1ST_OF_1) */{
            wcapDest = capSrc[i];
        }
        if(wcapDest >> 8 > 0) return INCOMPATIBLE_UNICODE;
        capDest_i = (char)wcapDest;
//        capDest[iNbUnicode++] = wcapDest;
        switch((unsigned char)capDest_i){
            case 0xA1: capDest[iNbUnicode] = 0xAD; break;
            case 0xA2: capDest[iNbUnicode] = 0xBD; break;
            case 0xA3: capDest[iNbUnicode] = 0x9C; break;
            case 0xA4: capDest[iNbUnicode] = 0xCF; break;
            case 0xA5: capDest[iNbUnicode] = 0xBE; break;
            case 0xA6: capDest[iNbUnicode] = 0xDD; break;
            case 0xA7: capDest[iNbUnicode] = 0xF5; break;
            case 0xA8: capDest[iNbUnicode] = 0xF9; break;
            case 0xA9: capDest[iNbUnicode] = 0xB8; break;
            case 0xAA: capDest[iNbUnicode] = 0xA6; break;
            case 0xAB: capDest[iNbUnicode] = 0xAE; break;
            case 0xAC: capDest[iNbUnicode] = 0xAA; break;
            case 0xAD: capDest[iNbUnicode] = 0xF0; break;
            case 0xAE: capDest[iNbUnicode] = 0xA9; break;
            case 0xAF: capDest[iNbUnicode] = 0xEE; break;
            case 0xB0: capDest[iNbUnicode] = 0xF8; break;
            case 0xB1: capDest[iNbUnicode] = 0xF1; break;
            case 0xB2: capDest[iNbUnicode] = 0xFD; break;
            case 0xB3: capDest[iNbUnicode] = 0xFC; break;
            case 0xB4: capDest[iNbUnicode] = 0xEF; break;
            case 0xB5: capDest[iNbUnicode] = 0xE6; break;
            case 0xB6: capDest[iNbUnicode] = 0xF4; break;
            case 0xB7: capDest[iNbUnicode] = 0xFA; break;
            case 0xB8: capDest[iNbUnicode] = 0xF7; break;
            case 0xB9: capDest[iNbUnicode] = 0xFB; break;
            case 0xBA: capDest[iNbUnicode] = 0xA7; break;
            case 0xBB: capDest[iNbUnicode] = 0xAF; break;
            case 0xBC: capDest[iNbUnicode] = 0xAC; break;
            case 0xBD: capDest[iNbUnicode] = 0xAB; break;
            case 0xBE: capDest[iNbUnicode] = 0xF3; break;
            case 0xBF: capDest[iNbUnicode] = 0xA8; break;
            case 0xC0: capDest[iNbUnicode] = 0xB7; break;
            case 0xC1: capDest[iNbUnicode] = 0xB5; break;
            case 0xC2: capDest[iNbUnicode] = 0xB6; break;
            case 0xC3: capDest[iNbUnicode] = 0xC7; break;
            case 0xC4: capDest[iNbUnicode] = 0x8E; break;
            case 0xC5: capDest[iNbUnicode] = 0x8F; break;
            case 0xC6: capDest[iNbUnicode] = 0x92; break;
            case 0xC7: capDest[iNbUnicode] = 0x80; break;
            case 0xC8: capDest[iNbUnicode] = 0xD4; break;
            case 0xC9: capDest[iNbUnicode] = 0x90; break;
            case 0xCA: capDest[iNbUnicode] = 0xD2; break;
            case 0xCB: capDest[iNbUnicode] = 0xD3; break;
            case 0xCC: capDest[iNbUnicode] = 0xDE; break;
            case 0xCD: capDest[iNbUnicode] = 0xD6; break;
            case 0xCE: capDest[iNbUnicode] = 0xD7; break;
            case 0xCF: capDest[iNbUnicode] = 0xD8; break;
            case 0xD0: capDest[iNbUnicode] = 0xD1; break;
            case 0xD1: capDest[iNbUnicode] = 0xA5; break;
            case 0xD2: capDest[iNbUnicode] = 0xE3; break;
            case 0xD3: capDest[iNbUnicode] = 0xE0; break;
            case 0xD4: capDest[iNbUnicode] = 0xE2; break;
            case 0xD5: capDest[iNbUnicode] = 0xE5; break;
            case 0xD6: capDest[iNbUnicode] = 0x99; break;
            case 0xD7: capDest[iNbUnicode] = 0x9E; break;
            case 0xD8: capDest[iNbUnicode] = 0x9D; break;
            case 0xD9: capDest[iNbUnicode] = 0xEB; break;
            case 0xDA: capDest[iNbUnicode] = 0xE9; break;
            case 0xDB: capDest[iNbUnicode] = 0xEA; break;
            case 0xDC: capDest[iNbUnicode] = 0x9A; break;
            case 0xDD: capDest[iNbUnicode] = 0xED; break;
            case 0xDE: capDest[iNbUnicode] = 0xE7; break;
            case 0xDF: capDest[iNbUnicode] = 0xE1; break;
            case 0xE0: capDest[iNbUnicode] = 0x85; break;
            case 0xE1: capDest[iNbUnicode] = 0xA0; break;
            case 0xE2: capDest[iNbUnicode] = 0x83; break;
            case 0xE3: capDest[iNbUnicode] = 0xC6; break;
            case 0xE4: capDest[iNbUnicode] = 0x84; break;
            case 0xE5: capDest[iNbUnicode] = 0x86; break;
            case 0xE6: capDest[iNbUnicode] = 0x91; break;
            case 0xE7: capDest[iNbUnicode] = 0x87; break;
            case 0xE8: capDest[iNbUnicode] = 0x8A; break;
            case 0xE9: capDest[iNbUnicode] = 0x82; break;
            case 0xEA: capDest[iNbUnicode] = 0x88; break;
            case 0xEB: capDest[iNbUnicode] = 0x89; break;
            case 0xEC: capDest[iNbUnicode] = 0x8D; break;
            case 0xED: capDest[iNbUnicode] = 0xA1; break;
            case 0xEE: capDest[iNbUnicode] = 0x8C; break;
            case 0xEF: capDest[iNbUnicode] = 0x8B; break;
            case 0xF0: capDest[iNbUnicode] = 0xD0; break;
            case 0xF1: capDest[iNbUnicode] = 0xA4; break;
            case 0xF2: capDest[iNbUnicode] = 0x95; break;
            case 0xF3: capDest[iNbUnicode] = 0xA2; break;
            case 0xF4: capDest[iNbUnicode] = 0x93; break;
            case 0xF5: capDest[iNbUnicode] = 0xE4; break;
            case 0xF6: capDest[iNbUnicode] = 0x94; break;
            case 0xF7: capDest[iNbUnicode] = 0xF6; break;
            case 0xF8: capDest[iNbUnicode] = 0x9B; break;
            case 0xF9: capDest[iNbUnicode] = 0x97; break;
            case 0xFA: capDest[iNbUnicode] = 0xA3; break;
            case 0xFB: capDest[iNbUnicode] = 0x96; break;
            case 0xFC: capDest[iNbUnicode] = 0x81; break;
            case 0xFD: capDest[iNbUnicode] = 0xEC; break;
            case 0xFE: capDest[iNbUnicode] = 0xE8; break;
            case 0xFF: capDest[iNbUnicode] = 0x98; break;

            default:
                if((unsigned char)capDest_i > ASCII) ret = INCOMPATIBLE_ISO8859;
                capDest[iNbUnicode] = capDest_i;
        }
        iNbUnicode++;
        if(iNbUnicode > iDestLen) return DEST_TOO_SMALL;
    }
    if (ret == 0) ret = iNbUnicode;
    capDest[ret] = 0;   // Par précaution
    return ret;
}

// d'après https://stackoverflow.com/questions/1031645/how-to-detect-utf-8-in-plain-c
// Teste si une chaine est valide en codage utf8 : attention ; il peut y avoir de faux positifs d'près ce forum !

bool is_utf8(const char * string)
{
    if(!string)
        return false;

    const unsigned char * bytes = (const unsigned char *)string;
    while(*bytes)
    {
        if( (// ASCII
             // use bytes[0] <= 0x7F to allow ASCII control characters
                bytes[0] == 0x09 ||
                bytes[0] == 0x0A ||
                bytes[0] == 0x0D ||
                (0x20 <= bytes[0] && bytes[0] <= 0x7E)
            )
        ) {
            bytes += 1;
            continue;
        }

        if( (// non-overlong 2-byte
                (0xC2 <= bytes[0] && bytes[0] <= 0xDF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF)
            )
        ) {
            bytes += 2;
            continue;
        }

        if( (// excluding overlongs
                bytes[0] == 0xE0 &&
                (0xA0 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// straight 3-byte
                ((0xE1 <= bytes[0] && bytes[0] <= 0xEC) ||
                    bytes[0] == 0xEE ||
                    bytes[0] == 0xEF) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            ) ||
            (// excluding surrogates
                bytes[0] == 0xED &&
                (0x80 <= bytes[1] && bytes[1] <= 0x9F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF)
            )
        ) {
            bytes += 3;
            continue;
        }

        if( (// planes 1-3
                bytes[0] == 0xF0 &&
                (0x90 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// planes 4-15
                (0xF1 <= bytes[0] && bytes[0] <= 0xF3) &&
                (0x80 <= bytes[1] && bytes[1] <= 0xBF) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            ) ||
            (// plane 16
                bytes[0] == 0xF4 &&
                (0x80 <= bytes[1] && bytes[1] <= 0x8F) &&
                (0x80 <= bytes[2] && bytes[2] <= 0xBF) &&
                (0x80 <= bytes[3] && bytes[3] <= 0xBF)
            )
        ) {
            bytes += 4;
            continue;
        }

        return false;
    }

    return true;
}
