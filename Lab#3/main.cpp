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




HINSTANCE hInstance;
/// Global Variables
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
    static HWND hRadioPen, hRadioBrush;                                 // Radio buttons

    RECT rcPenColor, rcBrushColor;
    RECT rcDrawingArea;


    SetRect(&rcPenColor, 30, 315, 75, 360);
    SetRect(&rcBrushColor, 85, 315, 130, 360);
    SetRect(&rcDrawingArea, 180, 10, 770, 550);

    HDC hdc = GetDC(hWnd);

    //static HWNDrcPenColordrawItemMark = 0;


    /*static HWND hWndList;
    static HWND hWndScroll, hWndWidthScroll, hWndHeightScroll;*/

    HBRUSH hBrushStatic;

    switch(msg) {
    case WM_CREATE: {

            //SetRect(&rectangle, 200, 300, 300, 300);
            //HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            //SelectObject (hdc, GetStockObject (WHITE_PEN)) ;

            //FillRect (hdc, &rectangle, hBrush);

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
                SelectObject(hdc, CreatePen(PS_SOLID, thicknessMark + 1, RGB(255, 255, 255)));
                SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));

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
            //RECT rcDrawingArea;
            //SetRect(&rcDrawingArea, 180, 10, 750, 530);
            //HPEN hpen = CreatePen(PS_SOLID, 10, RGB(0, 255, 0));
            SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(0, 0, 0)));
            SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
            Rectangle(hdc, rcDrawingArea.left, rcDrawingArea.top, rcDrawingArea.right, rcDrawingArea.bottom);

            SelectObject(hdc, CreateSolidBrush(RGB(penColor[0], penColor[1], penColor[2])));
            Rectangle(hdc, rcPenColor.left, rcPenColor.top, rcPenColor.right, rcPenColor.bottom);

            SelectObject(hdc, CreateSolidBrush(RGB(brushColor[0], brushColor[1], brushColor[2])));
            Rectangle(hdc, rcBrushColor.left, rcBrushColor.top, rcBrushColor.right, rcBrushColor.bottom);
            //FillRect(hdc, &rcDrawingArea, CreateSolidBrush(RGB(255, 255, 255)));
            //RECT rectangle;
            /*MoveToEx (hdc, 0, 0, NULL) ;
            LineTo (hdc, 100, 100) ;*/
            /*HDC hdc = BeginPaint(hWnd, &Ps);
            RECT rect;
            SetRect(&rect, 315, 40, 25, 150);
            HBRUSH hBrush = CreateSolidBrush(RGB(250, 25, 5));

            //HDC hdc = BeginPaint(hWnd, &Ps);


            /**
            * Draw Text
            */
/*
            // Second Text
            char foodNrMessage[40];
            char nr[50];
            LoadString (hInstance, IDS_FOODNUMBER, foodNrMessage, 40) ;
            wsprintf (nr, foodNrMessage, foodNumber);


            //strcat(foodNrMessage, itoa(, nr, 10));
            SetBkMode(hdc, TRANSPARENT);

            DrawText( hdc, nr, -1, &rcQuantity, DT_SINGLELINE | DT_NOCLIP) ;

            // First Text
            HFONT hFont = CreateFont(25,0,0,0,FW_DONTCARE,FALSE,TRUE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,NULL, VARIABLE_PITCH,TEXT("Impact"));

            /*fontColor[0] = 200;
            fontColor[0] = 0;
            fontColor[0] = 110;*/
