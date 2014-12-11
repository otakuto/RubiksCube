#include <GL/glfw.h>
#include <GL/glu.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "RubiksCube.hpp"

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
	{0xFF, 0xFF, 0xFF},
	{0xFF, 0x00, 0x00},
	{0x00, 0x00, 0xFF},
	{0x00, 0xFF, 0x00},
	{0xFF, 0xA0, 0x00},
	{0xFF, 0xFF, 0x00}
};

int main(int argc, char * argv[])
{
	RubiksCube<3> rc;
	
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
		glPopMatrix();
		glPushMatrix();

		static int theta = 0;
		static int iota = 0;
		if (glfwGetKey(GLFW_KEY_RIGHT))
		{
			++theta;
		}
		if (glfwGetKey(GLFW_KEY_LEFT))
		{
			--theta;
		}
		if (glfwGetKey(GLFW_KEY_UP))
		{
			++iota;
		}
		if (glfwGetKey(GLFW_KEY_DOWN))
		{
			--iota;
		}

		static bool b = true;
		if (glfwGetKey('A'))
		{
			if (b)
			{
				rc.rotate(Axis::Y, 0, false);
				b = false;
			}
		}
		else
		{
			b = true;
		}
		//glTranslated(-6, -6, -6);
		glRotated(theta, 0, 1, 0);
		glRotated(iota, 1, 0, 0);

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
		glRotated(90, 0, 1, 0);
		glRotated(-90, 0, 0, 1);
		f(1);
		glPopMatrix();
		glPushMatrix();
		glRotated(-90, 0, 0, 1);
		f(2);
		glPopMatrix();
		glPushMatrix();
		glTranslated(12, 0, 0);
		glRotated(-90, 0, 0, 1);
		f(3);
		glPopMatrix();
		glPushMatrix();
		glTranslated(0, 0, 12);
		glRotated(90, 0, 1, 0);
		glRotated(-90, 0, 0, 1);
		f(4);
		glPopMatrix();
		glPushMatrix();
		glTranslated(0, -12, 0);
		f(5);
		glPopMatrix();
		glfwSwapBuffers();
	}
	glfwTerminate();
}

