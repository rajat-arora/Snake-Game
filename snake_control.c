
#include "haharsw.h"

//set small cube's six surface, and their vertex
//barrier and snack body use that array vertex
static const float s_snake_cube_vertex[6][4][3] = 
{
    { { -HALF_U, -HALF_U,  HALF_U },{  HALF_U, -HALF_U,  HALF_U },
      {  HALF_U,  HALF_U,  HALF_U },{ -HALF_U,  HALF_U,  HALF_U } },
    { {  HALF_U, -HALF_U,  HALF_U },{  HALF_U, -HALF_U, -HALF_U },
      {  HALF_U,  HALF_U, -HALF_U },{  HALF_U,  HALF_U,  HALF_U } },
    { {  HALF_U, -HALF_U, -HALF_U },{ -HALF_U, -HALF_U, -HALF_U },
      { -HALF_U,  HALF_U, -HALF_U },{  HALF_U,  HALF_U, -HALF_U } },
    { { -HALF_U, -HALF_U, -HALF_U },{ -HALF_U, -HALF_U,  HALF_U },
      { -HALF_U,  HALF_U,  HALF_U },{ -HALF_U,  HALF_U, -HALF_U } },
    { { -HALF_U,  HALF_U,  HALF_U },{  HALF_U,  HALF_U,  HALF_U },
      {  HALF_U,  HALF_U, -HALF_U },{ -HALF_U,  HALF_U, -HALF_U } },
    { { -HALF_U, -HALF_U,  HALF_U },{ -HALF_U, -HALF_U, -HALF_U },
      {  HALF_U, -HALF_U, -HALF_U },{  HALF_U, -HALF_U,  HALF_U } }
};

//small cube's six face normal vector
static const float s_face_normal[6][3] = 
{
    {  0.0f,  0.0f,  1.0f },
    {  1.0f,  0.0f,  0.0f },
    {  0.0f,  0.0f, -1.0f },
    { -1.0f,  0.0f,  0.0f },
    {  0.0f,  1.0f,  0.0f },
    {  0.0f, -1.0f,  0.0f },
};

//golable variable for snack head
SNAKE_HEAD g_snake_head;

extern int     g_camera_type;
extern GLuint  g_texture[TEXTURE_COUNT];

//init snack
void snake_initialize()
{
    PSBODY_NODE pTmp;

    //init snack head
    g_snake_head.cube_face = SURFACE_1;     //on one of big cube surface
    g_snake_head.s_dir     = DIR_U;         //movement up
    g_snake_head.length    = 3;             //initial set snack length 3
    g_snake_head.pos_2d_x  = 1;             //initial snack head location 1,3
    g_snake_head.pos_2d_y  = 3;
    g_snake_head.pos_3d[0] = -HALF_L + HALF_U;          //inital snack head location in 3D space
    g_snake_head.pos_3d[1] = -HALF_L + UNIT*2 + HALF_U;
    g_snake_head.pos_3d[2] =  HALF_L + HALF_U;
    g_snake_head.color[0]  = 0.90f;                     //snack head colour
    g_snake_head.color[1]  = 0.01f;
    g_snake_head.color[2]  = 0.09f;

    //add one node to snack
    pTmp = (PSBODY_NODE)malloc(sizeof(SBODY_NODE));
    memset(pTmp,0,sizeof(SBODY_NODE));
    pTmp->cube_face = SURFACE_1;
    pTmp->pos_2d_x = 1;
    pTmp->pos_2d_y = 2;
    pTmp->pos_3d[0] = -HALF_L + HALF_U;
    pTmp->pos_3d[1] = -HALF_L + UNIT + HALF_U;
    pTmp->pos_3d[2] =  HALF_L + HALF_U;
    pTmp->color[0]  = 1.0f;
    pTmp->color[1]  = 1.0f;
    pTmp->color[2]  = 0.0f;
    g_snake_head.body_section = pTmp;
    g_snake_head.last_section = pTmp;

    //add second node to snack
    pTmp = (PSBODY_NODE)malloc(sizeof(SBODY_NODE));
    memset(pTmp,0,sizeof(SBODY_NODE));
    pTmp->cube_face = SURFACE_1;
    pTmp->pos_2d_x = 1;
    pTmp->pos_2d_y = 1;
    pTmp->pos_3d[0] = -HALF_L + HALF_U;
    pTmp->pos_3d[1] = -HALF_L + HALF_U;
    pTmp->pos_3d[2] =  HALF_L + HALF_U;
    pTmp->color[0]  = 1.0f;
    pTmp->color[1]  = 1.0f;
    pTmp->color[2]  = 0.0f;

    pTmp->previous                  = g_snake_head.last_section;
    g_snake_head.last_section->next = pTmp;
    g_snake_head.last_section       = pTmp;
}

//clean snack structure
void snake_cleanup()
{
    //clean node on snack body
    PSBODY_NODE pTmp1 = g_snake_head.body_section,pTmp2;
    while(pTmp1!=NULL)
    {
        pTmp2 = pTmp1;
        pTmp1 = pTmp1->next;
        free(pTmp2);
    }
}

