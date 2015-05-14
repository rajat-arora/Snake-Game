
#include "haharsw.h"

//set static constant for this particle system
static const int HALF_RAND = (RAND_MAX / 2);    
static const float FORCE     = 0.3f;    //accelerate
static const float SPEED_ORG = 0.0f;    //init speed
static const float SPEED_VAR = 2.0f;    //init speed variable
static const int   LIFE_ORG  = 100;     //lifetime on particle system
static const int   LIFE_VAR  = 15;      //variable on lifetime of particle system
static const int   EMITS_ORG = 100;     //number of particle
static const int   EMITS_VAR = 30;      //number of variable on particle system
static const float START_COLOR[3] = { 0.4f, 0.4f, 0.4f };      //init colour of particle
static const float END_COLOR[3]   = { 1.0f, 1.0f, 1.0f };      //end colour of particle
static const float STARRT_COLOR_VAR[3] = { 0.1f, 0.1f, 0.1f }; //init variable for particle colour
static const float END_COLOR_VAR[3]    = { 0.1f, 0.1f, 0.1f }; //end variable for particle colour

static Particle  *s_ParticlePool;     //particle list
static Particle  *org_particle;       //back up point for particle
static Particle  *s_ActiveP = NULL;   //activity particle 
static int       s_cur_particle_cnt = 0;//number activity particle 

//make randon number between -1 to 1
#define rand_float() ((float)(rand() - HALF_RAND) / (float)HALF_RAND)

//init particle system
void particle_initialize()
{
    int loop;

    srand( (unsigned)time( NULL ) );

    //save max amout of particle in particle list
    s_ParticlePool = (Particle *)malloc(MAX_PARTICLES * sizeof(Particle));
    org_particle = s_ParticlePool;
    for (loop = 0; loop < MAX_PARTICLES - 1; loop++)
    {   //set particle successor and save on list
        s_ParticlePool[loop].next = &s_ParticlePool[loop + 1];
    }
    s_ParticlePool[MAX_PARTICLES - 1].next = NULL;
    s_ActiveP = NULL;
    s_cur_particle_cnt = 0;
}

void particle_cleanup()
{
    //clean particle
    if(org_particle!=NULL)
    {
        free(org_particle);
        org_particle = NULL;
    }
}

//in big cube add one coordinate to activity particle list
static void my_add_particle(int surface,float pos[3])
{
    Particle *ptl;
    float start[3],end[3];

    if (s_cur_particle_cnt < MAX_PARTICLES)
    {
        //get one particle from list
        ptl = s_ParticlePool;
        s_ParticlePool = s_ParticlePool->next;

        //and put on activity particle list
        if (s_ActiveP != NULL)
            s_ActiveP->prev = ptl;
        ptl->next = s_ActiveP;
        ptl->prev = NULL;
        s_ActiveP = ptl;

        //random particle style 
        ptl->shape = rand()%2;

        //set particle init position
        ptl->pos[0] = pos[0];
        ptl->pos[1] = pos[1];
        ptl->pos[2] = pos[2];

        //base on the surface of big cube set different speed and vertex
        switch(surface)
        {
        case SURFACE_1:
            ptl->dir[0] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[1] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[2] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->force[0] =  0.0f;
            ptl->force[1] =  0.0f;
            ptl->force[2] = -FORCE;
            if(ptl->dir[2]<0.0f)
                ptl->dir[2] = -ptl->dir[2];
            break;
        case SURFACE_2:
            ptl->dir[0] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[1] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[2] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->force[0] = -FORCE;
            ptl->force[1] =  0.0f;
            ptl->force[2] =  0.0f;
            if(ptl->dir[0]<0.0f)
                ptl->dir[0] = -ptl->dir[0];
            break;
        case SURFACE_3:
            ptl->dir[0] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[1] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[2] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->force[0] =  0.0f;
            ptl->force[1] =  0.0f;
            ptl->force[2] =  FORCE;
            if(ptl->dir[2]>0.0f)
                ptl->dir[2] = -ptl->dir[2];
            break;
        case SURFACE_4:
            ptl->dir[0] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[1] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[2] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->force[0] =  FORCE;
            ptl->force[1] =  0.0f;
            ptl->force[2] =  0.0f;
            if(ptl->dir[0]>0.0f)
                ptl->dir[0] = -ptl->dir[0];
            break;
        case SURFACE_5:
            ptl->dir[0] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[1] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[2] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->force[0] =  0.0f;
            ptl->force[1] = -FORCE;
            ptl->force[2] =  0.0f;
            if(ptl->dir[1]<0.0f)
                ptl->dir[0] = -ptl->dir[0];
            break;
        case SURFACE_6:
            ptl->dir[0] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[1] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->dir[2] = SPEED_ORG + (SPEED_VAR * rand_float());
            ptl->force[0] =  0.0f;
            ptl->force[1] =  FORCE;
            ptl->force[2] =  0.0f;
            if(ptl->dir[1]>0.0f)
                ptl->dir[0] = -ptl->dir[0];
            break;
        default:
            break;
        }

        //set particle system's colour 
        start[0] = START_COLOR[0] + (STARRT_COLOR_VAR[0] * rand_float());
        start[1] = START_COLOR[1] + (STARRT_COLOR_VAR[1] * rand_float());
        start[2] = START_COLOR[2] + (STARRT_COLOR_VAR[2] * rand_float());
        end[0]   = END_COLOR[0]   + (END_COLOR_VAR[0]   * rand_float());
        end[1]   = END_COLOR[1]   + (END_COLOR_VAR[1]   * rand_float());
        end[2]   = END_COLOR[2]   + (END_COLOR_VAR[2]   * rand_float());

        //set particle init colour
        ptl->color[0] = start[0];
        ptl->color[1] = start[1];
        ptl->color[2] = start[2];

        //particle life time
        ptl->life = LIFE_ORG + (int)((float)LIFE_VAR * rand_float());

        //colour
        ptl->deltaColor[0] = (end[0] - start[0]) / ptl->life;
        ptl->deltaColor[1] = (end[1] - start[1]) / ptl->life;
        ptl->deltaColor[2] = (end[2] - start[2]) / ptl->life;

        //activity list + 1
        s_cur_particle_cnt++;
    }
}

