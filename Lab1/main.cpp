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

            /*HGDIOBJ hfDefault1 = GetStockObject(DEFAULT_GUI_FONT);
            HFONT hFont = CreateFont(25,0,0,0,FW_DONTCARE,FALSE,TRUE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,NULL, VARIABLE_PITCH,TEXT("Impact"));

            SendMessage(hFoodList,
                WM_SETFONT,
                (WPARAM)hfDefault1,
                MAKELPARAM(FALSE,0));
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

            /*SendMessage(hFoodList,
                WM_SETTEXT,INPUT_TEXT_SHOW_FOOD,
                (LPARAM) food);*/

            /**
            * Draw main Input food field
            */

            hInputFood = CreateWindowEx(
                (DWORD)NULL,
                TEXT("edit"),                                                   // The class name required is edit
                "",                                                             // Default text.
                WS_VISIBLE | WS_CHILD | WS_BORDER,                              // Textbox styles
                120,
                150,                                                         // the left and top co-ordinates
                190,
                25,                                                        // width and height
                hWnd,                                                           // parent window handle
                (HMENU)INPUT_TEXT_ADD_FOOD,                                          // the ID of your editbox
                GetModuleHandle(NULL),                                                          // the instance of your application
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
                SetTextColor(hdc, RGB(0, 0, 255));                            // Text color
                SetBkMode(hdc, TRANSPARENT);                                // EditBox Backround Mode
                SetBkColor(hdc,(LONG)color);
            }
            return (LONG) color;
        }
        break;

        case WM_CTLCOLORBTN: {
            HDC hdc = (HDC)wParam;
            HWND hwnd = (HWND)lParam;
            HBRUSH color;

            if (GetDlgCtrlID(hwnd) == BUTTON_DISPLAY_FOOD_NR) {
                color = CreateSolidBrush(RGB(225, 225, 225));
                SetTextColor(hdc, RGB(0, 0, 255));                            // Text color
                SetBkMode(hdc, TRANSPARENT);                                // EditBox Backround Mode
                SetBkColor(hdc,(LONG)color);
                return (LRESULT) color;
            }
        }

		case WM_COMMAND: {
            switch(LOWORD(wParam)) {
                case BUTTON_ADD_FOOD:{

                    char buffer[256];
                    SendMessage(hInputFood,
                        WM_GETTEXT,
                        sizeof(buffer)/sizeof(buffer[0]),
                        reinterpret_cast<LPARAM>(buffer));

                    if(strlen(buffer) > 0){
                        strcat(buffer, "\r\n");
                        SendMessage(hFoodList, EM_REPLACESEL, FALSE, (LPARAM)buffer);
                        SendMessage(hInputFood, WM_SETTEXT, NULL, (LPARAM)"");
                        foodNumber++;
                        InvalidateRect(hWnd, &updateRect, TRUE);
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



//RECT rect;
                /**
                * Drawing Buttons
                */



                /*
                BeginPaint(hWnd, &Ps);
                SetRect(&rect, 50, 50,700,200);
                DrawText(hdc, TEXT("Drawing Text with Impact"), -1,&rect, DT_NOCLIP);

                HFONT hFont = CreateFont(25,0,0,0,FW_DONTCARE,FALSE,TRUE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,NULL, VARIABLE_PITCH,TEXT("Impact"));
                SetRect(&rect, 50, 100, 700, 200);

                SelectObject(hdc, hFont);
                SetBkMode(hdc, OPAQUE);
                SetBkColor(hdc, RGB(0,255,255));
                SetTextColor(hdc, RGB(255,0,0));
                DrawText(hdc, TEXT("Drawing Text with Impact"), -1,&rect, DT_NOCLIP);
                */

                /*hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD|WS_VISIBLE|
				ES_MULTILINE|ES_AUTOVSCROLL|ES_AUTOHSCROLL,
				50,
				100,
				200,
				100,
				hWnd,
				(HMENU)IDC_MAIN_EDIT,
				GetModuleHandle(NULL),
				NULL);

				HGDIOBJ hfDefault=GetStockObject(DEFAULT_GUI_FONT);
                SendMessage(hEdit,
                    WM_SETFONT,
                    (WPARAM)hfDefault,
                    MAKELPARAM(FALSE,0));

                SendMessage(hEdit,
                    WM_SETTEXT,IDC_MAIN_EDIT,
                    (LPARAM)"Insert text here...");

                HWND hWndButton=CreateWindowEx(NULL,
				"BUTTON",
				"OK",
				WS_TABSTOP|WS_VISIBLE|
				WS_CHILD|BS_DEFPUSHBUTTON|BS_TOP,
				10,
				10,
				100,
				25,
				hWnd,
				(HMENU)IDC_MAIN_BUTTON,
				GetModuleHandle(NULL),
				NULL);

			SendMessage(hWndButton,
				WM_SETFONT,
				(WPARAM)hfDefault,
				MAKELPARAM(FALSE,0));*/
 /*MessageBox(NULL,
                    buffer,
                    "Information",
                    MB_ICONINFORMATION);
                    */
