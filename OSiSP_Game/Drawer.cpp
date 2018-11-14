#include "Drawer.h"



Drawer::Drawer(HWND hWND,PAINTSTRUCT ps)
{
	transparentColor = 0x00FFFFFF;
	this->hWND = hWND;
	this->ps = ps;
}

void Drawer::SetDrawInfo(int height, int width, MapInfo mapInfo)
{
	currHDC = BeginPaint(hWND, &ps);
	wndHeight = height;
	wndWidth = width;
	background = LoadImage(NULL, CA2W(mapInfo.backgroundPath), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	BackgroundHDC = CreateCompatibleDC(currHDC);
	SelectObject(BackgroundHDC, background);
	buffHDC = CreateCompatibleDC(currHDC);
	buffer = CreateCompatibleBitmap(currHDC, wndWidth, wndHeight);
	SelectObject(buffHDC, buffer);
	CreateMapTemplate(mapInfo);
	EndPaint(hWND, &ps);
}
void Drawer::StartPaint()
{
	currHDC = BeginPaint(hWND, &ps);
	BitBlt(buffHDC, 0, 0, wndWidth, wndHeight, BackgroundHDC, 0, 0, SRCCOPY);
}
void Drawer::FinishPaint()
{
	EndPaint(hWND, &ps);
}
void Drawer::DrawSprite(HDC hDC, HANDLE hBitmap, short x,
	short y, COLORREF cTransparentColor)
{
	BITMAP     bm;
	COLORREF   cColor;
	HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP    bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC        hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT      ptSize;

	hdcTemp = CreateCompatibleDC(hDC);
	SelectObject(hdcTemp, hBitmap);

	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdcTemp, &ptSize, 1);

	hdcBack = CreateCompatibleDC(hDC);
	hdcObject = CreateCompatibleDC(hDC);
	hdcMem = CreateCompatibleDC(hDC);
	hdcSave = CreateCompatibleDC(hDC);

	bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

	bmAndMem = CreateCompatibleBitmap(hDC, ptSize.x, ptSize.y);
	bmSave = CreateCompatibleBitmap(hDC, ptSize.x, ptSize.y);


	bmBackOld = (HBITMAP)SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);
	bmMemOld = (HBITMAP)SelectObject(hdcMem, bmAndMem);
	bmSaveOld = (HBITMAP)SelectObject(hdcSave, bmSave);

	SetMapMode(hdcTemp, GetMapMode(hDC));

	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
	cColor = SetBkColor(hdcTemp, cTransparentColor);
	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
	SetBkColor(hdcTemp, cColor);
	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hDC, x, y, SRCCOPY);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);
	BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);


	BitBlt(hDC, x, y, ptSize.x, ptSize.y, hdcMem, 0, 0, SRCCOPY);

	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));

	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}

void Drawer::CreateMapTemplate(MapInfo mapInfo)
{
	SpriteInfo currSprite;
	HANDLE bmp;
	MapHDC = CreateCompatibleDC(currHDC);
	mapTemplate = CreateCompatibleBitmap(currHDC, mapInfo.jCount * blockSize, mapInfo.iCount * blockSize);
	SelectObject(MapHDC, mapTemplate);
	BitBlt(MapHDC, 0, 0, wndWidth, wndHeight, BackgroundHDC, 0, 0, SRCCOPY);
	for (int i = 0; i < mapInfo.iCount; i++)
	{
		for (int j = 0; j < mapInfo.jCount; j++)
		{
			try
			{
				currSprite = GetSpriteInfo(mapInfo.Map[i][j], mapInfo.spritesInfo, mapInfo.spritesCount);
				bmp = LoadImage(NULL, CA2W(currSprite.pathToImg), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				DrawSprite(MapHDC, bmp, j*blockSize, i*blockSize, transparentColor);
			}
			catch (int i)
			{}
		}
	}
}

void Drawer::DrawMap()
{
	BitBlt(buffHDC, 0, 0, wndWidth, wndHeight, MapHDC, 0, 0, SRCCOPY);	
}

void Drawer::TestDraw(HANDLE hBitmap, short x, short y)
{
	DrawSprite(buffHDC, hBitmap, x, y, transparentColor);
}

void Drawer::DrawFromBuff()
{
	BitBlt(currHDC, 0, 0, wndWidth, wndHeight, buffHDC, 0, 0, SRCCOPY);
}

SpriteInfo Drawer::GetSpriteInfo(char code, SpriteInfo* spritesInfo,int count)
{
	for (int i = 0; i < count; i++)
	{
		if (code == spritesInfo[i].code)
			return spritesInfo[i];
	}
	throw 1;
}

Drawer::~Drawer()
{

}
