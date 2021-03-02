#include <iostream>
#include <string>
#include "glut.h"
#include "Group.h"
#include "Point.h"
#include "Vec2.h"

using namespace std;

int WIDTH = 800, HEIGHT = 800;
int active = -1;
GLubyte PointSize = 50;
vector<Group> groups;

// Функция для отрисовки информации
void DrawString(int x, int y, GLubyte col, string s)
{
   glColor3f(col, col, col);

   glPushMatrix();
   glTranslatef(x, y, 0);

   double fac = 0.15;
   glScalef(fac, fac, fac);

   for(size_t i = 0; i < s.size(); i++)
      glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);

   glPopMatrix();
}

// Функция вывода на экран 
void Display(void)
{
   glClearColor(0, 0, 0, 1);
   glClear(GL_COLOR_BUFFER_BIT);

   for(size_t i = 0; i < groups.size(); i++)
   {
      groups[i].Draw();
   }

   if(active >= 0)
      groups[active].DrawCasing();

   string s = "Total groups: " + to_string(groups.size());
   DrawString(10, 30, 200, s);

   s = active >=0 ? "Current group: " + to_string(active + 1) : "Current group: none";
   DrawString(10, 10, 200, s);

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

// Функция обработки сообщений от клавиатуры 1
void KeyboardLetters(unsigned char key, int x, int y)
{
   double speed = 5;

   switch(key)
   {
      // Передвижение точек по осям
   case 'w': groups[active].Move(speed * +0, speed * +1); break;
   case 'a': groups[active].Move(speed * -1, speed * +0); break;
   case 's': groups[active].Move(speed * +0, speed * -1); break;
   case 'd': groups[active].Move(speed * +1, speed * +0); break;
   }

   glutPostRedisplay();
}

// Функция обработки сообщений от клавиатуры 2
void KeyboardSpecials(int key, int x, int y)
{
   double speed = 5;

   switch(key)
   {
      // Переключение между активными группами
   case GLUT_KEY_LEFT: active = (active + 1) % groups.size(); break;
   case GLUT_KEY_RIGHT: active = (active - 1 + groups.size()) % groups.size(); break;

      // Добавление и удаление групп
   case GLUT_KEY_UP: groups.push_back(Group()); active = groups.size() - 1; break;
   case GLUT_KEY_DOWN: if(groups.size()) groups.erase(groups.begin() + active); active = groups.size() - 1; break;
   }

   glutPostRedisplay();
}

// Функция обработки сообщения от мыши
void Mouse(int button, int state, int x, int y)
{
   // Клавиша была нажата, но не отпущена
   if(state != GLUT_DOWN) return;

   // Новая точка по левому клику
   if(button == GLUT_LEFT_BUTTON)
   {
      if(!groups.size())
      {
         groups.push_back(Group());
         active = 0;
      }

      groups[active].points.push_back(Point(x, HEIGHT - y));
   }
   
   // Удаление последней точки по центральному клику
   if(button == GLUT_MIDDLE_BUTTON)
   {
      if(groups[active].points.size())
         groups[active].points.pop_back();
   }

   glutPostRedisplay();
}

int main(int argc, char** argv)
{
   //groups[0].Append(Point(WIDTH / 2, HEIGHT / 2));


   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB);
   glutInitWindowSize(WIDTH, HEIGHT);
   glutCreateWindow("Пожилое приложение");

   glutDisplayFunc(Display);
   glutReshapeFunc(Reshape);
   glutKeyboardFunc(KeyboardLetters);
   glutSpecialFunc(KeyboardSpecials);
   glutMouseFunc(Mouse);


   glutMainLoop();
}