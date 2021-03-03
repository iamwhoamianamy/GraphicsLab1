#pragma once
#include <vector>
#include "glut.h"
#include "Point.h"
#include "Vec2.h"

using namespace std;

class Group
{
public:
   vector<Point> points;
   Point center;
   GLubyte R = 200, G = 200, B = 200;
   GLfloat size = 10;

   Group()
   {

   }

   void CalcCenter()
   {
      center = Point(0, 0);

      for(size_t i = 0; i < points.size(); i++)
         center.loc += points[i].loc;

      center.loc /= points.size();
   }

   void AddPoint(Point point)
   {
      points.push_back(point);
      CalcCenter();
   }

   void DeletePoint()
   {
      if(points.size())
      {
         points.pop_back();
         CalcCenter();
      }
   }

   void Draw()
   {
      glColor3ub(R, G, B);
      glPointSize(size);
      glBegin(GL_POINTS);

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
      glColor3ub(R, G, B);
      glPointSize(size);

      for(size_t i = 0; i < points.size(); i++)
         points[i].DrawCasing(size * 1.2);
   }

   void Move(const double& x, const double& y)
   {
      for(size_t i = 0; i < points.size(); i++)
         points[i].loc += Vec2(x, y);

      CalcCenter();
   }

   void Translate(Vec2 around)
   {
      for(size_t i = 0; i < points.size(); i++)
         points[i].loc += Vec2(around.x, around.y);
   }

   void Rotate(const double& angle, Vec2 around)
   {
      Translate(around * -1);

      for(size_t i = 0; i < points.size(); i++)
      {
         points[i].loc.x = points[i].loc.x * cos(angle) - points[i].loc.y * sin(angle);
         points[i].loc.y = points[i].loc.x * sin(angle) + points[i].loc.y * cos(angle);
      }

      Translate(around);
   }

   void Scale(const double& factor, Vec2 from)
   {
      Translate(from * -1);

      for(size_t i = 0; i < points.size(); i++)
         points[i].loc *= factor;

      Translate(from);
   }
};