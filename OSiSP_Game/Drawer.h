#pragma once
#include <windows.h>
#include "Map.h"

class Drawer
{
public:
	Drawer(HWND hWND,PAINTSTRUCT ps);
	void SetDrawInfo(int height, int width, MapInfo mapInfo);
	void StartPaint();
	void FinishPaint();
	void DrawMap();
	void DrawFromBuff();
	void TestDraw(HANDLE hBitmap, short x,
		short y);
	~Drawer();
private:
	void DrawSprite(HDC hDC, HANDLE hBitmap, short x, short y, COLORREF cTransparentColor);
	SpriteInfo GetSpriteInfo(char code, SpriteInfo* spritesInfo, int count);
	void CreateMapTemplate(MapInfo mapInfo);
	const int blockSize = 32;
	int wndHeight, wndWidth;
	HANDLE background;
	HWND hWND;
	PAINTSTRUCT ps;
	HDC currHDC, buffHDC, MapHDC,BackgroundHDC;
	HBITMAP mapTemplate, buffer;
	COLORREF transparentColor;
};

