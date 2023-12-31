#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include "classes.h"

short sDirection = STAY;
int matr[ROWS][COLUMNS];
int posX=14, posY=1;
bool isOutside = true;
bool lavaShimmer = true;
int forShimmer = 0;
int curHealth = HEALTH;
float curTime = TIME;
int level = 1;
bool openDoor = false;

void createBullet(int x, int y, int discription);
void Gunner(int x, int y, int speed, int discription);
void drawMap();
void setBrick(int x, int y);
void drawHero();
void key(int x, int y, int whichKey);
void door(int x, int y, int whichDoor);
void brick1(int x, int y);
void brick2(int x, int y);
void brick3(int x, int y);
void space3(int x, int y);
void space2(int x, int y);
void space1(int x, int y);
void setspace(int x, int y);
void moveEntity();
void traper(int x, int y);
void lava(int x, int y);
void HP(int Hp);
void timer(float &);
void healthing(int x, int y, int whichHealth);
void clearMap();
void gameOver();
void timeBonus(int x, int y, int whichTime);

class bullet
{
private:
    int X;
    int Y;
    int direction;
    int interval;
    int count=0;
public:
    bullet(int x, int y, int direction) // для обычных пуль
    {
        this->X = x;
        this->Y = y;
        this->direction = direction;
    }
    bullet(int x, int y, int direction, int interval) // для пулемётчиков
    {
        this->X = x;
        this->Y = y;
        this->direction = direction;
        this->interval = interval;
    }
    void wait()
    {
        if (count == interval)
        {
            createBullet(X,Y,direction);
            count = 0;
        }
        else count++;
    }
    bool fly()
    {
        if (direction==UP)
        {
            glPushMatrix();
                glColor3f(1.0, 0.0, 0.0);
                glTranslatef(X+0.5, Y+0.5, 0);
                glutSolidSphere(0.3, 10, 10);
            glPopMatrix();
            matr[ROWS-1-Y][X] = 2;
            if (matr[ROWS-1-Y-1][X] != 1) this->Y += 1;
            else return false;
        }
        else if (direction==DOWN)
        {
            glPushMatrix();
                glColor3f(1.0, 0.0, 0.0);
                glTranslatef(X+0.5, Y+0.5, 0);
                glutSolidSphere(0.3, 10, 10);
            glPopMatrix();
            matr[ROWS-1-Y][X] = 2;
            if (matr[ROWS-1-Y+1][X] != 1) this->Y -= 1;
            else return false;
        }
        else if (direction==RIGHT)
        {
            glPushMatrix();
                glColor3f(1.0, 0.0, 0.0);
                glTranslatef(X+0.5, Y+0.5, 0);
                glutSolidSphere(0.3, 10, 10);
            glPopMatrix();
            matr[ROWS-1-Y][X] = 2;
            if (matr[ROWS-1-Y][X+1] != 1) this->X += 1;
            else return false;
        }
        else
        {
            glPushMatrix();
                glColor3f(1.0, 0.0, 0.0);
                glTranslatef(X+0.5, Y+0.5, 0);
                glutSolidSphere(0.3, 10, 10);
            glPopMatrix();
            matr[ROWS-1-Y][X] = 2;
            if (matr[ROWS-1-Y][X-1] != 1) this->X -= 1;
            else return false;
        }
        return true;
    }
    int getX()
    {
        return this->X;
    }
    int getY()
    {
        return this->Y;
    }
};

