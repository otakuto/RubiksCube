#pragma once

enum class Surface
{
	Left,
	Down,
	Back,
	Front,
	Up,
	Right
};

constexpr Surface inverse(Surface surface)
{
	return static_cast<Surface>(5 - static_cast<int>(surface));
}

