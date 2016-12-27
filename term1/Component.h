#pragma once

#include "vector.h"
#include <vector>

using namespace std;

class Vertex {
public:
	VECTOR pos;
	VECTOR normal;

	Vertex() {

	}

	Vertex(float _x, float _y, float _z) {
		pos.x = _x;
		pos.y = _y;
		pos.z = _z;
	}
};

class Face {
public:
	vector<VECTOR> vertexID;
	VECTOR normal;

	Face() {

	}

	Face(Vertex v1, Vertex v2, Vertex v3) {
		vertexID.push_back(v1.pos);
		vertexID.push_back(v2.pos);
		vertexID.push_back(v3.pos);
	}
};
