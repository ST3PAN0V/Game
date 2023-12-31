#include "classes.h"
#include <cctype>

#define FPS 10

extern short sDirection;
extern int matr;

void display_callback()
{
    glClear(GL_COLOR_BUFFER_BIT);
    playGame();
    glutSwapBuffers();
}

void reshape_callback(int width, int height)
{
    glViewport(0,0,(GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer_callback(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000/FPS, timer_callback, 0);
}

void keyboard_callback(unsigned char key, int, int)
{
    switch (key)
    {
    case 'w':
        sDirection=UP;
        break;
    case 's':
        sDirection=DOWN;
        break;
    case 'd':
        sDirection=RIGHT;
        break;
    case 'a':
        sDirection=LEFT;
        break;
    default:
        sDirection = STAY;
    }
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(COLUMNS*20, ROWS*20);
    glutCreateWindow("Game");
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutTimerFunc(0, timer_callback, 0);
    glutKeyboardFunc(keyboard_callback);
    glutMainLoop();
    return 0;
}

