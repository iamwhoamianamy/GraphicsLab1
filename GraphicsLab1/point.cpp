#include "point.h"
#include <math.h>


void Point:: DrawCasing(const double& size)
{
    double scale = 0.75;
    glBegin(GL_LINE_LOOP);

    glVertex2i(loc.x + size * scale, loc.y + size * scale);
    glVertex2i(loc.x - size * scale, loc.y + size * scale);
    glVertex2i(loc.x - size * scale, loc.y - size * scale);
    glVertex2i(loc.x + size * scale, loc.y - size * scale);

    glEnd();
}

void Point:: Move(Vec2 t_loc)
{
    loc += Vec2(t_loc.x, t_loc.y);
}

void Point:: Rotate(const double& angle, Vec2 around)
{
    Move(around * -1);

    loc.x = loc.x * cos(angle) - loc.y * sin(angle);
    loc.y = loc.x * sin(angle) + loc.y * cos(angle);

    Move(around);
}

void Point:: Scale(const double& factor, Vec2 from)
{
    Move(from * -1);

    loc *= factor;

    Move(from);
}

void Point:: AddColour(const GLubyte& t_r, const GLubyte& t_g, const GLubyte& t_b)
{
    r += t_r;
    g += t_g;
    b += t_b;
}