class trap
{
private:
    int x;
    int y;
    int curtime = 0;
    int waiting = 0;
    void bite()
    {
        setspace(x,y);
        matr[ROWS-1-y][x] = 2;
        glColor3f(0.3, 0.3, 0.3);
        glRectd(x+0.2,y+0.2,x+0.4,y+0.4);
        glRectd(x+0.6,y+0.2,x+0.8,y+0.4);
        glRectd(x+0.2,y+0.6,x+0.4,y+0.8);
        glRectd(x+0.6,y+0.6,x+0.8,y+0.8);
    }
public:
    trap()
    {
        this->x = -1;
        this->y = -1;
    }
    trap(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    void wait()
    {
        if (curtime == ((x*2+y*3)%5)+15)
        {
            if (waiting != 1)
            {
                bite();
                waiting++;
            }
            else
            {
                setspace(x, y);
                matr[ROWS-1-y][x] = 0;
                waiting = 0;
                curtime = 0;
            }

        }
        else 
        {
            setspace(x, y);
            matr[ROWS-1-y][x] = 0;
            curtime++;
        }
    }
};

std::vector<bullet> all_bullet;
std::vector<bullet> gunners_bullets;
std::map<int,int> doors;
std::map<int,int> healths;
std::map<int,int> times;
std::vector<trap> traps;

void setspace(int x, int y) // свободное пространство
{
    if ((x+y)%3==2) space3(x,y);
    else if ((x+y)%3==1) space2(x,y);
    else space1(x,y);
    matr[ROWS-1-y][x] = 0;
}

void lava(int x, int y) // лава
{
    matr[ROWS-1-y][x] = 2;
    if (lavaShimmer)
    {
        if ((x+y)%2)
        {
            glColor3f(1.0, 0.0, 0.0);
            glRectd(x,y,x+1,y+1);
            glColor3f(1.0, 0.5, 0.0);
            glRectd(x+0.4,y,x+0.6,y+0.2);
            glRectd(x+0.6,y,x+1,y+0.4);
            glRectd(x,y+0.4,x+0.6,y+0.6);
            glRectd(x,y+0.6,x+0.2,y+0.8);
            glRectd(x+0.2,y+0.8,x+0.4,y+1);
            glRectd(x+0.6,y+0.6,x+0.8,y+0.8);
            glColor3f(1.0, 1.0, 0.0);
            glRectd(x+0.2,y+0.6,x+0.6,y+0.8);
            glRectd(x+0.6,y,x+1,y+0.2);
        }
        else
        {
            glColor3f(1.0, 0.0, 0.0);
            glRectd(x,y,x+1,y+1);
            glColor3f(1.0, 0.5, 0.0);
            glRectd(x,y+0.8,x+0.4,y+1);
            glRectd(x+0.6,y+0.6,x+1,y+0.8);
            glRectd(x+0.4,y+0.4,x+0.6,y+0.6);
            glRectd(x+0.6,y+0.2,x+1,y+0.4);
            glRectd(x,y,x+0.4,y+0.2);
            glRectd(x,y+0.2,x+0.2,y+0.4);
            glColor3f(1.0, 1.0, 0.0);
            glRectd(x+0.6,y+0.4,x+1,y+0.6);
        }
    }
    else
    {
        if ((x+y)%2)
        {
            glColor3f(1.0, 0.0, 0.0);
            glRectd(x,y,x+1,y+1);
            glColor3f(1.0, 0.5, 0.0);
            glRectd(x,y+0.8,x+0.4,y+1);
            glRectd(x+0.6,y+0.6,x+1,y+0.8);
            glRectd(x+0.4,y+0.4,x+0.6,y+0.6);
            glRectd(x+0.6,y+0.2,x+1,y+0.4);
            glRectd(x,y,x+0.4,y+0.2);
            glRectd(x,y+0.2,x+0.2,y+0.4);
            glColor3f(1.0, 1.0, 0.0);
            glRectd(x+0.6,y+0.4,x+1,y+0.6);
        }
        else
        {
            glColor3f(1.0, 0.0, 0.0);
            glRectd(x,y,x+1,y+1);
            glColor3f(1.0, 0.5, 0.0);
            glRectd(x+0.4,y,x+0.6,y+0.2);
            glRectd(x+0.6,y,x+1,y+0.4);
            glRectd(x,y+0.4,x+0.6,y+0.6);
            glRectd(x,y+0.6,x+0.2,y+0.8);
            glRectd(x+0.2,y+0.8,x+0.4,y+1);
            glRectd(x+0.6,y+0.6,x+0.8,y+0.8);
            glColor3f(1.0, 1.0, 0.0);
            glRectd(x+0.2,y+0.6,x+0.6,y+0.8);
            glRectd(x+0.6,y,x+1,y+0.2);
        }
    }
}

void clearMap() // чистка карты
{
    for (int i=0; i<ROWS; i++)
    {
        for (int j=0; j<COLUMNS; j++)
        {
            matr[i][j] = 0;
        }
    }
    all_bullet.clear();
    gunners_bullets.clear();
    traps.clear();
}

void brick3(int x, int y) //текстурка камня 1
{
    //закраска всей площади
    glColor3f(0.5, 0.5, 0.5);
    glRectd(x,y,x+1,y+1);

    //отрисовка детали №1
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_POLYGON);
        glVertex2f(x,y+1);
        glVertex2f(x,y+0.6);
        glVertex2f(x+0.2,y+0.6);
        glVertex2f(x+0.2,y+0.8);
        glVertex2f(x+0.6,y+0.8);
        glVertex2f(x+0.6,y+1);
    glEnd();

