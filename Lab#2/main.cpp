#include <windows.h>
#include <stdio.h>
#include "resource.h"


#define BUTTON_ADD_FOOD         101
#define BUTTON_DISPLAY_FOOD_NR  102
#define INPUT_TEXT_SHOW_FOOD    103
#define INPUT_TEXT_ADD_FOOD     104


LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND hFoodList;
HWND hInputFood;
char food[500] = "";
char firstText[] = "Food List : ";
int foodNumber = 0;
static int scrollColor = 0;
HINSTANCE hInstance;




int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd) {

    hInstance = hInst;


    WNDCLASSEX wClass;
    //ZeroMemory(&wClass,sizeof(WNDCLASSEX));

    wClass.hInstance = hInst;
    wClass.lpszClassName = "Window Class";
    wClass.lpfnWndProc = (WNDPROC)WinProc;
    wClass.style = CS_HREDRAW|CS_VREDRAW;
    wClass.cbSize = sizeof(WNDCLASSEX);


    wClass.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
    wClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wClass.hIcon = NULL;
    wClass.hIconSm = NULL;
    wClass.lpszMenuName  = MAKEINTRESOURCE(IDR_MYMENU);
    //wClass.cbClsExtra = 0;                                                         // No extra bytes after the window class
    //wClass.cbWndExtra = 0;

    HMENU hmenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_MYMENU));

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
            500,
            200,
            400,
            300,
            NULL,
            hmenu,
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

    MSG msg;
    ZeroMemory(&msg,sizeof(MSG));

    while(GetMessage(&msg,NULL,0,0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


LRESULT CALLBACK WinProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam) {
    PAINTSTRUCT Ps;
    static HWND hWndList;
    static HWND hWndScroll;
    static RECT rcScroll, rcFoodList, rcInputFood, rcQuantity;
    HBRUSH hBrushStatic;
    static int fontColor[3];

    switch(msg) {
    case WM_CREATE: {

                SetRect(&rcScroll, 315, 40, 25, 150);
                SetRect(&rcFoodList, 10, 10, 100, 40);
                SetRect(&rcInputFood, 120, 150, 190, 25);
                SetRect(&rcQuantity, 210, 10, 300, 30);
                for (int i = 0; i < 3; i++) {
                    fontColor[i] = 0;
                }

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
                SetScrollPos(hWndScroll, SB_CTL, scrollColor, TRUE);

                //OldScroll = (WNDPROC) SetWindowLong(hWndSCroll, GWL_WNDPROC, (LONG) ScrollProc);
                /**
                * Create ListBox
                */
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


                /**
                * Create AddFood Button
                */
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

                /**
                * Create button ShowFoodNumber
                */
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

                /**
                * Draw main Input food field
                */

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

            }
            break;

    case WM_PAINT: {
            HDC hdc = BeginPaint(hWnd, &Ps);

            /**
            * Draw Text
            */

            // Second Text
            char foodNrMessage[256] = "Number : ";
            char nr[50];
            strcat(foodNrMessage, itoa(foodNumber, nr, 10));
            SetBkMode(hdc, TRANSPARENT);

            DrawText( hdc, foodNrMessage, -1, &rcQuantity, DT_SINGLELINE | DT_NOCLIP) ;

            // First Text
            HFONT hFont = CreateFont(25,0,0,0,FW_DONTCARE,FALSE,TRUE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,NULL, VARIABLE_PITCH,TEXT("Impact"));


            SelectObject(hdc, hFont);
            SetBkMode(hdc, OPAQUE);
            SetBkColor(hdc, RGB(scrollColor,scrollColor + 70, scrollColor+150));
            SetTextColor(hdc, RGB(fontColor[0], fontColor[1], fontColor[2]));
            DrawText(hdc, TEXT(firstText), -1, &rcFoodList, DT_NOCLIP);

            EndPaint(hWnd, &Ps);
        }
        break;

    case WM_CTLCOLOREDIT: {
            HDC hdc = (HDC)wParam;
            HWND hwnd = (HWND)lParam;
            HBRUSH color;

            if (GetDlgCtrlID(hwnd) == INPUT_TEXT_ADD_FOOD) {
                color = CreateSolidBrush(RGB(225, 225, 225));
                SetTextColor(hdc, RGB(0, 0, 255));
                SetBkMode(hdc, TRANSPARENT);
                SetBkColor(hdc,(LONG)color);
            }
            return (LONG) color;
        }
        break;

    case WM_COMMAND: {
        switch(LOWORD(wParam)) {
        case BUTTON_ADD_FOOD: {

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
                    //RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
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
            switch (LOWORD(wParam)) {
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
            InvalidateRect(hWnd, &rcFoodList, TRUE);
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
            switch (wParam) {

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
                    }
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

    return DefWindowProc(hWnd,msg,wParam,lParam);
}
