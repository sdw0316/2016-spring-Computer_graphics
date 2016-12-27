#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "flag.h"
#include "windmill.h"

#define NUM 3
#define L 30

int start = 0;

mass_flag * flag[NUM];
mass_cloth *cloth[NUM];
mass_pole * pole[NUM];

Mesh wing;
Mesh pin;

Mesh hat;
Mesh foot;
Mesh leg;
Mesh body;

static GLfloat glb_amb[] = { 0.1, 0.1, 0.1, 1.0 };
static GLfloat light_amb[] = { 0.8, 0.8, 0.8, 1.0 };
static GLfloat light_dif[] = { 0.5, 0.5, 0.5, 1.0 };
static GLfloat light_spc[] = { 0.5, 0.5, 0.5, 1.0 };

static GLfloat light0_pos[] = { 0.0, 20.0, 0.0, 0.0 };

static GLfloat mat_ambdif[] = { 0.8, 0.8, 0.8, 1.0 };
static GLfloat mat_spc[] = { 1.0, 1.0, 1.0, 1.0 };
static GLfloat mat_shn[] = { 10.0 };

// Rotation amounts

double radius;
double degree;
double rotate_angle;
double a = 1;

double wind_speed = 0.0;
double axis;

GLuint texbox[6];
GLuint photo[4];
GLuint windmill[6];

GLuint loadTexture(char *texfile) {
	GLuint texture = NULL;
	glGenTextures(1, &texture);// texture 포인터 하나 만들고

	//이미지 로드하고
	FILE *fp = fopen(texfile, "rb");
	if (!fp) {
		printf("ERROR : No %s.\n fail to bind %d\n", texfile, texture);  return false;
	}

	int width, height, channel;
	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 4);
	fclose(fp);

	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
																								
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);
  
	return texture;
}

void create() {

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, glb_amb);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_spc);
	glEnable(GL_LIGHT0);

	texbox[0] = loadTexture(".\\texture\\top.tga");
	texbox[1] = loadTexture(".\\texture\\bottom.tga");
	texbox[2] = loadTexture(".\\texture\\left.tga");
	texbox[3] = loadTexture(".\\texture\\right.tga");
	texbox[4] = loadTexture(".\\texture\\front.tga");
	texbox[5] = loadTexture(".\\texture\\back.tga");
	photo[0] = loadTexture("xae.tga");
	photo[1] = loadTexture("CG.tga");
	photo[2] = loadTexture("mul.tga");
	windmill[0] = loadTexture(".\\texture\\hat.tga");
	windmill[1] = loadTexture(".\\texture\\foot.tga");
	windmill[2] = loadTexture(".\\texture\\leg.tga");
	windmill[3] = loadTexture(".\\texture\\wall.tga");
	windmill[0] = loadTexture(".\\texture\\wall1.tga");
}

