#pragma once
class Vec2
{
public:

   double x, y;

   Vec2()
   {
      x = 0;
      y = 0;
   }

   Vec2(const double& t_x, const double& t_y)
   {
      x = t_x;
      y = t_y;
   }

   Vec2(const Vec2& vec)
   {
      x = vec.x;
      y = vec.y;
   }

   Vec2& operator +=(const Vec2& rhs)
   {
      x += rhs.x;
      y += rhs.y;
      return *this;
   }

   friend Vec2 operator +(Vec2 lhs, const Vec2& rhs)
   {
      lhs += rhs;
      return lhs;
   }

   friend Vec2 operator -(Vec2 lhs, const Vec2& rhs)
   {
      lhs -= rhs;
      return lhs;
   }

   Vec2& operator *=(const double& rhs)
   {
      x *= rhs;
      y *= rhs;
      return *this;
   }

   Vec2& operator /=(const double& rhs)
   {
      x /= rhs;
      y /= rhs;
      return *this;
   }

   Vec2& operator -=(const Vec2& rhs)
   {
      x -= rhs.x;
      y -= rhs.y;
      return *this;
   }

   friend Vec2 operator *(Vec2 lhs, const double& rhs)
   {
      lhs *= rhs;
      return lhs;
   }

   friend Vec2 operator /(Vec2 lhs, const double& rhs)
   {
      lhs /= rhs;
      return lhs;
   }
};