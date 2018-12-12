#include <Windows.h>
#include "Drawer.h"
#include "Player.h"
#include "Environment.h"
#include "bass.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
ATOM RegMyWindowClass(HINSTANCE, LPCTSTR);

int height = 450, width = 750;
int i = 50, j = 50;
bool isRunning = true,isMenu = true;
const int ticksInMs = 1800, frameLim = 30;

Drawer* drawer = NULL;
HANDLE hbitmap;
MapInfo mapInfo;
MenuControl menuControl;

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE         hPrevInstance,
	LPSTR             lpCmdLine,
	int               nCmdShow)
{
	MSG msg;
	RECT screen_rect;
	HWND hWnd;
	Player* player;
	Environment* environment;
	Controls controls;
	LARGE_INTEGER time, currTime;
	LPCTSTR lpzClass = TEXT("MyClass");
	if (!RegMyWindowClass(hInstance, lpzClass))
		return 1; 
	
	BASS_Init(-1, 44100, BASS_DEVICE_3D, 0, NULL);
	HSTREAM music = BASS_StreamCreateFile(FALSE, TEXT("music.wav"), 0, 0, 0);
	BASS_ChannelFlags(music, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
	BASS_ChannelPlay(music, TRUE);

	GetWindowRect(GetDesktopWindow(), &screen_rect);
	int posX = screen_rect.right / 2 - width / 2;
	int posY = screen_rect.bottom / 2 - height / 2;

	hWnd = CreateWindow(lpzClass, TEXT("Платформер"),
		WS_SYSMENU, posX, posY, width, height, NULL, NULL,
		hInstance, NULL);
	hbitmap = LoadImage(NULL, TEXT("player.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


	drawer = new Drawer(hWnd);

	player = new Player(64, 64,(HBITMAP)hbitmap,5,12,8);

	environment = new Environment(drawer, player, "map1.txt",1);

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
			if (isMenu)
			{
				InvalidateRect(hWnd, 0, true);
			}
			else if (currTime.QuadPart - time.QuadPart > 1000/frameLim * ticksInMs)
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
		if (isMenu)
		{
			int result = drawer->PaintMenu(menuControl);
			if (result == 1)
				isMenu = false;
			else if (result == 2)
			{
				isRunning = false;
				PostQuitMessage(0);
			}
			menuControl.isPressed = false;
		}
		else
			drawer->Paint();
		break;
	case WM_CHAR:
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			isMenu = !isMenu;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		menuControl.isPressed = true;
		break;
	case WM_MOUSEMOVE:
		menuControl.i = LOWORD(lParam);
		menuControl.j = HIWORD(lParam);
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