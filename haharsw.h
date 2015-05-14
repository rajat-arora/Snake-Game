

#ifndef _HAHARSW_H_
#define _HAHARSW_H_

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <GL/glut.h>

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE   0x812F
#endif

#ifndef GL_BGR_EXT
#define GL_BGR_EXT         0x80E0
#endif

#define  S_LEN             30.0f                //determine the length of big cube
#define  HALF_L            S_LEN*0.5f           //half length of big cube use for calculate vertex of big cube
#define  HALF_RAISED       HALF_L*1.005f
#define  LIGHT_COORD       S_LEN                //use for lighting location
#define  UNIT              1.0f                 //length of small cube
#define  HALF_U            UNIT*0.5f
#define  CAMERA_POS        S_LEN*1.8f           //origin camera position 
#define  SKY_POS           S_LEN*2.0f           //radius of big circle(sky)
#define  ROTATE_ANGLE      45.0f                //rotate angle
#define  TIMER_SKIP        150                  //time unit
#define  DELTA_CAM_POS     UNIT*6               //first position camera location
#define  PRE_CAM_LEN       UNIT*6               //first position camera location
#define  POST_CAM_LEN      UNIT*6               //first position camera location
#define  CAMERA_TYPE_A     0                    //third person camera view
#define  CAMERA_TYPE_B     1                    //first person camera view
#define  WINDOW_HEIGHT     600
#define  WINDOW_WIDTH      800
#define  WINDOW_TITLE      "Opengl Anakonda"

#ifdef _DRAW_AXIS_
#define  AXIS_ADDER        UNIT*4.0f
#endif
//texture
#define  GRASS_BMP          "grass.bmp"           //big cube
#define  BARRIER_BMP        "barrier.bmp"         //barrier
#define  SNAKESKIN_BMP      "snake_skin.bmp"      //snack
#define  SNAKEHEAD_BMP      "snake_head.bmp"      //snack head
#define  SKY_DOME           "skydome.bmp"         //sky
#define  OVER_BMP           "over.bmp"            //game over picture

//index of texture 
#define TEXTURE_COUNT       5
#define GRASS_TEXTURE       0
#define BARRIER_TEXTURE     1
#define SNAKESKIN_TEXTURE   2
#define SNAKEHEAD_TEXTURE   3
#define SKYDOME_TEXTURE     4

#define BORDER_MIN         0        //big cube surface'min coordinate on 2d plant
#define BORDER_MAX        31        //big cube surface'man coordinate on 2d plant
#define DIR_U              2        //up movement
#define DIR_R              3        //right
#define DIR_D              5        //down
#define DIR_L              7        //left movement
#define MIN_X             11        //x minimum coordinate on big cube's surface
#define MAX_X             13        //x maximum coordinate on big cube's surface
#define MIN_Y             17        //y minimum coordinate on big cube's surface
#define MAX_Y             19        //y maximum coordinate on big cube's surface
#define SURFACE_1         11        //index of six surface
#define SURFACE_2         13
#define SURFACE_3         17
#define SURFACE_4         19
#define SURFACE_5         23
#define SURFACE_6         29

#define INTEGER_LEN       30
#define GRID_TYPE_EMPTY   0        //mean its empty
#define GRID_TYPE_BLOCK   1        //barrier
#define GRID_TYPE_COIN    2        //coin
#define BARRIER_COUNT     10       //random barrier on surface
#define COIN_COUNT        20       //random coin on surface
int snakeP[3];


//make a two direction list for snack's body
typedef struct _tagS_BODY
{
    int cube_face;                 //which surface did this node on
    int pos_2d_x;                  //index x of this node's 2d coordinate
    int pos_2d_y;                  //index y of this node's 2d coordinate
    float pos_3d[3];               //index where did it on third coordinate
    float color[3];                //index its colour
    struct _tagS_BODY* next;       //index its next node
    struct _tagS_BODY* previous;   //index its previous node
}SBODY_NODE,*PSBODY_NODE;

//snack head structure
typedef struct _tagS_HEAD
{
	int   cube_face;               //which surface did snack head on
    int   s_dir;                   //direction
    int   pos_2d_x;                //index x of snack head's 2d coordinate
    int   pos_2d_y;                //index y of snack head's 2d coordinate
    int   length;                  //length
    float pos_3d[3];               //3d coordinate
    float color[3];                //colour

    int   cube_face_last;          //last surface did last snack node on
    int   pos_2d_x_last;           //its x coordinate
    int   pos_2d_y_last;           //its y coordinate
    float pos_last[3];             //its 3d coordinate

    struct _tagS_BODY* body_section;    //first node of snack body
    struct _tagS_BODY* last_section;    //last node of snack body
}SNAKE_HEAD,*PSNAKE_HEAD;

#define PI 3.1415926535897f
#define DTOR (PI/180.0f)
#define SQR(x) (x*x)

//sky's vertex and texture coordinate
typedef struct _tagVERTEX{
    float x,y,z;
    unsigned int color;
    float u, v;
}VERTEX;

//relate to scene
void scene_initialize();   
void scene_cleanup();        
int  snake_in_scene();
void draw_scene();
void scene_update();
void print_info();

//relate snack control function
void snake_initialize();
void snake_cleanup();
void snake_turn_head(int dir);
int  snake_update_pos();
void draw_snake();
void camera_follow_snake();



//relate to texture
void texture_initialize();
void texture_cleanup();
void render_over(int w,int h);


#define MAX_PARTICLES		4000     //define max particles

//define the variable of particle system
typedef struct _tagPARTICLE
{
    struct _tagPARTICLE *prev,*next;  //particle's successor and precursor
    int     shape;                    //shape of particle
    int		life;                     //lifetime of particle
    float	pos[3];                   //its coordinate
    float   dir[3];                   //its vertex
    float	force[3];                 //its speed
    float   color[3];                 //its colour
    float   deltaColor[3];
}Particle;



//relate to particle 
void particle_initialize();
void particle_cleanup();
void emit_particle(int surface,float pos[3]);
void update_emitter();
void render_emitter();

//relate to info show on screen
void font_initialize();
void font_cleanup();
void PrintString(char *s,int x,int y);

#endif
