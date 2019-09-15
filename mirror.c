#define UNICODE
#define _UNICODE

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define main __main

//#define DEBUG

int main() {
	/* Init some things */
	HDC scr = GetDC(HWND_DESKTOP); /* Screen DC */
	int halfWidth = GetDeviceCaps(scr, HORZRES)>>1;
	int height = GetDeviceCaps(scr, VERTRES);

	/* Make class */
	WNDCLASS wc = {0};
	wc.lpfnWndProc   = DefWindowProc;
	wc.lpszClassName = _T("Mirror");
	wc.hCursor       = LoadCursor(NULL, IDC_NO);
	RegisterClass(&wc);

	/* Make window */
	HWND mirrWnd = CreateWindowEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST,_T("Mirror"),_T("Mirror"),WS_VISIBLE
#ifndef DEBUG
		|WS_POPUP
#endif
		,
		halfWidth, /* x */
		0, /* y */
		halfWidth, /* width */
		height, /* height */
		NULL,NULL,NULL,NULL);
		
	/* Init some more things */
	HDC win = GetDC(mirrWnd);
	POINT p[3] = {{0}};
	p[0].x = p[2].x = halfWidth;
	p[2].y = height;

	/* Do endless loop */
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0) > 0) {
		switch(msg.message) {
			case WM_PAINT: {
#ifdef DEBUG
				static int pc = 0;
				static char a[200];
				sprintf(a, "Painted %d times", ++pc);
				SetWindowText(mirrWnd, a);
#endif			
				PlgBlt(win, p, scr, 0, 0, halfWidth, height, NULL, 0, 0);
				break;
			}
			case WM_SYSCOMMAND:
				if (msg.wParam != SC_CLOSE)
					break;
			case WM_LBUTTONUP:
				goto exit;
			default:
				DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
		}
	}
exit:
	return 0;
}