    //отрисовка детали №2
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.6,y+0.4);
        glVertex2f(x+0.4,y+0.4);
        glVertex2f(x+0.4,y+0.6);
        glVertex2f(x+0.8,y+0.6);
        glVertex2f(x+0.8,y+0.2);
        glVertex2f(x+0.6,y+0.2);
    glEnd();

    //отрисовка детали №3
    glColor3f(0.6, 0.6, 0.6);
    glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+0.4,y);
        glVertex2f(x+0.4,y+0.2);
        glVertex2f(x,y+0.2);
    glEnd();

    //отрисовка детали №4
    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.8,y+0.8);
        glVertex2f(x+0.8,y+1);
        glVertex2f(x+1,y+1);
        glVertex2f(x+1,y+0.8);
    glEnd();
}

void space3(int x, int y) //текстурка пространства 1
{
    //закраска всей площади
    glColor3f(0.75, 0.75, 0.75);
    glRectd(x,y,x+1,y+1);

    //отрисовка детали №1
    glColor3f(0.9, 0.9, 0.9);
    glBegin(GL_POLYGON);
        glVertex2f(x,y+1);
        glVertex2f(x,y+0.6);
        glVertex2f(x+0.2,y+0.6);
        glVertex2f(x+0.2,y+0.8);
        glVertex2f(x+0.6,y+0.8);
        glVertex2f(x+0.6,y+1);
    glEnd();

    //отрисовка детали №2
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.6,y+0.4);
        glVertex2f(x+0.4,y+0.4);
        glVertex2f(x+0.4,y+0.6);
        glVertex2f(x+0.8,y+0.6);
        glVertex2f(x+0.8,y+0.2);
        glVertex2f(x+0.6,y+0.2);
    glEnd();

    //отрисовка детали №3
    glColor3f(0.85, 0.85, 0.85);
    glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+0.4,y);
        glVertex2f(x+0.4,y+0.2);
        glVertex2f(x,y+0.2);
    glEnd();

    //отрисовка детали №4
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.8,y+0.8);
        glVertex2f(x+0.8,y+1);
        glVertex2f(x+1,y+1);
        glVertex2f(x+1,y+0.8);
    glEnd();
}

