# Perlin-noise
Algorithm for demonstrating the generation a terrain map based on Perlin noise.

# About
The project was written for educational purposes for an article about procedural generation. Matrix and image generation takes place on the processor.

# Principle
The algorithm works on the basis of the PIL library and generates an image from 2d matrix of the values of each pixel.

# perlin.py
File "perlin.py" contains description of the PerlinNoise class which creates an empty matrix when initialized.
The fill_random method takes a number of octaves and generates random noise with the given number of octaves.
Also, with the specified detail, you can use the fill_ordered method, which fills the matrix so that values close to one are in the middle (for example, to generate a temperature map), 
and the fill_cos method, which creates an interference picture (for generating a humidity map)
The class implements the noise addition method (immutable) and the exponentiation method (mutable).
The method interpolate emulates interpolation to smooth matrix values (reduces detail, but increases smoothness).
The make_image method generates a black and white noise image.

# main.py
For quick interaction and test, use a file main.py
Constatnt SIZE for determination of the linear size of the output image
Constatnt COUNT for determination of the count of the different noises
Constant SMOOTH_SCALE for determination of the interpolation value

# Comments
The larger the values of the constants, the longer the time required for generation.
