class PerlinNoise:
    def __init__(self, size=100):
        self._size: int = size
        self._matrix = [[0] * size for _ in range(size)]

    def __eq__(self, other) -> bool:
        if not isinstance(other, PerlinNoise):
            return False
        if self is other:
            return True
        if self._size != other._size:
            return False
        for i in range(self._size):
            for j in range(self._size):
                if self._matrix[i][j] != other._matrix[i][j]:
                    return False
        return True

    def copy(self, other):
        self._size = other.size()
        self._matrix = [row[:] for row in other.matrix()]

    def __add__(self, other):
        if self is other:
            return other
        if self._size != other.size():
            raise RuntimeError("Attempt of adding noises with different sizes")
        result = PerlinNoise(self._size)

        for i in range(result._size):
            for j in range(result._size):
                result._matrix[i][j] = (self._matrix[i][j] + other.matrix()[i][j]) / 2
        return result

    def __pow__(self, power, modulo=None):
        for i in range(self._size):
            for j in range(self._size):
                self._matrix[i][j] = self._matrix[i][j] ** power

    def __str__(self):
        result = f"Size: {self._size}\n"
        for i in range(self._size):
            result += "\t".join(map(str, self._matrix[i]))
            result += '\n'
        return result

    def matrix(self):
        return self._matrix

    def size(self):
        return self._size

    @staticmethod
    def _random_grey():
        from random import random
        return random()

    def fill_random(self, octave: int):
        octave = min(max(0, octave), self._size)
        details = self._size // octave
        correct: int = self._size % details
        for i in range(self._size):
            for j in range(self._size):
                value: float = self._random_grey()
                for __ in range(j * details, min((j + 1) * details, self._size)):
                    for _ in range(i * details, min((i + 1) * details, self._size)):
                        self._matrix[_][__] = value
        if correct == 0:
            return
        for i in range(self._size):
            for j in range(correct):
                self._matrix[self._size - i - 1][j] = self._matrix[i][j]
        for i in range(correct):
            for j in range(self._size):
                self._matrix[i][self._size - j - 1] = self._matrix[i][j]

    def fill_ordered(self, octave: int):
        octave = min(max(0, octave), self._size)
        details = self._size // octave
        equator = self._size // 2

        value = 1 / (equator + 1)
        for i in range(self._size):
            if i % details == 0:
                value = 1/(abs(equator - i) + 1)
            self._matrix[i] = [value] * self._size

    def fill_cos(self, octave):
        import math
        octave = min(max(0, octave), self._size)
        details = self._size // octave

        value = abs(math.cos(math.pi))
        for i in range(self._size):
            if i % details == 0:
                value = abs(math.cos(i+math.pi))
            self._matrix[i] = [value] * self._size

    def interpolate(self):
        result_matrix = [[0.0] * self._size for _ in range(self._size)]
        for i in range(self._size):
            for j in range(self._size):
                total = self._matrix[i][j]
                count = 1
                if i - 1 >= 0:
                    total += self._matrix[i - 1][j]
                    count += 1
                if i + 1 < self._size:
                    total += self._matrix[i + 1][j]
                    count += 1
                if j - 1 >= 0:
                    total += self._matrix[i][j - 1]
                    count += 1
                if j + 1 < self._size:
                    total += self._matrix[i][j + 1]
                    count += 1
                result_matrix[i][j] = total / count
        self._matrix = result_matrix

    def make_image(self, path="noise.png"):
        from PIL import Image
        image = Image.new('L', (self._size, self._size))
        for i in range(self._size):
            for j in range(self._size):
                image.putpixel((j, i), round(self._matrix[i][j] * 255))
        image.save(path)

    def make_map(self, path="map.png"):
        from PIL import Image
        image = Image.new('RGB', (self._size, self._size))
        for i in range(self._size):
            for j in range(self._size):
                value = self._matrix[i][j]
                if value < 0.05:
                    color = (0, 0, 16)
                elif (value > 0.05) and (value <= 0.1):
                    color = (0, 0, 32)
                elif (value > 0.1) and (value <= 0.2):
                    color = (0, 0, 64)
                elif (value > 0.2) and (value <= 0.4):
                    color = (0, 0, 128)
                elif (value > 0.4) and (value <= 0.5):
                    color = (0, 0, 255)
                elif (value > 0.5) and (value <= 0.55):
                    color = (255, 255, 0)
                elif (value >= 0.75) and (value < 0.85):
                    color = (128, 128, 128)
                elif value >= 0.85:
                    color = (255, 255, 255)
                else:
                    color = (0, 255, 0)
                image.putpixel((j, i), color)
        image.save(path)
