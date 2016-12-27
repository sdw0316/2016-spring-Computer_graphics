#pragma once

#include "Vector.h"
using namespace ::std;

class Face {
public:
	Vec3 vertexID;
	Vec3 normal;
public:
	Face() {

	}
	~Face() {
	}
};

class Mesh {
public:
	vector<Vec3> vertices;
	vector<Face> faces;
public:
	Mesh() {

	}
	~Mesh() {

	}
public:
	void loadObj(char *obj);
	void draw();
	void faceNormal();
};