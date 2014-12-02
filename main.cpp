#include <iostream>
#include <array>
#include "RubiksCube.hpp"
#include "Color.hpp"

/*constexpr char print(Color color)
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

int main()
{
	RubiksCube<7> rc;
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			for(int k = 0; k < 7; ++k)
			{
				std::cout << print(rc.Surface()[i][j][k]) << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
	rc.rotate(Axis::Y, 0, false);
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			for(int k = 0; k < 7; ++k)
			{
				std::cout << print(rc.Surface()[i][j][k]) << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}
*/
#include <GL/glfw.h>
#include <GL/glu.h>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <cmath>

GLdouble vertex[][3] =
{
	{1, 1, 1},
	{1, 1, -1},
	{1, -1, 1},
	{1, -1, -1},
	{-1, 1, 1},
	{-1, 1, -1},
	{-1, -1, 1},
	{-1, -1, -1},
};

GLdouble panel[][3] =
{
	{1, 0, 1},
	{3, 0, 1},
	{3, 0, 3},
	{1, 0, 3}
};

int plane[][4] =
{
	{0, 1, 3, 2},
	{4, 5, 7, 6},
	{0, 1, 5, 4},
	{2, 3, 7, 6},
	{1, 3, 7, 5},
	{0, 2, 6, 4}
};

GLubyte color[][3] =
{
	{0xFF, 0x00, 0x00},
	{0x00, 0xFF, 0x00},
	{0x00, 0x00, 0xFF},
	{0xFF, 0xFF, 0xFF},
	{0xFF, 0xA0, 0x00},
	{0xFF, 0xFF, 0x00}
};

int main(int argc, char * argv[])
{
	RubiksCube<3> rc;
	
	glOrtho(-5, 5, -5, 5, -5, 5);
	glfwInit();
	glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);

	glfwSetWindowSizeCallback([](int w, int h)
	{
		glViewport(0, 0, w, h);
		glLoadIdentity();
		gluPerspective(90, static_cast<double>(w) / static_cast<double>(h), 1, 100);
		gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);
	});

	while (glfwGetWindowParam(GLFW_OPENED))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		if (glfwGetKey(GLFW_KEY_ENTER))
		{
			glRotated(1, 1, 0, 0);
		}
		else
		{
			glRotated(1, 0, 1, 0);
		}
		
		auto f = [&](int i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3; ++k)
				{
					glPushMatrix();
					glTranslated(j * 4, 0, k * 4);
					glColor3ubv(color[static_cast<int>(rc.Surface()[i][j][k])]);
					glBegin(GL_QUADS);
					for (auto && e : panel)
					{
						glVertex3dv(e);
					}
					glEnd();
					glPopMatrix();
				}
			}
		};
		f(0);
		glPushMatrix();
		glRotated(90, 1, 0, 0);
		f(1);
		glPopMatrix();
		glPushMatrix();
		glRotated(-90, 0, 0, 1);
		f(2);
		glPopMatrix();

		glfwSwapBuffers();
	}
	glfwTerminate();
}