//draw snack head
static void draw_snake_head()
{
    //front face of square
    glFrontFace(GL_CCW);

    //length
    glLineWidth(1.0f);

    glPushMatrix();    //transform current location to snack head
    glTranslatef(g_snake_head.pos_3d[0],g_snake_head.pos_3d[1],g_snake_head.pos_3d[2]);

    //snack head colour
    glColor3fv(g_snake_head.color);

    //snack head texture
    glBindTexture(GL_TEXTURE_2D, g_texture[SNAKEHEAD_TEXTURE]);

    glBegin(GL_QUADS);   //draw six surface to make snack head

        glNormal3fv(s_face_normal[0]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[0][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[0][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[0][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[0][3]);

        glNormal3fv(s_face_normal[1]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[1][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[1][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[1][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[1][3]);

        glNormal3fv(s_face_normal[2]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[2][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[2][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[2][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[2][3]);

        glNormal3fv(s_face_normal[3]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[3][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[3][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[3][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[3][3]);

        glNormal3fv(s_face_normal[4]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[4][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[4][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[4][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[4][3]);

        glNormal3fv(s_face_normal[5]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[5][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_snake_cube_vertex[5][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[5][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_snake_cube_vertex[5][3]);

    glEnd();

    glPopMatrix();
}

//draw snack body
static void draw_snake_body()
{
    PSBODY_NODE pNode = g_snake_head.body_section;

    glFrontFace(GL_CCW);

    //length size 
    glLineWidth(1.0f);

    //snack body texture
    glBindTexture(GL_TEXTURE_2D, g_texture[SNAKESKIN_TEXTURE]);
    while(pNode!=NULL)
    {
        glPushMatrix(); //set current location to mid of snack body
        glTranslatef(pNode->pos_3d[0],pNode->pos_3d[1],pNode->pos_3d[2]);
        glColor3fv(pNode->color);
        glBegin(GL_QUADS);    //draw snack body

            glNormal3fv(s_face_normal[0]);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[0][0]);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[0][1]);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[0][2]);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[0][3]);

            glNormal3fv(s_face_normal[1]);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[1][0]);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[1][1]);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[1][2]);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[1][3]);

            glNormal3fv(s_face_normal[2]);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[2][0]);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[2][1]);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[2][2]);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[2][3]);

            glNormal3fv(s_face_normal[3]);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[3][0]);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[3][1]);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[3][2]);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[3][3]);

            glNormal3fv(s_face_normal[4]);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[4][0]);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[4][1]);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[4][2]);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[4][3]);

            glNormal3fv(s_face_normal[5]);
            glTexCoord2f(0.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[5][0]);
            glTexCoord2f(1.0f, 0.0f);
            glVertex3fv(s_snake_cube_vertex[5][1]);
            glTexCoord2f(1.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[5][2]);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3fv(s_snake_cube_vertex[5][3]);

        glEnd();
        glPopMatrix();
        pNode = pNode->next;
    }
}

//render snack
void draw_snake()
{
    draw_snake_head();    //head
    draw_snake_body();    //body
}

//while snack change direction
void snake_turn_head(int dir)
{
    if(g_snake_head.pos_2d_x>BORDER_MIN&&
        g_snake_head.pos_2d_x<BORDER_MAX&&
        g_snake_head.pos_2d_y>BORDER_MIN&&
        g_snake_head.pos_2d_y<BORDER_MAX)   //while snack in edge of big cube
    {                                       //change direction not allow
        
        if(g_camera_type==CAMERA_TYPE_A)    //use third character view
        {
            //snack not allow to direction switch to its opposite direction
            switch(dir)
            {
            case DIR_U:
                if(g_snake_head.s_dir==DIR_D)
                    return;
                break;
            case DIR_R:
                if(g_snake_head.s_dir==DIR_L)
                    return;
                break;
            case DIR_D:
                if(g_snake_head.s_dir==DIR_U)
                    return;
                break;
            case DIR_L:
                if(g_snake_head.s_dir==DIR_R)
                    return;
                break;
            default:
                break;
            }
            g_snake_head.s_dir = dir;
        }
        else    //first person view
        {   
            //only exit turn left and turn right on first person view
            switch(g_snake_head.s_dir)
            {
            case DIR_U:
                if(dir==DIR_R)
                    g_snake_head.s_dir = DIR_R;
                else if(dir==DIR_L)
                    g_snake_head.s_dir = DIR_L;
                break;
            case DIR_R:
                if(dir==DIR_R)
                    g_snake_head.s_dir = DIR_D;
                else if(dir==DIR_L)
                    g_snake_head.s_dir = DIR_U;
                break;
            case DIR_D:
                if(dir==DIR_R)
                    g_snake_head.s_dir = DIR_L;
                else if(dir==DIR_L)
                    g_snake_head.s_dir = DIR_R;
                break;
            case DIR_L:
                if(dir==DIR_R)
                    g_snake_head.s_dir = DIR_U;
                else if(dir==DIR_L)
                    g_snake_head.s_dir = DIR_D;
                break;
            default:
                //_asm int 3;
                break;
            }
        }
    }
}

