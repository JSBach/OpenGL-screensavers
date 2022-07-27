#pragma once
#ifndef __ARP_SPIROGRAPH__
#define __ARP_SPIROGRAPH__

class Spirograph
{
private:
	float* vertices;
	float* colors; //color array - i-index color is assigned to the vertex which life-time is i frames
	unsigned int current_max_vertex_count;
	unsigned int current_vertex_count;
	unsigned int next_vertex_index;
	double delta_phi, delta_psi;
	double phi_0, psi_0, phi, psi;
	float center_x, center_y;
	float base_r, rolling_r, pen_r;
	float screen_ratio; //height / width
	bool epi_flag;
	void CreateStaticColor(float r, float g, float b);
	void CreateGradientColor(float* stops, float* ratios, unsigned int count);

public:
	//CONSTANTS
	//-----------------
	static const unsigned int MAX_VERTEX_CAPACITY = 1000; //the object will not contain more vertices than this maximum
	static const unsigned int VERTEX_SIZE = 5; // 2 floats for x, y coords, 3 floats for solid rgb color
	static const unsigned int MAX_GRADIENT_INDEX = 2;

	//METHODS
	//---------------
	Spirograph();
	Spirograph(float screen_ratio);
	~Spirograph();
	float* GetVertices(unsigned int *count);
	void Start();
	void CalculateFrame();
	void SetColorMode(float r, float g, float b);
	void SetColorMode(unsigned int gradient_index);
};

#endif // !__ARP_SPIROGRAPH__
