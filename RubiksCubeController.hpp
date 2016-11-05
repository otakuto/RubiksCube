#pragma once
#include "RubiksCube.hpp"

template <int SIZE>
class RubiksCubeController
{
	RubiksCube<SIZE> rubiksCube;
	Axis axis;
	int index;
	bool isPrime;
	int angle;

public:
	RubiksCubeController()
	:
	rubiksCube(),
	axis(),
	index(),
	isPrime(),
	angle()
	{
	}

	void rotate(Axis axis, int index, bool isPrime)
	{
		rubiksCube.rotate(axis, index, isPrime);
		this->axis = axis;
		this->index = index;
		this->isPrime = isPrime;
		angle = 90;
	}

	void run()
	{
		angle -= 5;
	}

	void draw() const
	{
		constexpr GLdouble const vertex[6][4][3] =
		{
			{
				{2, 1, 1},
				{2, 1, -1},
				{2, -1, -1},
				{2, -1, 1}
			},
			{
				{1, 2, 1},
				{-1, 2, 1},
				{-1, 2, -1},
				{1, 2, -1}
			},
        	{
				{1, 1, 2},
				{1, -1, 2},
				{-1, -1, 2},
				{-1, 1, 2}
			},
			{
				{1, 1, -2},
				{1, -1, -2},
				{-1, -1, -2},
				{-1, 1, -2}
			},
			{
        		{1, -2, 1},
				{-1, -2, 1},
				{-1, -2, -1},
				{1, -2, -1}
			},
			{
				{-2, 1, 1},
				{-2, 1, -1},
				{-2, -1, -1},
				{-2, -1, 1}
			}
		};
		constexpr GLubyte color[][3] =
		{
			{0xFF, 0x00, 0x00},
			{0xFF, 0xFF, 0xFF},
			{0x00, 0x00, 0xFF},
        	{0x00, 0xFF, 0x00},
			{0xFF, 0xFF, 0x00},
			{0xFF, 0xA0, 0x00}
		};
		constexpr double edgeLength = 4.0;

		int l = (SIZE - 1) * 2;

		for (int i = 0; i < SIZE; ++i)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				for (int k = 0; k < SIZE; ++k)
				{
					for (auto && e : rubiksCube.getCube(i, j, k))
					{
						glPushMatrix();

						if (angle > 0)
						{
							std::array<int, 3> wrapper = {i, j, k};
							if (wrapper[static_cast<int>(axis)] == index)
							{
								glTranslated(((axis == Axis::X) ? 0 : l), ((axis == Axis::Y) ? 0 : l), ((axis == Axis::Z) ? 0 : l));
								glRotated(isPrime ? -angle : angle, ((axis == Axis::X) ? 1 : 0), ((axis == Axis::Y) ? 1 : 0), ((axis == Axis::Z) ? 1 : 0));
								glTranslated(((axis == Axis::X) ? 0 : -l), ((axis == Axis::Y) ? 0 : -l), ((axis == Axis::Z) ? 0 : -l));
							}
						}

						glTranslated(i * edgeLength, j * edgeLength, k * edgeLength);
						glColor3ubv(color[static_cast<int>(e.second)]);
						glBegin(GL_QUADS);
						for (auto && e : vertex[static_cast<int>(e.first)])
						{
							glVertex3dv(e);
						}
						glEnd();
						glPopMatrix();
					}
				}
			}
		}
	}
};