//while snack on edge of big cube 
//use six function to make a decision of snack's movement
//surface one
static void turn_in_surface_1(int border_type)
{
    switch(g_snake_head.s_dir*border_type)
    {
    case DIR_U*MIN_X:
    case DIR_U*MAX_X:
    case DIR_U*MIN_Y:
        //_asm int 3;
        break;
    case DIR_U*MAX_Y:
        g_snake_head.cube_face = SURFACE_5;
        g_snake_head.pos_2d_y  = BORDER_MIN + 1;
        break;
    case DIR_D*MIN_X:
    case DIR_D*MAX_X:
    case DIR_D*MAX_Y:
        //_asm int 3;
        break;
    case DIR_D*MIN_Y:
        g_snake_head.cube_face = SURFACE_6;
        g_snake_head.pos_2d_y  = BORDER_MAX - 1;
        break;
    case DIR_L*MAX_X:
    case DIR_L*MIN_Y:
    case DIR_L*MAX_Y:
        //_asm int 3;
        break;
    case DIR_L*MIN_X:
        g_snake_head.cube_face = SURFACE_4;
        g_snake_head.pos_2d_x  = BORDER_MAX - 1;
        break;
    case DIR_R*MIN_X:
    case DIR_R*MIN_Y:
    case DIR_R*MAX_Y:
        //_asm int 3;
        break;
    case DIR_R*MAX_X:
        g_snake_head.cube_face = SURFACE_2;
        g_snake_head.pos_2d_x  = BORDER_MIN + 1;
        break;
    default:
        //_asm int 3;
        break;
    }
}

//surface two
static void turn_in_surface_2(int border_type)
{
    switch(g_snake_head.s_dir*border_type)
    {
    case DIR_U*MIN_X:
    case DIR_U*MAX_X:
    case DIR_U*MIN_Y:
        //_asm int 3;
        break;
    case DIR_U*MAX_Y:
        g_snake_head.cube_face = SURFACE_5;
        g_snake_head.s_dir     = DIR_L;
        g_snake_head.pos_2d_y  = g_snake_head.pos_2d_x;
        g_snake_head.pos_2d_x  = BORDER_MAX - 1;
        break;
    case DIR_D*MIN_X:
    case DIR_D*MAX_X:
    case DIR_D*MAX_Y:
        //_asm int 3;
        break;
    case DIR_D*MIN_Y:
        g_snake_head.cube_face = SURFACE_6;
        g_snake_head.s_dir     = DIR_L;
        g_snake_head.pos_2d_y  = BORDER_MAX - g_snake_head.pos_2d_x;
        g_snake_head.pos_2d_x  = BORDER_MAX - 1;
        break;
    case DIR_L*MAX_X:
    case DIR_L*MIN_Y:
    case DIR_L*MAX_Y:
        //_asm int 3;
        break;
    case DIR_L*MIN_X:
        g_snake_head.cube_face = SURFACE_1;
        g_snake_head.pos_2d_x = BORDER_MAX - 1;
        break;
    case DIR_R*MIN_X:
    case DIR_R*MIN_Y:
    case DIR_R*MAX_Y:
        //_asm int 3;
        break;
    case DIR_R*MAX_X:
        g_snake_head.cube_face = SURFACE_3;
        g_snake_head.pos_2d_x  = BORDER_MIN + 1;
        break;
    default:
        //_asm int 3;
        break;
    }
}

//surface 3
static void turn_in_surface_3(int border_type)
{
    switch(g_snake_head.s_dir*border_type)
    {
    case DIR_U*MIN_X:
    case DIR_U*MAX_X:
    case DIR_U*MIN_Y:
        //_asm int 3;
        break;
    case DIR_U*MAX_Y:
        g_snake_head.cube_face = SURFACE_5;
        g_snake_head.s_dir     = DIR_D;
        g_snake_head.pos_2d_x  = BORDER_MAX - g_snake_head.pos_2d_x;
        g_snake_head.pos_2d_y  = BORDER_MAX - 1;
        break;
    case DIR_D*MIN_X:
    case DIR_D*MAX_X:
    case DIR_D*MAX_Y:
        //_asm int 3;
        break;
    case DIR_D*MIN_Y:
        g_snake_head.cube_face = SURFACE_6;
        g_snake_head.s_dir     = DIR_U;
        g_snake_head.pos_2d_x  = BORDER_MAX - g_snake_head.pos_2d_x;
        g_snake_head.pos_2d_y  = BORDER_MIN + 1;
        break;
    case DIR_L*MAX_X:
    case DIR_L*MIN_Y:
    case DIR_L*MAX_Y:
        //_asm int 3;
        break;
    case DIR_L*MIN_X:
        g_snake_head.cube_face = SURFACE_2;
        g_snake_head.pos_2d_x  = BORDER_MAX - 1;
        break;
    case DIR_R*MIN_Y:
    case DIR_R*MIN_X:
    case DIR_R*MAX_Y:
        //_asm int 3;
        break;
    case DIR_R*MAX_X:
        g_snake_head.cube_face = SURFACE_4;
        g_snake_head.pos_2d_x  = BORDER_MIN + 1;
        break;
    default:
        //_asm int 3;
        break;
    }
}

