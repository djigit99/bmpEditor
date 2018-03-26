#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp_header.h"


struct Clr
{
    unsigned char B;
    unsigned char G;
    unsigned char R;

} ** clr_copy;
struct Clr** myReadBMP(unsigned char * name, struct bmp_fileheader* BMPheader, struct bmp_infoheader* BMPInfoheader)
{
    short i,j;
    FILE* myFile = fopen(name, "rb");

    // FileHeader reading
    fread((char*)&(BMPheader->fileMarker1),sizeof(unsigned char),1,myFile);
    fread((char*)&(BMPheader->fileMarker2),sizeof(unsigned char),1,myFile);
    fread((char*)&(BMPheader->bfSize),sizeof(unsigned int),1,myFile);
    fread((char*)&(BMPheader->unused1),sizeof(unsigned short),1,myFile);
    fread((char*)&(BMPheader->unused2),sizeof(unsigned short),1,myFile);
    fread((char*)&(BMPheader->imageDataOffset),sizeof(unsigned int),1,myFile);
    // end

    // FileInfoHeader
    signed short s1,s2;
    fread((char*)&(BMPInfoheader->biSize),sizeof(unsigned int),1,myFile);

    fread((char*)&(BMPInfoheader->width),sizeof(signed int),1,myFile);

    fread((char*)&(BMPInfoheader->height),sizeof(signed int),1,myFile);


    fread((char*)&(BMPInfoheader->planes),sizeof(unsigned short),1,myFile);
    fread((char*)&(BMPInfoheader->bitPix),sizeof(unsigned short),1,myFile);
    fread((char*)&(BMPInfoheader->biCompression),sizeof(unsigned int),1,myFile);
    fread((char*)&(BMPInfoheader->biSizeImage),sizeof(unsigned int),1,myFile);
    fread((char*)&(BMPInfoheader->biXPelsPerMeter),sizeof(int),1,myFile);
    fread((char*)&(BMPInfoheader->biYPelsPerMeter),sizeof(int),1,myFile);
    fread((char*)&(BMPInfoheader->biClrUsed),sizeof(unsigned int),1,myFile);
    fread((char*)&(BMPInfoheader->biClrImportant),sizeof(unsigned int),1,myFile);
    // end
    struct Clr** clr = malloc( sizeof ( struct Clr*) * (BMPInfoheader->height));
    clr_copy = malloc(sizeof ( struct Clr*) * (BMPInfoheader->height));
    fseek(myFile,84,SEEK_CUR);
    for(i = BMPInfoheader->height - 1 ; i >= 0; i--)
    {
        clr[i] = malloc(sizeof (struct Clr) * BMPInfoheader->width);
        clr_copy[i] = malloc(sizeof (struct Clr) * BMPInfoheader->width);
        for(j = 0; j < BMPInfoheader->width; j++ )
        {
            fread((char*)&(clr[i][j]),sizeof(struct Clr),1,myFile);
            clr_copy[i][j] = clr[i][j];
        }
    }
    fclose(myFile);
    return clr;
}
void ret_val (struct Clr** clr, struct bmp_infoheader* BMPInfoHeader)
{
    int i,j;
    for(i = 0; i < BMPInfoHeader->height; i++)
    {
        for(j = 0;j < BMPInfoHeader->width; j++)
        {
            clr[i][j] = clr_copy[i][j];
        }
    }
}
void myWriteBMP(unsigned char * name, struct Clr** clr, struct bmp_fileheader* BMPheader, struct bmp_infoheader * BMPInfoheader)
{
    short i,j;
    FILE* myFile = fopen(name, "wb");

    // FileHeader writing

    fwrite((char*)&(BMPheader->fileMarker1),sizeof(unsigned char),1,myFile);
    fwrite((char*)&(BMPheader->fileMarker2),sizeof(unsigned char),1,myFile);
    fwrite((char*)&(BMPheader->bfSize),sizeof(unsigned int),1,myFile);
    fwrite((char*)&(BMPheader->unused1),sizeof(unsigned short),1,myFile);
    fwrite((char*)&(BMPheader->unused2),sizeof(unsigned short),1,myFile);
    fwrite((char*)&(BMPheader->imageDataOffset),sizeof(unsigned int),1,myFile);
    // end

    // FileInfoHeader
    fwrite((char*)&(BMPInfoheader->biSize),sizeof(unsigned int),1,myFile);
    fwrite((char*)&(BMPInfoheader->width),sizeof(signed int),1,myFile);
    fwrite((char*)&(BMPInfoheader->height),sizeof(signed int),1,myFile);
    fwrite((char*)&(BMPInfoheader->planes),sizeof(unsigned short),1,myFile);
    fwrite((char*)&(BMPInfoheader->bitPix),sizeof(unsigned short),1,myFile);
    fwrite((char*)&(BMPInfoheader->biCompression),sizeof(unsigned int),1,myFile);
    fwrite((char*)&(BMPInfoheader->biSizeImage),sizeof(unsigned int),1,myFile);
    fwrite((char*)&(BMPInfoheader->biXPelsPerMeter),sizeof(int),1,myFile);
    fwrite((char*)&(BMPInfoheader->biYPelsPerMeter),sizeof(int),1,myFile);
    fwrite((char*)&(BMPInfoheader->biClrUsed),sizeof(unsigned int),1,myFile);
    fwrite((char*)&(BMPInfoheader->biClrImportant),sizeof(unsigned int),1,myFile);
    // end
    unsigned char nul = 0;
    for(i = 0; i < BMPheader->imageDataOffset-54; i++)
        fwrite((char*)&nul,sizeof(unsigned char),1,myFile);
    for(i = BMPInfoheader->height - 1; i >= 0; i--)
    {
        for(j = 0; j < BMPInfoheader->width; j++ )
        {
            fwrite((char*)&(clr[i][j]),sizeof(struct Clr),1,myFile);
        }
    }
    if(myFile!=NULL)
    fclose(myFile);
}

