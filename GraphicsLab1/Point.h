#pragma once
#include "Vec2.h"
#include "glut.h"

class Point
{
private:


public:
   Vec2 loc;
   GLubyte r = 0, g = 0, b = 0;

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
   
   void DrawCasing(const double& size);
   void Move(Vec2 t_loc);
   void Rotate(const double& angle, Vec2 around);
   void Scale(const double& factor, Vec2 from);
   void AddColour(const GLubyte& t_r, const GLubyte& t_g, const GLubyte& t_b);
};