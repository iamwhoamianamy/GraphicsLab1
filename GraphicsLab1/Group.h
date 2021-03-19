#pragma once
#include <vector>
#include "Point.h"

using namespace std;

class Group
{
public:
   vector<Point> points;
   Point center;
   GLfloat size = 10;
   int active_point = -1;
   int center_mode = 0;
   bool is_smoothing = false;

   int render_mode = 0;

   Group()
   {
   }

	void CalcCenter(); // Вычисление геометрического центра группы
	void ChoseNextActivePoint(); // Функция вычисления следующей активной точки группы
	void ChosePrevActivePoint();  // Функция вычисления предыдущей активной точки группы
	void AddPoint(Point point); // Добавление точки в группу
	void DeletePoint();  // Удаление точки из группы
	void Draw(); // Отрисовка точек в соответствии с выбранным алгоритмом
	void Rotate(const double& angle, Vec2 around);
	void AddColour(const GLubyte& r, const GLubyte& g, const GLubyte& b);
	void DrawCenter(); // Отрисовка центра группы
	void DrawCasing(); // Отрисовка рамок для активных точек
	void Move(const double& x, const double& y);  // Передвижение точек группыа
	void Scale(const double& factor, Vec2 from);   // Масштабирование точек относительно центра
	void MirrorX(); // Отражение точек по оси X относительно центра
	void MirrorY(); // Отражение точек по оси Y относительно центра
	void OnEnableSmoothing();
	void OnDisableSmoothing();
};