void task1_(struct Clr** clr,
            struct bmp_fileheader* BMPFileHeader,
            struct bmp_infoheader* BMPInfoHeader,
            char* file_name
            )
{
    int i,j;
    for( i=0;i<BMPInfoHeader->height;i++)
    {
        for(j=0;j<BMPInfoHeader->width;j++)
        {
            unsigned char tm = (clr[i][j].B + clr[i][j].G + clr[i][j].R) / 3;
            clr[i][j].B = tm;
            clr[i][j].G = tm;
            clr[i][j].R = tm;
        }
    }
    char* pic_name = malloc(sizeof(char) * 40);
    char *ptr;
    ptr = strtok(file_name, ".");
    strcpy(pic_name, ptr);
    strcat(pic_name, "_black_white.bmp");
    myWriteBMP(pic_name,clr,BMPFileHeader,BMPInfoHeader);
    if(pic_name!=NULL)
    free(pic_name);
}
void task2_f1_(struct Clr** clr,
               struct bmp_fileheader* BMPFileHeader,
               struct bmp_infoheader* BMPInfoHeader,
               char* file_name
               )
{
    int i,j;
    /*  -1 , -1 , -1
        -1 , 8 , -1,
        -1 , -1 , -1    */
    struct Clr* help_line = malloc(sizeof(struct Clr) * BMPInfoHeader->width);
    for(i = 0; i < BMPInfoHeader->width; i++)
    {
        help_line[i] = clr[0][i];

        short cur_val_R = (short)( 8 * clr[0][i].R) ; // C C
        //printf("\n cur_val_R = %d\n",cur_val_R);
        if(i>0)
        {
            cur_val_R += (short) ( -1 * help_line[i-1].R); // ? L
            cur_val_R += (short) ( -1 * clr[1][i-1].R); // N L
        }
        if(i + 1 < BMPInfoHeader->width)
        {
            cur_val_R += (short)(-1 * clr[0][i+1].R); // C R
            cur_val_R += (short)(-1 * clr[1][i+1].R); // N R
        }
        cur_val_R += (short) (-1 * clr[1][i].R); // N C
        if(cur_val_R < 0)
            cur_val_R = 0;
        else if(cur_val_R>255)
            cur_val_R = 255;
        clr[0][i].R = cur_val_R; //  R finished

        short cur_val_G = (short)( 8 * clr[0][i].G) ; // C C
        //printf("\n cur_val_R = %d\n",cur_val_R);
        if(i>0)
        {
            cur_val_G += (short) ( -1 * help_line[i-1].G); // ? L
            cur_val_G += (short) ( -1 * clr[1][i-1].G); // N L
        }
        if(i + 1 < BMPInfoHeader->width)
        {
            cur_val_G += (short)(-1 * clr[0][i+1].G); // C R
            cur_val_G += (short)(-1 * clr[1][i+1].G); // N R
        }
        cur_val_G += (short) (-1 * clr[1][i].G); // N C
        if(cur_val_G < 0)
            cur_val_G = 0;
        else if(cur_val_G>255)
            cur_val_G = 255;
        clr[0][i].G = cur_val_G; // G finished

        short cur_val_B = (short)( 8 * clr[0][i].B) ; // C C
        //printf("\n cur_val_R = %d\n",cur_val_R);
        if(i>0)
        {
            cur_val_B += (short) ( -1 * help_line[i-1].B); // ? L
            cur_val_B += (short) ( -1 * clr[1][i-1].B); // N L
        }
        if(i + 1 < BMPInfoHeader->width)
        {
            cur_val_B += (short)(-1 * clr[0][i+1].B); // C R
            cur_val_B += (short)(-1 * clr[1][i+1].B); // N R
        }
        cur_val_B += (short) (-1 * clr[1][i].B); // N C
        if(cur_val_B < 0)
            cur_val_B = 0;
        else if(cur_val_B>255)
            cur_val_B = 255;
        clr[0][i].B = cur_val_B; // B finished
    }
    struct Clr left;
    for(i = 1; i < BMPInfoHeader->height;i++)
    {
        left.R = 0;
        left.G = 0;
        left.B = 0;
        for(j = 0; j < BMPInfoHeader->width; j++)
        {
            short cur_val_R =(short)( 8 * clr[i][j].R ); // C C
            cur_val_R += (short)( -1 * left.R ); // C L
            if(j>0)
            {
                cur_val_R += (short) ( -1 * help_line[j-1].R); // V L
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_R += (short) (-1 * clr[i+1][j-1].R); // N L
                }
            }
            if(j + 1 < BMPInfoHeader->width)
            {
                cur_val_R += (short) (-1 * clr[i][j+1].R) ; // C R
                if(i > 0)
                {
                    cur_val_R += (short) ( -1 * help_line[j+1].R); // V R
                }
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_R += (short) (-1 * clr[i+1][j+1].R); // N R
                }
            }
            if(i>0)
            {
                cur_val_R += (short) (-1 * help_line[j].R); // V C
            }
            if(i + 1 < BMPInfoHeader->height)
            {
                cur_val_R += (short) (-1 * clr[i+1][j].R); // N C
            }
            if(cur_val_R < 0)
                cur_val_R = 0;
            else if(cur_val_R>255)
                cur_val_R = 255;
            // R finished
            short cur_val_G =(short)( 8 * clr[i][j].G ); // C C
            cur_val_G += (short)( -1 * left.G ); // C L
            if(j>0)
            {
                cur_val_G += (short) ( -1 * help_line[j-1].G); // V L
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_G += (short) (-1 * clr[i+1][j-1].G); // N L
                }
            }
            if(j + 1 < BMPInfoHeader->width)
            {
                cur_val_G += (short) (-1 * clr[i][j+1].G) ; // C R
                if(i > 0)
                {
                    cur_val_G += (short) ( -1 * help_line[j+1].G); // V R
                }
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_G += (short) (-1 * clr[i+1][j+1].G); // N R
                }
            }
            if(i>0)
            {
                cur_val_G += (short) (-1 * help_line[j].G); // V C
            }
            if(i + 1 < BMPInfoHeader->height)
            {
                cur_val_G += (short) (-1 * clr[i+1][j].G); // N C
            }
            if(cur_val_G < 0)
                cur_val_G = 0;
            else if(cur_val_G>255)
                cur_val_G = 255;
            // G finished

            short cur_val_B =(short)( 8 * clr[i][j].B ); // C C
            cur_val_B += (short)( -1 * left.B ); // C L
            if(j>0)
            {
                cur_val_B += (short) ( -1 * help_line[j-1].B); // V L
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_B += (short) (-1 * clr[i+1][j-1].B); // N L
                }
            }
            if(j + 1 < BMPInfoHeader->width)
            {
                cur_val_B += (short) (-1 * clr[i][j+1].B) ; // C R
                if(i > 0)
                {
                    cur_val_B += (short) ( -1 * help_line[j+1].B); // V R
                }
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_B += (short) (-1 * clr[i+1][j+1].B); // N R
                }
            }
            if(i>0)
            {
                cur_val_B += (short) (-1 * help_line[j].B); // V C
            }
            if(i + 1 < BMPInfoHeader->height)
            {
                cur_val_B += (short) (-1 * clr[i+1][j].B); // N C
            }
            if(cur_val_B < 0)
                cur_val_B = 0;
            else if(cur_val_B>255)
                cur_val_B = 255;
            // B finished

            if(j>0)
                help_line[j-1] = left;
            if(j + 1 == BMPInfoHeader->width)
                help_line[j] = clr[i][j];
            left = clr[i][j];
            clr[i][j].R = cur_val_R;
            clr[i][j].G = cur_val_G;
            clr[i][j].B = cur_val_B;
        }
    }
    char* pic_name = malloc(sizeof(char) * 40);
    char *ptr;
    ptr = strtok(file_name, ".");
    strcpy(pic_name, ptr);
    strcat(pic_name, "_f1.bmp");

    myWriteBMP(pic_name,clr,BMPFileHeader,BMPInfoHeader);
    if(pic_name!=NULL)
    free(pic_name);
    if(help_line!=NULL)
    free(help_line);
}
void task2_f2_(struct Clr** clr,
               struct bmp_fileheader* BMPFileHeader,
               struct bmp_infoheader* BMPInfoHeader,
               char* file_name
               )
{
    int i,j;
    /*  0 , 1 , 0
        1 , -4 , 1,
        0 , 1 , 0    */
    struct Clr* help_line = malloc(sizeof(struct Clr) * BMPInfoHeader->width);
    for(i = 0; i < BMPInfoHeader->width; i++)
    {
        help_line[i] = clr[0][i];

        short cur_val_R = (short)( -4 * clr[0][i].R) ; // C C
        //printf("\n cur_val_R = %d\n",cur_val_R);
        if(i>0)
        {
            cur_val_R += (short) ( 1 * help_line[i-1].R); // ? L
            cur_val_R += (short) ( 0 * clr[1][i-1].R); // N L
        }
        if(i + 1 < BMPInfoHeader->width)
        {
            cur_val_R += (short)(1 * clr[0][i+1].R); // C R
            cur_val_R += (short)(0 * clr[1][i+1].R); // N R
        }
        cur_val_R += (short) (1 * clr[1][i].R); // N C
        if(cur_val_R < 0)
            cur_val_R = 0;
        else if(cur_val_R>255)
            cur_val_R = 255;
        clr[0][i].R = cur_val_R; //  R finished

        short cur_val_G = (short)( -4 * clr[0][i].G) ; // C C
        //printf("\n cur_val_R = %d\n",cur_val_R);
        if(i>0)
        {
            cur_val_G += (short) ( 1 * help_line[i-1].G); // ? L
            cur_val_G += (short) ( 0 * clr[1][i-1].G); // N L
        }
        if(i + 1 < BMPInfoHeader->width)
        {
            cur_val_G += (short)(1 * clr[0][i+1].G); // C R
            cur_val_G += (short)(0 * clr[1][i+1].G); // N R
        }
        cur_val_G += (short) (1 * clr[1][i].G); // N C
        if(cur_val_G < 0)
            cur_val_G = 0;
        else if(cur_val_G>255)
            cur_val_G = 255;
        clr[0][i].G = cur_val_G; // G finished

        short cur_val_B = (short)( -4 * clr[0][i].B) ; // C C
        //printf("\n cur_val_R = %d\n",cur_val_R);
        if(i>0)
        {
            cur_val_B += (short) ( 1 * help_line[i-1].B); // ? L
            cur_val_B += (short) ( 0 * clr[1][i-1].B); // N L
        }
        if(i + 1 < BMPInfoHeader->width)
        {
            cur_val_B += (short)(1 * clr[0][i+1].B); // C R
            cur_val_B += (short)(0 * clr[1][i+1].B); // N R
        }
        cur_val_B += (short) (1 * clr[1][i].B); // N C
        if(cur_val_B < 0)
            cur_val_B = 0;
        else if(cur_val_B>255)
            cur_val_B = 255;
        clr[0][i].B = cur_val_B; // B finished
    }
    struct Clr left;
    for(i = 1; i < BMPInfoHeader->height;i++)
    {
        left.R = 0;
        left.G = 0;
        left.B = 0;
        for(j = 0; j < BMPInfoHeader->width; j++)
        {
            short cur_val_R =(short)( -4 * clr[i][j].R ); // C C
            cur_val_R += (short)( 1 * left.R ); // C L
            if(j>0)
            {
                cur_val_R += (short) ( 0 * help_line[j-1].R); // V L
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_R += (short) (0 * clr[i+1][j-1].R); // N L
                }
            }
            if(j + 1 < BMPInfoHeader->width)
            {
                cur_val_R += (short) (1 * clr[i][j+1].R) ; // C R
                if(i > 0)
                {
                    cur_val_R += (short) ( 0 * help_line[j+1].R); // V R
                }
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_R += (short) (0 * clr[i+1][j+1].R); // N R
                }
            }
            if(i>0)
            {
                cur_val_R += (short) (1 * help_line[j].R); // V C
            }
            if(i + 1 < BMPInfoHeader->height)
            {
                cur_val_R += (short) (1 * clr[i+1][j].R); // N C
            }
            if(cur_val_R < 0)
                cur_val_R = 0;
            else if(cur_val_R>255)
                cur_val_R = 255;
            // R finished
            short cur_val_G =(short)( -4 * clr[i][j].G ); // C C
            cur_val_G += (short)( 1 * left.G ); // C L
            if(j>0)
            {
                cur_val_G += (short) ( 0 * help_line[j-1].G); // V L
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_G += (short) (0 * clr[i+1][j-1].G); // N L
                }
            }
            if(j + 1 < BMPInfoHeader->width)
            {
                cur_val_G += (short) (1 * clr[i][j+1].G) ; // C R
                if(i > 0)
                {
                    cur_val_G += (short) ( 0 * help_line[j+1].G); // V R
                }
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_G += (short) (0 * clr[i+1][j+1].G); // N R
                }
            }
            if(i>0)
            {
                cur_val_G += (short) (1 * help_line[j].G); // V C
            }
            if(i + 1 < BMPInfoHeader->height)
            {
                cur_val_G += (short) (1 * clr[i+1][j].G); // N C
            }
            if(cur_val_G < 0)
                cur_val_G = 0;
            else if(cur_val_G>255)
                cur_val_G = 255;
            // G finished

            short cur_val_B =(short)( -4 * clr[i][j].B ); // C C
            cur_val_B += (short)( 1 * left.B ); // C L
            if(j>0)
            {
                cur_val_B += (short) ( 0 * help_line[j-1].B); // V L
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_B += (short) (0 * clr[i+1][j-1].B); // N L
                }
            }
            if(j + 1 < BMPInfoHeader->width)
            {
                cur_val_B += (short) (1 * clr[i][j+1].B) ; // C R
                if(i > 0)
                {
                    cur_val_B += (short) ( 0 * help_line[j+1].B); // V R
                }
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_B += (short) (0 * clr[i+1][j+1].B); // N R
                }
            }
            if(i>0)
            {
                cur_val_B += (short) (1 * help_line[j].B); // V C
            }
            if(i + 1 < BMPInfoHeader->height)
            {
                cur_val_B += (short) (1 * clr[i+1][j].B); // N C
            }
            if(cur_val_B < 0)
                cur_val_B = 0;
            else if(cur_val_B>255)
                cur_val_B = 255;
            // B finished

            if(j>0)
                help_line[j-1] = left;
            if(j + 1 == BMPInfoHeader->width)
                help_line[j] = clr[i][j];
            left = clr[i][j];
            clr[i][j].R = cur_val_R;
            clr[i][j].G = cur_val_G;
            clr[i][j].B = cur_val_B;
        }
    }
    char* pic_name = malloc(sizeof(char) * 40);
    char *ptr;
    ptr = strtok(file_name, ".");
    strcpy(pic_name, ptr);
    strcat(pic_name, "_f2.bmp");

    myWriteBMP(pic_name,clr,BMPFileHeader,BMPInfoHeader);
    if(pic_name!=NULL)
    free(pic_name);
    if(help_line!=NULL)
    free(help_line);
}
void task2_f3_(struct Clr** clr,
               struct bmp_fileheader* BMPFileHeader,
               struct bmp_infoheader* BMPInfoHeader,
               char* file_name
               )
{
    int i,j;
    /*  1 , 0 , -1
        0 , 0 , 0,
        -1 , 0 , 1    */
    struct Clr* help_line = malloc(sizeof(struct Clr) * BMPInfoHeader->width);
    for(i = 0; i < BMPInfoHeader->width; i++)
    {
        help_line[i] = clr[0][i];

        short cur_val_R = (short)( 0 * clr[0][i].R) ; // C C
        //printf("\n cur_val_R = %d\n",cur_val_R);
        if(i>0)
        {
            cur_val_R += (short) ( 0 * help_line[i-1].R); // ? L
            cur_val_R += (short) ( -1 * clr[1][i-1].R); // N L
        }
        if(i + 1 < BMPInfoHeader->width)
        {
            cur_val_R += (short)(0 * clr[0][i+1].R); // C R
            cur_val_R += (short)(1 * clr[1][i+1].R); // N R
        }
        cur_val_R += (short) (0 * clr[1][i].R); // N C
        if(cur_val_R < 0)
            cur_val_R = 0;
        else if(cur_val_R>255)
            cur_val_R = 255;
        clr[0][i].R = cur_val_R; //  R finished

        short cur_val_G = (short)( 0 * clr[0][i].G) ; // C C
        //printf("\n cur_val_R = %d\n",cur_val_R);
        if(i>0)
        {
            cur_val_G += (short) ( 0 * help_line[i-1].G); // ? L
            cur_val_G += (short) ( -1 * clr[1][i-1].G); // N L
        }
        if(i + 1 < BMPInfoHeader->width)
        {
            cur_val_G += (short)(0 * clr[0][i+1].G); // C R
            cur_val_G += (short)(1 * clr[1][i+1].G); // N R
        }
        cur_val_G += (short) (0 * clr[1][i].G); // N C
        if(cur_val_G < 0)
            cur_val_G = 0;
        else if(cur_val_G>255)
            cur_val_G = 255;
        clr[0][i].G = cur_val_G; // G finished

        short cur_val_B = (short)( 0 * clr[0][i].B) ; // C C
        //printf("\n cur_val_R = %d\n",cur_val_R);
        if(i>0)
        {
            cur_val_B += (short) ( 0 * help_line[i-1].B); // ? L
            cur_val_B += (short) ( -1 * clr[1][i-1].B); // N L
        }
        if(i + 1 < BMPInfoHeader->width)
        {
            cur_val_B += (short)(0 * clr[0][i+1].B); // C R
            cur_val_B += (short)(1 * clr[1][i+1].B); // N R
        }
        cur_val_B += (short) (0 * clr[1][i].B); // N C
        if(cur_val_B < 0)
            cur_val_B = 0;
        else if(cur_val_B>255)
            cur_val_B = 255;
        clr[0][i].B = cur_val_B; // B finished
    }
    struct Clr left;
    for(i = 1; i < BMPInfoHeader->height;i++)
    {
        left.R = 0;
        left.G = 0;
        left.B = 0;
        for(j = 0; j < BMPInfoHeader->width; j++)
        {
            short cur_val_R =(short)( 0 * clr[i][j].R ); // C C
            cur_val_R += (short)( 0 * left.R ); // C L
            if(j>0)
            {
                cur_val_R += (short) ( 1 * help_line[j-1].R); // V L
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_R += (short) (-1 * clr[i+1][j-1].R); // N L
                }
            }
            if(j + 1 < BMPInfoHeader->width)
            {
                cur_val_R += (short) (0 * clr[i][j+1].R) ; // C R
                if(i > 0)
                {
                    cur_val_R += (short) ( -1 * help_line[j+1].R); // V R
                }
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_R += (short) (1 * clr[i+1][j+1].R); // N R
                }
            }
            if(i>0)
            {
                cur_val_R += (short) (0 * help_line[j].R); // V C
            }
            if(i + 1 < BMPInfoHeader->height)
            {
                cur_val_R += (short) (0 * clr[i+1][j].R); // N C
            }
            if(cur_val_R < 0)
                cur_val_R = 0;
            else if(cur_val_R>255)
                cur_val_R = 255;
            // R finished
            short cur_val_G =(short)( 0 * clr[i][j].G ); // C C
            cur_val_G += (short)( 0 * left.G ); // C L
            if(j>0)
            {
                cur_val_G += (short) ( 1 * help_line[j-1].G); // V L
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_G += (short) (-1 * clr[i+1][j-1].G); // N L
                }
            }
            if(j + 1 < BMPInfoHeader->width)
            {
                cur_val_G += (short) (0 * clr[i][j+1].G) ; // C R
                if(i > 0)
                {
                    cur_val_G += (short) ( -1 * help_line[j+1].G); // V R
                }
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_G += (short) (1 * clr[i+1][j+1].G); // N R
                }
            }
            if(i>0)
            {
                cur_val_G += (short) (0 * help_line[j].G); // V C
            }
            if(i + 1 < BMPInfoHeader->height)
            {
                cur_val_G += (short) (0 * clr[i+1][j].G); // N C
            }
            if(cur_val_G < 0)
                cur_val_G = 0;
            else if(cur_val_G>255)
                cur_val_G = 255;
            // G finished

            short cur_val_B =(short)( 0 * clr[i][j].B ); // C C
            cur_val_B += (short)( 0 * left.B ); // C L
            if(j>0)
            {
                cur_val_B += (short) ( 1 * help_line[j-1].B); // V L
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_B += (short) (-1 * clr[i+1][j-1].B); // N L
                }
            }
            if(j + 1 < BMPInfoHeader->width)
            {
                cur_val_B += (short) (0 * clr[i][j+1].B) ; // C R
                if(i > 0)
                {
                    cur_val_B += (short) ( -1 * help_line[j+1].B); // V R
                }
                if(i + 1 < BMPInfoHeader->height)
                {
                    cur_val_B += (short) (1 * clr[i+1][j+1].B); // N R
                }
            }
            if(i>0)
            {
                cur_val_B += (short) (0 * help_line[j].B); // V C
            }
            if(i + 1 < BMPInfoHeader->height)
            {
                cur_val_B += (short) (0 * clr[i+1][j].B); // N C
            }
            if(cur_val_B < 0)
                cur_val_B = 0;
            else if(cur_val_B>255)
                cur_val_B = 255;
            // B finished

            if(j>0)
                help_line[j-1] = left;
            if(j + 1 == BMPInfoHeader->width)
                help_line[j] = clr[i][j];
            left = clr[i][j];
            clr[i][j].R = cur_val_R;
            clr[i][j].G = cur_val_G;
            clr[i][j].B = cur_val_B;
        }
    }
    char* pic_name = malloc(sizeof(char) * 40);
    char *ptr;
    ptr = strtok(file_name, ".");
    strcpy(pic_name, ptr);
    strcat(pic_name, "_f3.bmp");

    myWriteBMP(pic_name,clr,BMPFileHeader,BMPInfoHeader);
    if(pic_name!=NULL)
    free(pic_name);
    if(help_line!=NULL)
    free(help_line);
}
unsigned char myAbs(unsigned char a, unsigned char b)
{
    if(b>a)
        return b - a;
    return a - b;
}
int checkForTrue(struct Clr clr1, struct Clr clr2, int  val)
{
    if((0ll + myAbs(clr1.B , clr2.B) + myAbs(clr1.G , clr2.G) + myAbs(clr1.R , clr2.R)) <= val)
        return 1;
    return 2;
}
struct Point
{
    short x;
    short y;
};
struct Node {
	struct Point data;
	struct Node* next;
};
// Two glboal variables to store address of front and rear nodes.
struct Node* front = NULL;
struct Node* rear = NULL;

