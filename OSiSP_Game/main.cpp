#include <windows.h>
#include "Map.h"
#include "Drawer.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);
int height = 600, width = 1000;
int i = 50, j = 50;
Drawer* drawer;
PAINTSTRUCT ps;
HANDLE hbitmap;
MapInfo mapInfo;
int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE         hPrevInstance,
	LPSTR             lpCmdLine,
	int               nCmdShow)
{
	MSG msg;
	RECT screen_rect;
	HWND hWnd;
	HMENU hMenu;
	LPCTSTR lpzClass = TEXT("MyClass");

	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1;

	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int posX = screen_rect.right / 2 - width / 2;
	int posY = screen_rect.bottom / 2 - height / 2;

	hWnd = CreateWindow(lpzClass, TEXT("Платформер"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, posX, posY, width, height, NULL, NULL,
		hInstance, NULL);
	hbitmap = LoadImage(NULL, TEXT("cat.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	Map* map = new Map();
	map->LoadMap("map1.txt");
	mapInfo = map->GetMapInfo();

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	InvalidateRect(hWnd, 0, true);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

ATOM RegMyWindowClass(HINSTANCE hInst, LPCTSTR lpzClassName)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = hInst;
	wcex.lpszClassName = lpzClassName;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(0x00772463);
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.lpszMenuName = NULL;
	wcex.hIconSm = wcex.hIcon;

	return RegisterClassEx(&wcex);
}

LRESULT CALLBACK WndProc(
	HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		HDC hDC;
		if (!drawer)
		{
			drawer = new Drawer(hWnd, ps);
			drawer->SetDrawInfo(height, width, mapInfo);
		}
		drawer->StartPaint();
		drawer->DrawMap();
		drawer->TestDraw(hbitmap, i, j);
		drawer->DrawFromBuff();
		drawer->FinishPaint();
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case 'w':
		case 'W':
			j -= 5;
			break;
		case 's':
		case 'S':
			j += 5;
			break;
		case 'a':
		case 'A':
			i -= 5;
			break;
		case 'd':
		case 'D':
			i += 5;
			break;
		}
		InvalidateRect(hWnd, 0, true);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			j -= 5;
			break;
		case VK_DOWN:
			j += 5;
			break;
		case VK_LEFT:
			i -= 5;
			break;
		case VK_RIGHT:
			i += 5;
			break;
		case VK_ESCAPE:
			if (MessageBox(hWnd, TEXT("Выход"), TEXT("событие"), MB_OKCANCEL) == IDOK)
				PostQuitMessage(0);
			break;
		}
		InvalidateRect(hWnd, 0, true);
		break;
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			if (GetKeyState(VK_SHIFT) & 0x8000)
				i += 5;
			else
				j += 5;
		else
			if (GetKeyState(VK_SHIFT) & 0x8000)
				i -= 5;
			else
				j -= 5;
		InvalidateRect(hWnd, 0, true);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		case WM_ERASEBKGND:
			break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}