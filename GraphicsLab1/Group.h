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

	void CalcCenter(); // ���������� ��������������� ������ ������
	void ChoseNextActivePoint(); // ������� ���������� ��������� �������� ����� ������
	void ChosePrevActivePoint();  // ������� ���������� ���������� �������� ����� ������
	void AddPoint(Point point); // ���������� ����� � ������
	void DeletePoint();  // �������� ����� �� ������
	void Draw(); // ��������� ����� � ������������ � ��������� ����������
	void Rotate(const double& angle, Vec2 around);
	void AddColour(const GLubyte& r, const GLubyte& g, const GLubyte& b);
	void DrawCenter(); // ��������� ������ ������
	void DrawCasing(); // ��������� ����� ��� �������� �����
	void Move(const double& x, const double& y);  // ������������ ����� �������
	void Scale(const double& factor, Vec2 from);   // ��������������� ����� ������������ ������
	void MirrorX(); // ��������� ����� �� ��� X ������������ ������
	void MirrorY(); // ��������� ����� �� ��� Y ������������ ������
	void OnEnableSmoothing();
	void OnDisableSmoothing();
};