#include "spirograph.h"
#include <cmath>

#define _USE_MATH_DEFINES
#include <math.h>

void TestFillVertices(float* _vertices, int count);
void TestFillColors(float* colors, int count);

Spirograph::Spirograph() : Spirograph(1.0)
{
}

Spirograph::Spirograph(float screen_ratio) 
{
	//TODO: ensure that the actual vertex size doesn't exceed the array capacity
	current_vertex_count = 0;
	current_max_vertex_count = 500;
	center_x = 0.0f;
	center_y = 0.0f;
	epi_flag = false;
	phi_0 = 0.0;
	psi_0 = 0.0;
	phi = phi_0;
	psi = psi_0;
	delta_phi = 0.05;
	base_r = 0.4f;
	rolling_r = 0.35f;
	pen_r = 0.1f;
	delta_psi = epi_flag ? delta_phi * (1 + base_r / rolling_r) : delta_phi * (1 - base_r / rolling_r);
	vertices = new float[MAX_VERTEX_CAPACITY * VERTEX_SIZE];
	colors = new float[4 * current_max_vertex_count];
	this->screen_ratio = screen_ratio;
	TestFillColors(colors, current_max_vertex_count);
}

Spirograph::~Spirograph()
{
	if (vertices)
		delete[] vertices;
}


/*********************************************
* count - the current vertex count will be returned here
*********************************************/
float* Spirograph::GetVertices(unsigned int* count)
{
	*count = current_vertex_count;
	
	//TODO: return as array of constants
	return vertices;
}


void Spirograph::Start()
{
	phi = phi_0;
	psi = psi_0;
	current_vertex_count = 1;
	next_vertex_index = 0;
}

void Spirograph::CalculateFrame()
{
	//TODO: calculate hypocycloid too

	//calculating the current vertex
	if (epi_flag)
	{
		vertices[next_vertex_index * VERTEX_SIZE] = center_x + (base_r + rolling_r) * cos(phi) + pen_r * cos(psi);
		vertices[next_vertex_index * VERTEX_SIZE + 1] = (center_y + (base_r + rolling_r) * sin(phi) + pen_r * sin(psi)) / screen_ratio;
	}
	else
	{
		vertices[next_vertex_index * VERTEX_SIZE] = center_x + (base_r - rolling_r) * cos(phi) + pen_r * cos(psi);
		vertices[next_vertex_index * VERTEX_SIZE + 1] = (center_y + (base_r - rolling_r) * sin(phi) + pen_r * sin(psi)) / screen_ratio;
	}

	int destindex = next_vertex_index * VERTEX_SIZE + 2;
	int srcindex = 0;
	for(int i = 0; i < current_vertex_count; i++)
	{
		srcindex = i * 4;
		vertices[destindex] = colors[srcindex];
		vertices[destindex + 1] = colors[srcindex + 1];
		vertices[destindex + 2] = colors[srcindex + 2];

		destindex -= VERTEX_SIZE;
		if (destindex < 0)
		{
			destindex = (current_max_vertex_count - 1) * VERTEX_SIZE + 2;
		}
	}
	
	//calculating the next vertex index
	next_vertex_index++;
	if (next_vertex_index >= current_max_vertex_count)
		next_vertex_index = 0;


	if (current_vertex_count < current_max_vertex_count)
		current_vertex_count++;

	phi += delta_phi;
	psi += delta_psi;

	if (phi > 2 * M_PI)
		phi -= (2 * M_PI);
	else if (phi < -2 * M_PI)
		phi += (2 * M_PI);

	if (psi > 2 * M_PI)
		psi -= (2 * M_PI);
	else if (psi < -2 * M_PI)
		psi += (2 * M_PI);
}




void TestFillVertices(float* _vertices, int count)
{
	float start = -0.8f, end = 0.8f;
	float delta = (end - start) / (count - 1);

	for (int i = 0; i < count; i++)
	{
		_vertices[i * 2] = start + delta * i;
		_vertices[i * 2 + 1] = 0.0f;
	}
}

void TestFillColors(float* colors, int count)
{
	for(int i = 0; i < count; i++)
	{
		float c = 1.0f - (float)i / (float)count;

		colors[4 * i] = c;
		colors[4 * i + 1] = c;
		colors[4 * i + 2] = c;
		colors[4 * i + 3] = 1.0f;
	}
}