//set if particle in big cube if them in big cube inactivity it and set them lifetime to 0 make them inactivity
static void is_in_maincube(Particle *ptl)
{
    if( -HALF_L < ptl->pos[0] && ptl->pos[0] <  HALF_L  &&
        -HALF_L < ptl->pos[1] && ptl->pos[1] <  HALF_L  &&
        -HALF_L < ptl->pos[2] && ptl->pos[2] <  HALF_L )
    {
        ptl->life = 0;
    }
}

static void my_update_particle(Particle *ptl)
{
    if (ptl != NULL && ptl->life > 0)
    {
        //update activity particle state

        //location
        ptl->pos[0] += ptl->dir[0];
        ptl->pos[1] += ptl->dir[1];
        ptl->pos[2] += ptl->dir[2];

        //speed
        ptl->dir[0] += ptl->force[0];
        ptl->dir[1] += ptl->force[1];
        ptl->dir[2] += ptl->force[2];

        //colour
        ptl->color[0] += ptl->deltaColor[0];
        ptl->color[1] += ptl->deltaColor[1];
        ptl->color[2] += ptl->deltaColor[2];

        //is in big cube?
        is_in_maincube(ptl);

        //lifetime
        ptl->life--;
    }
    else if (ptl != NULL && ptl->life <= 0)
    {
        //if life time <0 them inactivity them and put them in particle list
        if (ptl->prev != NULL)
            ptl->prev->next = ptl->next;
        else
            s_ActiveP = ptl->next;

        if (ptl->next != NULL)
            ptl->next->prev = ptl->prev;

        ptl->next = s_ParticlePool;
        s_ParticlePool = ptl;
        s_cur_particle_cnt--;
    }
}

//set particle function on big cube and their 3d coordinate
void emit_particle(int surface,float pos[3])
{
    int loop,emits;

    //set them amount of particle to emit
    emits = EMITS_ORG + (int)((float)EMITS_VAR * rand_float());

    //add emit particle to activity list
    for (loop = 0; loop < emits; loop++)
        my_add_particle(surface,pos);
}

//update particle state
void update_emitter()
{
    Particle *ptl, *next;

    if(s_ActiveP!=NULL)
    {
        //update all the activity particle on activity particle list 
        ptl = s_ActiveP;
        while (ptl!=NULL)
        {
            next = ptl->next;
            my_update_particle(ptl);
            ptl = next;
        }
    }
}

//render particle
void render_emitter()
{
    Particle *ptl;

    if (s_ActiveP!= NULL)
    {
        //render all the activity particle on list
        ptl = s_ActiveP;
        while (ptl!=NULL)
        {
            glPushMatrix();
                glColor3fv(ptl->color);
                glTranslatef(ptl->pos[0],ptl->pos[1],ptl->pos[2]);  //move coordinate to particle
                if(ptl->shape==0)
                    glutSolidSphere(0.15,15,15);   //draw circle particle 
                else
                    glutSolidCube(0.15);           //draw cube particle
            glPopMatrix();

            ptl = ptl->next;
        }
    }
}
