#include "PerlinNoise.h"

PerlinNoise::~PerlinNoise() {
	for (int i = 0; i < this->m_size; ++i) {
		delete[] this->m_matrix[i];
	}
	delete[] this->m_matrix;
}

PerlinNoise::PerlinNoise() {
	this->m_size = 100;

	this->m_matrix = new int* [this->m_size];
	for (int i = 0; i < this->m_size; ++i) { 
		this->m_matrix[i] = new int[this->m_size]; 
	}

	for (int i = 0; i < this->m_size; ++i) {
		for (int j = 0; j < this->m_size; ++j) {
			this->m_matrix[i][j] = 0;
		}
	}
}

PerlinNoise::PerlinNoise(const PerlinNoise& other) {
	this->m_size = other.m_size;

	this->m_matrix = new int* [this->m_size];
	for (int i = 0; i < this->m_size; ++i) {
		this->m_matrix[i] = new int[this->m_size];
	}

	for (int i = 0; i < this->m_size; ++i) {
		for (int j = 0; j < this->m_size; ++j) {
			this->m_matrix[i][j] = other.m_matrix[i][j];
		}
	}
}

PerlinNoise::PerlinNoise(int size) {
	this->m_size = size;

	this->m_matrix = new int* [this->m_size];
	for (int i = 0; i < this->m_size; ++i) {
		this->m_matrix[i] = new int[this->m_size];
	}

	for (int i = 0; i < this->m_size; ++i) {
		for (int j = 0; j < this->m_size; ++j) {
			this->m_matrix[i][j] = 0;
		}
	}
}

PerlinNoise& PerlinNoise::operator=(const PerlinNoise& other) {
	if (this != &other) {
		for (int i = 0; i < this->m_size; ++i) {
			delete[] this->m_matrix[i];
		}
		delete[] this->m_matrix;

		this->m_size = other.m_size;
		this->m_matrix = new int* [this->m_size];
		for (int i = 0; i < this->m_size; ++i) {
			this->m_matrix[i] = new int[this->m_size];
		}

		for (int i = 0; i < this->m_size; ++i) {
			for (int j = 0; j < this->m_size; ++j) {
				this->m_matrix[i][j] = other.m_matrix[i][j];
			}
		}
	}
	return *this;
}

bool PerlinNoise::operator==(const PerlinNoise& other) const {
	if (this == &other) return true;
	if (this->m_size != other.m_size) return false;
	for (int i = 0; i < this->m_size; ++i) {
		for (int j = 0; j < this->m_size; ++j) {
			if (this->m_matrix[i][j] != other.m_matrix[i][j]) {
				return false;
			}
		}
	}
	return true;
}


PerlinNoise PerlinNoise::operator+(const PerlinNoise& other) const {
	PerlinNoise result(this->m_size);
	if (*this == other) return other;
	if (this->m_size != other.m_size) {
		throw std::runtime_error("Attemp of adding noises with different sizes");
	}
	for (int i = 0; i < result.m_size; ++i) {
		for (int j = 0; j < result.m_size; ++j) {
			result.m_matrix[i][j] = (this->m_matrix[i][j] + other.m_matrix[i][j]) / 2;
		}
	}
	return result;
}

int** PerlinNoise::get_matrix() { return this->m_matrix; }
int PerlinNoise::get_size() { return this->m_size; }

int PerlinNoise::random_grey() {
	return 0 + rand() % 256;
}

void PerlinNoise::fill_mono(int value) {
	value = std::max(0, std::min(value, 255));
	for (int i = 0; i < this->m_size; ++i) {
		for (int j = 0; j < this->m_size; ++j) {
			this->m_matrix[i][j] = value;
		}
	}
}

