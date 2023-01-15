#pragma comment(linker, "/opt:nowin98")
#include <windows.h>

CHAR szClassName[]="window";

#define SNAP_PIXELS 50
RECT m_rDesktopRect;
SIZE m_szMoveOffset;

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg){
	case WM_CREATE:
		m_szMoveOffset.cx=0;
		m_szMoveOffset.cy=0;
		GetWindowRect(GetDesktopWindow(),&m_rDesktopRect);
		break;
	case WM_MOVING:
		{
			RECT *pRect=(RECT *)lParam;
			SIZE szSize={pRect->right-pRect->left,pRect->bottom-pRect->top};
			POINT pt={NULL};
			GetCursorPos(&pt);
			pRect->left=pt.x-m_szMoveOffset.cx;
			pRect->top=pt.y-m_szMoveOffset.cy;
			pRect->right=pRect->left+szSize.cx;
			pRect->bottom=pRect->top+szSize.cy;
			if(pRect->top < SNAP_PIXELS && pRect->top > -SNAP_PIXELS)
			{
				pRect->top=0;
				pRect->bottom=szSize.cy;
			}
			if(pRect->left < SNAP_PIXELS && pRect->left > -SNAP_PIXELS)
			{
				pRect->left=0;
				pRect->right=szSize.cx;
			}
			if(pRect->right < m_rDesktopRect.right+SNAP_PIXELS && pRect->right > m_rDesktopRect.right-SNAP_PIXELS)
			{
				pRect->right=m_rDesktopRect.right;
				pRect->left=m_rDesktopRect.right-szSize.cx;
			}
			if(pRect->bottom < m_rDesktopRect.bottom+SNAP_PIXELS && pRect->bottom > m_rDesktopRect.bottom-SNAP_PIXELS)
			{
				pRect->bottom=m_rDesktopRect.bottom;
				pRect->top=m_rDesktopRect.bottom-szSize.cy;
			}
		}
		return(DefWindowProc(hWnd,msg,wParam,lParam));
	case WM_NCLBUTTONDOWN:
		if(wParam == HTCAPTION)
		{
			POINT pt={LOWORD(lParam),HIWORD(lParam)};
			RECT rRect={NULL};
			GetWindowRect(hWnd,&rRect);
			m_szMoveOffset.cx=pt.x-rRect.left;
			m_szMoveOffset.cy=pt.y-rRect.top;
		}
		return(DefWindowProc(hWnd,msg,wParam,lParam));
	case WM_DISPLAYCHANGE:
		GetWindowRect(GetDesktopWindow(),&m_rDesktopRect);
		DefWindowProc(hWnd,msg,wParam,lParam);
		return(DefWindowProc(hWnd,msg,wParam,lParam));
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return(DefWindowProc(hWnd,msg,wParam,lParam));
	}
	return (0L);
}

int WINAPI WinMain(HINSTANCE hinst,HINSTANCE hPreInst,
				   LPSTR pCmdLine,int nCmdShow)
{
	HWND hWnd;
	MSG msg;
	WNDCLASS wndclass;
	if(!hPreInst){
		wndclass.style=CS_HREDRAW|CS_VREDRAW;
		wndclass.lpfnWndProc=WndProc;
		wndclass.cbClsExtra=0;
		wndclass.cbWndExtra=0;
		wndclass.hInstance =hinst;
		wndclass.hIcon=NULL;
		wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);
		wndclass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
		wndclass.lpszMenuName=NULL;
		wndclass.lpszClassName=szClassName;
		if(!RegisterClass(&wndclass))
			return FALSE;
	}
	hWnd=CreateWindow(szClassName,
		"ƒ^ƒCƒgƒ‹",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hinst,
		NULL);
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (msg.wParam);
}
