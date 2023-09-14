#include "classes.h"
#include <vector>
#include <fstream>
#include <algorithm>

short sDirection = STAY;
int matr[ROWS][COLUMNS];
int posX=14, posY=1;
bool isOutside = true;
bool lavaShimmer = true;
int forShimmer = 0;

int curHealth = HEALTH;
float curTime=TIME1;

void createBullet(int x, int y, int discription);
void Gunner(int x, int y, int speed, int discription);
void drawMap();
void setBrick(int x, int y);
void drawHero();
void brick1(int x, int y);
void brick2(int x, int y);
void brick3(int x, int y);
void space(int x, int y);
void moveEntity();
void lava(int x, int y);
void HP(int Hp);
void timer(float &);

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

std::vector<bullet> all_bullet;
std::vector<bullet> gunners_bullets; 

void space(int x, int y) // свободное пространство
{
    glColor3f(0.8, 0.9, 0.9);
    glRectd(x,y,x+1,y+1);
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

    //отрисовка детали №3
    glColor3f(0.4, 0.4, 0.4);
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
    glColor3f(0.0, 0.0, 0.0);
    glRectd(posX,posY,posX+1,posY+1);
}

void setBrick(int x, int y) //рандомизация расстановки камней
{
    if ((x+y)%3==2) brick3(x,y);
    else if ((x+y)%3==1) brick2(x,y);
    else brick1(x,y);
    matr[ROWS-1-y][x] = 1;
}

void drawMap() // отрисовка карты
{
    std::fstream file;
    file.open("FirstMap.txt", std::fstream::in);
    int num;
    for(int i=0;i<ROWS;i++)
    {
        for (int j=0;j<COLUMNS;j++)
        {
            file>>num;
            if (num == 0) space(j,COLUMNS+9-i);
            else if (num == 1) setBrick(j,COLUMNS+9-i);
            else if (num == 2) lava(j,COLUMNS+9-i);
            else if (num<=399 && num>=301) Gunner(j,COLUMNS+9-i, num-300, DOWN);
        }
    }
    file.close();
}

void Gunner(int x, int y, int speed, int direction) // создание создателя пуль)))))))))))))))))))))))
{
    space(x,y);
    matr[ROWS-1-y][x] = 1;
    switch (direction)
    {
    case UP:
        glColor3f(0.4, 0.1, 0.2);
        glRectd(x+0.2,y+0.6,x+0.8,y+1);
        glRectd(x,y,x+1,y+0.6);
        isOutside = true;
        for (int i=0; i<gunners_bullets.size(); i++)
        {
            if (gunners_bullets[i].getX() == x && gunners_bullets[i].getY() == y+1) isOutside = false; 
        }
        if (isOutside) gunners_bullets.emplace_back(x, y+1, UP, speed);
        break;
    case DOWN:
        glColor3f(0.4, 0.1, 0.2);
        glRectd(x+0.2,y+0.4,x+0.8,y);
        glRectd(x,y+1,x+1,y+0.4);
        isOutside = true;
        for (int i=0; i<gunners_bullets.size(); i++)
        {
            if (gunners_bullets[i].getX() == x && gunners_bullets[i].getY() == y-1) isOutside = false; 
        }
        if (isOutside) gunners_bullets.emplace_back(x, y-1, DOWN, speed);
        break;
    case RIGHT:
        glColor3f(0.4, 0.1, 0.2);
        glRectd(x+0.6,y+0.8,x+1,y+0.2);
        glRectd(x,y+1,x+0.6,y);
        isOutside = true;
        for (int i=0; i<gunners_bullets.size(); i++)
        {
            if (gunners_bullets[i].getX() == x+1 && gunners_bullets[i].getY() == y) isOutside = false; 
        }
        if (isOutside) gunners_bullets.emplace_back(x+1, y, RIGHT, speed);
        break;
    case LEFT:
        glColor3f(0.4, 0.1, 0.2);
        glRectd(x+0.4,y+0.8,x,y+0.2);
        glRectd(x+1,y+1,x+0.4,y);
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
        exit(100);
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
        exit(1);
    }
    glColor3f(1.0, 0.7, 0.3);
    glRectd(23,1,29-(6-6*timeValue/(TIME1*1.0)),2);
}

//--------------------------------------------------------------------------------

//--------------------------------------------------------------------------------
void playGame()
{
    drawMap();
    moveEntity();
    drawHero();
}
