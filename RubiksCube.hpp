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
	}	

	constexpr auto const & Surface()
	{
		return surface;
	}

	constexpr void rotate(Axis axis, int index, bool isPrime)
	{
		if ((index == 0) || (index == (SIZE - 1)))
		{
			int s = [axis, index]()
			{
				switch (axis)
				{
					case Axis::X:
						return 0;
					case Axis::Y:
						return (index == 0) ? 5 : 0;
					case Axis::Z:
						return 2;
				}
			}();

			if (isPrime)
			{
				for (int i = 0; i < ((SIZE / 2) + (SIZE % 2)); ++i)
				{
					for (int j = 0; j < SIZE / 2; ++j)
					{
						std::swap(surface[s][i][j], surface[s][SIZE - 1 - j][i]);
						std::swap(surface[s][i][j], surface[s][SIZE - 1 - i][SIZE - 1 - j]);
						std::swap(surface[s][i][j], surface[s][j][SIZE - 1 - i]);
					}
				}
			}
			else
			{
				for (int i = 0; i < ((SIZE / 2) + (SIZE % 2)); ++i)
				{
					for (int j = 0; j < SIZE / 2; ++j)
					{
						std::swap(surface[s][i][j], surface[s][j][SIZE - 1 - i]);
						std::swap(surface[s][i][j], surface[s][SIZE - 1 - i][SIZE - 1 - j]);
						std::swap(surface[s][i][j], surface[s][SIZE - 1 - j][i]);
					}
				}
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

