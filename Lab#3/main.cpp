#include <windows.h>
#include <stdio.h>




#define IDC_BUTTON_SET          7001
#define IDC_BUTTON_DRAW         7002
#define IDC_BUTTON_THICKNESS    7003
#define IDC_BUTTON_RUBBER       7004

#define IDC_INPUT_COLOR_R       7005
#define IDC_INPUT_COLOR_G       7006
#define IDC_INPUT_COLOR_B       7007

#define IDC_COMBOBOX_DRAWN_OBJ  7008
#define IDC_COMBOBOX_THICKNESS  7009
#define IDC_COMBOBOC_RUBBER     7010

#define IDC_CHECKBOX_FILLED     7011

#define IDC_RADIO_PEN           7012
#define IDC_RADIO_BRUSH         7013


LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void setupPen(int stroke, int color[3]);
void setupBrush(int color[3]);

void drawLine(POINT coord[4], HDC hdc) {
    MoveToEx(hdc, coord[0].x, coord[0].y, NULL);
    LineTo(hdc, coord[1].x, coord[1].y);
}

void drawCircle(POINT coord[4], HDC hdc) {
    Ellipse(hdc, coord[0].x, coord[0].y, coord[1].x, coord[1].y);
}

void drawRectangle(POINT coord[4], HDC hdc) {
    Rectangle(hdc, coord[0].x, coord[0].y, coord[1].x, coord[1].y);
}
void drawBezier(POINT coord[4], HDC hdc) {
    PolyBezier (hdc, coord, 4);
}

int checkBoundries(int xPos, int yPos, RECT drawingArea);


HINSTANCE hInstance;
/// Global Variables
char bmpfile[] = "skull.bmp";
int penColor[] = {0, 0, 0};
int brushColor[] = {255, 255, 255};
HBRUSH hBrush;
HPEN hPen;
POINT coords[4];

/**
* 0 - Draw mode
* 1 - Erase mode
*/
int eraseMode;

/**
* 1 - pen
* 2 - brush
*/
int radioMark;

/**
* 0 - Unfilled
* 1 - Filled
*/
int checkBoxMark;

/**
* 0 - thickness 1
* 1 - thickness 2
* 2 - thickness 3
* 3 - thickness 4
* 4 - thickness 5
*/
int thicknessMark = 0;

/**
* 0 - Line
* 1 - Circle
* 2 - Rectangle
* 3 - Beziere curve
*/
int drawItemMark;

/**
* 0 - 1
* 1 - 2
* 2 - 3
*/
int rubberMark;

int bezierStage = 0;

int outOfBoud = 0;

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd) {
    MSG msg;
    hInstance = hInst;

    setupPen(1, penColor);
    setupBrush(brushColor);


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

    while (GetMessage (&msg, NULL, 0, 0)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
    }

    return 0;


}

