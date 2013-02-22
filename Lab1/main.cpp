#include <windows.h>
#include <stdio.h>

#define BUTTON_ADD_FOOD         101
#define BUTTON_DISPLAY_FOOD_NR  102
#define INPUT_TEXT_SHOW_FOOD    103
#define INPUT_TEXT_ADD_FOOD     104

HWND hFoodList;
HWND hInputFood;
char food[500] = "";
char firstText[] = "Food List : ";
int foodNumber = 0;
RECT updateRect;

LRESULT CALLBACK WinProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nShowCmd) {
	WNDCLASSEX wClass;
	ZeroMemory(&wClass,sizeof(WNDCLASSEX));
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wClass.hIcon = NULL;
	wClass.hIconSm = NULL;
	wClass.hInstance = hInst;
	wClass.lpfnWndProc = (WNDPROC)WinProc;
	wClass.lpszClassName = "Window Class";
	wClass.lpszMenuName = NULL;
	wClass.style = CS_HREDRAW|CS_VREDRAW;

	if (!RegisterClassEx(&wClass)) {
		int nResult=GetLastError();
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
			335,
			260,
			NULL,
			NULL,
			hInst,
			NULL);

	if(!hWnd) {
		int nResult=GetLastError();

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
	switch(msg) {
    case WM_CREATE: {

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
            * Draw Food List (In a input box)
            */


            hFoodList = CreateWindowEx(WS_EX_CLIENTEDGE,
                "EDIT",
                "",
                WS_CHILD|WS_VISIBLE|WS_VSCROLL|ES_READONLY|
                ES_MULTILINE,
                10,
                40,
                300,
                100,
                hWnd,
                (HMENU)INPUT_TEXT_SHOW_FOOD,
                GetModuleHandle(NULL),
                NULL);


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
            RECT rect;
            /**
            * Draw Text
            */

            // Second Text
            char foodNrMessage[256] = "Number : ";
            char nr[50];
            strcat(foodNrMessage, itoa(foodNumber, nr, 10));
            SetBkMode(hdc, TRANSPARENT);
            SetRect(&updateRect, 210, 10, 300, 30);
            DrawText( hdc, foodNrMessage, -1, &updateRect, DT_SINGLELINE | DT_NOCLIP  ) ;

            // First Text
            HFONT hFont = CreateFont(25,0,0,0,FW_DONTCARE,FALSE,TRUE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,NULL, VARIABLE_PITCH,TEXT("Impact"));
            SetRect(&rect, 10, 10, 50, 50);

            SelectObject(hdc, hFont);
            SetBkMode(hdc, OPAQUE);
            SetBkColor(hdc, RGB(0,255,255));
            SetTextColor(hdc, RGB(255,0,0));
            DrawText(hdc, TEXT(firstText), -1,&rect, DT_NOCLIP);

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

                    if(strlen(buffer) > 0){
                        char newInput[255] = "";
                        char stat[30];
                        strcat(newInput, itoa((foodNumber+1), stat, 10) );
                        strcat(newInput, " ) ");
                        strcat(newInput, buffer);
                        strcat(newInput, "\r\n");

                        SendMessage(hFoodList, EM_REPLACESEL, FALSE, (LPARAM)newInput);
                        SendMessage(hInputFood, WM_SETTEXT, NULL, (LPARAM)"");
                        foodNumber++;
                        InvalidateRect(hWnd, &updateRect, TRUE);
                    }

                }
                break;

                case BUTTON_DISPLAY_FOOD_NR: {
                    char buffer[255] = "";

                    switch(foodNumber){
                    case 0:http://pastebin.com/62fGU90U
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
            }
        }
        break;

        case WM_SIZE: {
            INT nWidth = LOWORD(lParam);
            HWND hFunnyButton = GetDlgItem(hWnd, BUTTON_DISPLAY_FOOD_NR);

            MoveWindow(hFunnyButton, 10, 180, nWidth - 17, 40, TRUE);

            HWND hShowFoodInput = GetDlgItem(hWnd, INPUT_TEXT_SHOW_FOOD);
            HWND hAddFood = GetDlgItem(hWnd, INPUT_TEXT_ADD_FOOD);

            MoveWindow(hShowFoodInput, 10, 40, nWidth - 18, 100, TRUE);
            MoveWindow(hAddFood, 120, 150, nWidth - 128, 25, TRUE);
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

		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd,msg,wParam,lParam);
}


