//texture bmp loader; this code is sourced from here
// http://ubuntuforums.org/showthread.php?t=1730154
//and OpenGL Superbible 5th edition

#include "haharsw.h"

///////////////////////////////////////////////////////////////////////////////
// This function opens the "bitmap" file given (szFileName), verifies that it is
// a 24bit .BMP file and loads the bitmap bits needed so that it can be used
// as a texture. The width and height of the bitmap are returned in nWidth and
// nHeight. The memory block allocated and returned must be deleted with free();
// The returned array is an 888 BGR texture
// These structures match the layout of the equivalent Windows specific structs 
// used by Win32
#pragma pack(1)
typedef struct _tagRGB { 
    GLbyte blue;
    GLbyte green;
    GLbyte red;
    GLbyte alpha;
}RGB;

typedef struct _tagBMPInfoHeader {
    GLuint	size;
    GLuint	width;
    GLuint	height;
    GLushort  planes;
    GLushort  bits;
    GLuint	compression;
    GLuint	imageSize;
    GLuint	xScale;
    GLuint	yScale;
    GLuint	colors;
    GLuint	importantColors;
}BMPInfoHeader;

typedef struct _tagBMPHeader {
    GLushort	type; 
    GLuint	    size; 
    GLushort	unused; 
    GLushort	unused2; 
    GLuint	    offset; 
}BMPHeader; 

typedef struct _tagBMPInfo {
    BMPInfoHeader		header;
    RGB				    colors[1];
}BMPInfo;
#pragma pack(8)

//保存TEXTURE_COUNT个纹理贴图的标示符数组
GLuint g_texture[TEXTURE_COUNT];
static GLubyte* s_pImage     = NULL;   //game over image data
static GLsizei  s_img_width  = 0;      //width of game over image
static GLsizei  s_img_height = 0;      //high of game over image

static GLubyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight)
{
    FILE*	      pFile;
    BMPInfo       *pBitmapInfo = NULL;
    unsigned long lInfoSize = 0;
    unsigned long lBitSize = 0;
    GLubyte       *pBits = NULL;					// Bitmaps bits
    BMPHeader	  bitmapHeader;

    // Attempt to open the file
    pFile = fopen(szFileName, "rb");
    if(pFile == NULL)
        return NULL;

    // File is Open. Read in bitmap header information
    fread(&bitmapHeader, sizeof(BMPHeader), 1, pFile);

    // Read in bitmap information structure
    lInfoSize = bitmapHeader.offset - sizeof(BMPHeader);
    pBitmapInfo = (BMPInfo *) malloc(sizeof(GLubyte)*lInfoSize);
    if(fread(pBitmapInfo, lInfoSize, 1, pFile) != 1)
    {
        free(pBitmapInfo);
        fclose(pFile);
        return NULL;
    }

    // Save the size and dimensions of the bitmap
    *nWidth = pBitmapInfo->header.width;
    *nHeight = pBitmapInfo->header.height;
    lBitSize = pBitmapInfo->header.imageSize;

    // If the size isn't specified, calculate it anyway	
    if(pBitmapInfo->header.bits != 24)
    {
        free(pBitmapInfo);
        return NULL;
    }

    if(lBitSize == 0)
        lBitSize = (*nWidth * pBitmapInfo->header.bits + 7) / 8 * abs(*nHeight);

    // Allocate space for the actual bitmap
    free(pBitmapInfo);
    pBits = (GLubyte*)malloc(sizeof(GLubyte)*lBitSize);

    // Read in the bitmap bits, check for corruption
    if(fread(pBits, lBitSize, 1, pFile) != 1)
    {
        free(pBits);
        pBits = NULL;
    }

    // Close the bitmap file now that we have all the data we need
    fclose(pFile);

    return pBits;
}

void texture_initialize()
{
    GLubyte *pBytes;
    GLint   iWidth, iHeight;
    
    //load game over picture, save to s_pImage
    s_pImage     = gltReadBMPBits(OVER_BMP,&iWidth,&iHeight);
    s_img_width  = iWidth;
    s_img_height = iHeight;

    //enable gl texture mode
    glEnable(GL_TEXTURE_2D);

    glGenTextures(TEXTURE_COUNT, g_texture);

    //grass
    pBytes = gltReadBMPBits(GRASS_BMP,&iWidth,&iHeight);
    glBindTexture(GL_TEXTURE_2D, g_texture[GRASS_TEXTURE]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, iWidth, iHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    free(pBytes);

    //barrier
    pBytes = gltReadBMPBits(BARRIER_BMP,&iWidth,&iHeight);
    glBindTexture(GL_TEXTURE_2D, g_texture[BARRIER_TEXTURE]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, iWidth, iHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    free(pBytes);

    //snack body
    pBytes = gltReadBMPBits(SNAKESKIN_BMP,&iWidth,&iHeight);
    glBindTexture(GL_TEXTURE_2D, g_texture[SNAKESKIN_TEXTURE]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, iWidth, iHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    free(pBytes);

    //snack head
    pBytes = gltReadBMPBits(SNAKEHEAD_BMP,&iWidth,&iHeight);
    glBindTexture(GL_TEXTURE_2D, g_texture[SNAKEHEAD_TEXTURE]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, iWidth, iHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    free(pBytes);

    //sky
    pBytes = gltReadBMPBits(SKY_DOME,&iWidth,&iHeight);
    glBindTexture(GL_TEXTURE_2D, g_texture[SKYDOME_TEXTURE]);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, iWidth, iHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBytes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    free(pBytes);
}

void texture_cleanup()
{
    //clean game over picture
    if(s_pImage!=NULL)
    {
        free(s_pImage);
        s_pImage = NULL;
    }

    //delete texture
    glDeleteTextures(TEXTURE_COUNT, g_texture);
}

//while game over show a game over picture
void render_over(int w,int h)
{
    glClear(GL_COLOR_BUFFER_BIT);    //clean buffer


    //push gl state
    glPushAttrib(GL_LIGHTING_BIT|GL_DEPTH_BUFFER_BIT|GL_TEXTURE_BIT|GL_CURRENT_BIT);
        //close opengl texture lighting and depthtest
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);

        //push gl projection and reset it
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
            glLoadIdentity();
            gluOrtho2D( 0.0, (GLfloat)w, 0.0, (GLfloat)h);    //设置平行投影矩阵

            glRasterPos2i(0, 0);    
            glPixelZoom((GLfloat) w / (GLfloat)s_img_width, h / (GLfloat)s_img_height);     //scale down  image
            glDrawPixels(s_img_width, s_img_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, s_pImage);//draw game over

        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    glPopAttrib();
}
