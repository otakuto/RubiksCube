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

template<typename T>
T rotateRight(T t)
{
	std::rotate(t.rbegin(), t.rbegin() + 1, t.rend());
	return t;
}

template<typename T>
T rotateLeft(T t)
{
	std::rotate(t.begin(), t.begin() + 1, t.end());
	return t;
}

template<typename F, typename X>
constexpr auto nest(F f, int n, X x) -> decltype(f(x))
{
	for (int i = 0; i < n; ++i)
	{
		x = f(x);
	}
	return x;
};

template<typename T>
boost::optional<int> axisToInt(T axis)
{
	for (int i = 0; i < axis.size(); ++i)
	{
		if (axis[i])
		{
			return i;
		}
	}
	return boost::none;
};

template <int SIZE>
class RubiksCube
{
public:
	constexpr static int const SURFACE_LENGTH = 6;
private:
	std::array<std::array<std::array<Color, SIZE>, SIZE>, SURFACE_LENGTH> surface;
public:
	constexpr RubiksCube()
	{
		for (int i = 0; i < SURFACE_LENGTH; ++i)
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

public:
	std::map<Surface, Color> getCube(std::array<int, 3> p) const
	{
		std::map<Surface, Color> map;
		for (auto e : {Surface::Right, Surface::Up, Surface::Front})
		{
			auto q = nest(rotateLeft<decltype(p)>, static_cast<int>(e), p);
			if ((std::get<0>(q) == 0) || (std::get<0>(q) == (SIZE - 1)))
			{
				Surface s = std::get<0>(q) ? e : inverse(e);
				map.emplace(s, surface[static_cast<int>(s)][std::get<1>(q)][std::get<2>(q)]);
			}
		}

		return map;
	}

	void rotate(std::array<int, 3> axis, int index, bool isPrime)
	{
		auto s = surface;
		if ((index == 0) || (index == (SIZE - 1)))
		{
			auto e = (index == 0) ? inverse(static_cast<Surface>(axisToInt(axis).get())) : static_cast<Surface>(axisToInt(axis).get());

			if (isPrime)
			{
				for (int i = 0; i < SIZE; ++i)
				{
					for (int j = 0; j < SIZE; ++j)
					{
						s[static_cast<int>(e)][i][j] = surface[static_cast<int>(e)][j][(SIZE - 1) - i];
					}
				}
			}
			else
			{
				for (int i = 0; i < SIZE; ++i)
				{
					for (int j = 0; j < SIZE; ++j)
					{
						s[static_cast<int>(e)][i][j] = surface[static_cast<int>(e)][(SIZE - 1) - j][i];
					}
				}
			}
		}

		//side surface rotate
		for (int i = 0; i < SURFACE_LENGTH - 2; ++i)
		{
			auto rotateAxis = isPrime ? rotateRight<std::array<int, 3>> : rotateLeft<std::array<int, 3>>;
			auto si = nest(inverse, i / 2, static_cast<Surface>(axisToInt(nest(rotateAxis, (i % 2) + 1, axis)).get()));
			auto di = nest(inverse, (i + 1) / 2, static_cast<Surface>(axisToInt(nest(rotateAxis, ((i + 1) % 2) + 1, axis)).get()));
			if (i % 2 == isPrime)
			{
				for (int j = 0; j < SIZE; ++j)
				{
					s[static_cast<int>(di)][j][index] = surface[static_cast<int>(si)][index][j];
				}
			}
			else
			{
				for (int j = 0; j < SIZE; ++j)
				{
					s[static_cast<int>(di)][index][j] = surface[static_cast<int>(si)][(SIZE - 1) - j][index];
				}
			}
		}

		surface = s;
	}
};

