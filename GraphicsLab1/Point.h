#pragma once
#include "Vec2.h"

class Point
{
private:


public:
   Vec2 loc;

   Point()
   {
      loc = Vec2();
   }

   Point(const double& t_x, const double& t_y)
   {
      loc = Vec2(t_x, t_y);
   }

   Point(const Vec2& vec)
   {
      loc = vec;
   }
   
   void DrawCasing(const double& size)
   {
      double scale = 0.75;
      glBegin(GL_LINE_LOOP);

      glVertex2i(loc.x + size * scale, loc.y + size * scale);
      glVertex2i(loc.x - size * scale, loc.y + size * scale);
      glVertex2i(loc.x - size * scale, loc.y - size * scale);
      glVertex2i(loc.x + size * scale, loc.y - size * scale);

      glEnd();
   }

   void Move(Vec2 t_loc)
   {
      loc += Vec2(t_loc.x, t_loc.y);
   }

   void Rotate(const double& angle, Vec2 around)
   {
      Move(around * -1);

      loc.x = loc.x * cos(angle) - loc.y * sin(angle);
      loc.y = loc.x * sin(angle) + loc.y * cos(angle);

      Move(around);
   }
};