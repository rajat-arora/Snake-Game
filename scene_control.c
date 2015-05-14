//grammarly says the following are possible sources:
//sky dome code partially sourced from here: http://bbs.csdn.net/topics/370117480
//sky dome also referenced from here: http://www.gamedev.net/topic/569427-debug-help-needed-drawprimitive-failing/
//sky dome also referenced from here: http://www.cnblogs.com/donati1981/archive/2009/08/31/1557229.html

#include "haharsw.h"

//set all the big cube's gray colour to s_square_face_color
static const float s_square_face_color[6][3] =
{
    {  50.0f/255.0f,  50.0f/255.0f,  50.0f/255.0f},
    {  80.0f/255.0f,  80.0f/255.0f,  80.0f/255.0f},
    { 110.0f/255.0f, 110.0f/255.0f, 110.0f/255.0f},
    { 140.0f/255.0f, 140.0f/255.0f, 140.0f/255.0f},
    { 170.0f/255.0f, 170.0f/255.0f, 170.0f/255.0f},
    { 200.0f/255.0f, 200.0f/255.0f, 200.0f/255.0f}
};
//set big cube's six surface and ever surface four vertex and every vertex location
static const float s_squre_vertex[6][4][3] = 
{
    { { -HALF_L, -HALF_L,  HALF_L },{  HALF_L, -HALF_L,  HALF_L },
      {  HALF_L,  HALF_L,  HALF_L },{ -HALF_L,  HALF_L,  HALF_L } },
    { {  HALF_L, -HALF_L,  HALF_L },{  HALF_L, -HALF_L, -HALF_L },
      {  HALF_L,  HALF_L, -HALF_L },{  HALF_L,  HALF_L,  HALF_L } },
    { {  HALF_L, -HALF_L, -HALF_L },{ -HALF_L, -HALF_L, -HALF_L },
      { -HALF_L,  HALF_L, -HALF_L },{  HALF_L,  HALF_L, -HALF_L } },
    { { -HALF_L, -HALF_L, -HALF_L },{ -HALF_L, -HALF_L,  HALF_L },
      { -HALF_L,  HALF_L,  HALF_L },{ -HALF_L,  HALF_L, -HALF_L } },
    { { -HALF_L,  HALF_L,  HALF_L },{  HALF_L,  HALF_L,  HALF_L },
      {  HALF_L,  HALF_L, -HALF_L },{ -HALF_L,  HALF_L, -HALF_L } },
    { { -HALF_L, -HALF_L,  HALF_L },{ -HALF_L, -HALF_L, -HALF_L },
      {  HALF_L, -HALF_L, -HALF_L },{  HALF_L, -HALF_L,  HALF_L } }
};

//for small cube and barrier and snack
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

//cube six surface vertex normal
static const float s_face_normal[6][3] = 
{
    {  0.0f,  0.0f,  1.0f },
    {  1.0f,  0.0f,  0.0f },
    {  0.0f,  0.0f, -1.0f },
    { -1.0f,  0.0f,  0.0f },
    {  0.0f,  1.0f,  0.0f },
    {  0.0f, -1.0f,  0.0f },
};

//line colour
static const float s_line_color[3]  = { 0.1f,  0.9f,  0.0f};

//line wide
static const float s_line_width     = 1.0f;

//check for coin and barrier on big cube on 2d
static char        s_face_grid_type[6][INTEGER_LEN][INTEGER_LEN];

//check for coin and barrier on big cube on 3d
static float       s_face_barrier[6][BARRIER_COUNT][3];

//remember all the coin rest on big cube
//after been ate by snack coin will be delete from list of s_fixed_coin and add to snack
static PSBODY_NODE s_fixed_coin = NULL;

//all the coin remained on big cube
static int         s_coin_count = 0;

//remember all the vertex on sky and its location and texture vertex location
static VERTEX *s_vertex_skydome = NULL;

//number of vertex on sky 
static int    s_v_skydome_cnt   = 0;

//all the sky will rotate base on y axis to make animation
static float  s_y_rot           = 0.0f;

//output time info
static time_t s_old_time        = { 0 };

