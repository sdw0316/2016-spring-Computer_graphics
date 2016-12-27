#include <Windows.h>
#include <iostream>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "mesh.h"
#include "math.h"
#include "Matrix.h"

#define TRUE 1
#define FALSE 0
#define M_PI 3.14159265358979323846
using namespace std;

Mesh tetra;

Vertex v1(3.0f, 0.0f, 0.0f);
Vertex v2(0.0f, 5.0f, 0.0f);
Vertex v3(-3.0f,0.0f, 0.0f);
Vertex v4(0.0f, 2.0f, 5.0f);

Face f1(v1, v3, v2);//red
Face f2(v1, v2, v4);//green
Face f3(v1, v4, v3);//blue
Face f4(v2, v3, v4);//yellow

int flag1 = FALSE;//2번문제
int flag2 = FALSE;//4번문제

double deg = 20.0f;

/*임의의 회전 축 n1, n2*/
VECTOR n1;
VECTOR n2;

float color[5][3] = {
	{255.0,0.0,0.0},
	{0.0,255.0,0.0},
	{0.0,0.0,255.0},
	{255.0,255.0,0},
	{255.0,255.0,255.0}
};

char face[4][10] = {
	"red",
	"green",
	"blue",
	"yellow"
};

void init() {
	tetra.vertexArray.push_back(v1);
	tetra.vertexArray.push_back(v2);
	tetra.vertexArray.push_back(v3);
	tetra.vertexArray.push_back(v4);
	tetra.faceArray.push_back(f1);
	tetra.faceArray.push_back(f2);
	tetra.faceArray.push_back(f3);
	tetra.faceArray.push_back(f4);
}

void vecTransform(MATRIX m, VECTOR *v) {
	MATRIX matV(4, 1);
	matV.ele[0][0] = (*v).x;
	matV.ele[1][0] = (*v).y;
	matV.ele[2][0] = (*v).z;
	matV.ele[3][0] = 1.0f;

	matV = m.Multiply(matV);
	(*v).x = matV.ele[0][0];
	(*v).y = matV.ele[1][0];
	(*v).z = matV.ele[2][0];
}

