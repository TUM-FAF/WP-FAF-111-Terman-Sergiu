#include "Ball.h"

/**
* Give the initial values to a new ball
*/
Ball::Ball() {
    srand (time(NULL));
    _radius = rand() % RADIUS_LIMIT_MAX + RADIUS_LIMIT_MIN;
    _xPos_old = (LIMIT_LEFT + LIMIT_RIGHT) / 2;
    _yPos_old = (LIMIT_TOP + LIMIT_BOTTOM) / 2;

    _velocity = rand() % VELOCITY_LIMIT_MAX + VELOCITY_LIMIT_MIN;
    _angle = rand() % 360;
    randColor();
}

/**
* Randomises the color of the ball
*/
void Ball::randColor() {
    srand (time(NULL));
    _color[0] = rand() % 255;
    _color[1] = rand() % 255;
    _color[2] = rand() % 255;
}

/**
*
*/
void Ball::newPos() {
    _xPos_new = _xPos_old + cos(toRadians(_angle)) * _velocity;
    _yPos_new = _yPos_old - sin(toRadians(_angle)) * _velocity;
}

/**
* Transform the the angle value from degrees into radians
* @param int, value of angle to transform into radians
* @return double, value of the angle in radians
*/
double Ball::toRadians(int degrees) {
    return (double)degrees * M_PI/180;
}

void Ball::reshapeBoundaries() {
    while (1) {
        int boundaryPos;

        /// Check from left
        boundaryPos = _xPos_new - _radius;
        if (boundaryPos < LIMIT_LEFT) {

            int delta_x = abs(LIMIT_LEFT - boundaryPos);
            int delta_y = abs(delta_x * tan(toRadians(_angle)));
            _xPos_new += delta_x;


            if (_angle <= 180 && _angle >= 90){
                _yPos_new += delta_y;
                _angle = 180 - _angle;
            }


            if (_angle <= 270 && _angle > 180) {
                _yPos_new -= delta_y;
                _angle = 360 - (_angle - 180);
            }
            randColor();
            continue;
        }

        /// Check from top
        boundaryPos = _yPos_new - _radius;
        if (boundaryPos < LIMIT_TOP) {
            int delta_y = abs(LIMIT_TOP - boundaryPos);
            int delta_x = abs(delta_y / tan(toRadians(_angle)));
            _yPos_new += delta_y;

            if (_angle <= 90 && _angle >= 0) {
                _xPos_new -= delta_x;
                _angle = 360 - _angle;
            }

            if (_angle < 180 && _angle > 90) {
                _xPos_new += delta_x;
                _angle = 180 + (180 - _angle);
            }
            randColor();
            continue;
        }

        /// Check from right
        boundaryPos = _xPos_new + _radius;
        if (boundaryPos > LIMIT_RIGHT) {
            int delta_x = abs(boundaryPos - LIMIT_RIGHT);
            int delta_y = abs(delta_x * tan(toRadians(_angle)));
            _xPos_new -= delta_x;

            if (_angle <= 90 && _angle >= 0) {
                _yPos_new += delta_y;
                _angle = 180 - _angle;
            }

            if (_angle <= 360 && _angle >= 270) {
                _yPos_new -= delta_y;
                _angle = (360 - _angle) + 180;
            }

            randColor();
            continue;
        }

        /// Check from bottom
        boundaryPos = _yPos_new + _radius;
        if (boundaryPos > LIMIT_BOTTOM) {
            int delta_y = abs(boundaryPos - LIMIT_BOTTOM);
            int delta_x = abs(delta_y / tan(toRadians(_angle)));
            _yPos_new -= delta_y;

            if (_angle <= 270 && _angle >= 180) {
                _xPos_new += delta_x;
                _angle = 90 + 270 - _angle;
            }

            if (_angle <= 360 && _angle >= 270) {
                _xPos_new -= delta_x;
                _angle = 360 - _angle;
            }
            randColor();
            continue;
        }
        break;
    }
}

void Ball::drawBall(HDC hdc) {
    int tempRad = _radius - 1;
    int oldLeft = _xPos_old - tempRad;
    int oldTop = _yPos_old - tempRad;
    int oldRight = _xPos_old + tempRad;
    int oldBottom = _yPos_old + tempRad;


    int newLeft = _xPos_new - tempRad;
    int newTop = _yPos_new - tempRad;
    int newRight = _xPos_new + tempRad;
    int newBottom = _yPos_new + tempRad;


    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

    SelectObject(hdc, hPen);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, oldLeft, oldTop, oldRight, oldBottom);
    DeleteObject(hPen);
    DeleteObject(hBrush);

    hBrush = CreateSolidBrush(RGB(_color[0], _color[1], _color[2]));
    hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

    SelectObject(hdc, hPen);
    SelectObject(hdc, hBrush);
    Ellipse(hdc, newLeft, newTop, newRight, newBottom);
    DeleteObject(hPen);
    DeleteObject(hBrush);

    _xPos_old = _xPos_new;
    _yPos_old = _yPos_new;

}