void brick2(int x, int y) //текстурка камня 2
{
    //закраска всей площади
    glColor3f(0.6, 0.6, 0.6);
    glRectd(x,y,x+1,y+1);

    //отрисовка детали №1
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.4,y+0.8);
        glVertex2f(x+0.4,y+1);
        glVertex2f(x+1,y+1);
        glVertex2f(x+1,y+0.8);
    glEnd();

    //отрисовка детали №2
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+0.4,y);
        glVertex2f(x+0.4,y+0.2);
        glVertex2f(x,y+0.2);
    glEnd();

    //отрисовка детали №3
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.2,y+0.6);
        glVertex2f(x+0.2,y+0.8);
        glVertex2f(x,y+0.8);
        glVertex2f(x,y+0.4);
        glVertex2f(x+0.4,y+0.4);
        glVertex2f(x+0.4,y+0.6);
    glEnd();

    //отрисовка детали №4
    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.8,y+0.4);
        glVertex2f(x+1,y+0.4);
        glVertex2f(x+1,y+0.6);
        glVertex2f(x+0.6,y+0.6);
        glVertex2f(x+0.6,y);
        glVertex2f(x+0.8,y);
    glEnd();
}

void space2(int x, int y) //текстурка пространства 2
{
    //закраска всей площади
    glColor3f(0.8, 0.8, 0.8);
    glRectd(x,y,x+1,y+1);

    //отрисовка детали №1
    glColor3f(0.85, 0.85, 0.85);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.4,y+0.8);
        glVertex2f(x+0.4,y+1);
        glVertex2f(x+1,y+1);
        glVertex2f(x+1,y+0.8);
    glEnd();

    //отрисовка детали №2
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+0.4,y);
        glVertex2f(x+0.4,y+0.2);
        glVertex2f(x,y+0.2);
    glEnd();

    //отрисовка детали №3
    glColor3f(0.75, 0.75, 0.75);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.2,y+0.6);
        glVertex2f(x+0.2,y+0.8);
        glVertex2f(x,y+0.8);
        glVertex2f(x,y+0.4);
        glVertex2f(x+0.4,y+0.4);
        glVertex2f(x+0.4,y+0.6);
    glEnd();

    //отрисовка детали №4
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.8,y+0.4);
        glVertex2f(x+1,y+0.4);
        glVertex2f(x+1,y+0.6);
        glVertex2f(x+0.6,y+0.6);
        glVertex2f(x+0.6,y);
        glVertex2f(x+0.8,y);
    glEnd();
}

void brick1(int x, int y) // текстурка камня 3
{
    //закраска всей площади
    glColor3f(0.4, 0.4, 0.4);
    glRectd(x,y,x+1,y+1);

    //отрисовка детали №1
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.6,y+0.2);
        glVertex2f(x+0.6,y+0.4);
        glVertex2f(x+1,y+0.4);
        glVertex2f(x+1,y);
        glVertex2f(x+0.2,y);
        glVertex2f(x+0.2,y+0.2);
    glEnd();

    //отрисовка детали №2
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.2,y+0.6);
        glVertex2f(x+0.6,y+0.6);
        glVertex2f(x+0.6,y+0.8);
        glVertex2f(x,y+0.8);
        glVertex2f(x,y+0.4);
        glVertex2f(x+0.2,y+0.4);
    glEnd();

    //отрисовка детали №3
    glColor3f(0.3, 0.3, 0.3);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.8,y+0.6);
        glVertex2f(x+0.8,y+1);
        glVertex2f(x+1,y+1);
        glVertex2f(x+1,y+0.6);
    glEnd();
}

void space1(int x, int y) // текстурка пространства 3
{
    //закраска всей площади
    glColor3f(0.85, 0.85, 0.85);
    glRectd(x,y,x+1,y+1);

    //отрисовка детали №1
    glColor3f(0.75, 0.75, 0.75);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.6,y+0.2);
        glVertex2f(x+0.6,y+0.4);
        glVertex2f(x+1,y+0.4);
        glVertex2f(x+1,y);
        glVertex2f(x+0.2,y);
        glVertex2f(x+0.2,y+0.2);
    glEnd();

    //отрисовка детали №2
    glColor3f(0.8, 0.8, 0.8);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.2,y+0.6);
        glVertex2f(x+0.6,y+0.6);
        glVertex2f(x+0.6,y+0.8);
        glVertex2f(x,y+0.8);
        glVertex2f(x,y+0.4);
        glVertex2f(x+0.2,y+0.4);
    glEnd();

    //отрисовка детали №3
    glColor3f(0.7, 0.7, 0.7);
    glBegin(GL_POLYGON);
        glVertex2f(x+0.8,y+0.6);
        glVertex2f(x+0.8,y+1);
        glVertex2f(x+1,y+1);
        glVertex2f(x+1,y+0.6);
    glEnd();
}

