#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <cctype>
#include "RubiksCube.hpp"

constexpr GLdouble const vertex[6][4][3] =
{
	{
		{-2, 1, 1},
		{-2, 1, -1},
		{-2, -1, -1},
		{-2, -1, 1}
	},
	{
		{1, -2, 1},
		{-1, -2, 1},
		{-1, -2, -1},
		{1, -2, -1}
	},
	{
		{1, 1, -2},
		{1, -1, -2},
		{-1, -1, -2},
		{-1, 1, -2}
	},
	{
		{1, 1, 2},
		{1, -1, 2},
		{-1, -1, 2},
		{-1, 1, 2}
	},
	{
		{1, 2, 1},
		{-1, 2, 1},
		{-1, 2, -1},
		{1, 2, -1}
	},
	{
		{2, 1, 1},
		{2, 1, -1},
		{2, -1, -1},
		{2, -1, 1}
	},
};
GLubyte color[][3] =
{
	{0xFF, 0xA0, 0x00},
	{0xFF, 0xFF, 0x00},
	{0x00, 0xFF, 0x00},
	{0x00, 0x00, 0xFF},
	{0xFF, 0xFF, 0xFF},
	{0xFF, 0x00, 0x00}
};

int main()
{
	constexpr int SIZE = 3;
	RubiksCube<SIZE> rc;

	glfwInit();
	int count;
	GLFWmonitor ** monitors = glfwGetMonitors(&count);
	GLFWmonitor * monitor = monitors[0];
	GLFWvidmode const * mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	GLFWwindow * window = glfwCreateWindow(mode->width, mode->height, "RubiksCube", monitor, nullptr);
    glfwMakeContextCurrent(window);

	/*auto pa = [&](std::string s)
	{
		for (auto i = s.begin(); i != s.end(); ++i)
		{
			Axis axis = static_cast<Axis>(std::tolower(*i) - 'x');
			bool isPrime = std::isupper(*i);
			++i;
			int index = *i - '0';
			rc.rotate(axis, index, isPrime);
		}
	};*/

    while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glPopMatrix();
		glPushMatrix();

		//window size chagne
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glLoadIdentity();
		gluPerspective(90, static_cast<double>(width) / static_cast<double>(height), 1, 128);
		gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

		//view
		static int theta = 0;
		static int iota = 0;
		if (glfwGetKey(window, GLFW_KEY_RIGHT))
		{
			++theta;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT))
		{
			--theta;
		}
		if (glfwGetKey(window, GLFW_KEY_UP))
		{
			++iota;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
			--iota;
		}

		//animation
		static int index = 0;
		static Axis axis = {};
		static double angle = 0;
		static bool isPrime = {};

		//key
		static bool key[256] = {};
		for (int i = 0; i < 256; ++i)
		{
			if (glfwGetKey(window, static_cast<char>(i)))
			{
				if (!key[i])
				{
					switch (static_cast<char>(i))
					{
						case '1':
							index = 0;
							break;
						case '2':
							index = 1;
							break;
						case '3':
							index = 2;
							break;
						case 'X':
							axis = Axis::X;
							isPrime = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
							angle = 90;
							rc.rotate(axis, index, isPrime);
							break;
						case 'C':
							axis = Axis::Y;
							isPrime = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
							angle = 90;
							rc.rotate(axis, index, isPrime);
							break;
						case 'Z':
							axis = Axis::Z;
							angle = 90;
							isPrime = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
							rc.rotate(axis, index, isPrime);
							break;
						case 'R':
						{
							//pa("X2Y2Z0x2y1z0");
							static std::mt19937 engine(std::random_device{}());
							std::uniform_int_distribution<int> axis(0, 2);
							std::uniform_int_distribution<int> index(0, SIZE - 1);
							std::uniform_int_distribution<bool> isPrime(false, true);
							for (int i = 0; i < 128; ++i)
							{
								rc.rotate(static_cast<Axis>(axis(engine)), index(engine), isPrime(engine));
							}
							break;
						}
						case 'S':
							rc.solve();
							break;
						default:
							break;
					}
				}
				key[i] = true;
			}
			else
			{
				key[i] = false;
			}
		}

		//animation
		if (angle > 0)
		{
			angle -= 10;
		}
		//view
		glRotated(theta, 0, 1, 0);
		glRotated(iota, 1, 0, 0);
		//centralize
		glTranslated(-4, -4, -4);

		for (int i = 0; i < SIZE; ++i)
		{
			for (int j = 0; j < SIZE; ++j)
			{
				for (int k = 0; k < SIZE; ++k)
				{
					for (auto && e : rc.getCube(i, j, k))
					{
						glPushMatrix();

						//animation
						if (angle > 0)
						{
							std::array<int, 3> wrapper = {i, j, k};
							if (wrapper[static_cast<int>(axis)] == index)
							{
								glTranslated(((axis == Axis::X) ? 0 : 4), ((axis == Axis::Y) ? 0 : 4), ((axis == Axis::Z) ? 0 : 4));
								glRotated(isPrime ? -angle : angle, ((axis == Axis::X) ? 1 : 0), ((axis == Axis::Y) ? 1 : 0), ((axis == Axis::Z) ? 1 : 0));
								glTranslated(((axis == Axis::X) ? 0 : -4), ((axis == Axis::Y) ? 0 : -4), ((axis == Axis::Z) ? 0 : -4));
							}
						}

						glTranslated(i * 4, j * 4, k * 4);
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
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