//surface 4
static void turn_in_surface_4(int border_type)
{
    switch(g_snake_head.s_dir*border_type)
    {
    case DIR_U*MIN_X:
    case DIR_U*MAX_X:
    case DIR_U*MIN_Y:
        //_asm int 3;
        break;
    case DIR_U*MAX_Y:
        g_snake_head.cube_face = SURFACE_5;
        g_snake_head.s_dir     = DIR_R;
        g_snake_head.pos_2d_y  = BORDER_MAX - g_snake_head.pos_2d_x;
        g_snake_head.pos_2d_x  = BORDER_MIN + 1;
        break;
    case DIR_D*MIN_X:
    case DIR_D*MAX_X:
    case DIR_D*MAX_Y:
        //_asm int 3;
        break;
    case DIR_D*MIN_Y:
        g_snake_head.cube_face = SURFACE_6;
        g_snake_head.s_dir     = DIR_R;
        g_snake_head.pos_2d_y  = g_snake_head.pos_2d_x;
        g_snake_head.pos_2d_x  = BORDER_MIN + 1;
        break;
    case DIR_L*MAX_X:
    case DIR_L*MIN_Y:
    case DIR_L*MAX_Y:
        //_asm int 3;
        break;
    case DIR_L*MIN_X:
        g_snake_head.cube_face = SURFACE_3;
        g_snake_head.pos_2d_x  = BORDER_MAX -1;
        break;
    case DIR_R*MIN_X:
    case DIR_R*MIN_Y:
    case DIR_R*MAX_Y:
        //_asm int 3;
        break;
    case DIR_R*MAX_X:
        g_snake_head.cube_face = SURFACE_1;
        g_snake_head.pos_2d_x  = BORDER_MIN + 1;
        break;
    default:
        //_asm int 3;
        break;
    }
}
//surface 5
static void turn_in_surface_5(int border_type)
{
    switch(g_snake_head.s_dir*border_type)
    {
    case DIR_U*MIN_X:
    case DIR_U*MAX_X:
    case DIR_U*MIN_Y:
        //_asm int 3;
        break;
    case DIR_U*MAX_Y:
        g_snake_head.cube_face = SURFACE_3;
        g_snake_head.s_dir     = DIR_D;
        g_snake_head.pos_2d_x  = BORDER_MAX - g_snake_head.pos_2d_x;
        g_snake_head.pos_2d_y  = BORDER_MAX - 1;
        break;
    case DIR_D*MIN_X:
    case DIR_D*MAX_X:
    case DIR_D*MAX_Y:
        //_asm int 3;
        break;
    case DIR_D*MIN_Y:
        g_snake_head.cube_face = SURFACE_1;
        g_snake_head.pos_2d_y  = BORDER_MAX - 1;
        break;
    case DIR_L*MAX_X:
    case DIR_L*MIN_Y:
    case DIR_L*MAX_Y:
        //_asm int 3;
        break;
    case DIR_L*MIN_X:
        g_snake_head.cube_face = SURFACE_4;
        g_snake_head.s_dir     = DIR_D;
        g_snake_head.pos_2d_x  = BORDER_MAX - g_snake_head.pos_2d_y;
        g_snake_head.pos_2d_y  = BORDER_MAX - 1;
        break;
    case DIR_R*MIN_X:
    case DIR_R*MIN_Y:
    case DIR_R*MAX_Y:
        //_asm int 3;
        break;
    case DIR_R*MAX_X:
        g_snake_head.cube_face = SURFACE_2;
        g_snake_head.s_dir     = DIR_D;
        g_snake_head.pos_2d_x  = g_snake_head.pos_2d_y;
        g_snake_head.pos_2d_y  = BORDER_MAX - 1;
        break;
    default:
        //_asm int 3;
        break;
    }
}

//surface 6
static void turn_in_surface_6(int border_type)
{
    switch(g_snake_head.s_dir*border_type)
    {
    case DIR_U*MIN_X:
    case DIR_U*MAX_X:
    case DIR_U*MIN_Y:
        //_asm int 3;
        break;
    case DIR_U*MAX_Y:
        g_snake_head.cube_face = SURFACE_1;
        g_snake_head.pos_2d_y  = BORDER_MIN + 1;
        break;
    case DIR_D*MIN_X:
    case DIR_D*MAX_X:
    case DIR_D*MAX_Y:
        //_asm int 3;
        break;
    case DIR_D*MIN_Y:
        g_snake_head.cube_face = SURFACE_3;
        g_snake_head.s_dir     = DIR_U;
        g_snake_head.pos_2d_x  = BORDER_MAX - g_snake_head.pos_2d_x;
        g_snake_head.pos_2d_y  = BORDER_MIN + 1;
        break;
    case DIR_L*MAX_X:
    case DIR_L*MIN_Y:
    case DIR_L*MAX_Y:
        //_asm int 3;
        break;
    case DIR_L*MIN_X:
        g_snake_head.cube_face = SURFACE_4;
        g_snake_head.s_dir     = DIR_U;
        g_snake_head.pos_2d_x  = g_snake_head.pos_2d_y;
        g_snake_head.pos_2d_y  = BORDER_MIN + 1;
        break;
    case DIR_R*MIN_X:
    case DIR_R*MIN_Y:
    case DIR_R*MAX_Y:
        //_asm int 3;
        break;
    case DIR_R*MAX_X:
        g_snake_head.cube_face = SURFACE_2;
        g_snake_head.s_dir     = DIR_U;
        g_snake_head.pos_2d_x  = BORDER_MAX - g_snake_head.pos_2d_y;
        g_snake_head.pos_2d_y  = BORDER_MIN + 1;
        break;
    default:
        //_asm int 3;
        break;
    }
}

