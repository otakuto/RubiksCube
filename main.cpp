#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <cctype>
#include "RubiksCubeController.hpp"

int main()
{
	constexpr int SIZE = 4;
	RubiksCube<SIZE> rc;
	RubiksCubeController<SIZE> rcc;

	glfwInit();
	int count;
	GLFWmonitor ** monitors = glfwGetMonitors(&count);
	GLFWmonitor * monitor = monitors[0];
	GLFWvidmode const * mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	//GLFWwindow * window = glfwCreateWindow(mode->width, mode->height, "RubiksCube", monitor, nullptr);
	GLFWwindow * window = glfwCreateWindow(mode->width, mode->height, "RubiksCube", nullptr, nullptr);
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
		glPushMatrix();

		//window size chagne
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glLoadIdentity();
		gluPerspective(90, static_cast<double>(width) / static_cast<double>(height), 1, 128);
		gluLookAt(30, 30, 30, 0, 0, 0, 0, 1, 0);

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

		static int index = 0;

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
							rcc.rotate(Axis::X, index, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT));
							break;
						case 'C':
							rcc.rotate(Axis::Y, index, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT));
							break;
						case 'Z':
							rcc.rotate(Axis::Z, index, glfwGetKey(window, GLFW_KEY_LEFT_SHIFT));
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
								rcc.rotate(static_cast<Axis>(axis(engine)), index(engine), isPrime(engine));
							}
							break;
						}
						case 'S':
							//rcc.solve();
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
		//view
		glRotated(theta, 0, 1, 0);
		glRotated(iota, 1, 0, 0);
		//centralize
		glTranslated(-(SIZE - 1) * 2, -(SIZE - 1) * 2, -(SIZE - 1) * 2);

		rcc.run();
		rcc.draw();

		glPopMatrix();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}

