#include "group.h"


// Вычисление геометрического центра группы
void Group::CalcCenter()
{
    center = Point(0, 0);

    for (size_t i = 0; i < points.size(); i++)
        center.loc += points[i].loc;

    center.loc /= points.size();
}

// Функция вычисления следующей активной точки группы
void Group::ChoseNextActivePoint()
{
    if (!center_mode)
        if (points.size())
            active_point = (active_point + 1) % points.size();
}

// Функция вычисления предыдущей активной точки группы
void Group::ChosePrevActivePoint()
{
    if (!center_mode)
        if (points.size())
            active_point = (active_point - 1 + points.size()) % points.size();
}

// Добавление точки в группу
void Group::AddPoint(Point point)
{
    if (points.size() == 0)
        points.push_back(point);
    else if (points.size() == 1)
    {
        points.insert(points.begin() + active_point + 1, point);
        CalcCenter();
    }
    else
        points.insert(points.begin() + active_point + 1, point);

    ChoseNextActivePoint();
}

// Удаление точки из группы
void Group::DeletePoint()
{
    if (points.size())
        points.erase(points.begin() + active_point);

    ChosePrevActivePoint();
}

// Отрисовка точек в соответствии с выбранным алгоритмом
void Group::Draw()
{
    OnEnableSmoothing();
    glPointSize(size);

    glBegin(GLenum(render_mode));

    for (size_t i = 0; i < points.size(); i++)
    {
        glColor3ub(points[i].r, points[i].g, points[i].b);
        glVertex2i(points[i].loc.x, points[i].loc.y);
    }

    glEnd();

    OnDisableSmoothing();
}

void Group::AddColour(const GLubyte& r, const GLubyte& g, const GLubyte& b)
{
    if (center_mode)
        for (size_t i = 0; i < points.size(); i++)
            points[i].AddColour(r, g, b);
    else if (points.size())
        points[active_point].AddColour(r, g, b);
}

// Отрисовка центра группы
void Group::DrawCenter()
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

// Отрисовка рамок для активных точек
void Group::DrawCasing()
{
    glColor3ub(0, 0, 0);
    glPointSize(size);

    for (size_t i = 0; i < points.size(); i++)
        points[i].DrawCasing(size * 1.2);

    if (points.size())
    {
        if (center_mode == 1)
            center.DrawCasing(size * 1.5);
        else if (center_mode == 2)
        {
            center.DrawCasing(size * 1.5);
            center.DrawCasing(size * 2.0);
        }
        else
            points[active_point].DrawCasing(size * 1.5);
    }
}

// Передвижение точек группы
void Group::Move(const double& x, const double& y)
{
    if (center_mode == 1)
    {
        for (size_t i = 0; i < points.size(); i++)
            points[i].Move(Vec2(x, y));

        center.loc += Vec2(x, y);
    }
    else if (center_mode == 2)
        center.loc += Vec2(x, y);
    else if (points.size())
        points[active_point].Move(Vec2(x, y));
}

// Поворот точек вокруг центра
void Group::Rotate(const double& angle, Vec2 around)
{
    if (center_mode)
        for (size_t i = 0; i < points.size(); i++)
            points[i].Rotate(angle, around);
    else if (points.size())
        points[active_point].Rotate(angle, around);
}

// Масштабирование точек относительно центра
void Group::Scale(const double& factor, Vec2 from)
{
    if (center_mode)
        for (size_t i = 0; i < points.size(); i++)
            points[i].Scale(factor, from);
    else if (points.size())
        points[active_point].Scale(factor, from);
}

// Отражение точек по оси X относительно центра
void Group::MirrorX()
{
    if (center_mode)
        for (size_t i = 0; i < points.size(); i++)
            points[i].loc.x += (center.loc.x - points[i].loc.x) * 2;
    else if (points.size())
        points[active_point].loc.x += (center.loc.x - points[active_point].loc.x) * 2;
}

// Отражение точек по оси Y относительно центра
void Group::MirrorY()
{
    if (center_mode)
        for (size_t i = 0; i < points.size(); i++)
            points[i].loc.y += (center.loc.y - points[i].loc.y) * 2;
    else if (points.size())
        points[active_point].loc.y += (center.loc.y - points[active_point].loc.y) * 2;
}

void Group::OnEnableSmoothing()
{
    if (is_smoothing)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        if (render_mode == 0)
        {
            glEnable(GL_POINT_SMOOTH);
            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        }
        else if (render_mode > 0 && render_mode < 4)
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

void Group::OnDisableSmoothing()
{
    glDisable(GL_BLEND);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
}