LRESULT CALLBACK WinProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    PAINTSTRUCT Ps;
    static HWND hBtnSetColor, hBtnDraw, hBtnThickness, hBtnRubber;      // Buttons
    static HWND hInptColorR, hInptColorG, hInptColorB;                  // Input Fields
    static HWND hCmbxDrawnObj, hCmbxThickness, hCmbxRubber;             // Combo boxes
    static HWND hChbxFilled;                                            // Check box
    static HWND hRadioPen, hRadioBrush;                                // Radio buttons

    RECT rcPenColor, rcBrushColor;
    RECT rcDrawingArea;
    RECT rcGradient;


    SetRect(&rcPenColor, 30, 315, 75, 360);
    SetRect(&rcBrushColor, 85, 315, 130, 360);
    SetRect(&rcDrawingArea, 180, 10, 770, 550);
    SetRect(&rcGradient, 10, 370, 80, 410);

    HDC hdc = GetDC(hWnd);

    HBRUSH hBrushStatic;

    switch(msg) {
    case WM_CREATE: {
            // *** Set color button ***
            hBtnSetColor = CreateWindowEx(NULL,
                    TEXT("button"),
                    "Set",
                    WS_TABSTOP | WS_VISIBLE |
                    WS_CHILD | BS_DEFPUSHBUTTON | BS_TOP,
                    10, 25,
                    40, 25,
                    hWnd,
                    (HMENU)IDC_BUTTON_SET,
                    hInstance,
                    NULL);

            // *** Input color red ***
            hInptColorR = CreateWindowEx((DWORD)NULL,
                    TEXT("edit"),
                    "",
                    WS_VISIBLE | WS_CHILD | WS_BORDER,
                    55, 25,
                    30, 25,
                    hWnd,
                    (HMENU)IDC_INPUT_COLOR_R,
                    hInstance,
                    NULL);

            // *** Input color green ***
            hInptColorG = CreateWindowEx((DWORD)NULL,
                    TEXT("edit"),
                    "",
                    WS_VISIBLE | WS_CHILD | WS_BORDER,
                    90, 25,
                    30, 25,
                    hWnd,
                    (HMENU)IDC_INPUT_COLOR_G,
                    hInstance,
                    NULL);

            // *** Input color blue ***
            hInptColorB = CreateWindowEx((DWORD)NULL,
                    TEXT("edit"),
                    "",
                    WS_VISIBLE | WS_CHILD | WS_BORDER,
                    125, 25,
                    30, 25,
                    hWnd,
                    (HMENU)IDC_INPUT_COLOR_B,
                    hInstance,
                    NULL);

            // *** Radio Pen ***
            hRadioPen = CreateWindowEx((DWORD)NULL,
                    TEXT("button"), "Pen",
                    WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                    10, 55,
                    50, 15,
                    hWnd, (HMENU)IDC_RADIO_PEN,
                    hInstance,
                    NULL);
            SendMessage(hRadioPen, BM_SETCHECK, BST_CHECKED, 1);

            // *** Radio Brush ***
            hRadioBrush = CreateWindowEx((DWORD)NULL,
                    TEXT("button"), "Brush",
                    WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
                    95, 55,
                    60, 15,
                    hWnd,
                    (HMENU)IDC_RADIO_BRUSH,
                    hInstance,
                    NULL);

            // *** Draw object button ***
            hBtnDraw = CreateWindowEx(NULL,
                    TEXT("button"),
                    "Draw",
                    WS_TABSTOP | WS_VISIBLE |
                    WS_CHILD | BS_DEFPUSHBUTTON | BS_TOP,
                    10, 100,
                    50, 25,
                    hWnd,
                    (HMENU)IDC_BUTTON_DRAW,
                    hInstance,
                    NULL);

            // *** Drawn object combobox ***
            hCmbxDrawnObj = CreateWindowEx(NULL,
                    TEXT("combobox"),
                    NULL,
                    WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST ,
                    65, 100,
                    90, 300,
                    hWnd,
                    (HMENU)IDC_COMBOBOX_DRAWN_OBJ,
                    hInstance,
                    NULL);

            SendMessage(hCmbxDrawnObj,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"Line"));
            SendMessage(hCmbxDrawnObj,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"Rectangle"));
            SendMessage(hCmbxDrawnObj,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"Circle"));
            SendMessage(hCmbxDrawnObj,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"Bezier Curves"));

            // *** Filled object checkbox ***
            hChbxFilled = CreateWindowEx(0,
                        TEXT("button"),
                        "Filled Objects",
                        BS_AUTOCHECKBOX | WS_VISIBLE | WS_CHILD,
                        10, 130,
                        145, 25,
                        hWnd,
                        (HMENU)IDC_CHECKBOX_FILLED,
                        hInstance,
                        NULL);

            // *** Thickness Button ***
            hBtnThickness = CreateWindowEx(NULL,
                    TEXT("button"),
                    "Tickness",
                    WS_TABSTOP | WS_VISIBLE |
                    WS_CHILD | BS_DEFPUSHBUTTON | BS_TOP,
                    10, 185,
                    70, 25,
                    hWnd,
                    (HMENU)IDC_BUTTON_THICKNESS,
                    hInstance,
                    NULL);

            // *** Thickness combobox ***
            hCmbxThickness = CreateWindowEx(NULL,
                    TEXT("combobox"),
                    NULL,
                    WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST ,
                    85, 185,
                    70, 300,
                    hWnd,
                    (HMENU)IDC_COMBOBOX_THICKNESS,
                    hInstance,
                    NULL);

            SendMessage(hCmbxThickness,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"1"));
            SendMessage(hCmbxThickness,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"2"));
            SendMessage(hCmbxThickness,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"3"));
            SendMessage(hCmbxThickness,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"4"));
            SendMessage(hCmbxThickness,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"5"));

            // *** Rubber Button ***
            hBtnRubber = CreateWindowEx(NULL,
                    TEXT("button"),
                    "Rubber",
                    WS_TABSTOP | WS_VISIBLE |
                    WS_CHILD | BS_DEFPUSHBUTTON | BS_TOP,
                    10, 235,
                    60, 25,
                    hWnd,
                    (HMENU)IDC_COMBOBOC_RUBBER,
                    hInstance,
                    NULL);

             // *** Rubber combobox ***
            hCmbxRubber = CreateWindowEx(NULL,
                    TEXT("combobox"),
                    NULL,
                    WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_DROPDOWNLIST ,
                    75, 235,
                    80, 300,
                    hWnd,
                    (HMENU)IDC_COMBOBOC_RUBBER,
                    hInstance,
                    NULL);

            SendMessage(hCmbxRubber,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"1"));
            SendMessage(hCmbxRubber,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"2"));
            SendMessage(hCmbxRubber,
                        CB_ADDSTRING,
                        0,
                        reinterpret_cast<LPARAM>((LPCTSTR)"3"));

            // *** Drawing the static text
            CreateWindowEx(0, TEXT("static"), "R",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                55, 5, 30, 17,
                hWnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx(0, TEXT("static"), "G",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                90, 5, 30, 17,
                hWnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx(0, TEXT("static"), "B",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                125, 5, 30, 17,
                hWnd, (HMENU)0, hInstance, NULL);

            CreateWindowEx(0, TEXT("static"), "Selected Color",
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                30, 290, 100, 17,
                hWnd, (HMENU)0, hInstance, NULL);
        }
        break;

    case WM_LBUTTONDOWN: {
            if (wParam & MK_LBUTTON) {

                if(!checkBoundries(LOWORD (lParam), HIWORD (lParam), rcDrawingArea)) {
                    outOfBoud = 1;
                    return 0;
                }
                outOfBoud = 0;

                if(eraseMode) {
                    int centerX = LOWORD (lParam);
                    int centerY = HIWORD (lParam);
                    int delta = 5 + (rubberMark * 3);
                    SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(255, 255, 255)));
                    SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));

                    coords[0].x = centerX - delta;
                    coords[0].y = centerY + delta;

                    coords[1].y = centerY - delta;
                    coords[1].x = centerX + delta;

                    drawRectangle(coords, hdc);
                    return 0;
                }

                if (drawItemMark == 3) {
                    SelectObject(hdc, CreatePen(PS_SOLID, thicknessMark + 1, RGB(255, 255, 255)));
                    switch (bezierStage) {
                    case 0:
                        for (int i = 0; i < 4; i++) {
                            coords[i].x = LOWORD (lParam);
                            coords[i].y = HIWORD (lParam);
                        }
                        break;
                    case 1:
                        drawBezier(coords, hdc);
                        coords[1].x = LOWORD (lParam);
                        coords[1].y = HIWORD (lParam);
                        SelectObject(hdc, hPen);
                        drawBezier(coords, hdc);
                        break;
                    case 2:
                        drawBezier(coords, hdc);
                        coords[2].x = LOWORD (lParam);
                        coords[2].y = HIWORD (lParam);
                        SelectObject(hdc, hPen);
                        drawBezier(coords, hdc);
                        break;
                    }
                } else {
                    coords[0].x = LOWORD (lParam);
                    coords[0].y = HIWORD (lParam);
                    coords[1].x = LOWORD (lParam);
                    coords[1].y = HIWORD (lParam);
                    bezierStage = 0;
                }
            }
        }
        break;

    case WM_LBUTTONUP:{
        if (drawItemMark == 3) {
            switch (bezierStage) {
            case 0:
                bezierStage = 1;
                break;
            case 1:
                bezierStage = 2;
                break;
            case 2:
                bezierStage = 0;
                break;
            }
        }
    }
    break;

	case WM_MOUSEMOVE: {
            if (wParam & MK_LBUTTON) {
                if (outOfBoud) {
                    bezierStage --;
                    return 0;
                }

                if (!checkBoundries(LOWORD (lParam), HIWORD (lParam), rcDrawingArea))
                    return 0;
                SelectObject(hdc, CreatePen(PS_SOLID, thicknessMark + 1, RGB(255, 255, 255)));
                SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));

                if (eraseMode) {
                    int centerX = LOWORD (lParam);
                    int centerY = HIWORD (lParam);
                    int delta = 5 + (rubberMark * 3);
                    SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(255, 255, 255)));
                    SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));

                    coords[0].x = centerX - delta;
                    coords[0].y = centerY + delta;

                    coords[1].y = centerY - delta;
                    coords[1].x = centerX + delta;

                    drawRectangle(coords, hdc);
                    return 0;
                }

                switch (drawItemMark) {
                case 0:{                                // Line
                        drawLine(coords, hdc);
                        coords[1].x = LOWORD (lParam);
                        coords[1].y = HIWORD (lParam);
                        SelectObject(hdc, hPen);
                        SelectObject(hdc, hBrush);
                        drawLine(coords, hdc);
                    }
                    break;
                case 1: {                                // Rectangle
                        drawRectangle(coords, hdc);
                        coords[1].x = LOWORD (lParam);
                        coords[1].y = HIWORD (lParam);
                        SelectObject(hdc, hPen);
                        if (checkBoxMark) {
                            SelectObject(hdc, hBrush);
                        }
                        drawRectangle(coords, hdc);
                    }
                    break;
                case 2:{                                // Circle
                        drawCircle(coords, hdc);
                        coords[1].x = LOWORD (lParam);
                        coords[1].y = HIWORD (lParam);
                        SelectObject(hdc, hPen);
                        if (checkBoxMark) {
                            SelectObject(hdc, hBrush);
                        }
                        drawCircle(coords, hdc);
                    }
                    break;
                case 3: {                               // Bezier Curve
                        switch (bezierStage) {
                        case 0:
                            drawBezier(coords, hdc);
                            coords[3].x = LOWORD (lParam);
                            coords[3].y = HIWORD (lParam);
                            SelectObject(hdc, hPen);
                            drawBezier(coords, hdc);
                            break;
                        case 1:
                            drawBezier(coords, hdc);
                            coords[1].x = LOWORD (lParam);
                            coords[1].y = HIWORD (lParam);
                            SelectObject(hdc, hPen);
                            drawBezier(coords, hdc);
                            break;
                        case 2:
                            drawBezier(coords, hdc);
                            coords[2].x = LOWORD (lParam);
                            coords[2].y = HIWORD (lParam);
                            SelectObject(hdc, hPen);
                            drawBezier(coords, hdc);
                            break;
                        }
                    }
                    break;
                }
            }
        }
	    break;

    case WM_PAINT: {
            HDC hdc = BeginPaint(hWnd, &Ps);
            RECT temp;

            SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
            SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
            Rectangle(hdc, rcDrawingArea.left, rcDrawingArea.top, rcDrawingArea.right, rcDrawingArea.bottom);

            SelectObject(hdc, CreateSolidBrush(RGB(penColor[0], penColor[1], penColor[2])));
            Rectangle(hdc, rcPenColor.left, rcPenColor.top, rcPenColor.right, rcPenColor.bottom);

            SelectObject(hdc, CreateSolidBrush(RGB(brushColor[0], brushColor[1], brushColor[2])));
            Rectangle(hdc, rcBrushColor.left, rcBrushColor.top, rcBrushColor.right, rcBrushColor.bottom);

            // *** Gradient
            int r1 = 255, g1 = 0, b1 = 0;

            /* End Color; Change the R,G,B values
            to the color of your choice */
            int r2 = 255, g2 = 255, b2 = 0;

            /* loop to create the gradient */
            for(int i = rcGradient.left; i < rcGradient.right; i++)
            {
                int r,g,b;
                r = r1 + (i * (r2-r1) / rcGradient.right);
                g = g1 + (i * (g2-g1) / rcGradient.right);
                b = b1 + (i * (b2-b1) / rcGradient.right);
                temp.left = i;
                temp.top = rcGradient.top;
                temp.right = i + 1;
                temp.bottom = rcGradient.bottom;
                HBRUSH color = CreateSolidBrush(RGB(r, g, b));
                FillRect(hdc, &temp, color);
            }

            // Load a bitmap image
            // Static Bitmap Code:
            HWND hStatic = CreateWindowEx (WS_EX_STATICEDGE, "STATIC", NULL,
            WS_CHILD | WS_VISIBLE | SS_BITMAP, 0, 420, 0, 0, hWnd, 0, hInstance, NULL);

            HANDLE hImage = LoadImage(hInstance, bmpfile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            SendMessage(hStatic, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);


            EndPaint(hWnd, &Ps);
        }
        break;

    case WM_COMMAND: {

        switch (LOWORD(wParam)) {
        case IDC_BUTTON_SET: {
                char rBuffer[30], gBuffer[30], bBuffer[30];
                SendMessage(hInptColorR, WM_GETTEXT, 4, reinterpret_cast<LPARAM>(rBuffer));
                SendMessage(hInptColorG, WM_GETTEXT, 4, reinterpret_cast<LPARAM>(gBuffer));
                SendMessage(hInptColorB, WM_GETTEXT, 4, reinterpret_cast<LPARAM>(bBuffer));

                int red = atoi(rBuffer);
                int green = atoi(gBuffer);
                int blue = atoi(bBuffer);

                int radioPen = SendMessage(hRadioPen, BM_GETCHECK, 0, 0);
                if (radioPen) {
                    penColor[0] = red;
                    penColor[1] = green;
                    penColor[2] = blue;
                    setupPen(thicknessMark + 1, penColor);
                    InvalidateRect(hWnd, &rcPenColor, TRUE);
                } else {
                    brushColor[0] = red;
                    brushColor[1] = green;
                    brushColor[2] = blue;
                    setupBrush(brushColor);
                    InvalidateRect(hWnd, &rcBrushColor, TRUE);
                }
            }
            break;

        case IDC_BUTTON_DRAW: {
                drawItemMark = SendMessage((HWND) hCmbxDrawnObj, (UINT) CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                if (drawItemMark < 0) {
                    drawItemMark = 0;
                }
                checkBoxMark = SendMessage(hChbxFilled, BM_GETCHECK, 0, 0);
                eraseMode = 0;
            }
            break;

        case IDC_COMBOBOX_THICKNESS: {
                thicknessMark = SendMessage((HWND) hCmbxThickness, (UINT) CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                if (thicknessMark < 0) {
                    thicknessMark = 0;
                }
                setupPen(thicknessMark + 1, penColor);
            }
            break;

        case IDC_COMBOBOC_RUBBER: {
                rubberMark = SendMessage((HWND) hCmbxRubber, (UINT) CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
                if (rubberMark < 0) {
                    rubberMark = 0;
                }
                eraseMode = 1;
            }
            break;
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

void setupPen(int stroke, int color[3]) {
    hPen = CreatePen(PS_SOLID, stroke, RGB(color[0], color[1], color[2]));
}


void setupBrush(int color[3]) {
    hBrush = CreateSolidBrush(RGB(color[0], color[1], color[2]));
}

int checkBoundries(int xPos, int yPos, RECT drawingArea){
    int delta;
    if (eraseMode) {
        delta = 5 + (rubberMark * 3);
    } else {
        delta = (thicknessMark+1)/2  + 1;
    }

    if (xPos < (drawingArea.left + delta))
        return 0;
    if (xPos > (drawingArea.right - delta))
        return 0;
    if (yPos < (drawingArea.top + delta))
        return 0;
    if (yPos > (drawingArea.bottom - delta))
        return 0;
    return 1;
}