void drawHero() //отрисовка персонажа
{ 
    if (sDirection==UP)
    {
        if (matr[ROWS-1-posY-1][posX] != 1) 
        {
            posY++;
            sDirection=STAY;
        }
    }
    else if (sDirection==DOWN)
    {
        if (matr[ROWS-1-posY+1][posX] != 1) 
        {
            posY--;
            sDirection=STAY;
        }
    }
    else if (sDirection==RIGHT)
    {
        if (matr[ROWS-1-posY][posX+1] != 1) 
        {
            posX++;
            sDirection=STAY;
        }
    }
    else if (sDirection==LEFT)
    {
        if (matr[ROWS-1-posY][posX-1] != 1) 
        {
            posX--;
            sDirection=STAY;
        }
    }
    if (matr[ROWS-1-posY][posX] == 2)
    {
        curHealth--;
    }
    HP(curHealth);
    //тело
    glColor3f(0.4f, 0.2f, 0.1f);
    glBegin(GL_POLYGON);
        glVertex2f(posX, posY+0.3);
        glVertex2f(posX, posY+0.7);
        glVertex2f(posX+0.3, posY+1);
        glVertex2f(posX+0.75, posY+1);
        glVertex2f(posX+1, posY+0.7);
        glVertex2f(posX+1, posY+0.3);
        glVertex2f(posX+0.75, posY);
        glVertex2f(posX+0.3, posY);
    glEnd();

    //глаза
    glColor3f(1, 1, 1);
    glRectd(posX+0.2,posY+0.3,posX+0.4,posY+0.7);
    glRectd(posX+0.6,posY+0.3,posX+0.8,posY+0.7);

    //глазницы
    glColor3f(0.0, 0.3, 0.6);
    glRectd(posX+0.2,posY+0.45,posX+0.35,posY+0.65);
    glRectd(posX+0.65,posY+0.4,posX+0.8,posY+0.6);
    if (level == 3 && posY == ROWS-1)
    {
        exit(0);
    }
    if (posY == ROWS-1) // следующий уровень
    {
        level++;
        posX=14;
        posY=1;
        clearMap();
    }
    if (posY == 0) // предыдущий уровень
    {
        level--;
        posX=14;
        posY=38;
        clearMap();
    }
    
    if (matr[ROWS-1-posY][posX] >= 80 && matr[ROWS-1-posY][posX] <= 89) doors[matr[ROWS-1-posY][posX]-80] = 2;
    if (matr[ROWS-1-posY][posX] >= 31 && matr[ROWS-1-posY][posX] <= 39)
    {
        healths[matr[ROWS-1-posY][posX]-30]++;
        curHealth++;
    }
    if (matr[ROWS-1-posY][posX] >= 41 && matr[ROWS-1-posY][posX] <= 49)
    {
        times[matr[ROWS-1-posY][posX]-40]++;
        if (curTime+200 > TIME) curTime = TIME;
        else curTime += 200;
    }
}

void setBrick(int x, int y) //рандомизация расстановки камней
{
    if ((x+y)%3==2) brick3(x,y);
    else if ((x+y)%3==1) brick2(x,y);
    else brick1(x,y);
    matr[ROWS-1-y][x] = 1;
}

