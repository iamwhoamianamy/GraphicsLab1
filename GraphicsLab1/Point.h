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
};