void Render()
{
	unsigned int i;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();	
	gluLookAt(0.0f, 0.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// 여기에 코드를 작성하십시오.
	/*make tetraheadron*/


	/*처음에 삼각뿔 그리는 부분
	problem 1 정답
	*/
	glBegin(GL_TRIANGLES);
		for (i = 0; i < tetra.faceArray.size();i++) {
			glColor3fv(color[i]);
			VECTOR pos1 = tetra.faceArray[i].vertexID[0];
			VECTOR pos2 = tetra.faceArray[i].vertexID[1];
			VECTOR pos3 = tetra.faceArray[i].vertexID[2];
			glVertex3f(pos1.x, pos1.y, pos1.z);
			glVertex3f(pos2.x, pos2.y, pos2.z);
			glVertex3f(pos3.x, pos3.y, pos3.z);
		}
	glEnd();

	/*
	make faceNormal
	만약 flag1(normal vector 표현) on이면 normal vector 그림
	problem 2 정답
	*/
	if (flag1 == TRUE) {

		glLineWidth(2.0);
		glBegin(GL_LINES);
		for (i = 0; i < tetra.faceArray.size(); i++) {
			glColor3fv(color[i]);
			Face f = tetra.faceArray[i];
			VECTOR pos1 = tetra.faceArray[i].vertexID[0];
			VECTOR pos2 = tetra.faceArray[i].vertexID[1];
			VECTOR pos3 = tetra.faceArray[i].vertexID[2];
			float x = (pos1.x + pos2.x + pos3.x) / 3.0f;
			float y = (pos1.y + pos2.y + pos3.y) / 3.0f;
			float z = (pos1.z + pos2.z + pos3.z) / 3.0f;

			glVertex3f(x, y, z);
			glVertex3f(0.4*f.normal.x, 0.4*f.normal.y, 0.4*f.normal.z);
		}
		glEnd();
	}

	/*
	make one point(4,4,4) outof the Tetraheadron
	*/
	if (flag2 == TRUE) {
		glPointSize(3.0);
		glBegin(GL_POINTS);
		glColor3fv(color[4]);
		glVertex3f(4.0, 4.0, 4.0);
		glEnd();
	}

	/*arbitrary axis*/
	glLineWidth(2.0);
	glColor3f(255.0,255.0,255.0);
	glBegin(GL_LINES);
		glVertex3f(n1.x, n1.y, n1.z);
		glVertex3f(n2.x, n2.y, n2.z);
	glEnd();
	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, w/h, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void Mouse(int mouse_event, int state, int x, int y)
{
	switch(mouse_event)
	{
		case GLUT_LEFT_BUTTON:
			break;
		case GLUT_MIDDLE_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			break;
		default:
			break;		
	}
	glutPostRedisplay();
}


void Motion(int x, int y)
{
	
	glutPostRedisplay();
}


void Keyboard(unsigned char key, int x, int y)
{
	unsigned int i;
	double rad = deg*(M_PI / 180.0);//버튼 누를때 마다 라디안값 계속 증가

	/*x회전 행렬*/
	MATRIX rotX(4, 4);
	rotX.ele[0][0] = 1.0;
	rotX.ele[1][1] = cos(rad); rotX.ele[1][2] = -sin(rad);
	rotX.ele[2][1] = sin(rad); rotX.ele[2][2] = cos(rad);
	rotX.ele[3][3] = 1.0;

	/*y회전 행렬*/
	MATRIX rotY(4, 4);
	rotY.ele[0][0] = cos(rad); rotY.ele[0][2] = -sin(rad);
	rotY.ele[1][1] = 1.0;
	rotY.ele[2][0] = sin(rad); rotY.ele[2][2] = cos(rad);
	rotY.ele[3][3] = 1.0;

	/*z회전 행렬*/
	MATRIX rotZ(4, 4);
	rotZ.ele[0][0] = cos(rad); rotZ.ele[0][1] = -sin(rad);
	rotZ.ele[1][0] = sin(rad); rotZ.ele[1][1] = cos(rad);
	rotZ.ele[2][2] = 1.0;
	rotZ.ele[3][3] = 1.0;

	switch(key)
	{
		case 'q':
		case VK_ESCAPE:
			exit(0);
		break;

		/*normal벡터 그리는 switch on*/
		case '1':
			flag1 = -(flag1 - 1);
			break;

		/*
		각 면을 구성하는 두 벡터의 관계 계산
		problem3 정답
		*/
		case '2':
			for (i = 0; i < tetra.faceArray.size(); i++) {
				Face *f = &(tetra.faceArray[i]);
				VECTOR n1 = (*f).vertexID[1] - (*f).vertexID[0];
				VECTOR n2 = (*f).vertexID[2] - (*f).vertexID[0];
				float result = n1.InnerProduct(n2);
				float degree = acos(result / (n1.Magnitude()*n2.Magnitude()));
				degree *= (180.0f / M_PI);
				printf("%s의 두 vector 관계 : ", face[i]);
				if (result == 0)
					printf("직각 방향\n");
				else if (result > 0) {
					printf("%f도 (예각)\n",degree);
				}
				else if (result < 0) {
					printf("%f도 (둔각)\n", degree);
				}
			}
			break;

		/*
		flag3(외부의 점 표현) on/off
		그후 각 평면과 점의 관계 계산
		problem 4번 답
		*/
		case '3':
			if (flag2 == FALSE) {
				for (i = 0; i < tetra.faceArray.size(); i++) {
					Face *f = &(tetra.faceArray[i]);
					VECTOR p1 = (*f).vertexID[1];
					VECTOR p2(4.0, 4.0, 4.0);
					VECTOR n = (*f).normal;
					float d = p1.InnerProduct(n);
					float result = p2.InnerProduct(n) - d;

					printf("%s와 점의 관계 : ", face[i]);
					if (result > 0)
						printf("positive\n");
					else if (result < 0)
						printf("negative\n");
					else
						printf("in plane\n");
				}
				flag2 = TRUE;
			}
			else
				flag2 = FALSE;
			break;

		/*
		x,y,z 축 회전
		problem 5 정답.
		*/
		case 'x':
			/*vertex 죄다 회전*/
			for (i = 0; i < tetra.vertexArray.size(); i++) {
				Vertex *v = &(tetra.vertexArray[i]);
				//printf("%f %f %f\n", (*v).pos.x, (*v).pos.y, (*v).pos.z);
				vecTransform(rotX, &(*v).pos);
				//printf("%f %f %f\n", (*v).pos.x, (*v).pos.y, (*v).pos.z);
			}
			/*회전 된 vertex로 */
			for (i = 0; i < tetra.faceArray.size(); i++) {
				Face *f = &(tetra.faceArray[i]);
				for (int j = 0; j < 3; j++) {
					VECTOR *v = &((*f).vertexID[j]);
					vecTransform(rotX, v);
				}
			}
			break;
		case 'y':
			/*vertex 죄다 회전*/
			for (i = 0; i < tetra.vertexArray.size(); i++) {
				Vertex *v = &(tetra.vertexArray[i]);
				//printf("%f %f %f\n", (*v).pos.x, (*v).pos.y, (*v).pos.z);
				vecTransform(rotY, &(*v).pos);
				//printf("%f %f %f\n", (*v).pos.x, (*v).pos.y, (*v).pos.z);
			}
			/*face를 구성하는 vertex회전*/
			for (i = 0; i < tetra.faceArray.size(); i++) {
				Face *f = &(tetra.faceArray[i]);
				for (int j = 0; j < 3; j++) {
					VECTOR *v = &((*f).vertexID[j]);
					vecTransform(rotY, v);
				}
			}
			break;

		/*
		정해놓은 한 축 기준으로 회전
		*/
		case 'z':
			/*vertex 죄다 회전*/
			for (i = 0; i < tetra.vertexArray.size(); i++) {
				Vertex *v = &(tetra.vertexArray[i]);
				//printf("%f %f %f\n", (*v).pos.x, (*v).pos.y, (*v).pos.z);
				vecTransform(rotZ, &(*v).pos);
				//printf("%f %f %f\n", (*v).pos.x, (*v).pos.y, (*v).pos.z);
			}
			/*face를 구성하는 vertex회전*/
			for (i = 0; i < tetra.faceArray.size(); i++) {
				Face *f = &(tetra.faceArray[i]);
				for (int j = 0; j < 3; j++) {
					VECTOR *v = &((*f).vertexID[j]);
					vecTransform(rotZ, v);
				}
			}
			break;

		case '5':
			double a = n2.x - n1.x;
			double b = n2.y - n1.y;
			double c = n2.z - n1.z;
			double d = (sqrt(b*b + c*c));
			double l = sqrt(a*a + b*b + c*c);

			printf("v1.x:%f v1.y:%f v1.z:%f\nv2.x:%f v2.y:%f v2.z:%f\n\n", n1.x, n1.y, n1.z, n2.x, n2.y, n2.z);
			//printf("%f %f %f %f %f\n\n", a, b, c, d, l);
			//printf("%f %f\n", d / l, a / l);
			MATRIX T(4, 4);
			T.ele[0][0] = 1.0; T.ele[0][3] = -n1.x;
			T.ele[1][1] = 1.0; T.ele[1][3] = -n1.y;
			T.ele[2][2] = 1.0; T.ele[2][3] = -n1.z;
			T.ele[3][3] = 1.0;

			MATRIX IT(4, 4);
			IT.ele[0][0] = 1.0; IT.ele[0][3] = n1.x;
			IT.ele[1][1] = 1.0; IT.ele[1][3] = n1.y;
			IT.ele[2][2] = 1.0; IT.ele[2][3] = n1.z;
			IT.ele[3][3] = 1.0;

			MATRIX rotXa(4, 4);
			rotXa.ele[0][0] = 1.0;
			rotXa.ele[1][1] = c/d; rotXa.ele[1][2] = -(b/d);
			rotXa.ele[2][1] = b/d; rotXa.ele[2][2] = c/d;
			rotXa.ele[3][3] = 1.0;

			MATRIX IrotXa(4, 4);
			IrotXa.ele[0][0] = 1.0;
			IrotXa.ele[1][1] = c/d; IrotXa.ele[1][2] = (b/d);
			IrotXa.ele[2][1] = -(b/d); IrotXa.ele[2][2] = c/d;
			IrotXa.ele[3][3] = 1.0;

			MATRIX rotYa(4, 4);
			rotYa.ele[0][0] = d/l; rotYa.ele[0][2] = -(a/l);
			rotYa.ele[1][1] = 1.0;
			rotYa.ele[2][0] = a/l; rotYa.ele[2][2] = d/l;
			rotYa.ele[3][3] = 1.0;

			MATRIX IrotYa(4, 4);
			IrotYa.ele[0][0] = d / l; IrotYa.ele[0][2] = (a / l);
			IrotYa.ele[1][1] = 1.0;
			IrotYa.ele[2][0] = -(a / l); IrotYa.ele[2][2] = d / l;
			IrotYa.ele[3][3] = 1.0;

			for (i = 0; i < tetra.vertexArray.size(); i++) {
				Vertex *v = &(tetra.vertexArray[i]);
				//printf("%f %f %f\n", (*v).pos.x, (*v).pos.y, (*v).pos.z);
				vecTransform(T, &(*v).pos);
				vecTransform(rotXa, &(*v).pos);
				vecTransform(rotYa, &(*v).pos);
				vecTransform(rotZ, &(*v).pos);
				vecTransform(IrotYa, &(*v).pos);
				vecTransform(IrotXa, &(*v).pos);
				vecTransform(IT, &(*v).pos);
				//printf("%f %f %f\n", (*v).pos.x, (*v).pos.y, (*v).pos.z);
			}
			/*face를 구성하는 vertex회전*/
			for (i = 0; i < tetra.faceArray.size(); i++) {
				Face *f = &(tetra.faceArray[i]);
				for (int j = 0; j < 3; j++) {
					VECTOR *v = &((*f).vertexID[j]);
					//if (v->x == n1.x&& v->y == n1.y && v->z == n1.z)
					//		continue;
					//if (v->x == n2.x&& v->y == n2.y && v->z == n2.z) 
					//		continue;
					vecTransform(T, v);
					vecTransform(rotXa, v);
					vecTransform(rotYa, v);
					vecTransform(rotZ, v);
					vecTransform(IrotYa, v);
					vecTransform(IrotXa, v);
					vecTransform(IT, v);
				}
			}
			printf("v1.x:%f v1.y:%f v1.z:%f\nv2.x:%f v2.y:%f v2.z:%f\n\n", n1.x, n1.y, n1.z, n2.x, n2.y, n2.z);

			break;
	}
	glutPostRedisplay();
}

void SpecialKey(int key, int x, int y)
{
	switch(key) 
	{
		case GLUT_KEY_F1:
			int i;
			printf("총 face 개수 : %d\n", tetra.faceArray.size());
			for (i = 0; i < tetra.vertexArray.size(); i++) {
				MATRIX v(3, 1);

				//printf("vertex %d : %f %f %f \n", i, v.pos.x, v.pos.y, v.pos.z);
			}
			break;
	}
	glutPostRedisplay();
}

void Idle(void)
{
	/*compute normal vector*/
	for (int i = 0; i < tetra.faceArray.size(); i++) {
		Face *f = &(tetra.faceArray[i]);

		VECTOR n1 = (*f).vertexID[1] - (*f).vertexID[0];
		VECTOR n2 = (*f).vertexID[2] - (*f).vertexID[0];
		(*f).normal = n1.CrossProduct(n2);
	}
	n1 = tetra.faceArray[0].vertexID[0];
	n2 = tetra.faceArray[0].vertexID[2];

	glutPostRedisplay();
}
int main(int argc, char ** argv)
{
	init();
	printf("F1 - show information\n");
	printf("1. 1 누르면 현재 face의 normal vector 표시\n");
	printf("2. 2 누르면 각 face edge의 각 계산\n");
	printf("3. 3 누르면 (4.0, 4.0, 4.0)과 각 면과의 관계 계산\n");
	printf("4. x, y, z 누르고 있으면 그 축 기준으로 회전\n");
	printf("5. 5 누르고 있으면, 삼각뿔의 한 edge 기준 회전\n");
	printf("---------------------------------------------\n");
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	
	glutInitWindowSize(400,400);
	glutInitWindowPosition(0, 0);

	//Console Window 위치 변경
	HWND hWnd = ::FindWindow("ConsoleWindowClass" ,NULL );
	::MoveWindow(hWnd , 420 , 0 , 500 , 500, TRUE );

	//Window 제목은 학번과 학생 이름으로 작성하십시오.
	glutCreateWindow("2014210056_송도원");

	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);

	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey);
	glutIdleFunc(Idle);
	glutMainLoop();

	return 0;
}

