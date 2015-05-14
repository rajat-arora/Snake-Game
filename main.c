#include "haharsw.h"


//set scene's x and y axix rotate angle
GLfloat s_xRot = 0.0f;
GLfloat s_yRot = 0.0f;

//third person camera position
static float s_cameraX = 0.0f;
static float s_cameraY = 0.0f;
static float s_cameraZ = CAMERA_POS;


//8 light source position
static GLfloat s_light0Pos[] = { -LIGHT_COORD, LIGHT_COORD, LIGHT_COORD, 1.0f };
static GLfloat s_light1Pos[] = { LIGHT_COORD, LIGHT_COORD, LIGHT_COORD, 1.0f };
static GLfloat s_light2Pos[] = { LIGHT_COORD, LIGHT_COORD, -LIGHT_COORD, 1.0f };
static GLfloat s_light3Pos[] = { -LIGHT_COORD, LIGHT_COORD, -LIGHT_COORD, 1.0f };
static GLfloat s_light4Pos[] = { -LIGHT_COORD, -LIGHT_COORD, LIGHT_COORD, 1.0f };
static GLfloat s_light5Pos[] = { LIGHT_COORD, -LIGHT_COORD, LIGHT_COORD, 1.0f };
static GLfloat s_light6Pos[] = { LIGHT_COORD, -LIGHT_COORD, -LIGHT_COORD, 1.0f };
static GLfloat s_light7Pos[] =
		{ -LIGHT_COORD, -LIGHT_COORD, -LIGHT_COORD, 1.0f };

//type of camera A for third person B for first person
int g_camera_type = CAMERA_TYPE_A;

int s_b_pause = 0;     //pause or not
static int s_b_over = 0;     //game over or not
static int s_b_light = 1;     //light on or not
static int s_b_texture = 1;   //texture on or not
static int s_ware_line = 0;   //GL_FILL or GL_LINE

//windows width and hight
int g_width = 0;
int g_height = 0;

int initialize() {
	//light source
	GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat diffuseLigth[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	//line fill mode
	if (s_ware_line == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);

	glShadeModel(GL_FLAT);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH, GL_NICEST);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	//open light
	if (s_b_light == 1)
		glEnable(GL_LIGHTING);

	//1
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLigth);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glEnable(GL_LIGHT0);

	//2
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLigth);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
	glEnable(GL_LIGHT1);

	//3
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLigth);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight);
	glEnable(GL_LIGHT2);

	//4
	glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLigth);
	glLightfv(GL_LIGHT3, GL_SPECULAR, specularLight);
	glEnable(GL_LIGHT3);

	//5
	glLightfv(GL_LIGHT4, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuseLigth);
	glLightfv(GL_LIGHT4, GL_SPECULAR, specularLight);
	glEnable(GL_LIGHT4);

	//6
	glLightfv(GL_LIGHT5, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuseLigth);
	glLightfv(GL_LIGHT5, GL_SPECULAR, specularLight);
	glEnable(GL_LIGHT5);

	//7
	glLightfv(GL_LIGHT6, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, diffuseLigth);
	glLightfv(GL_LIGHT6, GL_SPECULAR, specularLight);
	glEnable(GL_LIGHT6);

	//8
	glLightfv(GL_LIGHT7, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT7, GL_DIFFUSE, diffuseLigth);
	glLightfv(GL_LIGHT7, GL_SPECULAR, specularLight);
	glEnable(GL_LIGHT7);

	//material colour
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specref);    //specular
	glMateriali(GL_FRONT, GL_SHININESS, 4);          //shininess

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	snake_initialize();
	scene_initialize();
	texture_initialize();
	particle_initialize();
	font_initialize();

	return 1;
}

//clean up
void cleanup() {
	snake_cleanup();
	scene_cleanup();
	texture_cleanup();
	particle_cleanup();
	font_cleanup();
}

//special keyboard function
void process_special_keys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		if (s_b_pause == 0)
			snake_turn_head(DIR_L);
		break;
	case GLUT_KEY_RIGHT:
		if (s_b_pause == 0)
			snake_turn_head(DIR_R);
		break;
	case GLUT_KEY_UP:
		if (s_b_pause == 0)
			snake_turn_head(DIR_U);
		break;
	case GLUT_KEY_DOWN:
		if (s_b_pause == 0)
			snake_turn_head(DIR_D);
		break;
	}
}

