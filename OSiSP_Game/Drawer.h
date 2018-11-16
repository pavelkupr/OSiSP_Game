#pragma once
#include <windows.h>
#include "Map.h"

struct SpriteDrawInfo
{
	char code;
	HBITMAP bitmap;
};

class Drawer
{
public:
	Drawer(HWND hWND,PAINTSTRUCT ps);
	void SetDrawInfo(int height, int width, MapInfo mapInfo);
	void TestDraw(HANDLE hBitmap, short x, short y);
	void StartPaint();
	void FinishPaint();
	void DrawMap();
	void DrawFromBuff();
	~Drawer();
private:
	const int blockSize = 32;
	int wndHeight, wndWidth;
	HANDLE background;
	HWND hWND;
	PAINTSTRUCT ps;
	HDC currHDC, buffHDC, backgroundHDC;
	HBITMAP mapTemplate, buffer;
	COLORREF transparentColor;
	void DrawSprite(HDC hDC, HANDLE hBitmap, short x, short y, COLORREF cTransparentColor);
	SpriteDrawInfo* GetSpritesDrawInfo(SpriteInfo* spritesInfo, int count);
	void DeleteSpritesDrawInfo(SpriteDrawInfo* spritesInfo, int count);
	void CreateMapTemplate(MapInfo mapInfo);
};

