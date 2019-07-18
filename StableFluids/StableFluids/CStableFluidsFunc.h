#include <GLFW\glfw3.h>
#include <iostream>

#define N 300
#define IX(i, j) ((i)+(N+2)*(j))
//#define SIZE ((N+2)*(N+2))
#define SWAP(x0, x) {float *tmp = x0; x0 = x; x = tmp;}

class StableFluidsFunc
{
public:
	int SIZE = (N + 2)*(N + 2);
	StableFluidsFunc();
	~StableFluidsFunc();

	float* u = new float[SIZE];
	float* v = new float[SIZE];
	float* u_prev = new float[SIZE];
	float* v_prev = new float[SIZE];
	float* dens = new float[SIZE];
	float* dens_prev = new float[SIZE];
	
	//void update(float* ptr, float* vertices, int count);
	void update(float* ptr, float* vertices);
	void init();
	void sourcing();
	void addvelocity();
	void update2(float * ptr, float * Array);//for test

private:
	float dt = 0.1f;
	float diff = 0.00001f;
	float visc = 0.000000f;

	bool isFirst = true;

	void add_source(float* x, float* s);
	void diffuse(int b, float * x, float* x0);
	void advect(int b, float * d, float * d0, float* u, float* v);
	void dens_step(float * x, float * x0, float * u, float * v);
	void vel_step(float * u, float* v, float * u0, float* v0);
	void project(float* u, float*v, float* p, float* div);
	void set_bnd(int b, float * x);
};