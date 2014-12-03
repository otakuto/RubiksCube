#pragma once
#include <array>
#include "Color.hpp"
#include "Axis.hpp"

template <int SIZE>
class RubiksCube
{
	std::array<std::array<std::array<Color, SIZE>, SIZE>, 6> surface;
public:
	constexpr RubiksCube()
	{
		for (int i = 0; i < surface.size(); ++i)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				for(int k = 0; k < SIZE; ++k)
				{
					surface[i][j][k] = static_cast<Color>(i);
				}
			}
		}
		surface[0][0][0] = Color::Yellow;
		surface[5][0][0] = Color::White;
	}	

	constexpr auto const & Surface()
	{
		return surface;
	}
private:
	constexpr void rotate(int surface, bool isPrime)
	{
		if (isPrime)
		{
			for (int i = 0; i < ((SIZE / 2) + (SIZE % 2)); ++i)
			{
				for (int j = 0; j < SIZE / 2; ++j)
				{
					std::swap(this->surface[surface][i][j], this->surface[surface][j][SIZE - 1 - i]);
					std::swap(this->surface[surface][i][j], this->surface[surface][SIZE - 1 - i][SIZE - 1 - j]);
					std::swap(this->surface[surface][i][j], this->surface[surface][SIZE - 1 - j][i]);
				}
			}
		}
		else
		{
			for (int i = 0; i < ((SIZE / 2) + (SIZE % 2)); ++i)
			{
				for (int j = 0; j < SIZE / 2; ++j)
				{
					std::swap(this->surface[surface][i][j], this->surface[surface][SIZE - 1 - j][i]);
					std::swap(this->surface[surface][i][j], this->surface[surface][SIZE - 1 - i][SIZE - 1 - j]);
					std::swap(this->surface[surface][i][j], this->surface[surface][j][SIZE - 1 - i]);
				}
			}
		}
	}

public:
	constexpr void rotate(Axis axis, int index, bool isPrime)
	{
		if ((index == 0) || (index == (SIZE - 1)))
		{
			switch (axis)
			{
				case Axis::X:
					rotate((index == 0 ? 1 : 4), isPrime);
				break;
				case Axis::Y:
					rotate((index == 0 ? 0 : 5), isPrime);
				break;
				case Axis::Z:
					rotate((index == 0 ? 2 : 3), isPrime);
				break;
			}
		}

		auto l = isPrime ? std::array<int, 3>{2, 4, 3} : std::array<int, 3>{3, 4, 2};
		for (int i : l)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				std::swap(surface[1][index][j], surface[i][index][j]);
			}
		}
	}
};