void door(int x, int y, int whichDoor)
{
    if (doors[whichDoor] == 0) doors[whichDoor]++;
    if (doors[whichDoor] == 2)
    {
        matr[ROWS-1-y][x] = 0;
        setspace(x,y);
    }
    else
    {
        matr[ROWS-1-y][x] = 1;
        // цвет всей двери
        glColor3f(0.48, 0.32, 0.16);
        glRectd(x,y,x+1,y+1);

        // перекладины
        glColor3f(0.36, 0.25, 0.2);
        glRectd(x+0.2,y,x+0.3,y+1);
        glRectd(x+0.45,y,x+0.55,y+1);
        glRectd(x+0.7,y,x+0.8,y+1);
        glRectd(x,y+0.7,x+1,y+0.8);
        glRectd(x,y+0.2,x+1,y+0.3);
        glRectd(x,y+0.45,x+1,y+0.55);
    }
}

void key(int x, int y, int whichKey)
{
    setspace(x,y);
    if (doors[whichKey] != 2)
    {
        matr[ROWS-1-y][x] = whichKey+80;
        glColor3f(0.8, 0.6, 0.2);
        glRectd(x,y+0.2,x+0.4,y+0.8);
        glRectd(x+0.1,y+0.3,x+0.3,y+0.7);
        glRectd(x+0.4,y+0.4,x+1,y+0.55);
        glRectd(x+0.6,y+0.2,x+0.7,y+0.4);
        glRectd(x+0.8,y+0.2,x+0.9,y+0.4);
    }
}

void traper(int x, int y)
{
    isOutside = true;
    for (int i=0; i<traps.size(); i++)
    {
        if (traps[i].getX() == x && traps[i].getY() == y) isOutside = false;
    }
    if (isOutside) traps.emplace_back(x, y);
}

void timeBonus(int x, int y, int whichTime)
{
    setspace(x,y);
    if (times[whichTime] == 0) times[whichTime]++;
    if (times[whichTime] == 1)
    {
        matr[ROWS-1-y][x] = 40+whichTime;
        glPushMatrix();
            glColor3f(1.0, 0.8, 0.0);
            glTranslatef(x+0.5, y+0.5, 0);
            glutSolidSphere(0.4, 10, 10);
            glColor3f(0.0, 0.0, 0.0);
            glutSolidSphere(0.12, 10, 10);
        glPopMatrix();
        glColor3f(0.0, 0.0, 0.0);
        glRectd(x+0.46,y+0.5,x+0.53,y+0.85);
        glRectd(x+0.46,y+0.46,x+0.78,y+0.53);
    }
}

void healthing(int x, int y, int whichHealth)
{
    setspace(x,y);
    if (healths[whichHealth] == 0) healths[whichHealth]++;
    if (healths[whichHealth] == 1)
    {
        matr[ROWS-1-y][x] = 30+whichHealth; 
        glColor3f(1, 0, 0);
        glRectd(x,y+0.3,x+1,y+0.7);
        glRectd(x+0.3,y,x+0.7,y+1);
    }
}

void drawMap() // отрисовка карты
{
    std::fstream file;
    std::string fileName;
    switch (level)
    {
    case 1:
        fileName = "FirstMap.txt";
        break;
    case 2:
        fileName = "SecondMap.txt";
        break;
    case 3:
        fileName = "ThirdMap.txt";
        break;
    }
    file.open(fileName, std::fstream::in);
    int num;
    for(int i=0;i<ROWS;i++)
    {
        for (int j=0;j<COLUMNS;j++)
        {
            file>>num;
            if (num == 0) setspace(j,COLUMNS+10-i);
            else if (num == 1) setBrick(j,COLUMNS+10-i);
            else if (num == 2) lava(j,COLUMNS+10-i);
            else if (num<=399 && num>=301) Gunner(j,COLUMNS+10-i, num-300, UP);
            else if (num<=499 && num>=401) Gunner(j,COLUMNS+10-i, num-400, DOWN);
            else if (num<=599 && num>=501) Gunner(j,COLUMNS+10-i, num-500, LEFT);
            else if (num<=699 && num>=601) Gunner(j,COLUMNS+10-i, num-600, RIGHT);
            else if (num<=99 && num>=90) door(j, COLUMNS+10-i, num-90);
            else if (num<=89 && num>=80) key(j, COLUMNS+10-i, num-80);
            else if (num<=39 && num>=31) healthing(j, COLUMNS+10-i, num-30);
            else if (num == 3) traper(j, COLUMNS+10-i);
            else if (num<=49 && num>=41) timeBonus(j, COLUMNS+10-i, num-40);
        }
    }
    file.close();
}

