#ifndef BALL_CLASS
#define BALL_CLASS

#define _USE_MATH_DEFINES

#include <windows.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <math.h>

class Ball {
private:
    int _xPos_old;
    int _yPos_old;

    int _xPos_new;
    int _yPos_new;

    int _radius;
    int _velocity;
    int _angle;

    int _color[3];

    int _personalIndex;


protected:
    void randColor();
    int checkQuadran();

public:
    int _livingTime = 0;

    Ball();
    static const int TIMER_TIME = 28;
    static const int RADIUS_LIMIT_MIN = 7;
    static const int RADIUS_LIMIT_MAX = 30;

    static const int VELOCITY_CRITIC_LIMIT_MIN = 5;
    static const int VELOCITY_CRITIC_LIMIT_MAX = 150;
    static const int VELOCITY_LIMIT_MIN = 2;
    static const int VELOCITY_LIMIT_MAX = 5;

    static const int LIMIT_LEFT = 10;
    static const int LIMIT_TOP = 10;
    static const int LIMIT_RIGHT = 770;
    static const int LIMIT_BOTTOM = 550;

    static const int QUADRAN_1 = 1;
    static const int QUADRAN_2 = 1;
    static const int QUADRAN_3 = 1;
    static const int QUADRAN_4 = 1;

    void newPos();
    void reshapeBoundaries();
    void reshapeInteractions();
    void drawBall(HDC hdc);
    void drawGhost(HDC hdc);
    int ballsAngle(int xPos, int yPos);
    bool interactBall(Ball &ball);

    static double toRadians(int degrees);
};

static std::vector<Ball*> balls;

#endif
