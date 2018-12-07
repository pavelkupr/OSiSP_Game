#pragma once
#include <windows.h>
#include "Map.h"
#include "IDynamicObject.h"
#include "Player.h"

struct SpriteDrawInfo
{
	char code;
	HBITMAP bitmap;
};

class Drawer
{
public:
	Drawer(HWND hWND,PAINTSTRUCT ps);
	void SetDrawInfo(MapInfo mapInfo);
	void StartPaint();
	void SetPlayer(Player*);
	void DrawDynamicObjects();
	void FinishPaint();
	void DrawMap();
	void DrawFromBuff();
	~Drawer();
private:
	Player* player;
	const int blockSize = 32;
	POINT wnd, map, camera;
	//int wndHeight, wndWidth;
	HANDLE background;
	HWND hWND;
	PAINTSTRUCT ps;
	HDC currHDC, buffHDC, backgroundHDC;
	HBITMAP mapTemplate,invMap, buffer;
	COLORREF transparentColor;
	void DrawSprite(HDC hDC, HANDLE hBitmap, short x, short y, COLORREF cTransparentColor);
	SpriteDrawInfo* GetSpritesDrawInfo(SpriteInfo* spritesInfo, int count);
	void DeleteSpritesDrawInfo(SpriteDrawInfo* spritesInfo, int count);
	void CreateMapTemplate(MapInfo mapInfo);
	void DrawDynamicObject(ObjDrawInfo, CoordAndSize);
	void Invert(HDC);
};

