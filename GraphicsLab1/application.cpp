#include "application.h"


// ������� ��� ��������� ������
void Application::DrawString(int x, int y, GLubyte col, string s)
{
    glColor3f(col, col, col);

    glPushMatrix();
    glTranslatef(x, y, 0);

    double fac = 0.15;
    glScalef(fac, fac, fac);

    for (size_t i = 0; i < s.size(); i++)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, s[i]);

    glPopMatrix();
}

// ������� ������ �� ����� 
void Application::Display()
{
    glClearColor(255, 255, 255, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    // ��������� ����� ���� �����
    for (size_t i = 0; i < groups.size(); i++)
        groups[i].Draw();

    // ��������� ���������� �������� ������
    if (groups.size())
    {
        groups[active_group].DrawCasing();

        if (groups[active_group].points.size() > 1)
            groups[active_group].DrawCenter();
    }

    // ��������� ���������� � ������� � �� ��������
    string s;
    s = "Total groups: " + to_string(groups.size());
    DrawString(10, 70, 0, s);
    s = active_group >= 0 ? "Current group: " + to_string(active_group + 1) : "Current group: none";
    DrawString(10, 50, 0, s);

    if (groups.size())
    {
        s = "Points in group: " + to_string(groups[active_group].points.size());
        DrawString(10, 30, 0, s);

        if (groups[active_group].points.size())
        {
            if (groups[active_group].center_mode)
                s = "Current point: center";
            else
                s = "Current point: " + to_string(groups[active_group].active_point + 1);

            DrawString(10, 10, 0, s);
        }
    }

    glFinish();
}

// ������� ��������� �������� ����
void Application::Reshape(GLint w, GLint h)
{
    WIDTH = w;    HEIGHT = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// ������� ��������� ��������� �� ���������� 1
void Application::KeyboardLetters(unsigned char key, int x, int y)
{
    double move_speed = 5;
    double colour_speed = 5;
    double size_speed = 1;
    double angle = 0.02;
    double scale = 1.05;

    if (groups.size())
    {
        Group* g = &groups[active_group];

        switch (key)
        {
            // ������������ ����� �� ����
        case 'w': g->Move(move_speed * +0, move_speed * +1); break;
        case 'a': g->Move(move_speed * -1, move_speed * +0); break;
        case 's': g->Move(move_speed * +0, move_speed * -1); break;
        case 'd': g->Move(move_speed * +1, move_speed * +0); break;

            // �������� �����
        case 'q': g->Rotate(angle, groups[active_group].center.loc);
            break;
        case 'e': g->Rotate(-angle, groups[active_group].center.loc);
            break;

            // ��������������� �����
        case 'z': g->Scale(scale, groups[active_group].center.loc); break;
        case 'x': g->Scale(1 / scale, groups[active_group].center.loc); break;

            // ��������� ����� �����
        case 'r': g->AddColour(colour_speed, 0, 0); break;
        case 'g': g->AddColour(0, colour_speed, 0); break;
        case 'b': g->AddColour(0, 0, colour_speed); break;

            // ��������� ������� �����
        case 'c': g->size += size_speed; break;
        case 'v': g->size -= size_speed; break;

            // ������������ ������ �������������� ������
        case 'f':
            if (g->points.size() > 1)
                g->center_mode = (g->center_mode + 1) % 3;
            break;

            // �������� ��������������� ������ ������
        case ' ':
            g->CalcCenter();
            break;

            // ����� ������ ���������
        case 't':
            if (groups.size())
                g->render_mode = (g->render_mode + 1) % 10;
            break;

            // ������������ ����������� ���������
        case 'h':
            g->is_smoothing = !g->is_smoothing;
            break;

            // ��������� �����
        case 'k':
            g->MirrorX();
            break;

        case 'l':
            g->MirrorY();
            break;
        }
    }


    switch (key)
    {
        // ���������� � �������� �����
    case '=':
        groups.push_back(Group());
        active_group = groups.size() - 1;
        break;
    case '-':
        if (groups.size())
            groups.erase(groups.begin() + active_group);
        active_group = groups.size() - 1;
        break;

    }
    glutPostRedisplay();
}

// ������� ��������� ��������� �� ���������� 2
void Application::KeyboardSpecials(int key, int x, int y)
{
    double speed = 5;

    switch (key)
    {
        // ������������ ����� ��������� ��������
    case GLUT_KEY_UP:
        active_group = (active_group + 1) % groups.size();
        if (groups.size())
            groups[active_group].center_mode = 1;
        break;
    case GLUT_KEY_DOWN:
        active_group = (active_group - 1 + groups.size()) % groups.size();
        if (groups.size())
            groups[active_group].center_mode = 1;
        break;

        // ������������ ����� ������� �������� ������
    case GLUT_KEY_RIGHT:
        if (active_group != -1)
            groups[active_group].ChoseNextActivePoint();
        break;
    case GLUT_KEY_LEFT:
        if (active_group != -1)
            groups[active_group].ChosePrevActivePoint();
        break;
    }

    glutPostRedisplay();
}

// ������� ��������� ��������� �� ����
void Application::Mouse(int button, int state, int x, int y)
{
    // ������� ���� ������, �� �� ��������
    if (state != GLUT_DOWN) return;

    // ����� ����� �� ������ �����
    if (button == GLUT_LEFT_BUTTON)
    {
        if (!groups.size())
        {
            groups.push_back(Group());
            active_group = 0;
        }

        groups[active_group].AddPoint(Point((double)x, (double)(HEIGHT - y)));
    }

    // �������� ��������� ����� �� ������� �����
    if (button == GLUT_MIDDLE_BUTTON)
    {
        if (groups.size())
            groups[active_group].DeletePoint();
    }

    glutPostRedisplay();
}