////track snack head's location, for switch surface movement
static void check_snake_pos()
{
    int border_type;

    //edge property
    if(g_snake_head.pos_2d_x<BORDER_MIN)
        border_type = MIN_X;
    else if(g_snake_head.pos_2d_x>BORDER_MAX)
        border_type = MAX_X;
    else if(g_snake_head.pos_2d_y<BORDER_MIN)
        border_type = MIN_Y;
    else if(g_snake_head.pos_2d_y>BORDER_MAX)
        border_type = MAX_Y;
    else   //return if snack not on edge
        return;

    switch(g_snake_head.cube_face)
    {
    case SURFACE_1:
        turn_in_surface_1(border_type);
        break;
    case SURFACE_2:
        turn_in_surface_2(border_type);
        break;
    case SURFACE_3:
        turn_in_surface_3(border_type);
        break;
    case SURFACE_4:
        turn_in_surface_4(border_type);
        break;
    case SURFACE_5:
        turn_in_surface_5(border_type);
        break;
    case SURFACE_6:
        turn_in_surface_6(border_type);
        break;
    default:
        //_asm int 3;
        break;
    }
}

//transform snack's head location from 2D surface to 3d space
static void calc_snake_coord_pos()
{
    int pos_x = g_snake_head.pos_2d_x;
    int pos_y = g_snake_head.pos_2d_y;

    switch(g_snake_head.cube_face)
    {
    case SURFACE_1:    //snack on surface one
        g_snake_head.pos_3d[0] = - HALF_L - UNIT + HALF_U + UNIT*pos_x;
        g_snake_head.pos_3d[1] = - HALF_L - UNIT + HALF_U + UNIT*pos_y;
        g_snake_head.pos_3d[2] =   HALF_L + HALF_U;
        break;
    case SURFACE_2:    //surface 2
        g_snake_head.pos_3d[0] =   HALF_L + HALF_U;
        g_snake_head.pos_3d[1] = - HALF_L - UNIT + HALF_U + UNIT*pos_y;
        g_snake_head.pos_3d[2] =   HALF_L + UNIT - HALF_U - UNIT*pos_x;
        break;
    case SURFACE_3:    //surface 3
        g_snake_head.pos_3d[0] =   HALF_L + UNIT - HALF_U - UNIT*pos_x;
        g_snake_head.pos_3d[1] = - HALF_L - UNIT + HALF_U + UNIT*pos_y;
        g_snake_head.pos_3d[2] = - HALF_L - HALF_U;
        break;
    case SURFACE_4:    //surface 4
        g_snake_head.pos_3d[0] = - HALF_L - HALF_U;
        g_snake_head.pos_3d[1] = - HALF_L - UNIT + HALF_U + UNIT*pos_y;
        g_snake_head.pos_3d[2] = - HALF_L - UNIT + HALF_U + UNIT*pos_x;
        break;
    case SURFACE_5:    //surface 5
        g_snake_head.pos_3d[0] = - HALF_L - UNIT + HALF_U + UNIT*pos_x;
        g_snake_head.pos_3d[1] =   HALF_L + HALF_U;
        g_snake_head.pos_3d[2] =   HALF_L + UNIT - HALF_U - UNIT*pos_y;
        break;
    case SURFACE_6:    //surface 6
        g_snake_head.pos_3d[0] = - HALF_L - UNIT + HALF_U + UNIT*pos_x;
        g_snake_head.pos_3d[1] = - HALF_L - HALF_U;
        g_snake_head.pos_3d[2] = - HALF_L - UNIT + HALF_U + UNIT*pos_y;
        break;
    default:
        //_asm int 3;
        break;
    }
}

//track did snack eat itself
//if return 1 game over else return 0
static int is_collide_myself()
{
    PSBODY_NODE pNode = g_snake_head.body_section;
    int face_idx,x,y;

    while(pNode!=NULL)
    {
        face_idx = g_snake_head.cube_face;
        x = g_snake_head.pos_2d_x;
        y = g_snake_head.pos_2d_y;

        switch(g_snake_head.s_dir)
        {
        case DIR_U:
            y++;
            break;
        case DIR_R:
            x++;
            break;
        case DIR_D:
            y--;
            break;
        case DIR_L:
            x--;
            break;
        default:
            break;
        }

        if((face_idx==pNode->cube_face &&
            x       ==pNode->pos_2d_x  &&
            y       ==pNode->pos_2d_y) ||
            (g_snake_head.cube_face==pNode->cube_face&&
            g_snake_head.pos_2d_x  == pNode->pos_2d_x&&
            g_snake_head.pos_2d_y  == pNode->pos_2d_y))
            return 1;

        pNode = pNode->next;
    }

    return 0;
}