/*
            SelectObject(hdc, hFont);
            SetBkMode(hdc, OPAQUE);
            SetBkColor(hdc, RGB(scrollColor,scrollColor + 70, scrollColor+150));
            SetTextColor(hdc, RGB(fontColor[0], fontColor[1], fontColor[2]));
            DrawText(hdc, TEXT(firstText), -1, &rcFoodList, DT_NOCLIP);
*/
            //EndPaint(hWnd, &Ps);
            EndPaint(hWnd, &Ps);
        }
        break;

    case WM_CTLCOLOREDIT: {
            /*HDC hdc = (HDC)wParam;
            HWND hwnd = (HWND)lParam;
            HBRUSH color;

            if (GetDlgCtrlID(hwnd) == INPUT_TEXT_ADD_FOOD) {
                color = CreateSolidBrush(RGB(225, 225, 225));
                SetTextColor(hdc, RGB(0, 0, 255));
                SetBkMode(hdc, TRANSPARENT);
                SetBkColor(hdc,(LONG)color);
            }
            return (LONG) color;*/
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

            /*TCHAR  ListItem[256];
            (TCHAR) SendMessage((HWND) hCmbxDrawnObj, (UINT) CB_GETLBTEXT,
                (WPARAM) ItemIndex, (LPARAM) ListItem);*/

        /*switch(LOWORD(wParam)) {
        case BUTTON_ADD_FOOD: {
8
                char buffer[256];
                SendMessage(hInputFood,
                    WM_GETTEXT,
                    sizeof(buffer)/sizeof(buffer[0]),
                    reinterpret_cast<LPARAM>(buffer));


                if (strlen(buffer) > 0){
                    char newInput[255] = "- ";

                    strcat(newInput, buffer);

                    SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)newInput);
                    SendMessage(hInputFood, WM_SETTEXT, NULL, (LPARAM)"");
                    foodNumber++;
                    InvalidateRect(hWnd, &rcQuantity, TRUE);
                    //RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);

                }
            }
            break;

        case BUTTON_DISPLAY_FOOD_NR: {
                char buffer[255] = "";

                switch(foodNumber){
                case 0:
                case 1:
                case 2:
                case 3:
                    strcat(buffer, "You are not hungry at all");
                    break;
                case 4:
                case 5:
                case 6:
                    strcat(buffer, "I see you are hungry now");
                    break;
                default:
                    strcat(buffer, "You are starvin... go get something to eat");
                    break;
                }
                MessageBox(NULL,
                    buffer,
                    "Funny",
                    MB_ICONINFORMATION);
            }
            break;
        case ID_FILE_EXIT: {
                PostQuitMessage(0);
            }
            break;
        case ID_STUFF_ABOUT: {
                char aboutText[255] = "Here is the info about the program.\n It's Created by Sergiu Terman in 2013";
                MessageBox(NULL,
                    aboutText,
                    "About",
                    MB_ICONINFORMATION);
            }
            break;
        case ID_EVENT_RED: {
                fontColor[0] = 255;
                fontColor[1] = 0;
                fontColor[2] = 0;
                InvalidateRect(hWnd, &rcFoodList, TRUE);
            }
            break;
        case ID_EVENT_GREEN: {
                fontColor[0] = 0;
                fontColor[1] = 255;
                fontColor[2] = 0;
                InvalidateRect(hWnd, &rcFoodList, TRUE);
            }
            break;
        case ID_EVENT_BLUE: {
                fontColor[0] = 0;
                fontColor[1] = 0;
                fontColor[2] = 255;
                InvalidateRect(hWnd, &rcFoodList, TRUE);
            }
            break;

        case IDC_FOOD_LIST:{
                if (HIWORD(wParam) == LBN_DBLCLK) {
                    int index = SendMessage(hWndList, LB_GETCURSEL, 0, 0);
                    SendMessage(hWndList, LB_DELETESTRING, (WPARAM)index, 0);
                    foodNumber--;
                    InvalidateRect(hWnd, &rcQuantity, TRUE);
                }
            }
            break;
        }
    }
    break;

    case WM_SIZE: {
            /*INT nWidth = LOWORD(lParam);
            HWND hFunnyButton = GetDlgItem(hWnd, BUTTON_DISPLAY_FOOD_NR);

            MoveWindow(hFunnyButton, 10, 180, nWidth - 17, 40, TRUE);

            HWND hShowFoodInput = GetDlgItem(hWnd, INPUT_TEXT_SHOW_FOOD);
            HWND hAddFood = GetDlgItem(hWnd, INPUT_TEXT_ADD_FOOD);

            MoveWindow(hShowFoodInput, 10, 40, nWidth - 18, 100, TRUE);
            MoveWindow(hAddFood, 120, 150, nWidth - 128, 25, TRUE);*/
        }
        break;

    case WM_VSCROLL: {

   /*         switch (LOWORD(wParam)) {
            case SB_LINEDOWN: {
                    scrollColor = min (255, scrollColor + 1);
                }
                break;
            case SB_LINEUP: {
                    scrollColor = min (255, scrollColor - 1);
                }
                break;
            case SB_PAGEDOWN: {
                    scrollColor += 15;
                }
                break;
            case SB_PAGEUP: {
                    scrollColor -= 15;
                }
                break;
            case SB_BOTTOM: {
                    scrollColor = 255;
                }
                break;
            case SB_TOP: {
                    scrollColor = 0;
                }
                break;
            case SB_THUMBPOSITION:
            case SB_THUMBTRACK: {
                    scrollColor = HIWORD(wParam);
                }
                break;
            }

            SetScrollPos(hWndScroll, SB_CTL, scrollColor, TRUE);
            InvalidateRect(hWnd, &rcFoodList, TRUE);*/
        }
        break;

    case WM_HSCROLL: {
            /*RECT rect;
            GetWindowRect(hWnd, &rect);
            int iSysWidth = GetSystemMetrics(SM_CXSCREEN);
            int iSysHeight = GetSystemMetrics(SM_CYSCREEN);
            int iWinWidth = rect.right - rect.left;
            int iWinHeight = rect.bottom - rect.top;

            switch (GetWindowLong((HWND)lParam, GWL_ID)) {
            case ID_WIDTH_SCROLL: {
                    switch(LOWORD(wParam)) {
                    case SB_LINELEFT:
                        widthScroll -= 1;
                        break;
                    case SB_LINERIGHT:
                        widthScroll += 1;
                        break;
                    case SB_THUMBPOSITION:
                        widthScroll = HIWORD(wParam);
                        break;
                    default:
                        break;
                    }
                    SetScrollPos(hWndWidthScroll, SB_CTL, widthScroll, TRUE);
                    MoveWindow(hWnd, rect.left, rect.top, (widthScroll * iSysWidth / 100), iWinHeight, TRUE);
                }
                break;
            case ID_HEIGHT_SCROLL: {
                    switch(LOWORD(wParam)) {
                    case SB_LINELEFT:
                        widthScroll--;
                        break;
                    case SB_LINERIGHT:
                        widthScroll++;
                        break;
                    case SB_THUMBPOSITION:
                        widthScroll = HIWORD(wParam);
                        break;
                    default:
                        break;
                    }
                    SetScrollPos(hWndHeightScroll, SB_CTL, widthScroll, TRUE);
                    MoveWindow(hWnd, rect.left, rect.top, iWinWidth, (widthScroll * iSysHeight / 100), TRUE);
                }
                break;
            }*/
        }
        break;

    case WM_GETMINMAXINFO: {
            MINMAXINFO * mmiStruct;
            mmiStruct = (MINMAXINFO*)lParam;

            POINT ptPoint;

            ptPoint.x = 335;    //Minimum width of the window.
            ptPoint.y = 260;    //Minimum height of the window.
            mmiStruct->ptMinTrackSize = ptPoint;

            ptPoint.x = GetSystemMetrics(SM_CXMAXIMIZED);   //Maximum width of the window.
            ptPoint.y = GetSystemMetrics(SM_CYMAXIMIZED);   //Maximum height of the window.
            mmiStruct->ptMaxTrackSize = ptPoint;
        }
        break;

    case WM_KEYDOWN: {

            /*switch (wParam) {

            case VK_SPACE : {
                    if (HIBYTE(GetAsyncKeyState(VK_LCONTROL))) {
                        scrollColor = 0;
                        SetScrollPos(hWndScroll, SB_CTL, scrollColor, TRUE);
                        InvalidateRect(hWnd, &rcFoodList, TRUE);
                        return 0;
                    }
                }
                break;
            case VK_F1: {
                    if (HIBYTE(GetAsyncKeyState(VK_LCONTROL))) {

                        SendMessage(hWndList, LB_RESETCONTENT, 0, 0);
                        foodNumber = 0;
                        InvalidateRect(hWnd, &rcQuantity, TRUE);
                        /*fontColor[0] = 255;
                        fontColor[1] = 255;
                        fontColor[2] = 255;
                        InvalidateRect(hWnd, &rcFoodList, TRUE);*/
              /*      }
                }
                break;
            }*/
        }
        break;

    case WM_SETFOCUS: {
            SetFocus(hWnd);
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

                /**
                //Create Scrolls
                hWndScroll = CreateWindowEx((DWORD)NULL,
                    TEXT("scrollbar"),
                    NULL,
                    WS_CHILD | WS_VISIBLE | SBS_VERT,
                    315, 40, 25, 150,
                    hWnd,
                    (HMENU) ID_SCROLL_BAR,
                    hInstance,
                    NULL);
                SetScrollRange(hWndScroll,SB_CTL, 0, 255, FALSE);
                SetScrollPos(hWndScroll, SB_CTL, 0, TRUE);

                hWndWidthScroll = CreateWindowEx((DWORD)NULL,
                    TEXT("scrollbar"),
                    NULL,
                    WS_CHILD | WS_VISIBLE | SBS_HORZ,
                    10, 230, 300, 20,
                    hWnd, /**
                //Create Scrolls
                hWndScroll = CreateWindowEx((DWORD)NULL,
                    TEXT("scrollbar"),
                    NULL,
                    WS_CHILD | WS_VISIBLE | SBS_VERT,
                    315, 40, 25, 150,
                    hWnd,
                    (HMENU)ID_WIDTH_SCROLL,
                    hInstance,
                    NULL);
                SetScrollRange(hWndWidthScroll, SB_CTL, 0, 100, TRUE);
                SetScrollPos(hWndWidthScroll, SB_CTL, 0, TRUE);

                hWndHeightScroll = CreateWindowEx((DWORD)NULL,
                    TEXT("scrollbar"),
                    NULL,
                    WS_CHILD | WS_VISIBLE | SBS_HORZ,
                    10, 260, 300, 20,
                    hWnd,
                    (HMENU)ID_HEIGHT_SCROLL,
                    hInstance,
                    NULL);
                SetScrollRange(hWndHeightScroll, SB_CTL, 0, 100, TRUE);
                SetScrollPos(hWndHeightScroll, SB_CTL, 45, TRUE);

                /**
                * Create ListBox
                */
                /**
                hWndList = CreateWindowEx((DWORD)NULL,
                    TEXT("listbox"),
                    "",
                    WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_STANDARD | WS_BORDER,
                    10, 40,
                    300, 100,
                    hWnd,
                    (HMENU) IDC_FOOD_LIST,
                    hInstance,
                    NULL);

                */
                /**
                * Create AddFood Button
                */
                /**
                HFONT hFont = CreateFont(30,0,0,0,FW_DONTCARE,FALSE,TRUE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                    CLIP_DEFAULT_PRECIS,NULL, VARIABLE_PITCH,TEXT("Impact"));

                HWND hButtonAddFood = CreateWindowEx(NULL,
                    "BUTTON",
                    "ADD FOOD",
                    WS_TABSTOP|WS_VISIBLE|
                    WS_CHILD|BS_DEFPUSHBUTTON|BS_TOP,
                    10,
                    150,
                    100,
                    25,
                    hWnd,
                    (HMENU)BUTTON_ADD_FOOD,
                    GetModuleHandle(NULL),
                    NULL);
                */
                /**
                * Create button ShowFoodNumber
                */
                /**
                HWND hShowFoodNumber = CreateWindowEx(NULL,
                    "BUTTON",
                    "Funny",
                    WS_TABSTOP|WS_VISIBLE|
                    WS_CHILD|BS_DEFPUSHBUTTON|BS_TOP,
                    10,
                    180,
                    300,
                    40,
                    hWnd,
                    (HMENU)BUTTON_DISPLAY_FOOD_NR,
                    GetModuleHandle(NULL),
                    NULL);
                SendMessage (hShowFoodNumber, WM_SETFONT, WPARAM (hFont), TRUE);
                */
                /**
                * Draw main Input food field
                */
                /**
                hInputFood = CreateWindowEx(
                    (DWORD)NULL,
                    TEXT("edit"),
                    "",
                    WS_VISIBLE | WS_CHILD | WS_BORDER,
                    120,
                    150,
                    190,
                    25,
                    hWnd,
                    (HMENU)INPUT_TEXT_ADD_FOOD,
                    GetModuleHandle(NULL),
                    NULL);

            }*/
