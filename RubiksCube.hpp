#pragma once
#include <iostream>
#include <array>
#include <functional>
#include <iterator>
#include <algorithm>
#include <map>
#include <boost/optional.hpp>
#include "Color.hpp"
#include "Axis.hpp"
#include "Surface.hpp"

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

private:
	boost::optional<Color &> get(int x, int y, int z)
	{
		if ((x == 0) || (x == (SIZE + 1)))
		{
			if ((1 <= y) && (y <= SIZE) && (1 <= z) && (z <= SIZE))
			{
				return surface[static_cast<int>(x ? Surface::Right : Surface::Left)][y - 1][z - 1];
			}
		}
		else if ((y == 0) || (y == (SIZE + 1)))
		{
			if ((1 <= z) && (z <= SIZE) && (1 <= x) && (x <= SIZE))
			{
				return surface[static_cast<int>(y ? Surface::Up : Surface::Down)][z - 1][x - 1];
			}
		}
		else if ((z == 0) || (z == (SIZE + 1)))
		{
			if ((1 <= x) && (x <= SIZE) && (1 <= y) && (y <= SIZE))
			{
				return surface[static_cast<int>(z ? Surface::Front : Surface::Back)][x - 1][y - 1];
			}
		}

		return boost::none;
	}

public:
	std::map<Surface, Color> getCube(int x, int y, int z)
	{
		std::map<Surface, Color> map;
		if ((x == 0) || (x == (SIZE - 1)))
		{
			Surface s = x ? Surface::Right : Surface::Left;
			map.emplace(s, surface[static_cast<int>(s)][y][z]);
		}
		if ((y == 0) || (y == (SIZE - 1)))
		{
			Surface s = y ? Surface::Up : Surface::Down;
			map.emplace(s, surface[static_cast<int>(s)][z][x]);
		}
		if ((z == 0) || (z == (SIZE - 1)))
		{
			Surface s = z ? Surface::Front : Surface::Back;
			map.emplace(s, surface[static_cast<int>(s)][x][y]);
		}

		return map;
	}

	void rotate(Axis axis, int index, bool isPrime)
	{
		//std::cout << static_cast<int>(axis) << ',' << index << ',' << isPrime << std::endl;
		auto f = [&]() -> std::function<boost::optional<Color &>(int, int, int)>
		{
			switch (axis)
			{
				case Axis::X:
					return [&](int x, int y, int z)
					{
						return get(x, y, z);
					};
	
				case Axis::Y:
					return [&](int x, int y, int z)
					{
						return get(z, x, y);
					};
	
				case Axis::Z:
					return [&](int x, int y, int z)
					{
						return get(y, z, x);
					};
			}
		}();

		if ((index == 0) || (index == (SIZE - 1)))
		{
			std::array<std::function<void (int, int)>, 3> fl = 
			{
				[&](int i, int j)
				{
					std::swap(*f((index != 0) * (SIZE + 1), i + 1, j + 1), *f((index != 0) * (SIZE + 1), j + 1, SIZE - i));
				},
				[&](int i, int j)
				{
					std::swap(*f((index != 0) * (SIZE + 1), i + 1, j + 1), *f((index != 0) * (SIZE + 1), SIZE - i, SIZE - j));
				},
				[&](int i, int j)
				{
					std::swap(*f((index != 0) * (SIZE + 1), i + 1, j + 1), *f((index != 0) * (SIZE + 1), SIZE - j, i + 1));
				}
			};
			auto g = [&]() -> std::function<void (int, int)>
			{
				if (isPrime)
				{
					return [&](int i, int j)
					{
						std::for_each(fl.rbegin(), fl.rend(), [&](auto && f)
						{
							f(i, j);
						});
					};
				}
				else
				{
					return [&](int i, int j)
					{
						std::for_each(fl.begin(), fl.end(), [&](auto && f)
						{
							f(i, j);
						});
					};
				}
			}();
			for (int i = 0; i < ((SIZE / 2) + (SIZE % 2)); ++i)
			{
				for (int j = 0; j < SIZE / 2; ++j)
				{
					g(i, j);
				}
			}
		}

		std::array<std::function<void (int)>, 3> fl = 
		{
			[&](int i)
			{
				std::swap(*f(index + 1, i + 1, 0), *f(index + 1, 0, SIZE - i));
			},
			[&](int i)
			{
				std::swap(*f(index + 1, i + 1, 0), *f(index + 1, SIZE - i, SIZE + 1));
			},
			[&](int i)
			{
				std::swap(*f(index + 1, i + 1, 0), *f(index + 1, SIZE + 1, i + 1));
			}
		};
		auto g = [&]() -> std::function<void (int)>
		{
			if (isPrime)
			{
				return [&](int i)
				{
					std::for_each(fl.rbegin(), fl.rend(), [&](auto && f)
					{
						f(i);
					});
				};
			}
			else
			{
				return [&](int i)
				{
					std::for_each(fl.begin(), fl.end(), [&](auto && f)
					{
						f(i);
					});
				};
			}
		}();
		for (int i = 0; i < SIZE; ++i)
		{
			g(i);
		}
	}

	void solve();
};

template<>
void RubiksCube<3>::solve()
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				auto c = getCube(i, j, k);
				if (c.size() == 2)
				{
					std::cout << i << ',' << j << ',' << k << std::endl;
				}
			}
		}
	}
}