// To Enqueue an integer
void Enqueue(struct Point xy) {
	struct Node* temp =
		(struct Node*)malloc(sizeof(struct Node));
	temp->data = xy;
	if(front == NULL && rear == NULL){
		front = rear = temp;
		return;
	}
	rear->next = temp;
	rear = temp;
}

// To Dequeue an integer.
void Dequeue() {
	struct Node* temp = front;
	if(front == rear) {
		front = rear = NULL;
	}
	else {
		front = front->next;
	}
	if(temp!=NULL)
	free(temp);
}

struct Point Front() {
	return front->data;
}

void bfs(struct Clr** clr, unsigned short width, unsigned short height, int val)
{
    short i, j;
    unsigned char** isUsed = (unsigned char**)malloc(height * sizeof(unsigned char*));
    for(i = 0; i < height; i++)
        isUsed[i] = (unsigned char*)malloc(width * sizeof(unsigned char));
    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++)
        isUsed[i][j] = 0;
    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            //printf("i = %d j = %d\n",i,j);
            if(isUsed[i][j] == 1)
                continue;
            struct Point point;
            struct Point frt_point;
            frt_point.x = -1;
            point.x = i;
            point.y = j;
            isUsed[point.x][point.y] = 1;
            Enqueue(point);
            int ans = 0;
            while(front != NULL)
            {
                ans++;
                point = Front();
                //printf("x = %d y = %d\n",point.x,point.y);
                Dequeue();
                if(frt_point.x == -1)
                {
                    frt_point.x = point.x;
                    frt_point.y = point.y;
                }
                else
                {
                    //printf("%u %u\n",clr[point.x][point.y].B,clr[frt_point.x][frt_point.y].B);
                    clr[point.x][point.y].B = clr[frt_point.x][frt_point.y].B;
                    clr[point.x][point.y].G = clr[frt_point.x][frt_point.y].G;
                    clr[point.x][point.y].R = clr[frt_point.x][frt_point.y].R;
                    //clr[point.x][point.y] = clr[frt_point->x][frt_point->y];
                }
                struct Point new_point;
                if((point.x + 1 < height) && isUsed[point.x + 1][point.y] ==  0 && checkForTrue(clr[frt_point.x][frt_point.y],clr[point.x + 1][point.y],val) == 1)
                {
                    isUsed[point.x + 1][point.y] = 1;
                    new_point = point;
                    new_point.x++;
                    Enqueue(new_point);
                }
                if((point.x >= 1) && isUsed[point.x - 1][point.y] ==  0 && checkForTrue(clr[frt_point.x][frt_point.y],clr[point.x - 1][point.y],val)==1)
                {
                    isUsed[point.x - 1][point.y] = 1;
                    new_point = point;
                    new_point.x--;
                    Enqueue(new_point);
                }
                if((point.y + 1 < width) && isUsed[point.x][point.y + 1] == 0 && checkForTrue(clr[frt_point.x][frt_point.y],clr[point.x][point.y + 1],val) == 1)
                {
                    isUsed[point.x][point.y + 1] = 1;
                    new_point = point;
                    new_point.y++;
                    Enqueue(new_point);
                }
                if((point.y >= 1) && isUsed[point.x][point.y - 1] ==  0 && checkForTrue(clr[frt_point.x][frt_point.y],clr[point.x][point.y - 1],val) == 1)
                {
                    isUsed[point.x][point.y - 1] = 1;
                    new_point = point;
                    new_point.y--;
                    Enqueue(new_point);
                }
            }
        }
    }
    for(i = 0; i < height; i++)
        if(isUsed[i]!=NULL)free(isUsed[i]);
    if(isUsed!=NULL)
        free(isUsed);


}
void task3_(struct Clr** clr,
            struct bmp_fileheader* BMPheader,
            struct bmp_infoheader* BMPInfoheader,
            char* file_name,
            int val
            )
{
    signed int width = BMPInfoheader->width, height = BMPInfoheader->height;
    bfs(clr,width,height,val);
    char* pic_name = malloc(sizeof(char) * 40);
    char *ptr;
    short i,j;
    ptr = strtok(file_name, ".");
    strcpy(pic_name, ptr);
    strcat(pic_name, "_compressed.bin");

    FILE* file = fopen("compressed.bin","wb");

     // FileHeader writing

    fwrite((char*)&(BMPheader->fileMarker1),sizeof(unsigned char),1,file);
    fwrite((char*)&(BMPheader->fileMarker2),sizeof(unsigned char),1,file);
    fwrite((char*)&(BMPheader->bfSize),sizeof(unsigned int),1,file);
    fwrite((char*)&(BMPheader->unused1),sizeof(unsigned short),1,file);
    fwrite((char*)&(BMPheader->unused2),sizeof(unsigned short),1,file);
    fwrite((char*)&(BMPheader->imageDataOffset),sizeof(unsigned int),1,file);
    // end

    // FileInfoHeader
    fwrite((char*)&(BMPInfoheader->biSize),sizeof(unsigned int),1,file);
    fwrite((char*)&(BMPInfoheader->width),sizeof(signed int),1,file);
    fwrite((char*)&(BMPInfoheader->height),sizeof(signed int),1,file);
    fwrite((char*)&(BMPInfoheader->planes),sizeof(unsigned short),1,file);
    fwrite((char*)&(BMPInfoheader->bitPix),sizeof(unsigned short),1,file);
    fwrite((char*)&(BMPInfoheader->biCompression),sizeof(unsigned int),1,file);
    fwrite((char*)&(BMPInfoheader->biSizeImage),sizeof(unsigned int),1,file);
    fwrite((char*)&(BMPInfoheader->biXPelsPerMeter),sizeof(int),1,file);
    fwrite((char*)&(BMPInfoheader->biYPelsPerMeter),sizeof(int),1,file);
    fwrite((char*)&(BMPInfoheader->biClrUsed),sizeof(unsigned int),1,file);
    fwrite((char*)&(BMPInfoheader->biClrImportant),sizeof(unsigned int),1,file);
    // end
    unsigned char nul = 0;
    for(i = 0; i < BMPheader->imageDataOffset - 54; i++)
        fwrite((char*)&nul,sizeof(unsigned char),1,file);
    for(i = 1; i <= height; i++)
    {
        for( j = 1; j <= width; j++)
        {
            if(i+1<=height && j+1<=width && i>=2 && j>=2
               && clr[i][j-1].B == clr[i-1][j-1].B && clr[i][j-1].G == clr[i-1][j-1].G && clr[i][j-1].R == clr[i-1][j-1].R
               && clr[i-2][j-1].B == clr[i-1][j-1].B && clr[i-2][j-1].G == clr[i-1][j-1].G && clr[i-2][j-1].R == clr[i-1][j-1].R
               && clr[i-1][j].B == clr[i-1][j-1].B && clr[i-1][j].G == clr[i-1][j-1].G && clr[i-1][j].R == clr[i-1][j-1].R
               && clr[i-1][j-2].B == clr[i-1][j-1].B && clr[i-1][j-2].G == clr[i-1][j-1].G && clr[i-1][j-2].R == clr[i-1][j-1].R
               //&& clr[i-2][j-2].B == clr[i-1][j-1].B && clr[i-2][j-2].G == clr[i-1][j-1].G && clr[i-2][j-2].R == clr[i-1][j-1].R
               //&& clr[i][j-2].B == clr[i-1][j-1].B && clr[i][j-2].G == clr[i-1][j-1].G && clr[i][j-2].R == clr[i-1][j-1].R
               //&& clr[i-2][j].B == clr[i-1][j-1].B && clr[i-2][j].G == clr[i-1][j-1].G && clr[i-2][j].R == clr[i-1][j-1].R
               //&& clr[i][j].B == clr[i-1][j-1].B && clr[i][j].G == clr[i-1][j-1].G && clr[i][j].R == clr[i-1][j-1].R

               ) continue;
            fwrite(&i,sizeof(short),1,file);
            fwrite(&j,sizeof(short),1,file);
            fwrite((char*)&(clr[i-1][j-1].R),sizeof(unsigned char),1,file);
            fwrite((char*)&(clr[i-1][j-1].G),sizeof(unsigned char),1,file);
            fwrite((char*)&(clr[i-1][j-1].B),sizeof(unsigned char),1,file);
        }
    }
    fclose(file);
    if(pic_name!=NULL)
    free(pic_name);

}
void bfs_task4(struct Clr** clr, struct bmp_infoheader* BMPInfoheader, FILE* file)
{
    short i, j;
    short height = BMPInfoheader->height,width = BMPInfoheader->width;
    unsigned char** isUsed = (unsigned char**)malloc(height * sizeof(unsigned char*));
    for(i = 0; i < height; i++)
        isUsed[i] = (unsigned char*)malloc(width * sizeof(unsigned char));
    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++)
        isUsed[i][j] = 0,clr[i][j].B = clr[i][j].G = clr[i][j].R = 0;
    for(i = 1; i <= BMPInfoheader->height; i++)
    {
        for( j = 1; j <= BMPInfoheader->width; j++)
        {
            short indx,indy;
            fread((char*)&(indx),sizeof(short),1,file);
            fread((char*)&(indy),sizeof(short),1,file);
            fread((char*)&(clr[indx-1][indy-1].R),sizeof(unsigned char),1,file);
            fread((char*)&(clr[indx-1][indy-1].G),sizeof(unsigned char),1,file);
            fread((char*)&(clr[indx-1][indy-1].B),sizeof(unsigned char),1,file);
            //printf("%d %d\n",indx,indy);
            isUsed[indx-1][indy-1] = 1;
            if(indx == BMPInfoheader->height && indy == BMPInfoheader->width)
            {
                i = BMPInfoheader->height + 1;
                j = BMPInfoheader->width + 1;
            }
        }
    }
    fclose(file);
    for(i = 0; i < height; i++)
    {
        for(j = 0; j < width; j++)
        {
            //printf("i = %d j = %d\n",i,j);
            if(isUsed[i][j] != 1)
                continue;
            struct Point point;
            struct Point frt_point;
            frt_point.x = -1;
            point.x = i;
            point.y = j;
            isUsed[point.x][point.y] = 1;
            Enqueue(point);
            while(front != NULL)
            {
                point = Front();
                //printf("x = %d y = %d\n",point.x,point.y);
                Dequeue();
                if(frt_point.x == -1)
                {
                    frt_point.x = point.x;
                    frt_point.y = point.y;
                }
                else
                {
                    //printf("%u %u\n",clr[point.x][point.y].B,clr[frt_point.x][frt_point.y].B);
                    clr[point.x][point.y].B = clr[frt_point.x][frt_point.y].B;
                    clr[point.x][point.y].G = clr[frt_point.x][frt_point.y].G;
                    clr[point.x][point.y].R = clr[frt_point.x][frt_point.y].R;
                    //clr[point.x][point.y] = clr[frt_point->x][frt_point->y];
                }
                struct Point new_point;
                if((point.x + 1 < height) && isUsed[point.x + 1][point.y] ==  0 )
                {
                    isUsed[point.x + 1][point.y] = 1;
                    new_point = point;
                    new_point.x++;
                    Enqueue(new_point);
                }
                if((point.x >= 1) && isUsed[point.x - 1][point.y] ==  0 )
                {
                    isUsed[point.x - 1][point.y] = 1;
                    new_point = point;
                    new_point.x--;
                    Enqueue(new_point);
                }
                if((point.y + 1 < width) && isUsed[point.x][point.y + 1] == 0)
                {
                    isUsed[point.x][point.y + 1] = 1;
                    new_point = point;
                    new_point.y++;
                    Enqueue(new_point);
                }
                if((point.y >= 1) && isUsed[point.x][point.y - 1] ==  0 )
                {
                    isUsed[point.x][point.y - 1] = 1;
                    new_point = point;
                    new_point.y--;
                    Enqueue(new_point);
                }
            }
        }
    }
    for(i = 0; i < height; i++)
        if(isUsed[i]!=NULL)free(isUsed[i]);
    if(isUsed!=NULL)
        free(isUsed);


}
void task4_(struct Clr** clr,
            struct bmp_fileheader* BMPheader,
            struct bmp_infoheader* BMPInfoheader,
            char* file_name
            )
{

    int i,j;
    FILE* file = fopen(file_name,"rb");
     // FileHeader writing

    fread((char*)&(BMPheader->fileMarker1),sizeof(unsigned char),1,file);
    fread((char*)&(BMPheader->fileMarker2),sizeof(unsigned char),1,file);
    fread((char*)&(BMPheader->bfSize),sizeof(unsigned int),1,file);
    fread((char*)&(BMPheader->unused1),sizeof(unsigned short),1,file);
    fread((char*)&(BMPheader->unused2),sizeof(unsigned short),1,file);
    fread((char*)&(BMPheader->imageDataOffset),sizeof(unsigned int),1,file);
    // end

    // FileInfoHeader
    fread((char*)&(BMPInfoheader->biSize),sizeof(unsigned int),1,file);
    fread((char*)&(BMPInfoheader->width),sizeof(signed int),1,file);
    fread((char*)&(BMPInfoheader->height),sizeof(signed int),1,file);
    fread((char*)&(BMPInfoheader->planes),sizeof(unsigned short),1,file);
    fread((char*)&(BMPInfoheader->bitPix),sizeof(unsigned short),1,file);
    fread((char*)&(BMPInfoheader->biCompression),sizeof(unsigned int),1,file);
    fread((char*)&(BMPInfoheader->biSizeImage),sizeof(unsigned int),1,file);
    fread((char*)&(BMPInfoheader->biXPelsPerMeter),sizeof(int),1,file);
    fread((char*)&(BMPInfoheader->biYPelsPerMeter),sizeof(int),1,file);
    fread((char*)&(BMPInfoheader->biClrUsed),sizeof(unsigned int),1,file);
    fread((char*)&(BMPInfoheader->biClrImportant),sizeof(unsigned int),1,file);
    // end
    fseek(file,BMPheader->imageDataOffset - 54,SEEK_CUR);


    bfs_task4(clr,BMPInfoheader,file);
    myWriteBMP("decompressed.bmp",clr,BMPheader,BMPInfoheader);
}
int main(int argc, char *argv[])
{
    short i,j;
    struct bmp_fileheader BMPFileHeader;
    struct bmp_infoheader BMPInfoHeader;

    FILE* mainName = fopen("input.txt","r");
    char file1[40],file2[40];
    unsigned int threshold;
    //FILE* fileout = fopen("output.txt","w");

    fgets(file1,sizeof(file1),mainName);
    fscanf(mainName,"%d\n",&threshold);
    fgets(file2,sizeof(file2),mainName);
    fclose(mainName);
    for ( i = 0; i < 40; i++ )
    {
        if ( file1[i] == '\n' )
        {
            file1[i] = '\0';
            break;
        }
    }
    for ( i = 0; i < 40; i++ )
    {
        if ( file2[i] == '\n' )
        {
            file2[i] = '\0';
            break;
        }
    }
    struct Clr** clr = myReadBMP(file1,&BMPFileHeader,&BMPInfoHeader);
    task1_(clr,&BMPFileHeader,&BMPInfoHeader,file1);
    task2_f1_(clr,&BMPFileHeader,&BMPInfoHeader,file1);
    ret_val(clr,&BMPInfoHeader);
    task1_(clr,&BMPFileHeader,&BMPInfoHeader,file1);
    task2_f2_(clr,&BMPFileHeader,&BMPInfoHeader,file1);
    ret_val(clr,&BMPInfoHeader);
    task1_(clr,&BMPFileHeader,&BMPInfoHeader,file1);
    task2_f3_(clr,&BMPFileHeader,&BMPInfoHeader,file1);
    ret_val(clr,&BMPInfoHeader);
    task3_(clr,&BMPFileHeader,&BMPInfoHeader,file1,threshold);
    ret_val(clr,&BMPInfoHeader);
    task4_(clr,&BMPFileHeader,&BMPInfoHeader,file2);
    for(i = 0; i < BMPInfoHeader.height; i++)
        if(clr[i]!=NULL)free(clr[i]);
    if(clr!=NULL)free(clr);
    //free(file1);

    return 0;
}
