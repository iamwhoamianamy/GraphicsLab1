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

   void Append(Point p)
   {
      points.push_back(p);
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
};