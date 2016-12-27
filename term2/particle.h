#pragma once

#include "Vector.h"
#include "stb_image.h"
#include <GL/glut.h>

#define GRID_X 40
#define GRID_Y 40
#define DX 0.10
#define DY 0.10
#define PI 3.141592

using namespace ::std;

class particle{
public:
	double	mass;
	Vec3 force;
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	Vec3 normal;
	double friction;	//air friction
	
	bool fixed = false;

public:
	particle(void)
	{
	}
	particle(Vec3 init_pos)
	{
		pos = init_pos;
		mass = 1.0;
		friction = 0.005;
	}
	particle(Vec3 init_pos, Vec3 init_vel)
	{
		pos = init_pos;
		vel = init_vel;
		mass = 1.0;
		friction = 0.005;
	}
	~particle(void)
	{
	}
public:
	double getPosX(){ 
		return pos.getX(); 
	}
	double getPosY() {
		return pos.getY();
	}
	double getPosZ() {
		return pos.getZ();
	}
	void integrate(double dt, Vec3 gravity){
		if (!fixed)
		{
			//air friction
			force += vel * friction * (-1);

			force += gravity * mass;
			accel = force / mass;
			vel += accel*dt;
			pos += vel*dt;
		}
		force.x = force.y = force.z = 0.0;
	}

	void add_force(Vec3 ext_force)
	{
		force += ext_force;
	}

	void draw();
};

class mass_spring {
public:
	double		spring_coef;
	double		damping_coef;
	particle	*p1;
	particle	*p2;
	double		len;

public:
	mass_spring() {
		spring_coef = 100.0;
		damping_coef = 1.0;
		p1 = p2 = NULL;
		init();
	}
	mass_spring(particle *p1, particle *p2)
	{
		spring_coef = 100.0;
		damping_coef = 1.0;
		this->p1 = p1;
		this->p2 = p2;
		init();
	}

public:
	void init() {
		Vec3 dp = (p2->pos - p1->pos);

		len = sqrt(dp.x*dp.x + dp.y*dp.y);
	}
	/*idle*/
	void integrate(double dt, Vec3 gravity)
	{
		Vec3 dp = p2->pos - p1->pos;
		double dx = dp.length() - len;
		Vec3 dv = p2->vel - p1->vel;

		Vec3 hook_force = dp * (dx * spring_coef);
		Vec3 damping_force = dv * damping_coef;

		p2->add_force(damping_force * -1);
		p2->add_force(hook_force * -1);

		p1->add_force(damping_force);
		p1->add_force(hook_force);

		p2->integrate(dt, gravity);
		p1->integrate(dt, gravity);
	}
	/*입자간의 내력*/
	void internal_force(double dt)
	{
		Vec3 dp = p2->pos - p1->pos;
		double dx = dp.length() - len;
		Vec3 dv = p2->vel - p1->vel;
		Vec3 hook_force;
		if (dx > DX) {
			hook_force = dp * 1000;
		}
		else {
			hook_force = dp * (dx * spring_coef);
		}
		Vec3 damping_force = dv * damping_coef;

		p2->add_force(damping_force * -1);
		p2->add_force(hook_force * -1);

		p1->add_force(damping_force);
		p1->add_force(hook_force);
	}
	/*외력 작용시*/
	void add_force(Vec3 ext_force)
	{
		p1->force += ext_force;
		p2->force += ext_force;
	}

	void draw();
};


class mass_cloth {
public:
	vector<particle *> p;
	vector<mass_spring *> s;

	Vec3 base;	//중심 좌표
	Vec3 direction;	//flag 방향
	int width, height;
	double dx, dy;
	int iteration_n = 10;

