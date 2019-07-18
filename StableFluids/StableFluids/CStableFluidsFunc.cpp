#include "CStableFluidsFunc.h"


StableFluidsFunc::StableFluidsFunc()
{
	init();
}

void StableFluidsFunc::init()
{
	for (int i = 0; i < N + 2; i++)
	{
		for (int j = 0; j < N + 2; j++)
		{
			dens[IX(i, j)] = 0.0f;
			u[IX(i, j)] = 0.0f;
			v[IX(i, j)] = 0.0f;

			dens_prev[IX(i, j)] = 0.0f;
			u_prev[IX(i, j)] = 0.0f;
			v_prev[IX(i, j)] = 0.0f;
		}
	}
}

void StableFluidsFunc::sourcing()
{
	int i = N/2+1;
	int j = N;
	for (i = (N / 2) ; i < (N / 2) + 3; i++)
	{
		for(j = N-2; j > N-5; j--)
			dens[IX(i, j)] = 1.0f;
			//u_prev[IX(i, j)] = 3.0f;
			v_prev[IX(i, j)] = -30.0f;
	}
}

//like wind
void StableFluidsFunc::addvelocity()	
{
	int i = N / 2 + 1;
	int j = N-1;
	for (i = (N / 2); i < (N / 2) + 3; i++)
	{
		//u_prev[IX(i, j)] = -10.0f;
		v_prev[IX(i, j )] = -30.0f;
	}
}

void StableFluidsFunc::update2(float* ptr, float* Array)
{
	for (int i = 0; i < SIZE; i++)
	{
		if (*Array <= 0.0f)
			*Array = 0.0f;
		else
			*Array -= 0.05f;

		*ptr = *Array;
		ptr++; Array++;
	}
}

void StableFluidsFunc::update(float * ptr, float * vertices)
{
	if (!ptr || !vertices)
		return;

	vel_step(u, v, u_prev, v_prev);
	dens_step(dens, dens_prev, u, v);

	for (int i = 0; i < SIZE; i++)
	{
		*vertices = dens[i];
		*ptr = *vertices;
		*ptr++; vertices++;
	}
	
}

void StableFluidsFunc::add_source(float * x, float * s)
{
	for (int i = 0; i < SIZE; i++)
	{
		x[i] += dt * s[i];
	}
}

void StableFluidsFunc::diffuse(int b, float * x, float * x0)
{
	float a = dt*diff*N*N;
	//float a = 1.0f;
	for (int k = 0; k < 20; k++)
	{
		for (int i = 1; i <= N; i++)
		{
			for (int j = 1; j <= N; j++)
			{
				x[IX(i, j)] = (x0[IX(i, j)] + a*(x[IX(i-1,j)] + x[IX(i+1, j)] + x[IX(i, j-1)] + x[IX(i, j+1)])) / (1+4*a);
			}
		}
		set_bnd(b, x);
	}
}

void StableFluidsFunc::advect(int b, float * d, float * d0, float * u, float * v)
{
	int i, j, i1, j1, i0, j0;
	float x, y, s0, t0, s1, t1;
	float dt0 = dt*N;
	for (i = 1; i <= N; i++)
	{
		for (j = 1; j <= N; j++)
		{
			x = i - dt0*u[IX(i, j)];
			y = j - dt0*v[IX(i, j)];
			if (x < 0.5f)
				x = 0.5f;
			else if (x > N + 0.5f)
				x = (float)(N + 0.5f);
			i0 = (int)x;
			i1 = i0 + 1;

			if (y < 0.5f)
				y = 0.5f;
			else if (y > N + 0.5f)
				y = N + 0.5f;
			j0 = (int)y;
			j1 = j0 + 1;

			s1 = x - i0;
			s0 = 1 - s1;
			t1 = y - j0;
			t0 = 1 - t1;

			d[IX(i, j)] = s0*(t0*d0[IX(i0, j0)] + t1*d0[IX(i0, j1)]) +
				s1*(t0*d0[IX(i1, j0)] + t1*d0[IX(i1, j1)]);
		}
	}

	set_bnd(b, d);
}
void StableFluidsFunc::dens_step(float * x, float * x0, float * u, float * v)
{
	if (isFirst)
	{
		add_source(x, x0);
		isFirst = false;
	}
	SWAP(x0, x);
	diffuse(0, x, x0);
	SWAP(x0, x);
	advect(0, x, x0, u, v);
}

void StableFluidsFunc::vel_step(float * u, float * v, float * u0, float * v0)
{
	add_source(u, u0); add_source(v, v0);
	SWAP(u0, u); 
	diffuse(1, u, u0);
	SWAP(v0, v);
	diffuse(2, v, v0);
	project(u, v, u0, v0);
	SWAP(u0, u); SWAP(v0, v);
	advect(1, u, u0, u0, v0);
	advect(2, v, v0, u0, v0);
	project(u, v, u0, v0);
}

void StableFluidsFunc::project(float * u, float * v, float * p, float * div)
{
	float h = (float)(1.0f / N);

	int i, j, k;
	for (i = 1; i <= N; i++)
	{
		for (j = 1; j <= N; j++)
		{
			div[IX(i, j)] = -0.5f*h*(u[IX(i+1, j)] - u[IX(i-1, j)] + v[IX(i, j+1)] - v[IX(i, j-1)]);
			p[IX(i, j)] = 0.0f;
		}
	}

	set_bnd(0, div);
	set_bnd(0, p);

	for (k = 0; k < 20; k++)
	{
		for (i = 1; i <= N; i++)
		{
			for (j = 1; j <= N; j++)
			{
				p[IX(i, j)] = (div[IX(i, j)] + p[IX(i-1, j)] + p[IX(i+1, j)] + p[IX(i, j-1)] + p[IX(i, j+1)]) / 4;
			}
		}
		set_bnd(0, p);
	}

	for (i = 1; i <= N; i++)
	{
		for (j = 1; j <= N; j++)
		{
			u[IX(i, j)] -= 0.5f*(p[IX(i + 1, j)] - p[IX(i - 1, j)]) / h;
			v[IX(i, j)] -= 0.5f*(p[IX(i, j + 1)] - p[IX(i, j - 1)]) / h;
		}
	}
	set_bnd(1, u);
	set_bnd(2, v);
}

void StableFluidsFunc::set_bnd(int b, float * x)
{
	for (int i = 1; i <= N; i++)
	{
		x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
		x[IX(N + 1, i)] = b == 1 ? -x[IX(N, i)] : x[IX(N, i)];
		x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
		x[IX(i, N + 1)] = b == 2 ? -x[IX(i, N)] : x[IX(i, N)];
	}
	x[IX(0, 0)] = 0.5f*(x[IX(1, 0)] + x[IX(0, 1)]);
	x[IX(0, N + 1)] = 0.5f*(x[IX(1, N + 1)] + x[IX(0, N)]);
	x[IX(N + 1, 0)] = 0.5f*(x[IX(N, 0)] + x[IX(N + 1, 1)]);
	x[IX(N + 1, N + 1)] = 0.5f*(x[IX(N, N + 1)] + x[IX(N + 1, N)]);
}
