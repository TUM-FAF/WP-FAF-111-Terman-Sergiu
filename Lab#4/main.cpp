#include "Ball.h"

#define ID_TIMER 000001


LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE hInstance;
Ball ball;
int TIMER_TIME = 28;
HBITMAP jamaicaNyan[12];
HBITMAP jamaicaRainbow;
int nyanPosition = 0;
bool nyanTime = true;
int raynbowPos = 0;
int globalIncrementor = 1;

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd) {

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
            0, 0,
            1366, 730,
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
    RECT nyanRect;
    HPEN hPen;
    HBRUSH hBrush;
    HDC hdc = GetDC(hWnd);

    SetRect(&nyanRect, 0, 2, 1340, 185);
    SetRect(&rcDrawingArea, 10, 190, 1340, 690);
    static RECT rainbowRect;
    rainbowRect.left = 0;
    rainbowRect.right = 700;
    rainbowRect.top = 3;
    rainbowRect.bottom = 176;

    switch(msg) {
    case WM_CREATE: {
            PlaySoundW(L"Jamaican nyan cat.wav", NULL,  SND_FILENAME | SND_ASYNC | SND_LOOP);//play nyan song
            for (int i = 0; i < 12; i++) {
                char buffer [20];
                sprintf(buffer, "bmp/%i.bmp", i);
                jamaicaNyan[i] = (HBITMAP)LoadImage(hInstance, buffer, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            }
            jamaicaRainbow = (HBITMAP)LoadImage(hInstance, "bmp/jamaicarainbow.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

        }
        break;

    case WM_LBUTTONDOWN: {
            if (wParam & MK_LBUTTON) {
                Ball* newBall = new Ball(TIMER_TIME);
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
            Rectangle(hdc, nyanRect.left, nyanRect.top, nyanRect.right, nyanRect.bottom);
            Rectangle(hdc, rcDrawingArea.left, rcDrawingArea.top, rcDrawingArea.right, rcDrawingArea.bottom);
            DeleteObject(hPen);
            DeleteObject(hBrush);


            EndPaint(hWnd, &Ps);
        }
        break;

    case WM_TIMER: {
            if (wParam == ID_TIMER) {

                if (nyanTime) {
                    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
                    SelectObject(hdc, brush);
                    FillRect(hdc, &rainbowRect, brush);
                    DeleteObject(brush);

                    HDC hdcMem = CreateCompatibleDC(hdc);
                    SelectObject (hdcMem, jamaicaNyan[nyanPosition]);
                    BitBlt(hdc, 700, 3, 304, 176, hdcMem, 0, 0, SRCCOPY);
                    //DeleteDC(hdcMem);
                    nyanPosition++;
                    if (nyanPosition > 11)
                        nyanPosition = 0;

                    nyanTime = false;

                    // Rainbow


                    int startPos = raynbowPos;
                    int incrementor = 1;
                    int yCoord = 646;



                    SelectObject (hdcMem, jamaicaRainbow);
                    for (int i = 0; i < 13; i++) {
                        SelectObject (hdcMem, jamaicaRainbow);
                        BitBlt(hdc, yCoord, 45 + startPos * 5, 54, 96, hdcMem, 0, 0, SRCCOPY);
                        if (startPos >= 2 || startPos <= -2) {
                            incrementor = -incrementor;
                        }
                        startPos += incrementor;
                        yCoord -= 54;
                    }

                    if (raynbowPos >= 3 || raynbowPos <= -3) {
                        globalIncrementor = -globalIncrementor;
                    }
                    raynbowPos += globalIncrementor;

                    DeleteDC(hdcMem);
                    //54 96
                } else
                    nyanTime = true;


                for(int i = 0; i < balls.size(); i++) {

                    balls[i]->newPos();

                    for (int j = 0; j < balls.size(); j++) {
                        if (j == i)
                            continue;
                        balls[i]->interactBall(*balls[j]);
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

    case WM_MOUSEWHEEL: {
            if(GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
                if(TIMER_TIME < 80) {
                    TIMER_TIME += 1;
                    for (int i = 0; i < balls.size(); i++) {
                        balls[i]->TIMER_TIME = TIMER_TIME;
                    }
                }

            } else {
                if(TIMER_TIME > 5) {
                    TIMER_TIME -= 1;
                    for (int i = 0; i < balls.size(); i++) {
                        balls[i]->TIMER_TIME = TIMER_TIME;
                    }
                }

            }
            KillTimer(hWnd, ID_TIMER);
            SetTimer(hWnd, ID_TIMER, TIMER_TIME, NULL);
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

