#pragma once
#include <vector>
#include "glut.h"
#include "Point.h"

using namespace std;

class Group
{
public:
   vector<Point> points;
   Point center;
   GLubyte R = 255, G = 255, B = 255;
   GLfloat size = 10;
   int active_point = -1;
   bool is_center_active = false;

   int mode = 0;

   Group()
   {

   }

   // Вычисление геометрического центра группы
   void CalcCenter()
   {
      center = Point(0, 0);

      for(size_t i = 0; i < points.size(); i++)
         center.loc += points[i].loc;

      center.loc /= points.size();
   }

   void ChoseNextActivePoint()
   {
      if(!is_center_active)
         if(points.size())
            active_point = (active_point + 1) % points.size();
   }

   void ChosePrevActivePoint()
   {
      if(!is_center_active)
         if(points.size())
            active_point = (active_point - 1 + points.size()) % points.size();
   }

   // Добавление точки в группу
   void AddPoint(Point point)
   {
      if(points.size())
         points.insert(points.begin() + active_point + 1, point);
      else
         points.push_back(point);

      ChoseNextActivePoint();
      CalcCenter();
   }

   // Удаление точки из группы
   void DeletePoint()
   {
      if(points.size())
      {
         points.erase(points.begin() + active_point);
         CalcCenter();
      }

      ChosePrevActivePoint();

      /*if(points.size())
         active_point = points.size() - 1;
      else
         active_point = -1;*/
   }

   // Отрисовка точек в соответствии с выбранным алгоритмом
   void Draw()
   {
      glColor3ub(R, G, B);
      glPointSize(size);
      glBegin(GLenum(mode));
      cout << mode << endl;

      for(size_t i = 0; i < points.size(); i++)
         glVertex2i(points[i].loc.x, points[i].loc.y);

      glEnd();
   }


   void DrawCenter()
   {
      glColor3ub(255, 255, 255);
      glPointSize(size / 2);
      glBegin(GL_POINTS);

      glVertex2i(center.loc.x + size / 2, center.loc.y);
      glVertex2i(center.loc.x - size / 2, center.loc.y);
      glVertex2i(center.loc.x, center.loc.y + size / 2);
      glVertex2i(center.loc.x, center.loc.y - size / 2);
      glVertex2i(center.loc.x, center.loc.y);

      glEnd();
   }

   void DrawCasing()
   {
      glColor3ub(255, 255, 255);
      glPointSize(size);

      for(size_t i = 0; i < points.size(); i++)
         points[i].DrawCasing(size * 1.2);

      if(points.size())
      {
         if(is_center_active)
            center.DrawCasing(size * 1.5);
         else
            points[active_point].DrawCasing(size * 1.5);
      }
   }

   void Move(const double& x, const double& y)
   {
      if(is_center_active)
         for(size_t i = 0; i < points.size(); i++)
            points[i].Move(Vec2(x, y));
      else if(points.size())
         points[active_point].Move(Vec2(x, y));

      CalcCenter();
   }

   /*void Translate(Vec2 around)
   {
      for(size_t i = 0; i < points.size(); i++)
         points[i].loc += Vec2(around.x, around.y);
   }*/

   void Rotate(const double& angle, Vec2 around)
   {
      if(is_center_active)
         for(size_t i = 0; i < points.size(); i++)
            points[i].Rotate(angle, around);
      else if(points.size())
         points[active_point].Rotate(angle, around);
   }

   void Scale(const double& factor, Vec2 from)
   {
      if(is_center_active)
         for(size_t i = 0; i < points.size(); i++)
            points[i].Scale(factor, from);
      else if(points.size())
         points[active_point].Scale(factor, from);
   }

   void OnEnableSmoothing()
   {
       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       glEnable(GL_BLEND);

       switch (mode)
       {
       case(0):
           glEnable(GL_POINT_SMOOTH);
           glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
       case(1):
       case(2):
       case(3):
       case(7):
       case(8):
           glEnable(GL_LINE_SMOOTH);
           glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
       case(4):
       case(5):
       case(6):
       case(9):
           glEnable(GL_POLYGON_SMOOTH);
           glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
       default:
           break;
       }
   }

   void OnDisableSmoothing()
   {
       glDisable(GL_BLEND);
       glDisable(GL_POINT_SMOOTH);
       glDisable(GL_LINE_SMOOTH);
       glDisable(GL_POLYGON_SMOOTH);
   }

};