//update rest of snack
static void snake_update_body()
{
    PSBODY_NODE pNode = g_snake_head.last_section;
	//update the last snack node after snack eat coin
	//cube_face_last¡¢pos_2d_x_last¡¢pos_2d_y_last¡¢pos_last
    g_snake_head.cube_face_last = g_snake_head.last_section->cube_face;
    g_snake_head.pos_2d_x_last  = g_snake_head.last_section->pos_2d_x;
    g_snake_head.pos_2d_y_last  = g_snake_head.last_section->pos_2d_y;
    g_snake_head.pos_last[0] = g_snake_head.last_section->pos_3d[0];
    g_snake_head.pos_last[1] = g_snake_head.last_section->pos_3d[1];
    g_snake_head.pos_last[2] = g_snake_head.last_section->pos_3d[2];

	//all the node use previous node information
    while(pNode->previous!=NULL)
    {
        pNode->cube_face = pNode->previous->cube_face;
        pNode->pos_2d_x  = pNode->previous->pos_2d_x;
        pNode->pos_2d_y  = pNode->previous->pos_2d_y;
        pNode->pos_3d[0] = pNode->previous->pos_3d[0];
        pNode->pos_3d[1] = pNode->previous->pos_3d[1];
        pNode->pos_3d[2] = pNode->previous->pos_3d[2];

        pNode = pNode->previous;
    }
	//first node on snack use snack head location
    pNode->cube_face = g_snake_head.cube_face;
    pNode->pos_2d_x  = g_snake_head.pos_2d_x;
    pNode->pos_2d_y  = g_snake_head.pos_2d_y;
    pNode->pos_3d[0] = g_snake_head.pos_3d[0];
    pNode->pos_3d[1] = g_snake_head.pos_3d[1];
    pNode->pos_3d[2] = g_snake_head.pos_3d[2];


    snakeP[0] = g_snake_head.pos_3d[0];
      snakeP[1] = g_snake_head.pos_3d[1];
     snakeP[2] = g_snake_head.pos_3d[2];
}

//update snack
int snake_update_pos()
{
    //first update snack body location 
    snake_update_body();

    //base on snack head's movement update snack's 2d location 
    switch(g_snake_head.s_dir)
    {
    case DIR_U:
        g_snake_head.pos_2d_y++;
        break;
    case DIR_D:
        g_snake_head.pos_2d_y--;
        break;
    case DIR_L:
        g_snake_head.pos_2d_x--;
        break;
    case DIR_R:
        g_snake_head.pos_2d_x++;
        break;
    default:
        //_asm int 3;
        break;
    }

    //check snack head location for jump surface movement
    check_snake_pos();

    //transform snack's head location from 2D surface to 3d space
    calc_snake_coord_pos();

    //check did snack eat it self
    return is_collide_myself();



}

