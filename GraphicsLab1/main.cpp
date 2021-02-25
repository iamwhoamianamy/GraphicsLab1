#include <iostream>
#include "glut.h"
#include "Group.h"
#include "Point.h"
#include "Vec2.h"

using namespace std;

int WIDTH = 400, HEIGHT = 400;
//GLubyte ColorR = 0, ColorG = 0, ColorB = 0;
GLubyte PointSize = 50;
vector<Group> groups(1);

// Функция вывода на экран 
void Display(void)
{
   glClearColor(0, 0, 0, 1);
   glClear(GL_COLOR_BUFFER_BIT);

   for(size_t i = 0; i < groups.size(); i++)
   {
      groups[i].Draw();
   }

   glFinish();

  
}

// Функция изменения размеров окна
void Reshape(GLint w, GLint h)
{
   WIDTH = w;    HEIGHT = h;
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0, w, 0, h, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Функция обработки сообщения от мыши
void Mouse(int button, int state, int x, int y)
{
   /* клавиша была нажата, но не отпущена */
   if(state != GLUT_DOWN) return;

   /* новая точка по левому клику */
   if(button == GLUT_LEFT_BUTTON)
   {
      groups[0].points.push_back(Point(x, HEIGHT - y));
   }
   /* удаление последней точки по центральному клику */
   if(button == GLUT_MIDDLE_BUTTON)
   {
      if(groups[0].points.size())
         groups[0].points.pop_back();
   }

   glutPostRedisplay();
}

int main(int argc, char** argv)
{

   groups[0].Append(Point(100, 200));
   groups[0].Append(Point(200, 100));


   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB);
   glutInitWindowSize(WIDTH, HEIGHT);
   glutCreateWindow("Пожилое приложение");

   glutDisplayFunc(Display);
   glutReshapeFunc(Reshape);
   glutMouseFunc(Mouse);

   glutMainLoop();

}