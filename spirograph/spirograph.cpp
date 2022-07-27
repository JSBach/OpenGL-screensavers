#include "spirograph.h"
#include <exception>
#include <cmath>

#define _USE_MATH_DEFINES
#include <math.h>

void TestFillVertices(float* _vertices, int count);
void TestFillColors(float* colors, int count);

float gradient_scheme_0_stops[6] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; //red to dark
float gradient_scheme_1_stops[6] = { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f }; //green to dark
float gradient_scheme_2_stops[6] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }; //blue to dark

float gradient_scheme_0_ratios[1] = { 1.0f };
float gradient_scheme_1_ratios[1] = { 1.0f };
float gradient_scheme_2_ratios[1] = { 1.0f };

struct GradientScheme
{
	float* stops;
	float* ratios;
	unsigned int count;
};

GradientScheme gradient_scheme_0 = { gradient_scheme_0_stops, gradient_scheme_0_ratios, 1 };
GradientScheme gradient_scheme_1 = { gradient_scheme_1_stops, gradient_scheme_1_ratios, 1 };
GradientScheme gradient_scheme_2 = { gradient_scheme_2_stops, gradient_scheme_2_ratios, 1 };

GradientScheme gradient_schemes[3] = { gradient_scheme_0, gradient_scheme_1, gradient_scheme_2 };

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
	CreateStaticColor(1.0f, 1.0f, 1.0f); //default color
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


void Spirograph::SetColorMode(float r, float g, float b)
{
	CreateStaticColor(r, g, b);
}

void Spirograph::SetColorMode(unsigned int gradient_index)
{
	if(gradient_index <= MAX_GRADIENT_INDEX)
	{
		CreateGradientColor(
			gradient_schemes[gradient_index].stops,
			gradient_schemes[gradient_index].ratios,
			gradient_schemes[gradient_index].count);
	}
}

void Spirograph::CreateStaticColor(float r, float g, float b)
{
	//colors array should be allocated
	if (colors == 0)
		return;

	//just filling in the color array with the given color
	for (int i = 0; i < current_max_vertex_count; i++)
	{
		colors[4 * i] = r;
		colors[4 * i + 1] = g;
		colors[4 * i + 2] = b;
		colors[4 * i + 3] = 1.0f;
	}
}

/************************************************
* CreateGradientColor
* 
* Fills in the color array with linear gradients
* 
* Input
*  - stops: array of color stops (3 floats for r, g and b)
*  - ratios: ratios of color array lengths between adjacent stops
*    . must be positive
*    . no need to sum up to 1 - will be normilized
*  - count: fragment count (if equal to 0 then static color will be created)
*************************************************/
void Spirograph::CreateGradientColor(float* stops, float* ratios, unsigned int count)
{
	//colors array should be allocated
	if (colors == 0)
		return;

	//should we create a static color?
	if (count == 0)
	{
		CreateStaticColor(stops[0], stops[1], stops[2]);
		return;
	}
		

	//otherwise normilizing the ratios
	float ratio_sum = 0.0f;
	float* normalized_ratios = new float[count];

	for (int i = 0; i < count; i++)
	{
		if (ratios[i] <= 0.0f)
			throw new std::exception("gradient fragment length should be greater then zero.");

		ratio_sum += ratios[i];
	}

	for (int i = 0; i < count; i++)
	{
		normalized_ratios[i] = ratios[i] / ratio_sum;
	}

	float length_acc = 0.0f;
	float current_length = 0.0f;
	float current_x = 0.0f;
	float current_r = stops[0];
	float current_g = stops[1];
	float current_b = stops[2];
	float delta_r = (stops[3] - stops[0]) / normalized_ratios[0];
	float delta_g = (stops[4] - stops[1]) / normalized_ratios[0];
	float delta_b = (stops[5] - stops[2]) / normalized_ratios[0];
	int current_stop = 0;

	//calculating the colors
	for (int i = 0; i < current_max_vertex_count; i++)
	{
		//the length corresponding to the current index in the color array
		current_length = (float)i / (float)current_max_vertex_count;

		//the distance from the last stop
		current_x = current_length - length_acc;
		if (current_x >= normalized_ratios[current_stop])
		{
			length_acc += normalized_ratios[current_stop];
			current_stop++;
			current_r = stops[3 * current_stop];
			current_g = stops[3 * current_stop + 1];
			current_b = stops[3 * current_stop + 2];
			delta_r = (stops[3 * (current_stop + 1)] - stops[3 * current_stop]) / normalized_ratios[current_stop];
			delta_g = (stops[3 * (current_stop + 1) + 1] - stops[3 * current_stop + 1]) / normalized_ratios[current_stop];
			delta_b = (stops[3 * (current_stop + 1) + 2] - stops[3 * current_stop + 2]) / normalized_ratios[current_stop];
		}

		colors[4 * i] = current_r + delta_r * current_x;
		colors[4 * i + 1] = current_g + delta_g * current_x;
		colors[4 * i + 2] = current_b + delta_b * current_x;
		colors[4 * i + 3] = 1.0f;
	}

	delete[] normalized_ratios;
}