//third person view
//set camera information let it follow head
void camera_follow_snake()
{
    float eye_x,eye_y,eye_z;                //camera position
    float center_x,center_y,center_z;       //camera view location
    float up_x,up_y,up_z;                   //camera view vector
    int dir     = g_snake_head.s_dir;
    int surface = g_snake_head.cube_face;


	//base on snack's head and moving direction there are 6 face 4 direction and 24 combo
    switch(surface*dir)
    {
    case SURFACE_1*DIR_U:
        eye_x    = g_snake_head.pos_3d[0];
        eye_y    = g_snake_head.pos_3d[1] - POST_CAM_LEN;
        eye_z    = g_snake_head.pos_3d[2] + DELTA_CAM_POS;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1] + PRE_CAM_LEN;
        center_z = g_snake_head.pos_3d[2];
        up_x     = 0.0f;
        up_y     = 0.0f;
        up_z     = 1.0f;
        break;
    case SURFACE_1*DIR_R:
        eye_x    = g_snake_head.pos_3d[0] - POST_CAM_LEN;
        eye_y    = g_snake_head.pos_3d[1];
        eye_z    = g_snake_head.pos_3d[2] + DELTA_CAM_POS;
        center_x = g_snake_head.pos_3d[0] + PRE_CAM_LEN;
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2];
        up_x     = 0.0f;
        up_y     = 0.0f;
        up_z     = 1.0f;
        break;
    case SURFACE_1*DIR_D:
        eye_x    = g_snake_head.pos_3d[0];
        eye_y    = g_snake_head.pos_3d[1] + POST_CAM_LEN;
        eye_z    = g_snake_head.pos_3d[2] + DELTA_CAM_POS;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1] - PRE_CAM_LEN;
        center_z = g_snake_head.pos_3d[2];
        up_x     = 0.0f;
        up_y     = 0.0f;
        up_z     = 1.0f;
        break;
    case SURFACE_1*DIR_L:
        eye_x    = g_snake_head.pos_3d[0] + POST_CAM_LEN;
        eye_y    = g_snake_head.pos_3d[1];
        eye_z    = g_snake_head.pos_3d[2] + DELTA_CAM_POS;
        center_x = g_snake_head.pos_3d[0] - PRE_CAM_LEN;
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2];
        up_x     = 0.0f;
        up_y     = 0.0f;
        up_z     = 1.0f;
        break;
    case SURFACE_2*DIR_U:
        eye_x    = g_snake_head.pos_3d[0] + DELTA_CAM_POS;
        eye_y    = g_snake_head.pos_3d[1] - POST_CAM_LEN;
        eye_z    = g_snake_head.pos_3d[2];
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1] + PRE_CAM_LEN;
        center_z = g_snake_head.pos_3d[2];
        up_x     = 1.0f;
        up_y     = 0.0f;
        up_z     = 0.0f;
        break;
    case SURFACE_2*DIR_R:
        eye_x    = g_snake_head.pos_3d[0] + DELTA_CAM_POS;
        eye_y    = g_snake_head.pos_3d[1];
        eye_z    = g_snake_head.pos_3d[2] + POST_CAM_LEN;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2] - PRE_CAM_LEN;
        up_x     = 1.0f;
        up_y     = 0.0f;
        up_z     = 0.0f;
        break;
    case SURFACE_2*DIR_D:
        eye_x    = g_snake_head.pos_3d[0] + DELTA_CAM_POS;
        eye_y    = g_snake_head.pos_3d[1] + POST_CAM_LEN;
        eye_z    = g_snake_head.pos_3d[2];
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1] - PRE_CAM_LEN;
        center_z = g_snake_head.pos_3d[2];
        up_x     = 1.0f;
        up_y     = 0.0f;
        up_z     = 0.0f;
        break;
    case SURFACE_2*DIR_L:
        eye_x    = g_snake_head.pos_3d[0] + DELTA_CAM_POS;
        eye_y    = g_snake_head.pos_3d[1];
        eye_z    = g_snake_head.pos_3d[2] - POST_CAM_LEN;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2] + PRE_CAM_LEN;
        up_x     = 1.0f;
        up_y     = 0.0f;
        up_z     = 0.0f;
        break;
    case SURFACE_3*DIR_U:
        eye_x    = g_snake_head.pos_3d[0];
        eye_y    = g_snake_head.pos_3d[1] - POST_CAM_LEN;
        eye_z    = g_snake_head.pos_3d[2] - DELTA_CAM_POS;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1] + PRE_CAM_LEN;
        center_z = g_snake_head.pos_3d[2];
        up_x     =  0.0f;
        up_y     =  0.0f;
        up_z     = -1.0f;
        break;
    case SURFACE_3*DIR_R:
        eye_x    = g_snake_head.pos_3d[0];
        eye_y    = g_snake_head.pos_3d[1] + POST_CAM_LEN;
        eye_z    = g_snake_head.pos_3d[2] - DELTA_CAM_POS;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1] - PRE_CAM_LEN;
        center_z = g_snake_head.pos_3d[2];
        up_x     =  0.0f;
        up_y     =  0.0f;
        up_z     = -1.0f;
        break;
    case SURFACE_3*DIR_D:
        eye_x    = g_snake_head.pos_3d[0];
        eye_y    = g_snake_head.pos_3d[1] + POST_CAM_LEN;
        eye_z    = g_snake_head.pos_3d[2] - DELTA_CAM_POS;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1] - PRE_CAM_LEN;
        center_z = g_snake_head.pos_3d[2];
        up_x     =  0.0f;
        up_y     =  0.0f;
        up_z     = -1.0f;
        break;
    case SURFACE_3*DIR_L:
        eye_x    = g_snake_head.pos_3d[0] - POST_CAM_LEN;
        eye_y    = g_snake_head.pos_3d[1];
        eye_z    = g_snake_head.pos_3d[2] - DELTA_CAM_POS;
        center_x = g_snake_head.pos_3d[0] + PRE_CAM_LEN;
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2];
        up_x     =  0.0f;
        up_y     =  0.0f;
        up_z     = -1.0f;
        break;
    case SURFACE_4*DIR_U:
        eye_x    = g_snake_head.pos_3d[0] - DELTA_CAM_POS;
        eye_y    = g_snake_head.pos_3d[1] - POST_CAM_LEN;
        eye_z    = g_snake_head.pos_3d[2];
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1] + PRE_CAM_LEN;
        center_z = g_snake_head.pos_3d[2];
        up_x     = -1.0f;
        up_y     =  0.0f;
        up_z     =  0.0f;
        break;
    case SURFACE_4*DIR_R:
        eye_x    = g_snake_head.pos_3d[0] - DELTA_CAM_POS;
        eye_y    = g_snake_head.pos_3d[1];
        eye_z    = g_snake_head.pos_3d[2] - POST_CAM_LEN;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2] + PRE_CAM_LEN;
        up_x     = -1.0f;
        up_y     =  0.0f;
        up_z     =  0.0f;
        break;
    case SURFACE_4*DIR_D:
        eye_x    = g_snake_head.pos_3d[0] - DELTA_CAM_POS;
        eye_y    = g_snake_head.pos_3d[1] + POST_CAM_LEN;
        eye_z    = g_snake_head.pos_3d[2];
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1] - PRE_CAM_LEN;
        center_z = g_snake_head.pos_3d[2];
        up_x     = -1.0f;
        up_y     =  0.0f;
        up_z     =  0.0f;
        break;
    case SURFACE_4*DIR_L:
        eye_x    = g_snake_head.pos_3d[0] - DELTA_CAM_POS;
        eye_y    = g_snake_head.pos_3d[1];
        eye_z    = g_snake_head.pos_3d[2] + POST_CAM_LEN;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2] - PRE_CAM_LEN;
        up_x     = -1.0f;
        up_y     =  0.0f;
        up_z     =  0.0f;
        break;
    case SURFACE_5*DIR_U:
        eye_x    = g_snake_head.pos_3d[0];
        eye_y    = g_snake_head.pos_3d[1] + DELTA_CAM_POS;
        eye_z    = g_snake_head.pos_3d[2] + POST_CAM_LEN;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2] - PRE_CAM_LEN;
        up_x     = 0.0f;
        up_y     = 1.0f;
        up_z     = 0.0f;
        break;
    case SURFACE_5*DIR_R:
        eye_x    = g_snake_head.pos_3d[0] - POST_CAM_LEN;
        eye_y    = g_snake_head.pos_3d[1] + DELTA_CAM_POS;
        eye_z    = g_snake_head.pos_3d[2];
        center_x = g_snake_head.pos_3d[0] + PRE_CAM_LEN;
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2];
        up_x     = 0.0f;
        up_y     = 1.0f;
        up_z     = 0.0f;
        break;
    case SURFACE_5*DIR_D:
        eye_x    = g_snake_head.pos_3d[0];
        eye_y    = g_snake_head.pos_3d[1] + DELTA_CAM_POS;
        eye_z    = g_snake_head.pos_3d[2] - POST_CAM_LEN;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2] + PRE_CAM_LEN;
        up_x     = 0.0f;
        up_y     = 1.0f;
        up_z     = 0.0f;
        break;
    case SURFACE_5*DIR_L:
        eye_x    = g_snake_head.pos_3d[0] + POST_CAM_LEN;
        eye_y    = g_snake_head.pos_3d[1] + DELTA_CAM_POS;
        eye_z    = g_snake_head.pos_3d[2];
        center_x = g_snake_head.pos_3d[0] - PRE_CAM_LEN;
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2];
        up_x     = 0.0f;
        up_y     = 1.0f;
        up_z     = 0.0f;
        break;
    case SURFACE_6*DIR_U:
        eye_x    = g_snake_head.pos_3d[0];
        eye_y    = g_snake_head.pos_3d[1] - DELTA_CAM_POS;
        eye_z    = g_snake_head.pos_3d[2] - POST_CAM_LEN;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2] + PRE_CAM_LEN;
        up_x     =  0.0f;
        up_y     = -1.0f;
        up_z     =  0.0f;
        break;
    case SURFACE_6*DIR_R:
        eye_x    = g_snake_head.pos_3d[0] - POST_CAM_LEN;
        eye_y    = g_snake_head.pos_3d[1] - DELTA_CAM_POS;
        eye_z    = g_snake_head.pos_3d[2];
        center_x = g_snake_head.pos_3d[0] + PRE_CAM_LEN;
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2];
        up_x     =  0.0f;
        up_y     = -1.0f;
        up_z     =  0.0f;
        break;
    case SURFACE_6*DIR_D:
        eye_x    = g_snake_head.pos_3d[0];
        eye_y    = g_snake_head.pos_3d[1] - DELTA_CAM_POS;
        eye_z    = g_snake_head.pos_3d[2] + POST_CAM_LEN;
        center_x = g_snake_head.pos_3d[0];
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2] - PRE_CAM_LEN;
        up_x     =  0.0f;
        up_y     = -1.0f;
        up_z     =  0.0f;
        break;
    case SURFACE_6*DIR_L:
        eye_x    = g_snake_head.pos_3d[0] + POST_CAM_LEN;
        eye_y    = g_snake_head.pos_3d[1] - DELTA_CAM_POS;
        eye_z    = g_snake_head.pos_3d[2];
        center_x = g_snake_head.pos_3d[0] - PRE_CAM_LEN;
        center_y = g_snake_head.pos_3d[1];
        center_z = g_snake_head.pos_3d[2];
        up_x     =  0.0f;
        up_y     = -1.0f;
        up_z     =  0.0f;
        break;
    default:
        //_asm int 3;
        break;
    }

    //use opengl function to set camera
    gluLookAt(eye_x, eye_y, eye_z,
        center_x, center_y, center_z,
        up_x, up_y, up_z);
}
