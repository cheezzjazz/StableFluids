#include <GLFW\glfw3.h>

#define N 100
#define IX(i, j) ((i)+(N+2)*(j))
#define SIZE ((N+2)*(N+2))
#define SWAP(x0, x) {float *tmp = x0; x0 = x; x = tmp;}

class StableFluidsFunc
{
public:
	StableFluidsFunc();

	float u[SIZE];
	float v[SIZE];
	float u_prev[SIZE];
	float v_prev[SIZE];
	float dens[SIZE];
	float dens_prev[SIZE];
	
	//void update(float* ptr, float* vertices, int count);
	void update(float* ptr, float* vertices);
	void init();
	void sourcing();

private:
	float dt = 0.1f;
	float diff = 0.00001f;
	float visc = 0.000000f;

	void add_source(float* x, float* s);
	void diffuse(int b, float * x, float* x0);
	void advect(int b, float * d, float * d0, float* u, float* v);
	void dens_step(float * x, float * x0, float * u, float * v);
	void vel_step(float * u, float* v, float * u0, float* v0);
	void project(float* u, float*v, float* p, float* div);
	void set_bnd(int b, float * x);
};