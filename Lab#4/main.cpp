#include "Ball.h"

#define ID_TIMER 000001


LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE hInstance;
Ball ball;

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd) {

    int TIMER_TIME = Ball::TIMER_TIME;
    MSG msg;
    hInstance = hInst;

    WNDCLASSEX wClass;

    wClass.hInstance = hInst;
    wClass.lpszClassName = "Window Class";
    wClass.lpfnWndProc = (WNDPROC)WinProc;
    wClass.style = CS_HREDRAW|CS_VREDRAW;
    wClass.cbSize = sizeof(WNDCLASSEX);

    wClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wClass.hIconSm = NULL;
    wClass.lpszMenuName  = NULL;
    wClass.hIcon  = NULL;

    wClass.lpszMenuName = NULL;
    if (!RegisterClassEx(&wClass)) {
        GetLastError();
        MessageBox(NULL,
            "Window class creation failed\r\n",
            "Window Class Failed",
            MB_ICONERROR);
    }

    HWND hWnd=CreateWindowEx(NULL,
            "Window Class",
            "Windows application",
            WS_OVERLAPPEDWINDOW,
            250, 100,
            800, 600,
            NULL,
            NULL,
            hInst,
            NULL);

    if(!hWnd) {
        GetLastError();
        MessageBox(NULL,
            "Window creation failed\r\n",
            "Window Creation Failed",
            MB_ICONERROR);
    }

    ShowWindow(hWnd,nShowCmd);
    srand (time(NULL));
    SetTimer(hWnd, ID_TIMER, TIMER_TIME, NULL);
    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

    return 0;
}

Ball test1;
LRESULT CALLBACK WinProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    srand (time(NULL));
    PAINTSTRUCT Ps;
    RECT rcDrawingArea;
    HPEN hPen;
    HBRUSH hBrush;
    HDC hdc = GetDC(hWnd);

    SetRect(&rcDrawingArea, 10, 10, 770, 550);

    switch(msg) {
    case WM_CREATE: {

        }
        break;

    case WM_LBUTTONDOWN: {
            if (wParam & MK_LBUTTON) {
                Ball* newBall = new Ball();
                balls.push_back(newBall);
            }
        }
        break;

    case WM_PAINT: {
            HDC hdc = BeginPaint(hWnd, &Ps);

            hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            hBrush = CreateSolidBrush(RGB(255, 255, 255));
            SelectObject(hdc, hPen);
            SelectObject(hdc, hBrush);
            Rectangle(hdc, rcDrawingArea.left, rcDrawingArea.top, rcDrawingArea.right, rcDrawingArea.bottom);
            DeleteObject(hPen);
            DeleteObject(hBrush);


            EndPaint(hWnd, &Ps);
        }
        break;

    case WM_COMMAND: {
            switch (LOWORD(wParam)) {

            }
        }
        break;
    case WM_TIMER: {
            if (wParam == ID_TIMER) {
                for(int i = 0; i < balls.size(); i++) {

                    balls[i]->newPos();

                    for (int j = 0; j < balls.size(); j++) {
                        if (j == i)
                            continue;
                        balls[i]->interactBall(*balls[j]);
                        //if (balls[i]->interactBall(*balls[j])) {
                            //j = 0;
                          //  continue;
                        //}
                    }

                    balls[i]->reshapeBoundaries();
                    balls[i]->drawGhost(hdc);
                    balls[i]->drawBall(hdc);
                    if (balls[i]->_livingTime > 1000 * 10) {
                        Ball* tempBall = balls[i];
                        balls.erase(balls.begin() + i);
                        tempBall->drawGhost(hdc);
                        delete tempBall;
                        i--;
                    }
                }
            }
        }
        break;

    case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        break;
    }

    ReleaseDC(hWnd, hdc);
    return DefWindowProc(hWnd,msg,wParam,lParam);
}

