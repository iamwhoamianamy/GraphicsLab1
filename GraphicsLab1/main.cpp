#include <iostream>
#include "glut.h"

const int WIDTH = 400, HEIGHT = 400;
GLubyte ColorR = 0, ColorG = 0, ColorB = 0;
GLubyte PointSize = 5;

// Функция вывода на экран 
void Display(void)
{
   glClearColor(0.5, 0.5, 0.5, 1); glClear(GL_COLOR_BUFFER_BIT);
   glColor3ub(ColorR, ColorG, ColorB);
   glPointSize(PointSize);

   glFinish();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB);
   glutInitWindowSize(WIDTH, HEIGHT);
   glutCreateWindow("Пожилое приложение");

   glutDisplayFunc(Display);

   glutMainLoop();
}