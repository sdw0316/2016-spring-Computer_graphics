#include "windmill.h"
#include <GL/glut.h>

void Mesh::loadObj(char *obj) {
	Vec3 v;
	Face f;
	char dest[1000] = ".\\";
	strcat(dest, obj);

	FILE *fp;
	char ch;
	int endpoint = 1;
	if (fp = fopen(dest, "r")) {

		while (endpoint == 1) {
			endpoint = fscanf(fp, "%c", &ch);
			//if (!endpoint) break;
			if (ch == 'v') {
				fscanf(fp, "%lf", &v.x);
				fscanf(fp, "%lf", &v.y);
				fscanf(fp, "%lf\n", &v.z);
				vertices.push_back(v);
			}
			else if (ch == 'f') {
				fscanf(fp, "%lf", &v.x);
				fscanf(fp, "%lf", &v.y);
				fscanf(fp, "%lf\n", &v.z);
				f.vertexID = v;
				faces.push_back(f);
			}
			else if (ch == '#') {
				while (ch != '\n') {
					if (fscanf(fp, "%c", &ch) == -1) break;
				}
			}
			fflush(stdin);
		}
	}
	faceNormal();

}
void Mesh::draw() {
	for (int i = 0; i < faces.size(); i++) {
		glBegin(GL_TRIANGLES);
			glNormal3f(faces[i].normal.x, faces[i].normal.y, faces[i].normal.z);
			glVertex3f(vertices[faces[i].vertexID.x - 1].x, vertices[faces[i].vertexID.x - 1].y, vertices[faces[i].vertexID.x - 1].z);
			glVertex3f(vertices[faces[i].vertexID.y - 1].x, vertices[faces[i].vertexID.y - 1].y, vertices[faces[i].vertexID.y - 1].z);
			glVertex3f(vertices[faces[i].vertexID.z - 1].x, vertices[faces[i].vertexID.z - 1].y, vertices[faces[i].vertexID.z - 1].z);
		glEnd();
	}
}

void Mesh::faceNormal() {
	for (int i = 0; i < faces.size(); i++) {
		faces[i].normal = (vertices[faces[i].vertexID.z - 1] - vertices[faces[i].vertexID.x - 1]).crossProduct(vertices[faces[i].vertexID.y - 1] - vertices[faces[i].vertexID.x - 1]);
	}
}