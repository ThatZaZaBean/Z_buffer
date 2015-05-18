#include "stdafx.h"

#include "point3d.h"
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

vector< Point3D > Clicks;
vector< Point3D > pastClicks;
//~ unsigned numBez = Clicks.size();

// Renders a quad at cell (x, y) with dimensions CELL_LENGTH
void renderPixel(int x, int y, float r = 1, float g = 0, float b = 0)
{
	// Easier to seeeeeeeee
	//~ glPointSize(3.0f);
	// Draw request    
	glBegin(GL_POINTS);
	glColor3f(r, g, b);
	glVertex2f((float)x, (float)y);
	glEnd();
}

// Renders a Line segment from (x_one, y_one) to (x_two, y_two)
void renderLine(int x_one, int y_one, int x_two, int y_two, float r, float g, float b)
{
	int x_diff = x_two - x_one, y_diff = y_two - y_one, lrgN;

	if (x_one > x_two && y_one > y_two)
	{
		swap(x_one, x_two);
		swap(y_one, y_two);
	}
	if (x_one > x_two && y_one < y_two)
	{
		swap(x_one, x_two);
		swap(y_one, y_two);
	}

	//~ cout << "x_one " << x_one << " x_two " << x_two
	//~ << " y_one " << y_one << " y_two " << y_two << endl;

	if (abs(x_diff) > abs(y_diff))
	{
		lrgN = abs(x_diff);
	}
	else
	{
		lrgN = abs(y_diff);
	}

	float m = (float)y_diff / (float)x_diff;

	float X = x_one, Y = y_one;

	if (m <= 1 && m > -1)
	{
		for (int i = 0; i < lrgN; i++)
		{
			renderPixel((int)X, (int)Y, r, g, b);
			X = X + 1;
			Y = Y + m;
		}
	}
	else if (m <= -1)
	{
		for (int i = 0; i < lrgN; i++)
		{
			renderPixel((int)X, (int)Y, r, g, b);
			Y = Y - 1;
			X = X + 1 / m * -1;
		}
	}
	else
	{
		for (int i = 0; i < lrgN; i++)
		{
			renderPixel((int)X, (int)Y, r, g, b);
			X = X + 1 / m;
			Y = Y + 1;
		}
	}
}


bool compare( Point3D lhs, Point3D rhs)
{
	return lhs.y > rhs.y;
}

void drawTriangle(int i) // index
{
	srand(time(NULL));
	float red, blue, green;

	//~ if(i == 3)
	//~ {
	//~ red = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//~ green = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//~ blue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	//~ }
	// random bitches!
	/*for (int x = -3; x < i / 3; ++x)
	{
		red = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		green = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		blue = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}*/

	// Creates triangle
	renderLine(Clicks[i].x, Clicks[i].y,
		Clicks[i + 1].x, Clicks[i + 1].y,
		red, green, blue);
	renderLine(Clicks[i + 1].x, Clicks[i + 1].y,
		Clicks[i + 2].x, Clicks[i + 2].y,
		red, green, blue);
	renderLine(Clicks[i + 2].x, Clicks[i + 2].y,
		Clicks[i].x, Clicks[i].y,
		red, green, blue);

	// set high and low y vals
	int high_y, low_y, middle_y;
	vector<pair<int, int > > sizes;

	for (int j = i; j < i + 3; j++)
		sizes.push_back(make_pair(Clicks[j].x, Clicks[j].y));
	sort(sizes.begin(), sizes.end(), compare);

	high_y = sizes[0].y, middle_y = sizes[1].y, low_y = sizes[2].y;

	// determine if triagle is right or left slanted
	bool isRight = false;

	// Checks if right or left side is the continuous side


	float l = (float)(sizes[1].x- sizes[2].x) / (float)(sizes[1].y - sizes[2].y);
	float r = (float)(sizes[0].x- sizes[2].x) / (float)(sizes[0].y - sizes[2].y);

	if (r > l)
		isRight = true;

	// Fill in triangles
	float x_inc_s = sizes[1].x;
	float tp_m0 = (float)(sizes[0].x- sizes[1].x) / (float)(sizes[0].y - sizes[1].y);
	float tp_m1 = (float)(sizes[0].x- sizes[2].x) / (float)(sizes[0].y - sizes[2].y);

	// only need to swap slopes to account for left/right favored tri's
	if (!isRight)
		swap(tp_m0, tp_m1);

	float top_l_x = sizes[0].x, top_r_x = sizes[0].x;

	for (int i = sizes[0].y; i >= sizes[1].y; i--)
	{
		for (float j = top_l_x; j < top_r_x; j++)
			renderPixel((int)j, i, red, green, blue);
		top_l_x -= tp_m0;
		top_r_x -= tp_m1;
	}

	// Middle left to bottom slope
	float md_m0 = (float)(sizes[1].x- sizes[2].x) / (float)(sizes[1].y - sizes[2].y);

	if (!isRight)
	{
		float middle_x = sizes[1].x;
		// accouts for changes between z and y half
		swap(tp_m1, tp_m0);
		swap(tp_m1, md_m0);

		for (int i = sizes[1].y; i >= sizes[2].y; i--)
		{
			for (float j = top_l_x; j <= middle_x; j++)
				renderPixel((int)j, i, red, green, blue);

			top_l_x -= md_m0;
			middle_x -= tp_m1;
		}
	}
	else for (int i = sizes[1].y; i >= sizes[2].y; i--)
	{
		for (float j = x_inc_s; j < top_r_x; j++)
			renderPixel((int)j, i, red, green, blue);
		x_inc_s -= md_m0;
		top_r_x -= tp_m1;
	}
}

void mouseHandler(int button, int state, int x, int y)
{
	//~ glutPostRedisplay();

	switch (state)
	{
	case GLUT_DOWN:
	{
		Clicks.push_back(make_pair(x, WINDOW_HEIGHT - y));
		cout << "x: " << x << " y: " << y << endl;
	}
	case GLUT_UP:
	{
		// do nothing
	}
	}
	glutPostRedisplay();
}

// factorials are fuuuun
float fact(float n)
{
	if (n <= 1)
		return 1;
	else
		return n*fact(n - 1);
}
// Choose function
float NCR(float n, float k)
{
	return fact(n) / (fact(k)*fact(n - k));
}

//Output funtion to OpenGl Buffer
void GL_render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	for (unsigned i = 0; i < Clicks.size() / 3; i++)
	{
		drawTriangle(3 * i);
	}

	glutSwapBuffers();
}

//Initializes OpenGL attributes
void GLInit(int* argc, char** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutCreateWindow("CS 130 - Tony!!!");
	glutDisplayFunc(GL_render);

	// The default view coordinates is (-1.0, -1.0) bottom left & (1.0, 1.0) top right.
	// For the purposes of this lab, this is set to the number of pixels
	// in each dimension.

	glMatrixMode(GL_PROJECTION_MATRIX);
	glOrtho(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
}

int main(int argc, char** argv)
{
	GLInit(&argc, argv);
	glutMouseFunc(mouseHandler);
	glutMainLoop();

	return 0;
}

