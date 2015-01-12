#pragma once
#include <cmath>

enum class Surface
{
	Left,
	Down,
	Back,
	Front,
	Up,
	Right
};

Surface inside(Surface && surface)
{
	return static_cast<Surface>(std::abs(static_cast<int>(surface) - 5));
}

