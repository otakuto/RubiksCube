#pragma once

enum class Surface : int
{
	Right,
	Up,
	Front,
	Back,
	Down,
	Left
};

constexpr Surface inverse(Surface surface)
{
	return static_cast<Surface>(5 - static_cast<int>(surface));
}

