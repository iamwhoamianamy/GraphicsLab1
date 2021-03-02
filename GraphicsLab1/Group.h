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
   GLubyte R = 200, G = 200, B = 200;
   GLfloat size = 10;

   Group()
   {

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
   }
};