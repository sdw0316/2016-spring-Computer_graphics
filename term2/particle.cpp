#include "particle.h"
#include <GL/glut.h>

void particle::draw() {
	glPointSize(2.0);
	glBegin(GL_POINTS);
	glColor3f(0.0, 2.0, 0.0);
	glVertex3f(getPosX(), getPosY(), getPosZ());
	glEnd();
	
	//draw vertex normal
	/*
	glBegin(GL_LINES);
	glColor3f(0.0, 2.0, 1.0);
		glVertex3f(getPosX(), getPosY(), getPosZ());
		glVertex3f(getPosX() + normal.getX(), getPosY() + normal.getY(), getPosZ() +  normal.getZ());
	glEnd();
	*/
}

void mass_spring::draw() {
	/*glLineWidth(1.0);
	glBegin(GL_LINES);
	glColor3f(2.0, 2.0, 0.0);
	glVertex3f(p1->getPosX(), p1->getPosY(), p1->getPosZ());
	glVertex3f(p2->getPosX(), p2->getPosY(), p2->getPosZ());
	glEnd();*/
}

void mass_cloth::draw() {
	for (int i = 0; i < p.size(); i++)
		p[i]->draw();
	for (int i = 0; i < s.size(); i++)
		s[i]->draw();

	for (int i = 0; i < p.size(); i++) {
		int cur = i;
		int left = cur - 1;
		int right = cur + 1;
		int up = cur + width;
		int down = cur - width;

		int x = cur % width;
		int y = cur / width;
		if (x != 0 && y > 0) {
			/*compute v2*/
			glBegin(GL_TRIANGLES);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(p[cur]->getPosX(), p[cur]->getPosY(), p[cur]->getPosZ());
			glVertex3f(p[left]->getPosX(), p[left]->getPosY(), p[left]->getPosZ());
			glVertex3f(p[down]->getPosX(), p[down]->getPosY(), p[down]->getPosZ());
			glEnd();

		}
		if (x < width - 1 && y < height - 1) {
			/*compute v4*/glBegin(GL_TRIANGLES);
			glColor3f(0.0, 1.0, 1.0);
			glVertex3f(p[cur]->getPosX(), p[cur]->getPosY(), p[cur]->getPosZ());
			glVertex3f(p[right]->getPosX(), p[right]->getPosY(), p[right]->getPosZ());
			glVertex3f(p[up]->getPosX(), p[up]->getPosY(), p[up]->getPosZ());
			glEnd();
		}
	}
}

void mass_cloth::Texdraw() {
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < p.size(); i++) {
		int cur = i;
		int left = cur - 1;
		int right = cur + 1;
		int up = cur + width;
		int down = cur - width;

		int x = cur % width;
		int y = cur / width;
		if (x != 0 && y > 0) {
			/*compute v2*/
			glBegin(GL_TRIANGLES);
			glTexCoord2f(x / ((double)width - 1), 1 - (y / ((double)height - 1)));
			glVertex3f(p[cur]->getPosX(), p[cur]->getPosY(), p[cur]->getPosZ());
			glTexCoord2f((x - 1) / ((double)width - 1), 1 - (y / ((double)height - 1)));
			glVertex3f(p[left]->getPosX(), p[left]->getPosY(), p[left]->getPosZ());
			glTexCoord2f(x / ((double)width - 1), 1 - ((y - 1) / ((double)height - 1)));
			glVertex3f(p[down]->getPosX(), p[down]->getPosY(), p[down]->getPosZ());
			glEnd();

		}
		if (x < width - 1 && y < height - 1) {
			/*compute v4*/
			glBegin(GL_TRIANGLES);
			glTexCoord2f(x / ((double)width - 1), 1 - (y / ((double)height - 1)));
			glVertex3f(p[cur]->getPosX(), p[cur]->getPosY(), p[cur]->getPosZ());
			glTexCoord2f((x + 1) / ((double)width - 1), 1 - (y / ((double)height - 1)));
			glVertex3f(p[right]->getPosX(), p[right]->getPosY(), p[right]->getPosZ());
			glTexCoord2f(x / ((double)width - 1), 1 - ((y + 1) / ((double)height - 1)));
			glVertex3f(p[up]->getPosX(), p[up]->getPosY(), p[up]->getPosZ());
			glEnd();
		}
	}
}


GLuint mass_cloth::loadTex(char *texfile) {
	GLuint texture = 0;

	FILE *fp = fopen(texfile, "rb");
	if (!fp) {
		printf("ERROR : No %s.\n fail to bind %d\n", texfile, texture);  return false;
	}

	int width, height, channel;
	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 4);
	fclose(fp);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	return texture;
}