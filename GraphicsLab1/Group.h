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
   GLubyte R = 0, G = 0, B = 0;
   GLfloat size = 10;
   int active_point = -1;
   bool is_center_active = false;
   bool is_smoothing = false;

   int mode = 0;

   Group()
   {

   }

   // ���������� ��������������� ������ ������
   void CalcCenter()
   {
      center = Point(0, 0);

      for(size_t i = 0; i < points.size(); i++)
         center.loc += points[i].loc;

      center.loc /= points.size();
   }

   // ������� ���������� ��������� �������� ����� ������
   void ChoseNextActivePoint()
   {
      if(!is_center_active)
         if(points.size())
            active_point = (active_point + 1) % points.size();
   }

   // ������� ���������� ���������� �������� ����� ������
   void ChosePrevActivePoint()
   {
      if(!is_center_active)
         if(points.size())
            active_point = (active_point - 1 + points.size()) % points.size();
   }

   // ���������� ����� � ������
   void AddPoint(Point point)
   {
      if(points.size())
         points.insert(points.begin() + active_point + 1, point);
      else
         points.push_back(point);

      ChoseNextActivePoint();
      CalcCenter();
   }

   // �������� ����� �� ������
   void DeletePoint()
   {
      if(points.size())
      {
         points.erase(points.begin() + active_point);
         CalcCenter();
      }

      ChosePrevActivePoint();
   }

   // ��������� ����� � ������������ � ��������� ����������
   void Draw()
   {
      OnEnableSmoothing();

      glColor3ub(R, G, B);
      glPointSize(size);
      glBegin(GLenum(mode));

      for(size_t i = 0; i < points.size(); i++)
         glVertex2i(points[i].loc.x, points[i].loc.y);

      glEnd();

      OnDisableSmoothing();
   }

   // ��������� ������ ������
   void DrawCenter()
   {
      glColor3ub(0, 0, 0);
      glPointSize(size / 2);
      glBegin(GL_POINTS);

      glVertex2i(center.loc.x + size / 2, center.loc.y);
      glVertex2i(center.loc.x - size / 2, center.loc.y);
      glVertex2i(center.loc.x, center.loc.y + size / 2);
      glVertex2i(center.loc.x, center.loc.y - size / 2);
      glVertex2i(center.loc.x, center.loc.y);

      glEnd();
   }

   // ��������� ����� ��� �������� �����
   void DrawCasing()
   {
      glColor3ub(0, 0, 0);
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

   // ������������ ����� ������
   void Move(const double& x, const double& y)
   {
       if (is_center_active)
       {
           for (size_t i = 0; i < points.size(); i++)
               points[i].Move(Vec2(x, y));
           center.loc += Vec2(x, y);
       }
      else if (points.size())
      {
          points[active_point].Move(Vec2(x, y));

          CalcCenter();
      }

   }

   // ������� ����� ������ ������
   void Rotate(const double& angle, Vec2 around)
   {
      if(is_center_active)
         for(size_t i = 0; i < points.size(); i++)
            points[i].Rotate(angle, around);
      else if(points.size())
         points[active_point].Rotate(angle, around);
   }

   // ��������������� ����� ������������ ������
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
       if (is_smoothing)
       {
           glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
           glEnable(GL_BLEND);

           if(mode == 0)
           {
              glEnable(GL_POINT_SMOOTH);
              glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
           }
           else if(mode > 0 && mode < 4)
           {
              glEnable(GL_LINE_SMOOTH);
              glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
           }
           else
           {
              glEnable(GL_POLYGON_SMOOTH);
              glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
           }
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