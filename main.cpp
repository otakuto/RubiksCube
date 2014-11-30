#include <iostream>
#include <array>

enum class Color
{
	Red,
	Green,
	Blue,
	White,
	Orange,
	Yellow
};

constexpr char print(Color color)
{
	switch (color)
	{
		case Color::Red:
			return 'R';
		case Color::Green:
			return 'G';
		case Color::Blue:
			return 'B';
		case Color::White:
			return 'W';
		case Color::Orange:
			return 'O';
		case Color::Yellow:
			return 'Y';
		default:
			return '5';
	}
}

template <int SIZE>
class RC
{
	std::array<std::array<std::array<Color, SIZE>, SIZE>, 6> surface;
public:
	constexpr RC()
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
	constexpr void rotateX(int index, bool isRight)
	{

	}
	constexpr void rotateY(int index, bool isRight)
	{
		if ((index == 0) || (index == (SIZE - 1)))
		{
			int s = (index == 0) ? 0 : 5;
			if (isRight)
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

		auto l = isRight ? std::array<int, 3>{2, 4, 3} : std::array<int, 3>{3, 4, 2};
		for (int i : l)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				std::swap(surface[1][index][j], surface[i][index][j]);
			}
		}
	}
	constexpr void rotateZ(int index, bool isRight);
};

int main()
{
	RC<5> rc;
	rc.rotateY(0, false);
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			for(int k = 0; k < 5; ++k)
			{
				std::cout << print(rc.Surface()[i][j][k]) << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}
