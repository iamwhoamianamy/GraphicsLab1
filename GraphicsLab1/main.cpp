#include <iostream>
#include <string>
#include "glut.h"
#include "Group.h"
#include "Point.h"
#include "Vec2.h"
#include <Windows.h>

using namespace std;

int WIDTH = 800, HEIGHT = 800;
int active_group = -1;
bool is_action = false;
vector<Group> groups;

enum keys { Empty, KeyR, KeyG, KeyB, KeyW, KeyA, KeyS, KeyD, KeyQ, KeyE,
    KeyZ, KeyX, KeyC, KeyV, KeyF, KeyT, KeyH, KeyK, KeyL, KeyPlus, KeyMinus,
    KeyUp, KeyDown, KeyLeft, KeyRight, KeySpace
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
   {
      groups[active_group].DrawCasing();

      if(groups[active_group].points.size() > 1)
         groups[active_group].DrawCenter();
   }
   
   // Отрисовка информации о группах и их вершинах
   string s;
   s = "Total groups: " + to_string(groups.size());
   DrawString(10, 70, 0, s);
   s = active_group >=0 ? "Current group: " + to_string(active_group + 1) : "Current group: none";
   DrawString(10, 50, 0, s);

   if(groups.size())
   {
      s = "Points in group: " + to_string(groups[active_group].points.size());
      DrawString(10, 30, 0, s);

      if(groups[active_group].points.size())
      {
         if(groups[active_group].center_mode)
            s = "Current point: center";
         else
            s = "Current point: " + to_string(groups[active_group].active_point + 1);
         
         DrawString(10, 10, 0, s);
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
   double colour_speed = 5;
   double size_speed = 1;
   double angle = 0.02;
   double scale = 1.05;

   if(groups.size())
   {
      Group* g = &groups[active_group];

      switch(key)
      {
         // Передвижение точек по осям
      case 'w': g->Move(move_speed * +0, move_speed * +1); break;
      case 'a': g->Move(move_speed * -1, move_speed * +0); break;
      case 's': g->Move(move_speed * +0, move_speed * -1); break;
      case 'd': g->Move(move_speed * +1, move_speed * +0); break;

         // Вращение точек
      case 'q': g->Rotate(angle, groups[active_group].center.loc);
         break;
      case 'e': g->Rotate(-angle, groups[active_group].center.loc);
         break;

         // Масштабирование точек
      case 'z': g->Scale(scale, groups[active_group].center.loc); break;
      case 'x': g->Scale(1 / scale, groups[active_group].center.loc); break;

         // Изменение цвета точек
      case 'r': g->AddColour(colour_speed, 0, 0); break;
      case 'g': g->AddColour(0, colour_speed, 0); break;
      case 'b': g->AddColour(0, 0, colour_speed); break;

         // Изменение размера точек
      case 'c': g->size += size_speed; break;
      case 'v': g->size -= size_speed; break;

         // Переключение режима редактирования группы
      case 'f':
         if(g->points.size() > 1)
            g->center_mode = (g->center_mode + 1) % 3;
         break;

         // Пересчет геометрического центра группы
      case ' ':
         g->CalcCenter();
         break;

         // Выбор режима отрисовки
      case 't':
         if(groups.size())
            g->render_mode = (g->render_mode + 1) % 10;
         break;

         // Переклбчение сглаживания отрисовки
      case 'h':
         g->is_smoothing = !g->is_smoothing;
         break;

         // Отражение точек
      case 'k':
         g->MirrorX();
         break;

      case 'l':
         g->MirrorY();
         break;
      }
   }


   switch(key)
   {
      // Добавление и удаление групп
   case '=':
      groups.push_back(Group());
      active_group = groups.size() - 1;
      break;
   case '-':
      if(groups.size())
         groups.erase(groups.begin() + active_group);
      active_group = groups.size() - 1;
      break;

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
         groups[active_group].center_mode = 1;
      break;
   case GLUT_KEY_DOWN:
      active_group = (active_group - 1 + groups.size()) % groups.size();
      if(groups.size())
         groups[active_group].center_mode = 1;
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

    case KeyK: KeyboardLetters('k', 0, 0); break;
    case KeyL: KeyboardLetters('l', 0, 0); break;

    case KeyPlus: KeyboardLetters('=', 0, 0); break;
    case KeyMinus: KeyboardLetters('-', 0, 0); break;

    case KeyLeft: KeyboardSpecials(GLUT_KEY_LEFT, 0, 0); break;
    case KeyRight: KeyboardSpecials(GLUT_KEY_RIGHT, 0, 0); break;
    case KeyUp: KeyboardSpecials(GLUT_KEY_UP, 0, 0); break;
    case KeyDown: KeyboardSpecials(GLUT_KEY_DOWN, 0, 0); break;

    case KeyF: KeyboardLetters('f', 0, 0); break;
    case KeyT: KeyboardLetters('t', 0, 0); break;
    case KeyH: KeyboardLetters('h', 0, 0); break;
    case KeySpace: KeyboardLetters(' ', 0, 0); break;

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

        int menu_rotate = glutCreateMenu(Menu);
        glutAddMenuEntry("Повернуть против часовой", KeyQ);
        glutAddMenuEntry("Повернуть по часовой", KeyE);

        int menu_size = glutCreateMenu(Menu);
        glutAddMenuEntry("Увеличить", KeyC);
        glutAddMenuEntry("Уменьшить", KeyV);

        int menu_scale = glutCreateMenu(Menu);
        glutAddMenuEntry("Расстянуть", KeyZ);
        glutAddMenuEntry("Сжать", KeyX);

        int menu_mirror = glutCreateMenu(Menu);
        glutAddMenuEntry("Отразить по X", KeyK);
        glutAddMenuEntry("Отразить по Y", KeyL);

        int menu_group = glutCreateMenu(Menu);
        glutAddMenuEntry("Добавить группу", KeyPlus);
        glutAddMenuEntry("Удалить группу", KeyMinus);

        int menu_select = glutCreateMenu(Menu);
        glutAddMenuEntry("Выбрать следующую вершину группы", KeyRight);
        glutAddMenuEntry("Выбрать предыдущую вершину группы", KeyLeft);
        glutAddMenuEntry("Выбрать следующую группу", KeyUp);
        glutAddMenuEntry("Выбрать предыдущую группу", KeyDown);

        int menu_misc = glutCreateMenu(Menu);
        glutAddMenuEntry("Выбрать центр", KeyF);
        glutAddMenuEntry("Переключение режима отрисовки", KeyT);
        glutAddMenuEntry("Переключение режима сглаживания", KeyH);
        glutAddMenuEntry("Пересчитать центр", KeySpace);

        int menu = glutCreateMenu(Menu);
        glutAddSubMenu("Смена цвета", menu_color);
        glutAddSubMenu("Перемещение", menu_move);
        glutAddSubMenu("Вращение", menu_rotate);
        glutAddSubMenu("Изменение размера точки", menu_size);
        glutAddSubMenu("Масштабирование", menu_scale);
        glutAddSubMenu("Отражение", menu_mirror);
        glutAddSubMenu("Управление группами", menu_group);
        glutAddSubMenu("Переключение между точками и группами", menu_select);
        glutAddSubMenu("Дополнительно", menu_misc);

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