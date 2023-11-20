from perlin import PerlinNoise

SIZE = 256
COUNT = 6
SMOOTH_SCALE = 10

noises = [PerlinNoise(SIZE)] * COUNT
octaves = [2**i for i in range(0, COUNT)]

noise = PerlinNoise(SIZE)

for i in range(COUNT):
    noises[i].fill_random(octaves[i])
    noise += noises[i]
for i in range(SMOOTH_SCALE):
    noise.interpolate()

noise.make_image()
noise.make_map()