//extern other variable
extern SNAKE_HEAD  g_snake_head;
extern GLuint      g_texture[TEXTURE_COUNT];
extern int         g_width;
extern int         g_height;
extern int         s_b_pause;

//make on coin and put in s_fixed_coin
static void make_fixed_coin(int face_idx,int xPos,int yPos,
                float x,float y,float z)
{
    PSBODY_NODE pTmp = (PSBODY_NODE)malloc(sizeof(SBODY_NODE));
    memset(pTmp,0,sizeof(SBODY_NODE));

    pTmp->cube_face = face_idx;
    pTmp->pos_2d_x = xPos;
    pTmp->pos_2d_y = yPos;
    pTmp->pos_3d[0] = x;
    pTmp->pos_3d[1] = y;
    pTmp->pos_3d[2] = z;
    pTmp->color[0]  = 1.0f;
    pTmp->color[1]  = 1.0f;
    pTmp->color[2]  = 0.0f;

    pTmp->next = s_fixed_coin;

    if(s_fixed_coin!=NULL)
    {
        s_fixed_coin->previous = pTmp;
    }

    s_fixed_coin = pTmp;
    s_coin_count++;
}


//make all the random coin and barrier from 2d to 3d coordinate
static void make_3d_coord()
{
    int face_idx,x,y,barrier_idx;

    for(face_idx=0;face_idx<6;face_idx++)
    {
        barrier_idx = 0;
        for(y=0;y<INTEGER_LEN;y++)
            for(x=0;x<INTEGER_LEN;x++)
            {
                switch(face_idx)
                {
                case 0:   //in big cube surface one
                    if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_BLOCK)  //this is barrier location
                    {
                        s_face_barrier[face_idx][barrier_idx][0] = - HALF_L - UNIT + HALF_U + UNIT*x;
                        s_face_barrier[face_idx][barrier_idx][1] = - HALF_L - UNIT + HALF_U + UNIT*y;
                        s_face_barrier[face_idx][barrier_idx][2] =   HALF_L + HALF_U;
                        barrier_idx++;
                    }
                    else if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_COIN)  //this is coin location
                    {
                        make_fixed_coin(SURFACE_1,x,y,
                            - HALF_L - UNIT + HALF_U + UNIT*x,
                            - HALF_L - UNIT + HALF_U + UNIT*y,
                              HALF_L + HALF_U);
                    }
                    break;
                case 1:    //in big cube surface 2
                    if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_BLOCK)
                    {
                        s_face_barrier[face_idx][barrier_idx][0] =   HALF_L + HALF_U;
                        s_face_barrier[face_idx][barrier_idx][1] = - HALF_L - UNIT + HALF_U + UNIT*y;
                        s_face_barrier[face_idx][barrier_idx][2] =   HALF_L + UNIT - HALF_U - UNIT*x;
                        barrier_idx++;
                    }
                    else if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_COIN) 
                    {
                        make_fixed_coin(SURFACE_2,x,y,
                              HALF_L + HALF_U,
                            - HALF_L - UNIT + HALF_U + UNIT*y,
                              HALF_L + UNIT - HALF_U - UNIT*x);
                    }
                    break;
                case 2:   //in big cube surface 3
                    if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_BLOCK)
                    {
                        s_face_barrier[face_idx][barrier_idx][0] =   HALF_L + UNIT - HALF_U - UNIT*x;
                        s_face_barrier[face_idx][barrier_idx][1] = - HALF_L - UNIT + HALF_U + UNIT*y;
                        s_face_barrier[face_idx][barrier_idx][2] = - HALF_L - HALF_U;
                        barrier_idx++;
                    }
                    else if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_COIN)
                    {
                        make_fixed_coin(SURFACE_3,x,y,
                              HALF_L + UNIT - HALF_U - UNIT*x,
                            - HALF_L - UNIT + HALF_U + UNIT*y,
                            - HALF_L - HALF_U);
                    }
                    break;
                case 3:   //in big cube surface 4
                    if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_BLOCK)
                    {
                        s_face_barrier[face_idx][barrier_idx][0] = - HALF_L - HALF_U;
                        s_face_barrier[face_idx][barrier_idx][1] = - HALF_L - UNIT + HALF_U + UNIT*y;
                        s_face_barrier[face_idx][barrier_idx][2] = - HALF_L - UNIT + HALF_U + UNIT*x;
                        barrier_idx++;
                    }
                    else if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_COIN)
                    {
                        make_fixed_coin(SURFACE_4,x,y,
                            - HALF_L - HALF_U,
                            - HALF_L - UNIT + HALF_U + UNIT*y,
                            - HALF_L - UNIT + HALF_U + UNIT*x);
                    }
                    break;
                case 4:    //in big cube surface 5
                    if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_BLOCK) 
                    {
                        s_face_barrier[face_idx][barrier_idx][0] = - HALF_L - UNIT + HALF_U + UNIT*x;
                        s_face_barrier[face_idx][barrier_idx][1] =   HALF_L + HALF_U;
                        s_face_barrier[face_idx][barrier_idx][2] =   HALF_L + UNIT - HALF_U - UNIT*y;
                        barrier_idx++;
                    }
                    else if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_COIN) 
                    {
                        make_fixed_coin(SURFACE_5,x,y,
                            - HALF_L - UNIT + HALF_U + UNIT*x,
                              HALF_L + HALF_U,
                              HALF_L + UNIT - HALF_U - UNIT*y);
                    }
                    break;
                case 5:   //in big cube surface 6
                    if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_BLOCK)  
                    {
                        s_face_barrier[face_idx][barrier_idx][0] = - HALF_L - UNIT + HALF_U + UNIT*x;
                        s_face_barrier[face_idx][barrier_idx][1] = - HALF_L - HALF_U;
                        s_face_barrier[face_idx][barrier_idx][2] = - HALF_L - UNIT + HALF_U + UNIT*y;
                        barrier_idx++;
                    }
                    else if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_COIN)  
                    {
                        make_fixed_coin(SURFACE_6,x,y,
                            - HALF_L - UNIT + HALF_U + UNIT*x,
                            - HALF_L - HALF_U,
                            - HALF_L - UNIT + HALF_U + UNIT*y);
                    }
                    break;
                default:
                    //_asm int 3;
                    break;
                }
            }
    }
}

