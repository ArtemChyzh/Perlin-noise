#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#pragma once

class PerlinNoise
{
private:
	int** m_matrix;
	int m_size;
	
	int random_grey();

public:
	PerlinNoise();
	PerlinNoise(const PerlinNoise&);
	PerlinNoise(int);
	~PerlinNoise();

	void fill_mono(int);
	void fill_random(int);

	void linear_interpolate();
	void bilinear_interpolate();

	int get_size();
	int** get_matrix();
	double** get_normalize();

	std::string to_str();

	PerlinNoise& operator=(const PerlinNoise& other);
	bool operator==(const PerlinNoise& other) const;
	PerlinNoise operator+(const PerlinNoise&) const;
};

