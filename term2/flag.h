#pragma once

#include "particle.h"

class mass_pole {
public:
	Vec3 center;	//깃대 박는 위치
	Vec3 direction;	//깃대가 선 방향
	double radius;
	double height;
public:
	mass_pole() {
		center = Vec3(0.0, 0.0, 0.0);
		direction = Vec3(0.0, 0.0, 1.0);
		radius = 0.15;
		height = 10.0;
	}
	mass_pole(Vec3 v1,Vec3 v2, double r, double h) {
		center = v1;
		direction = v2;
		radius = r;
		height = h;
	}
	~mass_pole() {

	}
public:
	void changeCenter(Vec3 v) {
		center = v;
	}
	void changeDirection(Vec3 v) {
		direction = v;
	}
	void draw();

};
class mass_flag {
public:
	mass_pole *pole;
	mass_cloth *cloth;
	Vec3 base;	//깃발 박는 위치
	Vec3 direction;	//깃발 세우는 방향
public:
	mass_flag() {
		base = Vec3(0.0, 0.0, 0.0);
		direction = Vec3(0.0, 1.0, 0.0);
	}
	mass_flag(double bx, double by, double bz, double dx, double dy, double dz){
		base = Vec3(bx, by, bz);
		direction = Vec3(dx, dy, dz);
	}
	~mass_flag() {

	}
public:
	void init() {
		mass_pole *newpole = new mass_pole();
		pole = newpole;
		mass_cloth *newcloth = new mass_cloth();
		cloth = newcloth;
		pole->changeCenter(base);
		pole->changeDirection(direction);

		cloth->changeBase(base);
		cloth->changeDirection(direction);

		cloth->init();
	}
	void draw();
	void changeBase(Vec3 v) {
		base = v;
		pole->changeCenter(base);
		cloth->changeBase(base);
	}
	void changeDir(Vec3 v) {
		direction = v;
		pole->changeDirection(v);
		cloth->changeDirection(v);
	}
	void rotateFlag(double degree) {
		Vec3 temp = direction;
		changeDir(direction.rotateZ(degree));
		changeBase(base.rotateZ(degree));
		cloth->rotate(temp);
	}
};