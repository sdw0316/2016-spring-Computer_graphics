#include "flag.h"
#include <GL/glut.h>

#define PI 3.141592

void mass_pole::draw() {
	double cx, cy, cz;
	double hx, hy, hz;

	Vec3 normal(0.0, 1.0, 0.0);
	double degree = acos(normal.innerProduct(direction) / (normal.length()*direction.length()));	//ÇöÀç direction°ú normal°£ÀÇ °¢µµ

	if (direction.x<0) {
		degree = 2 * PI - degree;
	}

	cx = center.getX();
	cy = center.getY();
	cz = center.getZ();

	Vec3 hpos(cx, cy + height, cz);

	//printf("pole degree : %lf\n", degree);

	hx = hpos.getX();
	hy = hpos.getY();
	hz = hpos.getZ();


	//printf("%lf %lf %lf\n", hx, hy, hz);
	//¹Ø¸é
	
	double angle;
	
	for (angle = 0.0; angle <= PI*2.0; angle += (PI / 8.0)) {
		double nangle = angle + (PI / 8.0);

		Vec3 pos(cx + radius*sin(angle), cy, cz + radius*cos(angle));
		Vec3 npos(cx + radius*sin(nangle), cy, cz + radius*cos(nangle));
		pos = pos.rotateZ(-degree);
		npos = npos.rotateZ(-degree);

		glBegin(GL_TRIANGLES);
		glVertex3f(cx, cy, cz);
		glVertex3f(pos.x,pos.y,pos.z);
		glVertex3f(npos.x,npos.y,npos.z);
		glEnd();
	}
	
	//¿·¸é
	
	
	glBegin(GL_QUAD_STRIP);            //¿ø±âµÕÀÇ ¿·¸é
	for (angle = 0.0f; angle < (2.0f*PI); angle += (PI / 8.0f)){
		Vec3 pos(cx + radius*sin(angle), cy, cz + radius*cos(angle));
		Vec3 npos(hx + radius*sin(angle), hy, hz + radius*cos(angle));
		pos = pos.rotateZ(-degree);
		npos = npos.rotateZ(-degree);

		glNormal3f(sin(angle), direction.getY() , cos(angle));
		glVertex3f(pos.x,pos.y,pos.z);
		glVertex3f(npos.x,npos.y,npos.z);
	}
	glEnd();
	

	//À­¸é
	for (angle = 0.0; angle <= PI*2.0; angle += (PI / 8.0)) {
		double nangle = angle + (PI / 8.0);
		Vec3 cur(hx, hy, hz);
		Vec3 pos(hx + radius*sin(angle), hy, hz + radius*cos(angle));
		Vec3 npos(hx + radius*sin(nangle), hy, hz + radius*cos(nangle));
		cur = cur.rotateZ(-degree);
		pos = pos.rotateZ(-degree);
		npos = npos.rotateZ(-degree);

		glBegin(GL_TRIANGLES);
		glVertex3f(cur.x,cur.y,cur.z);
		glVertex3f(pos.x, pos.y, pos.z);
		glVertex3f(npos.x, npos.y, npos.z);
		glEnd();
	}
	//direction

	/*glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 1.0);
	glVertex3f(cx, cy, cz);
	glVertex3f(direction.getX()*10, direction.getY()*10, direction.getZ()*10);
	glEnd();
	*/
}

void mass_flag::draw() {
	glColor3f(1.0, 0.0, 0.0);
	pole->draw();
	if (!(cloth->texture)) {
		cloth->Texdraw();
	}
	else {
		printf("ÅØ½ºÃÄ À½½¿\n");
		cloth->draw();
	}
}