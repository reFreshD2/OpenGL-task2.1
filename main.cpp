#include <GL\freeglut.h>
#include <math.h> 
#include <time.h>
#include <cstdlib>
#define PI 3.14159265

extern unsigned char Terminal5x12[26][6] = {
		{0x04, 0x3C, 0x12, 0x12, 0x3C, 0x00},  // Code for char A 1
		{0x04, 0xFE, 0x92, 0x92, 0x6C, 0x00},  // Code for char B 2
		{0x04, 0x7E, 0x81, 0x81, 0x42, 0x00},  // Code for char C 3
		{0x04, 0xFE, 0x03, 0x02, 0x02, 0x02},  // Code for char D 4
		{0x04, 0xFE, 0x03, 0x22, 0x02, 0x22},  // Code for char E 5
		{0x04, 0xFE, 0x03, 0x22, 0x00, 0x22},  // Code for char F 6
		{0x04, 0xFC, 0x01, 0x02, 0x02, 0x42},  // Code for char G 7
		{0x04, 0xFE, 0x03, 0x20, 0x00, 0x20},  // Code for char H 8
		{0x04, 0x00, 0x00, 0x02, 0x02, 0xFE},  // Code for char I 9 
		{0x04, 0x80, 0x01, 0x02, 0x02, 0xFE},  // Code for char J 10
		{0x04, 0xFE, 0x03, 0x20, 0x00, 0xD8},  // Code for char K 11
		{0x04, 0xFE, 0x03, 0x00, 0x02, 0x00},  // Code for char L 12 
		{0x04, 0xFE, 0x03, 0x18, 0x00, 0x18},  // Code for char M 13
		{0x04, 0xFE, 0x03, 0x18, 0x00, 0x60},  // Code for char N 14
		{0x04, 0xFC, 0x01, 0x02, 0x02, 0x02},  // Code for char O 15
		{0x04, 0xFE, 0x03, 0x22, 0x00, 0x22},  // Code for char P 16
		{0x04, 0xFC, 0x01, 0x02, 0x02, 0x02},  // Code for char Q 17
		{0x04, 0xFE, 0x03, 0x62, 0x00, 0xA2},  // Code for char R 18
		{0x04, 0x9C, 0x01, 0x22, 0x02, 0x22},  // Code for char S 19
		{0x05, 0x02, 0x00, 0x02, 0x00, 0xFE},  // Code for char T 20
		{0x04, 0xFE, 0x01, 0x00, 0x02, 0x00},  // Code for char U 21
		{0x04, 0x3E, 0x00, 0xC0, 0x00, 0x00},  // Code for char V 22
		{0x04, 0xFE, 0x03, 0x80, 0x00, 0x80},  // Code for char W 23
		{0x04, 0x8E, 0x03, 0x70, 0x00, 0x70},  // Code for char X 24
		{0x05, 0x0E, 0x00, 0x30, 0x00, 0xC0},  // Code for char Y 25
		{0x04, 0x82, 0x03, 0x62, 0x02, 0x1A}  // Code for char Z 26
};

void DDA(double x1, double y1, double x2, double y2) {
	double x;
	double y, slope = ((y2 - y1) / (x2 - x1))/100;
	x = x1;
	y = y1 + 0.005;
	while (x <= x2) {
		glVertex2d(x, floor(y*100)/100);
		y += slope;
		x+= 0.01;
	}
}

void Bresenhamv1(double x1, double y1, double x2, double y2) {
	double x = x1, y = y1;
	glVertex2d(x, y);
	double dx = (x2 - x1), dy = (y2 - y1);
	double count = dx;
	while (count > 0) {
		count -= 0.01;
		double f = dy * (x + 0.01) - dx * (y + 0.005) - (x1*dy - y1 * dx);
		if (f > 0) {
			y += 0.01;
		}
		x += 0.01;
		glVertex2d(x, y);
	}
}

void Bresenhamv2(double x1, double y1, double x2, double y2) {
	double x = x1, y = y1;
	double dx = (x2 - x1), dy = (y2 - y1);
	double f = dy - dx/0.2;
	glVertex2d(x, y);
	double count = dx;
	while (count > 0) {
		count -= 0.01;
		if (f > 0) {
			y += 0.01;
			f = f + (dy - dx);
		}
		else {
			f = f + dy;
		}
		x += 0.01;
		glVertex2d(x, y);
	}
}

void Bresenhamv3(double x1, double y1, double x2, double y2) {
	double x = x1, y = y1;
	double dx = (x2 - x1), dy = (y2 - y1);
	double e = 0.2*dy - dx, incrE = 0.2 * dy, incrNE = 0.2*dy - 0.2*dx;
	glVertex2d(x, y);
	double count = dx;
	while (count > 0) {
		count -= 0.01;
		double f = dy * (x + 0.01) - dx * (y + 0.005) - (x1*dy - y1 * dx);
		if (f > 0) {
			y += 0.01;
			f = f + incrNE;
		}
		else {
			f = f + incrE;
		}
		x += 0.01;
		glVertex2d(x, y);
	}
}

void setPixel4(double R) {
	glVertex2d(0, R);
	glVertex2d(0, -R);
	glVertex2d(R, 0);
	glVertex2d(-R, 0);
}

void setPixel8(double x, double y) {
	glVertex2d(x, y);
	glVertex2d(-x, y);
	glVertex2d(x, -y);
	glVertex2d(-x, -y);
	glVertex2d(y, x);
	glVertex2d(y, -x);
	glVertex2d(-y, x);
	glVertex2d(-y, -x);
}

void BresenhamCircle(double R) {
	double x = 0, y = R;
	setPixel4(R);
	while (x <= y) {
		double f = pow(x,2)+pow(y,2)-pow(R,2);
		if (f > 0) {
			y -= 0.01;
		}
		setPixel8(x, y);
		x += 0.01;
	}
}

void printKG() {
	unsigned char *symbol = Terminal5x12[2];
	int W = symbol[0];
	double i = 0;
	while (i<((double)W/10))  {
		for (int j = 0; j < 9; j++) {
			unsigned char Pixel = symbol[(int)(i * 10) + 1];
			int isPixel = (int) Pixel & (int)pow(2, j);
			if (isPixel != 0){
				glVertex2d(i, 0.8 - ((double)j / 10));
			}
		}
		i += 0.1;
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_NORMALIZE);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glPointSize(5);
	glBegin(GL_POINTS);
	/*DDA(0.1, 0.1, 0.3, 0.5);
	glColor3d(1, 0, 0);
	Bresenhamv1(0.1, 0.1, 0.4, 0.7);
	glColor3d(0, 1, 0);
	Bresenhamv2(0.1, 0.1, 0.5, 0.2);
	glColor3d(0, 0, 1);
	Bresenhamv3(0.1, 0.1, 0.55, 0.25);
	glColor3d(1, 0, 1);
	BresenhamCircle(0.3);*/
	printKG();
	glEnd();
	glPopMatrix();
	glFlush();
}

int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Ex 2.1");
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}