//normal keyboard function
void process_normal_keys(unsigned char key, int x, int y) {
	switch (key) {
	case 27:            //press ese to exit
	{
		cleanup();
		exit(0);
	}
		break;
	case 'w':
	case 'W': {
		if (g_camera_type == CAMERA_TYPE_A) {
			s_xRot -= ROTATE_ANGLE;
			glutPostRedisplay();
		}
	}
		break;
	case 's':
	case 'S': {
		if (g_camera_type == CAMERA_TYPE_A) {
			s_xRot += ROTATE_ANGLE;
			glutPostRedisplay();
		}
	}
		break;
	case 'a':
	case 'A': {
		if (g_camera_type == CAMERA_TYPE_A) {
			s_yRot -= ROTATE_ANGLE;
			glutPostRedisplay();
		}
	}
		break;
	case 'd':
	case 'D': {
		if (g_camera_type == CAMERA_TYPE_A) {
			s_yRot += ROTATE_ANGLE;
			glutPostRedisplay();
		}
	}
		break;
	case 'c':
	case 'C': {
		//switch between first and third view
		if (g_camera_type == CAMERA_TYPE_A)
			g_camera_type = CAMERA_TYPE_B;
		else
			g_camera_type = CAMERA_TYPE_A;
	}
		break;
	case 'p':
	case 'P': {
		//pause or start game
		if (s_b_over == 0)
			s_b_pause = !s_b_pause;
	}
		break;
	case 'l':
	case 'L': {
		//light on or off
		s_b_light = !s_b_light;
		if (s_b_light == 1)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);
		glutPostRedisplay();
	}
		break;
	case 'q':
	case 'Q': {
		//line or fill mode
		s_ware_line = !s_ware_line;
		if (s_ware_line == 0)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glutPostRedisplay();
	}
		break;
	case 't':
	case 'T': {
		//switch on/off texture
		s_b_texture = !s_b_texture;
		if (s_b_texture == 1)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		glutPostRedisplay();
	}
		break;
	case 'r':
	case 'R': {
		//use r to restart game while game over
		if (s_b_over == 1) {
			s_b_over = 0;
			snake_cleanup();
			scene_cleanup();
			particle_cleanup();
			snake_initialize();
			scene_initialize();
			particle_initialize();
			s_b_pause = 0;
			s_xRot = 0.0f;
			s_yRot = 0.0f;
		}
	}
		break;
	}
}

//windows size
void change_size(int width, int height) {
	GLfloat fAspect;

	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	fAspect = (GLfloat) width / (GLfloat) height;
	gluPerspective(54.0, fAspect, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//8 light source
	glLightfv(GL_LIGHT0, GL_POSITION, s_light0Pos);
	glLightfv(GL_LIGHT1, GL_POSITION, s_light1Pos);
	glLightfv(GL_LIGHT2, GL_POSITION, s_light2Pos);
	glLightfv(GL_LIGHT3, GL_POSITION, s_light3Pos);
	glLightfv(GL_LIGHT4, GL_POSITION, s_light4Pos);
	glLightfv(GL_LIGHT5, GL_POSITION, s_light5Pos);
	glLightfv(GL_LIGHT6, GL_POSITION, s_light6Pos);
	glLightfv(GL_LIGHT7, GL_POSITION, s_light7Pos);

	g_width = width;
	g_height = height;
}

//time function
void mytimer_function(int value) {
	int bRet01, bRet02;

	if (s_b_pause == 0) {
		scene_update();
		update_emitter();

		bRet01 = snake_update_pos();   //refresh snack and see if it eat it self
		bRet02 = snake_in_scene();      //or coin or barrier

		//if snack touch itself or barrier then game over
		if (bRet01 == 1 || bRet02 == 1) {
			s_b_pause = 1;
			s_b_over = 1;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(TIMER_SKIP, mytimer_function, 1);
}

//render function
void render_scene(void) {

	if (snakeP[2] == 15) {
		s_xRot = 0.0f;
		s_yRot = 0.0f;

	}

	if (snakeP[2] == -15) {
		s_xRot = 0.0f;
		s_yRot = 180.0f;

	}
	if (snakeP[0] == 15) {
		s_xRot = 0.0f;
		s_yRot = -90.0f;

	}

	if (snakeP[0] == -15) {
		s_xRot = 0.0f;
		s_yRot = 90.0f;

	}

	if (snakeP[1] == 15) {
		s_xRot = 90.0f;
		s_yRot = 0.0f;

	}
	if (snakeP[1] == -15) {
		s_xRot = -90.0f;
		s_yRot = 0.0f;

	}

	if (s_b_over == 1)    //game over picture while game over
			{
		render_over(g_width, g_height);
	} else {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();

		//base on view type and set camera position
		if (g_camera_type == CAMERA_TYPE_A)       //third person view
		{
			gluLookAt(s_cameraX, s_cameraY, s_cameraZ, 0.0f, 0.0f, 0.0f, 0.0,
					1.0, 0.0);
			glRotatef(s_xRot, 1.0f, 0.0f, 0.0f);
			glRotatef(s_yRot, 0.0f, 1.0f, 0.0f);
		} else    //first person view
		{
			camera_follow_snake();
		}

		draw_scene();
		draw_snake();
		render_emitter();

		glLoadIdentity();
		print_info();
	}

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), GetModuleHandle(0), SND_RESOURCE | SND_ASYNC);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_TITLE);

	glutSpecialFunc(process_special_keys);
	glutKeyboardFunc(process_normal_keys);
	glutReshapeFunc(change_size);
	glutDisplayFunc(render_scene);
	glutTimerFunc(TIMER_SKIP, mytimer_function, 1);

	initialize();

	glutMainLoop();

	cleanup();

	return 0;
}