void Gunner(int x, int y, int speed, int direction) // создание создателя пуль)))))))))))))))))))))))
{
    setspace(x,y);
    matr[ROWS-1-y][x] = 1;
    switch (direction)
    {
    case UP:
        glColor3f(0.5, 0.3, 0.1);//корпус
        glBegin(GL_POLYGON);
            glVertex2f(x+0.2, y);
            glVertex2f(x, y+0.4);
            glVertex2f(x+0.2, y+0.8);
            glVertex2f(x+0.8, y+0.8);
            glVertex2f(x+1, y+0.4);
            glVertex2f(x+0.8, y);
        glEnd();
        glColor3f(0.2, 0.15, 0.1);//стрелка на корпусe
        glBegin(GL_POLYGON);
            glVertex2f(x+0.4, y+0.4);
            glVertex2f(x+0.2, y+0.4);
            glVertex2f(x+0.5, y+0.6);
            glVertex2f(x+0.8, y+0.4);
            glVertex2f(x+0.6, y+0.4);
            glVertex2f(x+0.6, y+0.2);
            glVertex2f(x+0.4, y+0.2);
        glEnd();
        glColor3f(0.08, 0.02, 0.04);//дуло
        glRectd(x+0.2,y+0.8,x+0.8,y+1);

        isOutside = true;
        for (int i=0; i<gunners_bullets.size(); i++)
        {
            if (gunners_bullets[i].getX() == x && gunners_bullets[i].getY() == y+1) isOutside = false; 
        }
        if (isOutside) gunners_bullets.emplace_back(x, y+1, UP, speed);
        break;
    case DOWN:
        glColor3f(0.5, 0.3, 0.1);//корпус
        glBegin(GL_POLYGON);
            glVertex2f(x+0.2, y+0.2);
            glVertex2f(x, y+0.6);
            glVertex2f(x+0.2, y+1);
            glVertex2f(x+0.8, y+1);
            glVertex2f(x+1, y+0.6);
            glVertex2f(x+0.8, y+0.2);
        glEnd();
        glColor3f(0.2, 0.15, 0.1);//стрелка на корпусe
        glBegin(GL_POLYGON);
            glVertex2f(x+0.4, y+0.6);
            glVertex2f(x+0.2, y+0.6);
            glVertex2f(x+0.5, y+0.4);
            glVertex2f(x+0.8, y+0.6);
            glVertex2f(x+0.6, y+0.6);
            glVertex2f(x+0.6, y+0.8);
            glVertex2f(x+0.4, y+0.8);
        glEnd();
        glColor3f(0.08, 0.02, 0.04);//дуло
        glRectd(x+0.2,y,x+0.8,y+0.2);
        
        isOutside = true;
        for (int i=0; i<gunners_bullets.size(); i++)
        {
            if (gunners_bullets[i].getX() == x && gunners_bullets[i].getY() == y-1) isOutside = false; 
        }
        if (isOutside) gunners_bullets.emplace_back(x, y-1, DOWN, speed);
        break;
    case RIGHT:
        glColor3f(0.5, 0.3, 0.1);//корпус
        glBegin(GL_POLYGON);
            glVertex2f(x, y+0.2);
            glVertex2f(x, y+0.8);
            glVertex2f(x+0.4, y+1);
            glVertex2f(x+0.8, y+0.8);
            glVertex2f(x+0.8, y+0.2);
            glVertex2f(x+0.4, y);
        glEnd();
        glColor3f(0.2, 0.15, 0.1);//стрелка на корпусe
        glBegin(GL_POLYGON);
            glVertex2f(x+0.4, y+0.4);
            glVertex2f(x+0.4, y+0.2);
            glVertex2f(x+0.6, y+0.5);
            glVertex2f(x+0.4, y+0.8);
            glVertex2f(x+0.4, y+0.6);
            glVertex2f(x+0.2, y+0.6);
            glVertex2f(x+0.2, y+0.4);
        glEnd();
        glColor3f(0.08, 0.02, 0.04);//дуло
        glRectd(x+0.8,y+0.2,x+1,y+0.8);

        isOutside = true;
        for (int i=0; i<gunners_bullets.size(); i++)
        {
            if (gunners_bullets[i].getX() == x+1 && gunners_bullets[i].getY() == y) isOutside = false; 
        }
        if (isOutside) gunners_bullets.emplace_back(x+1, y, RIGHT, speed);
        break;
    case LEFT:
        glColor3f(0.5, 0.3, 0.1);//корпус
        glBegin(GL_POLYGON);
            glVertex2f(x+1, y+0.2);
            glVertex2f(x+1, y+0.8);
            glVertex2f(x+0.6, y+1);
            glVertex2f(x+0.2, y+0.8);
            glVertex2f(x+0.2, y+0.2);
            glVertex2f(x+0.6, y);
        glEnd();
        glColor3f(0.2, 0.15, 0.1);//стрелка на корпусe
        glBegin(GL_POLYGON);
            glVertex2f(x+0.6, y+0.4);
            glVertex2f(x+0.6, y+0.2);
            glVertex2f(x+0.4, y+0.5);
            glVertex2f(x+0.6, y+0.8);
            glVertex2f(x+0.6, y+0.6);
            glVertex2f(x+0.8, y+0.6);
            glVertex2f(x+0.8, y+0.4);
        glEnd();
        glColor3f(0.08, 0.02, 0.04);//дуло
        glRectd(x+0.2,y+0.2,x,y+0.8);

        isOutside = true;
        for (int i=0; i<gunners_bullets.size(); i++)
        {
            if (gunners_bullets[i].getX() == x-1 && gunners_bullets[i].getY() == y) isOutside = false; 
        }
        if (isOutside) gunners_bullets.emplace_back(x-1, y, LEFT, speed);
        break;
    }
}