void PerlinNoise::fill_random(int details = 1) {
	int correct = this->m_size % details;
	srand(time(0));
	for (int l = 0; l < this->m_size / details; ++l) {
		for (int k = 0; k < this->m_size / details; ++k) {
			int value = this->random_grey();
			for (int i = k * details; i < (k + 1) * details && i < this->m_size; ++i) {
				for (int j = l * details; j < (l + 1) * details && j < this->m_size; ++j) {
					this->m_matrix[i][j] = value;
				}
			}
		}
	}
	if (correct == 0) return;
	for (int i = 0; i < correct; ++i) {
		for (int j = 0; j < this->m_size; ++j) {
			this->m_matrix[this->m_size - i - 1][j] = this->m_matrix[i][j];
		}
	}
	for (int i = 0; i < this->m_size; ++i) {
		for (int j = 0; j < correct; ++j) {
			this->m_matrix[i][this->m_size - j - 1] = this->m_matrix[i][j];
		}
	}
}

double** PerlinNoise::get_normalize() {
	double** matrix = new double*[this->m_size];
	for (int i = 0; i < this->m_size; ++i) {
		matrix[i] = new double;
	}
	for (int i = 0; i < this->m_size; ++i) {
		for (int j = 0; j < this->m_size; ++j) {
			double norm = this->m_matrix[i][j] / 255.0;
			matrix[i][j] = std::round(norm * 1000) / 1000;
		}
	}
	return matrix;
}

std::string PerlinNoise::to_str()
{
	std::string result = "Size: ";
	result += std::to_string(this->m_size);
	result += '\n';
	for (int i = 0; i < this->m_size; ++i) {
		for (int j = 0; j < this->m_size; ++j) {
			result += std::to_string(this->m_matrix[i][j]);
			result += '\t';
		}
		result += '\n';
	}

	return result;
}

void PerlinNoise::linear_interpolate() {
	int** inter_matrix = new int* [this->m_size];
	for (int i = 0; i < this->m_size; ++i)
		inter_matrix[i] = new int[this->m_size];

	for (int i = 0; i < this->m_size; ++i) {
		for (int j = 0; j < this->m_size; ++j) {
			if (i == 0 || j == 0 || i == this->m_size - 1 || j == this->m_size - 1) {
				inter_matrix[i][j] = this->m_matrix[i][j];
				continue;
			}

			int top = (this->m_matrix[i + 1][j - 1] + this->m_matrix[i + 1][j + 1]) / 2;
			int bottom = (this->m_matrix[i - 1][j - 1] + this->m_matrix[i - 1][j + 1]) / 2;
			inter_matrix[i][j] = (top + bottom) / 2;
		}
	}

	for (int i = 0; i < this->m_size; ++i) {
		std::copy(inter_matrix[i], inter_matrix[i] + this->m_size, this->m_matrix[i]);
		delete[] inter_matrix[i];
	}
	delete[] inter_matrix;
}

void PerlinNoise::bilinear_interpolate() {
	int** inter_matrix = new int* [this->m_size];
	for (int i = 0; i < this->m_size; ++i)
		inter_matrix[i] = new int[this->m_size];

	for (int i = 0; i < this->m_size; ++i) {
		for (int j = 0; j < this->m_size; ++j) {
			if (i == 0 || j == 0 || i == this->m_size - 1 || j == this->m_size - 1) {
				inter_matrix[i][j] = this->m_matrix[i][j];
				continue;
			}
			double dx = static_cast<double>(j) / (this->m_size - 1);
			double dy = static_cast<double>(i) / (this->m_size - 1);

			int top_left = this->m_matrix[i - 1][j - 1];
			int top_right = this->m_matrix[i - 1][j + 1];
			int bottom_left = this->m_matrix[i + 1][j - 1];
			int bottom_right = this->m_matrix[i + 1][j + 1];

			int top = static_cast<int>((1.0 - dx) * top_left + dx * top_right);
			int bottom = static_cast<int>((1.0 - dx) * bottom_left + dx * bottom_right);

			inter_matrix[i][j] = static_cast<int>((1.0 - dy) * top + dy * bottom);

			for (int i = 0; i < this->m_size; ++i) {
				std::copy(inter_matrix[i], inter_matrix[i] + this->m_size, this->m_matrix[i]);
				delete[] inter_matrix[i];
			}
			delete[] inter_matrix;
		}
	}
}