void init() {
	degree = 0.0;
	rotate_angle = 0.0;
	axis = 0.0;
	radius = 30.0;

	flag[0] = new mass_flag(-10.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	flag[1] = new mass_flag(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	flag[2] = new mass_flag(10.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	for (int i = 0; i < NUM; i++) {
		flag[i]->init();

		cloth[i] = flag[i]->cloth;
		pole[i] = flag[i]->pole;
	}

	create();

	wing.loadObj(".\\windmill\\wing.obj");
	pin.loadObj(".\\windmill\\pin.obj");
	hat.loadObj(".\\windmill\\hat.obj");
	leg.loadObj(".\\windmill\\leg.obj");
	body.loadObj(".\\windmill\\body.obj");
	foot.loadObj(".\\windmill\\foot.obj");
}

void Render()
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(radius*sin(degree), 10.0, radius*cos(degree),
		0, 10.0, 0,
		0, 1, 0);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambdif);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_ambdif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spc);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shn);


	//draw axis
	/*glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(100.0, 0.0, 0.0);
		glEnd();
	glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 100.0, 0.0);
		glEnd();
	glBegin(GL_LINES);
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 100.0);
	glEnd();
	*/

	//glRotatef(axis, 0.0, 0.0, 1.0);

	//draw box
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texbox[0]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1 - 1.0);
		glVertex3f(L, L, -L);
		glTexCoord2f(0.0, 1 - 0.0);
		glVertex3f(L, L, L);
		glTexCoord2f(1.0, 1 - 0.0);
		glVertex3f(-L, L, L);
		glTexCoord2f(1.0, 1 - 1.0);
		glVertex3f(-L, L, -L);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texbox[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1 - 0.0);
		glVertex3f(L, 0, -L);
		glTexCoord2f(0.0, 1 - 1.0);
		glVertex3f(L, 0, L);
		glTexCoord2f(1.0, 1 - 1.0);
		glVertex3f(-L, 0, L);
		glTexCoord2f(1.0, 1 - 0.0);
		glVertex3f(-L, 0, -L);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texbox[2]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 1 - 1.0);
		glVertex3f(-L, L, -L);
		glTexCoord2f(0.0, 1 - 1.0);
		glVertex3f(-L, L, L);
		glTexCoord2f(0.0, 1 - 0.0);
		glVertex3f(-L, -0, L);
		glTexCoord2f(1.0, 1 - 0.0);
		glVertex3f(-L, -0, -L);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texbox[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1 - 1.0);
		glVertex3f(L, L, -L);
		glTexCoord2f(0.0, 1 - 0.0);
		glVertex3f(L, -0, -L);
		glTexCoord2f(1.0, 1 - 0.0);
		glVertex3f(L, -0, L);
		glTexCoord2f(1.0, 1 - 1.0);
		glVertex3f(L, L, L);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texbox[4]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0, 1 - 0.0);
		glVertex3f(L, -0, -L);
		glTexCoord2f(1.0, 1 - 1.0);
		glVertex3f(L, L, -L);
		glTexCoord2f(0.0, 1 - 1.0);
		glVertex3f(-L, L, -L);
		glTexCoord2f(0.0, 1 - 0.0);
		glVertex3f(-L, -0, -L);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texbox[5]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1 - 1.0);
		glVertex3f(L, L, L);
		glTexCoord2f(0.0, 1 - 0.0);
		glVertex3f(L, -0, L);
		glTexCoord2f(1.0, 1 - 0.0);
		glVertex3f(-L, -0, L);
		glTexCoord2f(1.0, 1 - 1.0);
		glVertex3f(-L, L, L);
	glEnd();

	//draw flag
	for (int i = 0; i < NUM; i++) {
		glBindTexture(GL_TEXTURE_2D, photo[i]);
		if (i == 1) {
			glPushMatrix();
			glTranslatef(0.0, 0.0, 10.0);
			flag[i]->draw();
			glPopMatrix();
		}
		else {
			flag[i]->draw();// 자체 텍스쳐 아이디 갖고있음
		}
	}
	
	//wing
	glBindTexture(GL_TEXTURE_2D, windmill[1]);
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(0.0, 80.0, 4.0);
	glRotatef(rotate_angle, 0.0, 0.0, 1.0);
	wing.draw();
	glPopMatrix();

	//pin
	glBindTexture(GL_TEXTURE_2D, windmill[0]);
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(0.0, 80.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	pin.draw();
	glPopMatrix();

	//haed
	glBindTexture(GL_TEXTURE_2D, windmill[3]);
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(0.0, 80.0, -23.0);
	glColor3f(0.0, 1.0, 0.0);
	hat.draw();
	glPopMatrix();

	//body
	glBindTexture(GL_TEXTURE_2D, windmill[2]);
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(0.0, 49.0, -35.0);
	glColor3f(0.0, 0.0, 1.0);
	body.draw();
	glPopMatrix();
	
	//leg
	glBindTexture(GL_TEXTURE_2D, windmill[3]);
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(0.0, 18.0, -45.0);
	glColor3f(1.0, 0.0, 1.0);
	leg.draw();
	glPopMatrix();
	
	//foot
	glBindTexture(GL_TEXTURE_2D, windmill[0]);
	glPushMatrix();
	glScalef(0.2, 0.2, 0.2);
	glTranslatef(0.0, 3.0, -40.6);
	glColor3f(1.0, 1.0, 0.0);
	foot.draw();
	glPopMatrix();
	
	glutSwapBuffers();
	glutPostRedisplay();
}


void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (double)w / h, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Key_func(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case 'q':
	case 'Q':
		degree -= 0.1;
		break;
	case 'e':
	case 'E':
		degree += 0.1;
		break;
	case 'r':
	case 'R':
		init();
		break;
	case 'a':
	case 'A':
		break;
	case 'd':
	case 'D':
		break;
	case 'w':
	case 'W':
		if(wind_speed<200)
			wind_speed += 5;
		break;
	case 's':
	case 'S':
		if(wind_speed>-200)
		wind_speed -= 5;
		break;
	case 'f':
	case 'F':
		start = (start+1)%2;
		break;
	}
}
void mouse_func(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
		}
	}
}

void update()
{
	Vec3 gravity(0.0, -9.8, 0.0);
	float dt = 0.005;

	for (int i = 0; i < NUM; i++) {
		cloth[i]->integrate(dt, gravity);
		if(i!=1)
			cloth[i]->airForce(Vec3(wind_speed*(i+1)/0.7, wind_speed*0.8, wind_speed*0.5));
		else
			cloth[i]->airForce(Vec3(wind_speed, wind_speed*0.8, wind_speed*0.5));
	}
	rotate_angle -= wind_speed * 2 / 50;

	if (start) {
		if (flag[1]->direction.innerProduct(Vec3(0.0, 1.0, 0.0)) < 0.5)
			a *= -1;
		flag[1]->rotateFlag(0.02*a);
	}
	printf("%lf\n", wind_speed);
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("life ruined");

	init();

	glutIdleFunc(update);
	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutMouseFunc(mouse_func);
	glutKeyboardFunc(Key_func);
	glutMainLoop();

	return 0;
}