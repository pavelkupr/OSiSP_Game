#include <windows.h>
#include "Map.h"
#include "Drawer.h"
#include "Player.h"
#include "IDynamicObject.h"
#include "Environment.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

int height = 450, width = 750;//450 750 600 1000;
int i = 50, j = 50;
bool isRunning = true;
const int ticksInMs = 1800, frameLim = 30;

Drawer* drawer = NULL;
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
	Map* map;
	HMENU hMenu;
	Player* player;
	Environment* environment;
	Controls controls;
	LARGE_INTEGER time, currTime;
	LPCTSTR lpzClass = TEXT("MyClass");
	
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1; 

	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int posX = screen_rect.right / 2 - width / 2;
	int posY = screen_rect.bottom / 2 - height / 2;

	hWnd = CreateWindow(lpzClass, TEXT("Платформер"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, posX, posY, width, height, NULL, NULL,
		hInstance, NULL);
	hbitmap = LoadImage(NULL, TEXT("player.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	drawer = new Drawer(hWnd, ps);
	map = new Map();
	map->LoadMap("map1.txt");
	mapInfo = map->GetMapInfo();
	drawer->SetDrawInfo(mapInfo); 

	player = new Player(40, 57,(HBITMAP)hbitmap,5,12,8);
	drawer->SetPlayer(player);

	environment = new Environment(player,mapInfo,1);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	InvalidateRect(hWnd, 0, true);

	QueryPerformanceCounter(&time);
	while (isRunning) {
		if (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			QueryPerformanceCounter(&currTime);
			if (currTime.QuadPart - time.QuadPart > 1000/frameLim * ticksInMs)
			{
				QueryPerformanceCounter(&time);
				if (GetKeyState(VK_UP) & 0x8000)
					controls.up = true;
				else
					controls.up = false;
				if (GetKeyState(VK_RIGHT) & 0x8000)
					controls.right = true;
				else
					controls.right = false;
				if (GetKeyState(VK_LEFT) & 0x8000)
					controls.left = true;
				else
					controls.left = false;

				environment->Cycle(controls);
				InvalidateRect(hWnd, 0, true);
			}
			
		}
	}
	map->DeleteMapInfo(mapInfo);

	delete map;
	delete drawer;
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
	wcex.hbrBackground = CreateSolidBrush(0x00000000);
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
	
		drawer->StartPaint();
		drawer->DrawMap();
		drawer->DrawDynamicObjects();
		drawer->DrawFromBuff();
		drawer->FinishPaint();
		break;
	case WM_CHAR:
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			if (MessageBox(hWnd, TEXT("Выход"), TEXT("событие"), MB_OKCANCEL) == IDOK)
				PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		isRunning = false;
		PostQuitMessage(0);
		break;
	case WM_ERASEBKGND:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}