	GLuint texture;

public:
	mass_cloth()
	{
		width = GRID_X;
		height = GRID_Y;
		dx = DX;
		dy = DY;
		texture = NULL;

		base = Vec3(0.0, 10.0 - dy*height, 0.0);
		direction = Vec3(0.0, 1.0, 0.0);
	}
	mass_cloth(Vec3 v1, Vec3 v2) {
		width = GRID_X;
		height = GRID_Y;
		dx = DX;
		dy = DY;
		texture = NULL;

		base = v1;
		direction = v2;
		base.y += 10.0 - dy*height;
	}
	~mass_cloth()
	{
		//asdfasdf
		for (int i = 0; i < p.size(); i++) {
			delete p[i];
		}
		for (int i = 0; i < s.size(); i++) {
			delete s[i];
		}
		p.clear();
		s.clear();
	}

public:
	void init()
	{
		Vec3 normal(0.0, 1.0, 0.0);
		double degree = acos(normal.innerProduct(direction) / (normal.length()*direction.length()));	//현재 direction과 normal간의 각도
		if (direction.x < 0)
			degree = 2 * PI - degree;
		// init particle
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				/*반복문 돌면서 해당 x, y 위치에 있는 particle 생성*/
				Vec3 pos(base.x + x*dx, base.y + y*dy, base.z + 0.0);
				pos = pos.rotateZ(-degree);
				particle *newparticle = new particle(pos);
				/*pole 에 붙어있는 입자들은 fixed*/
				if (x == 0)
					newparticle->fixed = true;
				p.push_back(newparticle);
			}
		}

		//init springs
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				/*반복문 돌면서 해당 particle의 왼쪽, 아래에 있는 particle과 연결하는 스프링 생성*/
				int cur = y*(width)+x;
				int left = cur + 1;
				int up = cur + width;
				int ld = up + 1;
				int rd = up - 1;
				if (x + 1 < width) {
					mass_spring *newspring = new mass_spring(p[cur], p[left]);
					s.push_back(newspring);
				}

				if (y + 1 < height) {
					mass_spring *newspring = new mass_spring(p[cur], p[up]);
					s.push_back(newspring);
				}

				if (y + 1<height && x + 1 < width) {
					mass_spring *newspring = new mass_spring(p[cur], p[ld]);
					newspring->spring_coef = 10.0;
					s.push_back(newspring);
				}
				if (y + 1<height && x - 1 >= 0) {
					mass_spring *newspring = new mass_spring(p[cur], p[rd]);
					newspring->spring_coef = 10.0;
					s.push_back(newspring);
				}
				/*pole 에 붙어있는 입자들은 fixed*/
			}
		}

	}

	void add_force(Vec3 ext_force) {
		for (int i = 0; i < p.size(); i++) {
			p[i]->add_force(ext_force);
		}
	}

	void integrate(double dt, Vec3 gravity)
	{
		/*입자들이 받는 중력*/
		for (int i = 0; i < p.size(); i++) {
			p[i]->add_force(gravity);
		}
		/*입자들간의 내력으로 인한 영향*/
		for (int iter = 0; iter < iteration_n; iter++)
		{
			for (int i = 0; i < s.size(); i++) {
				mass_spring *ep = s[i];
				ep->internal_force(dt);
			}
			for (int i = 0; i < p.size(); i++) {
				particle *ep = p[i];
				ep->integrate(dt, Vec3(0, 0, 0));
			}
		}
		computenormal();
	}

	void computenormal() {
		//compute particle normal
		for (int i = 0; i < p.size(); i++) {
			/*normal을 구하기 위한 5개의 파티클*/
			int cur = i;
			int left = cur - 1;
			int right = cur + 1;
			int up = cur + width;
			int down = cur - width;

			int x = cur % width;
			int y = cur / width;

			int num = 0;
			Vec3 result = Vec3(0.0, 0.0, 0.0);

			if (x != 0 && y < height - 1) {
				/*compute v1*/
				Vec3 p1 = p[up]->pos - p[cur]->pos;
				Vec3 p2 = p[left]->pos - p[cur]->pos;
				Vec3 v = p1.crossProduct(p2);
				result += v;
				num++;
			}
			if (x != 0 && y > 0) {
				/*compute v2*/
				Vec3 p1 = p[left]->pos - p[cur]->pos;
				Vec3 p2 = p[down]->pos - p[cur]->pos;
				Vec3 v = p1.crossProduct(p2);
				result += v;
				num++;
			}
			if (x < width - 1 && y > 0) {
				/*compute v3*/
				Vec3 p1 = p[down]->pos - p[cur]->pos;
				Vec3 p2 = p[right]->pos - p[cur]->pos;
				Vec3 v = p1.crossProduct(p2);
				result += v;
				num++;
			}
			if (x < width - 1 && y < height - 1) {
				/*compute v4*/
				Vec3 p1 = p[right]->pos - p[cur]->pos;
				Vec3 p2 = p[up]->pos - p[cur]->pos;
				Vec3 v = p1.crossProduct(p2);
				result += v;
				num++;
			}

			result = result / num;
			p[cur]->normal = result / result.dist();
		}
	}

	void airForce(Vec3 external) {
		for (int i = 0; i < p.size(); i++) {
			double coef = p[i]->normal.innerProduct(external);
			Vec3 force = (p[i]->normal)*coef;
			p[i]->add_force(force);
		}
	}

	GLuint loadTex(char *texfile);
	void draw();
	void Texdraw();
	void changeBase(Vec3 v) {
		base = v;
		base.y = 10 - height*dy;
	}

	void changeDirection(Vec3 v) {
		direction = v;
	}

	void rotate(Vec3 v) {
		Vec3 normal = v;
		double degree = acos(normal.innerProduct(direction) / (normal.length()*direction.length()));	//현재 direction과 normal간의 각도
		if (v.x- direction.x < 0)
			degree = 2 * PI - degree;

		for (int i = 0; i < p.size(); i+=width) 
			p[i]->pos = p[i]->pos.rotateZ(degree);
	}
};