void createBullet(int x, int y, int direction) // создание пули
{
    all_bullet.emplace_back(x,y,direction);
}

void moveEntity() // каждый объект делает действие за отрисовку 1 кадра
{
    for (int i=0; i<traps.size(); i++)
    {
        traps[i].wait();
    }
    int i = 0;
    while (i < all_bullet.size())
    {
        if (!all_bullet[i].fly()) all_bullet.erase(all_bullet.begin() + i);
        else i++;
    }
    for (int j=0; j<gunners_bullets.size(); j++)
    {
        gunners_bullets[j].wait();
    }

    if (forShimmer == 10) // часота сияния;
    {
        forShimmer = 0;
        lavaShimmer = lavaShimmer ? false : true;
    }
    else forShimmer++;
    timer(curTime);
}

void HP(int Hp)
{
    if (curHealth==0)
    {
        gameOver();
    }
    else
    {
        glColor3f(0.2, 0.8, 0.2);
        glRectd(1,1,1+(Hp*2),2);
    }
}

void timer(float &timeValue)
{
    timeValue--;
    if (timeValue == 0)
    {
        gameOver();
    }
    glColor3f(1.0, 0.7, 0.3);
    glRectd(22,1,28-(6-6*timeValue/(TIME*1.0)),2);
}

void gameOver()
{
    level = 1;
    posX=14;
    posY=1;
    all_bullet.clear();
    gunners_bullets.clear();
    doors.clear();
    healths.clear();
    curHealth = HEALTH;
    curTime = TIME;
}

void playGame()
{
    drawMap();
    moveEntity();
    drawHero();
}