//sky vertex location and texture location
static void GenerateDome()
{
    const float radius  = SKY_POS;
    const float dtheta  = 5.0f;
    const float dphi    = 5.0f;
    const float hTile   = 1.0f;
    const float vTile   = 1.0f;

    int theta, phi, n, i;

    // Used to calculate the UV coordinates
    float vx, vy, vz, mag;

    // Make sure our vertex array is clear
    if (s_vertex_skydome!=NULL) 
    {
        free(s_vertex_skydome);
        s_vertex_skydome = NULL;
        s_v_skydome_cnt  = 0;
    }

    // Initialize our Vertex array
    s_v_skydome_cnt  = (int)((360/dtheta)*(180/dphi)*4);
    s_vertex_skydome = (VERTEX *)malloc(sizeof(VERTEX)*s_v_skydome_cnt);
    memset(s_vertex_skydome,0,sizeof(VERTEX)*s_v_skydome_cnt);

    // Generate the dome
    n = 0;
    for (phi=0; phi <= 180 - dphi; phi += (int)dphi)
    {
        for (theta=0; theta <= 360 - dtheta; theta += (int)dtheta)
        {
            // Calculate the vertex at phi, theta
            s_vertex_skydome[n].x = radius * sinf(phi*DTOR) * cosf(DTOR*theta);
            s_vertex_skydome[n].y = radius * sinf(phi*DTOR) * sinf(DTOR*theta);
            s_vertex_skydome[n].z = radius * cosf(phi*DTOR);

            // Create a vector from the origin to this vertex
            vx = s_vertex_skydome[n].x;
            vy = s_vertex_skydome[n].y;
            vz = s_vertex_skydome[n].z;

            // Normalize the vector
            mag = sqrt(SQR(vx)+SQR(vy)+SQR(vz));
            vx /= mag;
            vy /= mag;
            vz /= mag;

            // Calculate the spherical texture coordinates
            s_vertex_skydome[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
            s_vertex_skydome[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
            n++;

            // Calculate the vertex at phi+dphi, theta
            s_vertex_skydome[n].x = radius * sinf((phi+dphi)*DTOR) * cosf(theta*DTOR);
            s_vertex_skydome[n].y = radius * sinf((phi+dphi)*DTOR) * sinf(theta*DTOR);
            s_vertex_skydome[n].z = radius * cosf((phi+dphi)*DTOR);

            // Calculate the texture coordinates
            vx = s_vertex_skydome[n].x;
            vy = s_vertex_skydome[n].y;
            vz = s_vertex_skydome[n].z;

            mag = sqrt(SQR(vx)+SQR(vy)+SQR(vz));
            vx /= mag;
            vy /= mag;
            vz /= mag;

            s_vertex_skydome[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
            s_vertex_skydome[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
            n++;

            // Calculate the vertex at phi, theta+dtheta
            s_vertex_skydome[n].x = radius * sinf(DTOR*phi) * cosf(DTOR*(theta+dtheta));
            s_vertex_skydome[n].y = radius * sinf(DTOR*phi) * sinf(DTOR*(theta+dtheta));
            s_vertex_skydome[n].z = radius * cosf(DTOR*phi);

            // Calculate the texture coordinates
            vx = s_vertex_skydome[n].x;
            vy = s_vertex_skydome[n].y;
            vz = s_vertex_skydome[n].z;

            mag = sqrt(SQR(vx)+SQR(vy)+SQR(vz));
            vx /= mag;
            vy /= mag;
            vz /= mag;

            s_vertex_skydome[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
            s_vertex_skydome[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
            n++;

            if (0<=phi && phi<=180)
            {
                // Calculate the vertex at phi+dphi, theta+dtheta
                s_vertex_skydome[n].x = radius * sinf((phi+dphi)*DTOR) * cosf(DTOR*(theta+dtheta));
                s_vertex_skydome[n].y = radius * sinf((phi+dphi)*DTOR) * sinf(DTOR*(theta+dtheta));
                s_vertex_skydome[n].z = radius * cosf((phi+dphi)*DTOR);

                // Calculate the texture coordinates
                vx = s_vertex_skydome[n].x;
                vy = s_vertex_skydome[n].y;
                vz = s_vertex_skydome[n].z;

                mag = sqrt(SQR(vx)+SQR(vy)+SQR(vz));
                vx /= mag;
                vy /= mag;
                vz /= mag;

                s_vertex_skydome[n].u = hTile * (float)(atan2(vx, vz)/(PI*2)) + 0.5f;
                s_vertex_skydome[n].v = vTile * (float)(asinf(vy) / PI) + 0.5f;		
                n++;
            }
        }
    }

    // Fix the problem at the seam
    for (i=0; i < s_v_skydome_cnt - 3; i++)
    {
        if (s_vertex_skydome[i].u - s_vertex_skydome[i+1].u > 0.9f)
            s_vertex_skydome[i+1].u += 1.0f;

        if (s_vertex_skydome[i+1].u - s_vertex_skydome[i].u > 0.9f)
            s_vertex_skydome[i].u += 1.0f;

        if (s_vertex_skydome[i].u - s_vertex_skydome[i+2].u > 0.9f)
            s_vertex_skydome[i+2].u += 1.0f;

        if (s_vertex_skydome[i+2].u - s_vertex_skydome[i].u > 0.9f)
            s_vertex_skydome[i].u += 1.0f;

        if (s_vertex_skydome[i+1].u - s_vertex_skydome[i+2].u > 0.9f)
            s_vertex_skydome[i+2].u += 1.0f;

        if (s_vertex_skydome[i+2].u - s_vertex_skydome[i+1].u > 0.9f)
            s_vertex_skydome[i+1].u += 1.0f;

        if (s_vertex_skydome[i].v - s_vertex_skydome[i+1].v > 0.8f)
            s_vertex_skydome[i+1].v += 1.0f;

        if (s_vertex_skydome[i+1].v - s_vertex_skydome[i].v > 0.8f)
            s_vertex_skydome[i].v += 1.0f;

        if (s_vertex_skydome[i].v - s_vertex_skydome[i+2].v > 0.8f)
            s_vertex_skydome[i+2].v += 1.0f;

        if (s_vertex_skydome[i+2].v - s_vertex_skydome[i].v > 0.8f)
            s_vertex_skydome[i].v += 1.0f;

        if (s_vertex_skydome[i+1].v - s_vertex_skydome[i+2].v > 0.8f)
            s_vertex_skydome[i+2].v += 1.0f;

        if (s_vertex_skydome[i+2].v - s_vertex_skydome[i+1].v > 0.8f)
            s_vertex_skydome[i+1].v += 1.0f;
    }
}

//initial all scene
void scene_initialize()
{
    int face_idx,idx,x,y,tmp;

    s_old_time = time(NULL);

    srand(time(NULL));

    memset(s_face_grid_type,0,sizeof(char)*INTEGER_LEN*INTEGER_LEN*6);

    //make random coin on six surface
    for(face_idx=0;face_idx<6;face_idx++)
    {
        idx = 0;
        while(idx < COIN_COUNT)
        {
            //random 2d coordinate
            x = rand()%(INTEGER_LEN-4) + 2;
            y = rand()%(INTEGER_LEN-4) + 2;

            tmp = s_face_grid_type[face_idx][y-1][x-1] + 
                s_face_grid_type[face_idx][y-1][x] + 
                s_face_grid_type[face_idx][y-1][x+1] + 
                s_face_grid_type[face_idx][y][x-1] + 
                s_face_grid_type[face_idx][y][x] + 
                s_face_grid_type[face_idx][y][x+1] + 
                s_face_grid_type[face_idx][y+1][x-1] + 
                s_face_grid_type[face_idx][y+1][x] + 
                s_face_grid_type[face_idx][y+1][x+1];
            if(tmp>0)
                continue;

            s_face_grid_type[face_idx][y][x] = GRID_TYPE_COIN;

            idx++;
        }
    }

    //make random barrier on six location
    for(face_idx=0;face_idx<6;face_idx++)
    {
        idx = 0;
        while(idx < BARRIER_COUNT)
        {
            x = rand()%(INTEGER_LEN-4) + 2;
            y = rand()%(INTEGER_LEN-4) + 2;

            tmp = s_face_grid_type[face_idx][y-1][x-1] + 
                s_face_grid_type[face_idx][y-1][x] + 
                s_face_grid_type[face_idx][y-1][x+1] + 
                s_face_grid_type[face_idx][y][x-1] + 
                s_face_grid_type[face_idx][y][x] + 
                s_face_grid_type[face_idx][y][x+1] + 
                s_face_grid_type[face_idx][y+1][x-1] + 
                s_face_grid_type[face_idx][y+1][x] + 
                s_face_grid_type[face_idx][y+1][x+1];
            if(tmp>0)
                continue;

            s_face_grid_type[face_idx][y][x] = GRID_TYPE_BLOCK;

            idx++;
        }
    }

    //make all the coin and barrier from 2d to 3d
    make_3d_coord();

    //make sky vertex location
    GenerateDome();
}

//clean scene
void scene_cleanup()
{
    //clean all the coin rest 
    PSBODY_NODE pTmp1 = s_fixed_coin,pTmp2;
    while(pTmp1!=NULL)
    {
        pTmp2 = pTmp1;
        pTmp1 = pTmp1->next;
        free(pTmp2);
    }

    s_fixed_coin = NULL;

    if (s_vertex_skydome!=NULL)
    {
        free(s_vertex_skydome);
        s_vertex_skydome = NULL;
    }
}

//render sky
static void skydome_render()
{
    int i;

    //ָ������ζ����˳ʱ��˳��Ϊ����ε����棬�������Ǿ��������ε��ڲ��������Ч��
	//decide which vertex should be circle front so we can see sky
    glFrontFace(GL_CW);

    glColor3f(0.5f, 0.5f, 0.5f);    //set sky colour

    //binding sky texture coordinate
    glBindTexture(GL_TEXTURE_2D, g_texture[SKYDOME_TEXTURE]);

    glPushMatrix();
    glRotatef(s_y_rot, 0.0f, 1.0f, 0.0f);    //rotate sky with y axis make it animation
    glBegin(GL_TRIANGLE_STRIP);    //make triangle to combo the sky
    for (i=0; i < s_v_skydome_cnt; i++)
    {
        glTexCoord2f(s_vertex_skydome[i].u, s_vertex_skydome[i].v);
        glVertex3f(s_vertex_skydome[i].x, s_vertex_skydome[i].y, s_vertex_skydome[i].z);
    }
    glEnd();
    glPopMatrix();
}

//make big cube
static void draw_scene_cube()
{
    //make cube's front 
    glFrontFace(GL_CCW);

    //width
    glLineWidth(s_line_width);

    //grass texture
    glBindTexture(GL_TEXTURE_2D, g_texture[GRASS_TEXTURE]);

    glBegin(GL_QUADS);    //make six square and make then to be big cube

        //draw big cube surface one
        glColor3fv(s_square_face_color[0]);
        glNormal3fv(s_face_normal[0]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_squre_vertex[0][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_squre_vertex[0][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_squre_vertex[0][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_squre_vertex[0][3]);

        //draw big cube surface 2
        glColor3fv(s_square_face_color[1]);
        glNormal3fv(s_face_normal[1]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_squre_vertex[1][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_squre_vertex[1][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_squre_vertex[1][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_squre_vertex[1][3]);

        //draw big cube surface 3
        glColor3fv(s_square_face_color[2]);
        glNormal3fv(s_face_normal[2]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_squre_vertex[2][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_squre_vertex[2][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_squre_vertex[2][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_squre_vertex[2][3]);

        //draw big cube surface 4
        glColor3fv(s_square_face_color[3]);
        glNormal3fv(s_face_normal[3]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_squre_vertex[3][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_squre_vertex[3][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_squre_vertex[3][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_squre_vertex[3][3]);

        //draw big cube surface 5
        glColor3fv(s_square_face_color[4]);
        glNormal3fv(s_face_normal[4]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_squre_vertex[4][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_squre_vertex[4][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_squre_vertex[4][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_squre_vertex[4][3]);

        //draw big cube surface 6
        glColor3fv(s_square_face_color[5]);
        glNormal3fv(s_face_normal[5]);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(s_squre_vertex[5][0]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(s_squre_vertex[5][1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(s_squre_vertex[5][2]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(s_squre_vertex[5][3]);

    glEnd();
}

//make network on big cube
static void draw_scene_cube_lines()
{
    float fDelta;

    glColor3fv(s_line_color);
    glLineWidth(s_line_width);

    //make bunch of line on big cube to combo square
    glBegin(GL_LINES);
    for(fDelta=-HALF_L;fDelta<=HALF_L;fDelta+=1.0f)
    {
        glVertex3f(fDelta,HALF_L,HALF_RAISED);
        glVertex3f(fDelta,-HALF_L,HALF_RAISED);
        glVertex3f(-HALF_L,fDelta,HALF_RAISED);
        glVertex3f(HALF_L,fDelta,HALF_RAISED);

        glVertex3f(HALF_RAISED,HALF_L,fDelta);
        glVertex3f(HALF_RAISED,-HALF_L,fDelta);
        glVertex3f(HALF_RAISED,fDelta,HALF_L);
        glVertex3f(HALF_RAISED,fDelta,-HALF_L);

        glVertex3f(fDelta,HALF_L,-HALF_RAISED);
        glVertex3f(fDelta,-HALF_L,-HALF_RAISED);
        glVertex3f(-HALF_L,fDelta,-HALF_RAISED);
        glVertex3f(HALF_L,fDelta,-HALF_RAISED);

        glVertex3f(-HALF_RAISED,-HALF_L,fDelta);
        glVertex3f(-HALF_RAISED,HALF_L,fDelta);
        glVertex3f(-HALF_RAISED,fDelta,HALF_L);
        glVertex3f(-HALF_RAISED,fDelta,-HALF_L);

        glVertex3f(-HALF_L,HALF_RAISED,fDelta);
        glVertex3f(HALF_L,HALF_RAISED,fDelta);
        glVertex3f(fDelta,HALF_RAISED,HALF_L);
        glVertex3f(fDelta,HALF_RAISED,-HALF_L);

        glVertex3f(-HALF_L,-HALF_RAISED,fDelta);
        glVertex3f(HALF_L,-HALF_RAISED,fDelta);
        glVertex3f(fDelta,-HALF_RAISED,HALF_L);
        glVertex3f(fDelta,-HALF_RAISED,-HALF_L);
    }
    glEnd();
}

#ifdef _DRAW_AXIS_  //draw world coordinate
static void draw_coordinate()
{
    glLineWidth(10.0f);

    glBegin(GL_LINES);
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(-HALF_L-AXIS_ADDER,0.0f,0.0f);
        glVertex3f(HALF_L+AXIS_ADDER,0.0f,0.0f);

        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(0.0f,-HALF_L-AXIS_ADDER,0.0f);
        glVertex3f(0.0f,HALF_L+AXIS_ADDER,0.0f);

        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(0.0f,0.0f,-HALF_L-AXIS_ADDER);
        glVertex3f(0.0f,0.0f,HALF_L+AXIS_ADDER);
    glEnd();

    glPushMatrix();
        glColor3f(1.0f,0.0f,0.0f);
        glTranslatef(HALF_L+AXIS_ADDER,0.0f,0.0f);
        glutSolidSphere(0.8,15,15);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.0f,1.0f,0.0f);
        glTranslatef(0.0f,HALF_L+AXIS_ADDER,0.0f);
        glutSolidSphere(0.8,15,15);
    glPopMatrix();

    glPushMatrix();
        glColor3f(0.0f,0.0f,1.0f);
        glTranslatef(0.0f,0.0f,HALF_L+AXIS_ADDER);
        glutSolidSphere(0.8,15,15);
    glPopMatrix();
}
#endif

//draw barrier on big cube
static void draw_barrier()
{
    int face_idx,idx;

    //make square's vertex counter-clockwise to be its front face
    glFrontFace(GL_CCW);

    glLineWidth(1.0f);
    glColor3f(0.0f,0.5f,0.5f);

    //binging barrier texture
    glBindTexture(GL_TEXTURE_2D, g_texture[BARRIER_TEXTURE]);
    for(face_idx=0;face_idx<6;face_idx++)
        for(idx=0;idx<BARRIER_COUNT;idx++)
        {
            glPushMatrix();   //make coordinate in barrier's centre
            glTranslatef(s_face_barrier[face_idx][idx][0],   //and save them in s_face_barrier
                    s_face_barrier[face_idx][idx][1],        
                    s_face_barrier[face_idx][idx][2]);
            glBegin(GL_QUADS);    //make six square to combo a barrier
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
}

//make coin
static void draw_fixed_coin()
{
    PSBODY_NODE pNode = s_fixed_coin;

    //make counter-clockwise order to be its front 
    glFrontFace(GL_CCW);
    glLineWidth(1.0f);

    //coin texture
    glBindTexture(GL_TEXTURE_2D, g_texture[SNAKESKIN_TEXTURE]);
    while(pNode!=NULL)    //read coin location from s_fixed_coin
    {
        glPushMatrix();  //make centre of coin to be its coordinate
        glTranslatef(pNode->pos_3d[0],pNode->pos_3d[1],pNode->pos_3d[2]);
        glColor3fv(pNode->color);
        glBegin(GL_QUADS);    //make a coin cube
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

//render scene
void draw_scene()
{
    skydome_render();          //draw sky
    draw_scene_cube();         //draw big cube
    draw_scene_cube_lines();   //draw network on big cube

#ifdef _DRAW_AXIS_
    draw_coordinate();     
#endif

    draw_barrier();
    draw_fixed_coin();
}

void scene_update()
{
    s_y_rot += 1.0f;    //when sky rotate one unit update it
}
//decide what did snack touch
//1 for barrier
//else 0
int snake_in_scene()
{
    int face_idx,x,y;
    PSBODY_NODE pNode;

    switch(g_snake_head.cube_face)
    {
    case SURFACE_1:
        face_idx = 0;
        break;
    case SURFACE_2:
        face_idx = 1;
        break;
    case SURFACE_3:
        face_idx = 2;
        break;
    case SURFACE_4:
        face_idx = 3;
        break;
    case SURFACE_5:
        face_idx = 4;
        break;
    case SURFACE_6:
        face_idx = 5;
        break;
    default:
        //_asm int 3;
        break;
    }

    x = g_snake_head.pos_2d_x;
    y = g_snake_head.pos_2d_y;

    pNode = s_fixed_coin;
    while(pNode!=NULL)    //s_fixed_coin save all the coin rest on big cube
    {   
        //if snack head and coin in same coordinate
        if(g_snake_head.cube_face==pNode->cube_face&&
            g_snake_head.pos_2d_x==pNode->pos_2d_x&&
            g_snake_head.pos_2d_y==pNode->pos_2d_y)
        {
            //delete it and add to coin
            if(pNode->next!=NULL)
                pNode->next->previous = pNode->previous;
            
            if(pNode->previous!=NULL)
            {
                pNode->previous->next = pNode->next;
            }
            else
            {
                s_fixed_coin = pNode->next;
                s_fixed_coin->previous = NULL;
            }

            pNode->next                     = NULL;
            pNode->previous                 = g_snake_head.last_section;
            g_snake_head.last_section->next = pNode;
            g_snake_head.last_section       = pNode;

            s_face_grid_type[face_idx][y][x] = GRID_TYPE_EMPTY;
            emit_particle(pNode->cube_face,pNode->pos_3d);    //particle effect after snack ate one coin

            g_snake_head.length++;
            s_coin_count--;

            //if touch coin then its wont be barrier
            return 0;
        }

        pNode = pNode->next;
    }

    //if touch barrier then return 1
    if(s_face_grid_type[face_idx][y][x]==GRID_TYPE_BLOCK)
        return 1;

    //base on snack head to see there are barrier or not
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
    return s_face_grid_type[face_idx][y][x]==GRID_TYPE_BLOCK;
}

//output info on screen
void print_info()
{
    char buf[128] = {0};
    int surface;
    time_t now;
    double time_diff;
    int rows = g_height / 14;
    int cols = g_width  / 10;

    glColor3f(1.0f,1.0f,1.0f);

    //output time
    now = time(NULL);
    time_diff = difftime(now,s_old_time);
    sprintf(buf,"time:%4.0lfs",time_diff);
    PrintString(buf,1,1);

    switch(g_snake_head.cube_face)
    {
    case SURFACE_1:
        surface = 1;
        break;
    case SURFACE_2:
        surface = 2;
        break;
    case SURFACE_3:
        surface = 3;
        break;
    case SURFACE_4:
        surface = 4;
        break;
    case SURFACE_5:
        surface = 5;
        break;
    case SURFACE_6:
        surface = 6;
        break;
    default:
        break;
    }

    //output where snack on 
    sprintf(buf,"surface:%2d",surface);
    PrintString(buf,1,2);

    //and its direction
    switch(g_snake_head.s_dir)
    {
    case DIR_U:
        sprintf(buf,"dir: up");
        break;
    case DIR_R:
        sprintf(buf,"dir: right");
        break;
    case DIR_D:
        sprintf(buf,"dir: down");
        break;
    case DIR_L:
        sprintf(buf,"dir: left");
        break;
    default:
        break;
    }
    PrintString(buf,1,3);

    //length
    sprintf(buf,"snake length:%4d",g_snake_head.length);
    PrintString(buf,1,4);

    //coin left
    sprintf(buf,"left coin:%4d",s_coin_count);
    PrintString(buf,1,5);

    //pause game and pause game info on screen
    if(s_b_pause==1)
    {
        sprintf(buf,"PAUSE...");
        PrintString(buf,1,7);
    }

    //author
    glColor3f(1.0f, 0.0f, 0.0f);
    sprintf(buf,"Author: Chao.Ye Rajat.Arora ");
    PrintString(buf,cols-25,rows - 2);
}
