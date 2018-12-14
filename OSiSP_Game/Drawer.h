#pragma once
#include <Windows.h>
#include "Map.h"
#include "NonPlayble.h"
#include "Player.h"
#include "Records.h"

struct SpriteDrawInfo
{
	char code;
	HBITMAP bitmap;
};

struct MenuControl
{
	bool isPressed=false;
	char symbol = ' ';
	int i=0, j=0;
};

class Drawer
{
public:
	Drawer(HWND hWND);
	void SetDrawInfo(MapInfo mapInfo);
	void SetPlayer(Player*);
	void SetNonPlaybleObjects(NonPlayble**, int* count);
	void Paint();
	int PaintMenu(MenuControl);
	~Drawer();
private:
	Player* player;
	Records* record;
	Record* records;
	NonPlayble** nonPlaybleObjects;
	HFONT hFont;
	const int blockSize = 32,maxObj = 100;
	int* currObjCount, menuType,countOfRecords;
	POINT wnd, map, camera;
	HANDLE background,life,key,menu;
	HWND hWND;
	HFONT menuFont;
	PAINTSTRUCT ps;
	HDC currHDC, buffHDC, backgroundHDC, menuHDC;
	HBITMAP mapTemplate,invMap, buffer;
	COLORREF transparentColor;
	void DrawSprite(HDC hDC, HANDLE hBitmap, short x, short y, COLORREF cTransparentColor);
	SpriteDrawInfo* GetSpritesDrawInfo(SpriteInfo* spritesInfo, int count);
	void DrawDynamicObjects();
	void DrawMap();
	void DrawUI();
	void SetOffset();
	void DrawFromBuff();
	void StartPaint();
	void FinishPaint();
	void DeleteSpritesDrawInfo(SpriteDrawInfo* spritesInfo, int count);
	void CreateMapTemplate(MapInfo mapInfo);
	void DrawDynamicObject(ObjDrawInfo, CoordAndSize);
};

