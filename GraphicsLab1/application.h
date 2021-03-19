#pragma once
#include <iostream>
#include <string>
#include "Group.h"
#include <Windows.h>

class Application
{
public:
    Application() {}

    int WIDTH = 800, HEIGHT = 800;
    int active_group = -1;
    bool is_action = false;
    vector<Group> groups;


    void DrawString(int x, int y, GLubyte col, string s);  // ������� ��� ��������� ������    
    void Display(); // ������� ������ �� ����� 
    void Reshape(GLint w, GLint h); // ������� ��������� �������� ����
    void KeyboardLetters(unsigned char key, int x, int y);  // ������� ��������� ��������� �� ���������� 1
    void KeyboardSpecials(int key, int x, int y); // ������� ��������� ��������� �� ���������� 2
    void Mouse(int button, int state, int x, int y); // ������� ��������� ��������� �� ����
};