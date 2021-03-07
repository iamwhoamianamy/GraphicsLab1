#include <iostream>
#include <string>
#include "glut.h"
#include "Group.h"
#include "Point.h"
#include "Vec2.h"
#include <Windows.h>

using namespace std;

int WIDTH = 250, HEIGHT = 250;
int active_group = -1;
bool is_action = false;
vector<Group> groups;

enum keys { Empty, KeyR, KeyG, KeyB, KeyW, KeyA, KeyS, KeyD, KeyQ, KeyE,
    KeyZ, KeyX, KeyC, KeyV, KeyF, KeyT, KeyH, KeyPlus, KeyMinus,
    KeyUp, KeyDown, KeyLeft, KeyRight
};

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
   glClearColor(255, 255, 255, 1);
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
           {
               if (groups[active_group].points.size() > 1)
                   groups[active_group].is_center_active = !groups[active_group].is_center_active;
               groups[active_group].CalcCenter();
           }
           break;

           // Выбор режима отрисовки
       case 't':
           if (groups.size())
               groups[active_group].mode = (groups[active_group].mode + 1) % 10;
           break;

       case 'h':
           if (groups.size())
               if (groups[active_group].is_smoothing && groups[active_group].counter > 3)
               {
                   //groups[active_group].OnDisableSmoothing();
                   groups[active_group].counter = 0;
               }
               else 
               {
                   groups[active_group].counter = (groups[active_group].counter + 1) % 4;
                   cout << groups[active_group].counter;
                   groups[active_group].is_smoothing = true;
                   //groups[active_group].OnEnableSmoothing(counter);
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
   if(button == GLUT_MIDDLE_BUTTON)
   {
      if(groups.size())
          groups[active_group].DeletePoint();
   }

   glutPostRedisplay();
}

void Menu(int pos)
{
    int key = (keys)pos;

    switch (key)
    {
    case KeyR: KeyboardLetters('r', 0, 0); break;
    case KeyG: KeyboardLetters('g', 0, 0); break;
    case KeyB: KeyboardLetters('b', 0, 0); break;

    case KeyW: KeyboardLetters('w', 0, 0); break;
    case KeyS: KeyboardLetters('s', 0, 0); break;
    case KeyA: KeyboardLetters('a', 0, 0); break;
    case KeyD: KeyboardLetters('d', 0, 0); break;

    case KeyQ: KeyboardLetters('q', 0, 0); break;
    case KeyE: KeyboardLetters('e', 0, 0); break;

    case KeyC: KeyboardLetters('c', 0, 0); break;
    case KeyV: KeyboardLetters('v', 0, 0); break;

    case KeyZ: KeyboardLetters('z', 0, 0); break;
    case KeyX: KeyboardLetters('x', 0, 0); break;

    case KeyF: KeyboardLetters('f', 0, 0); break;
    case KeyT: KeyboardLetters('t', 0, 0); break;
    case KeyH: KeyboardLetters('h', 0, 0); break;

    case KeyPlus: KeyboardLetters('=', 0, 0); break;
    case KeyMinus: KeyboardLetters('-', 0, 0); break;

    case KeyLeft: KeyboardSpecials(GLUT_KEY_LEFT, 0, 0); break;
    case KeyRight: KeyboardSpecials(GLUT_KEY_RIGHT, 0, 0); break;
    case KeyUp: KeyboardSpecials(GLUT_KEY_UP, 0, 0); break;
    case KeyDown: KeyboardSpecials(GLUT_KEY_DOWN, 0, 0); break;

    default:
        int menu_color = glutCreateMenu(Menu);
        glutAddMenuEntry("Компонента R", KeyR);
        glutAddMenuEntry("Компонента G", KeyG);
        glutAddMenuEntry("Компонента B", KeyB);

        int menu_move = glutCreateMenu(Menu);
        glutAddMenuEntry("Вверх", KeyW);
        glutAddMenuEntry("Вниз", KeyS);
        glutAddMenuEntry("Bлево", KeyA);
        glutAddMenuEntry("Вправо", KeyD);

        int menu_size = glutCreateMenu(Menu);
        glutAddMenuEntry("Увеличить", KeyC);
        glutAddMenuEntry("Уменьшить", KeyV);

        int menu_scale = glutCreateMenu(Menu);
        glutAddMenuEntry("Расстянуть", KeyZ);
        glutAddMenuEntry("Сжать", KeyX);

        int menu_rotate = glutCreateMenu(Menu);
        glutAddMenuEntry("Повернуть против часовой", KeyQ);
        glutAddMenuEntry("Повернуть по часовой", KeyE);

        int menu_group = glutCreateMenu(Menu);
        glutAddMenuEntry("Добавить группу", KeyPlus);
        glutAddMenuEntry("Удалить группу", KeyMinus);

        int menu_misc = glutCreateMenu(Menu);
        glutAddMenuEntry("Выбрать центр", KeyF);
        glutAddMenuEntry("Переключение режима отрисовки", KeyT);
        glutAddMenuEntry("Переключение режима сглаживания", KeyH);

        int menu_select = glutCreateMenu(Menu);
        glutAddMenuEntry("Выбрать следующую вершину группы", KeyRight);
        glutAddMenuEntry("Выбрать предыдущую вершину группы", KeyLeft);
        glutAddMenuEntry("Выбрать следующую группу", KeyUp);
        glutAddMenuEntry("Выбрать предыдущую группу", KeyDown);

        int menu = glutCreateMenu(Menu);
        glutAddSubMenu("Смена цвета", menu_color);
        glutAddSubMenu("Изменение размера точки", menu_size);
        glutAddSubMenu("Перемещение", menu_move);
        glutAddSubMenu("Масштабирование", menu_scale);
        glutAddSubMenu("Вращение", menu_rotate);
        glutAddSubMenu("Управление группами", menu_group);
        glutAddSubMenu("Переключение между точками и группами", menu_select);
        glutAddSubMenu("Дополнительные", menu_misc);

        glutAttachMenu(GLUT_RIGHT_BUTTON);
        KeyboardLetters(Empty, 0, 0);
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("КГ Лабораторная работа №1");
    Menu(Empty);
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(KeyboardLetters);
    glutSpecialFunc(KeyboardSpecials);
    glutMouseFunc(Mouse);


    glutMainLoop();
}