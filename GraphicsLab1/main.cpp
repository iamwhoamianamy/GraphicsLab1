#include <iostream>
#include <string>
#include "glut.h"
#include "Group.h"
#include "Point.h"
#include "Vec2.h"
#include <Windows.h>

using namespace std;

int WIDTH = 1680, HEIGHT = 1050;
int active_group = -1;
int counter = 0;
bool is_action = false;
vector<Group> groups;

// Функция для отрисовки строки
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
void Display()
{
   glClearColor(0, 0, 0, 1);
   glClear(GL_COLOR_BUFFER_BIT);

   // Отрисовка точек всех групп
   for(size_t i = 0; i < groups.size(); i++)
      groups[i].Draw();

   // Отрисовка указателей активной группы
   if(groups.size())
      groups[active_group].DrawCasing();

   if(groups.size() && groups[active_group].points.size() > 1)
      groups[active_group].DrawCenter();

   string s;
   s = "Total groups: " + to_string(groups.size());
   DrawString(10, 70, 200, s);
   s = active_group >=0 ? "Current group: " + to_string(active_group + 1) : "Current group: none";
   DrawString(10, 50, 200, s);

   if(active_group != -1)
   {
      s = "Points in group: " + to_string(groups[active_group].points.size());
      DrawString(10, 30, 200, s);
   }
   if(active_group >= 0)
   {
      if(groups[active_group].active_point == -2)
      {
         s = "Current point: center";
         DrawString(10, 10, 200, s);
      }
      else
         if(groups[active_group].active_point != -1)
         {
            s = "Current point: " + to_string(groups[active_group].active_point + 1);
            DrawString(10, 10, 200, s);
         }
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

// Функция обработки сообщений от клавиатуры 1
void KeyboardLetters(unsigned char key, int x, int y)
{
   double move_speed = 5;
   double color_speed = 5;
   double size_speed = 1;
   double angle = 0.02;
   double scale = 1.05;


   if (active_group != -1)
   {
       switch (key)
       {
           // Передвижение точек по осям
       case 'w': groups[active_group].Move(move_speed * +0, move_speed * +1); break;
       case 'a': groups[active_group].Move(move_speed * -1, move_speed * +0); break;
       case 's': groups[active_group].Move(move_speed * +0, move_speed * -1); break;
       case 'd': groups[active_group].Move(move_speed * +1, move_speed * +0); break;

           // Вращение точек
       case 'q': groups[active_group].Rotate(angle, groups[active_group].center.loc);
           break;
       case 'e': groups[active_group].Rotate(-angle, groups[active_group].center.loc);
           break;

           // Масштабирование точек
       case 'z': groups[active_group].Scale(scale, groups[active_group].center.loc); break;
       case 'x': groups[active_group].Scale(1 / scale, groups[active_group].center.loc); break;

           // Изменение цвета точек
       case 'r': groups[active_group].R += color_speed; break;
       case 'g': groups[active_group].G += color_speed; break;
       case 'b': groups[active_group].B += color_speed; break;

           // Изменение размера точек
       case 'c': groups[active_group].size += size_speed; break;
       case 'v': groups[active_group].size -= size_speed; break;

           // Добавление и удаление групп
       case '=':
           groups.push_back(Group());
           active_group = groups.size() - 1;
           break;
       case '-':
           if (groups.size())
               groups.erase(groups.begin() + active_group);
           active_group = groups.size() - 1;
           break;

           // Выбор центральной точки группы
       case 'f':
           if (groups.size())
               if (groups[active_group].points.size() > 1)
                   groups[active_group].is_center_active = !groups[active_group].is_center_active;
           break;

           // Выбор режима отрисовки
       case 't':
           if (groups.size())
               groups[active_group].mode = (groups[active_group].mode + 1) % 10;
           break;

       case 'h':
           if (groups.size())
               if (groups[active_group].is_smoothing && counter > 3)
               {
                   groups[active_group].OnDisableSmoothing();
                   counter = 0;
               }
               else 
               {
                   counter++;
                   groups[active_group].is_smoothing = true;
                   groups[active_group].OnEnableSmoothing(counter);
               }
           break;
       }
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
   case GLUT_KEY_UP:
      active_group = (active_group + 1) % groups.size();
      if(groups.size())
         groups[active_group].is_center_active = true;
      break;
   case GLUT_KEY_DOWN:
      active_group = (active_group - 1 + groups.size()) % groups.size();
      if(groups.size())
         groups[active_group].is_center_active = true;
      break;

      // Переключение между точками активной группы
   case GLUT_KEY_RIGHT: 
      if(active_group != -1)
         groups[active_group].ChoseNextActivePoint();
      break;
   case GLUT_KEY_LEFT:
      if(active_group != -1)
         groups[active_group].ChosePrevActivePoint();
      break;
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
         active_group = 0;
      }

      groups[active_group].AddPoint(Point((double)x, (double)(HEIGHT - y)));
   }
   
   // Удаление последней точки по правому клику
   if(button == GLUT_RIGHT_BUTTON)
   {
      if(groups.size())
          groups[active_group].DeletePoint();
   }

   glutPostRedisplay();
}

int main(int argc, char** argv)
{
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