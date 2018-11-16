#include "Drawer.h"

Drawer::Drawer(HWND hWND, PAINTSTRUCT ps)
{
	transparentColor = 0x00FFFFFF;
	background = NULL;
	backgroundHDC = NULL;
	buffer = NULL;
	buffHDC = NULL;
	mapTemplate = NULL;
	this->hWND = hWND;
	this->ps = ps;
}
 
void Drawer::SetDrawInfo(int height, int width, MapInfo mapInfo)
{
	if (background != NULL)
		DeleteObject(background);
	if (backgroundHDC != NULL)
		DeleteDC(backgroundHDC);
	if (buffer != NULL)
		DeleteObject(buffer);
	if (buffHDC != NULL)
		DeleteDC(buffHDC);
	currHDC = BeginPaint(hWND, &ps);
	wndHeight = height;
	wndWidth = width;
	background = LoadImage(NULL, CA2W(mapInfo.backgroundPath), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	backgroundHDC = CreateCompatibleDC(currHDC);
	SelectObject(backgroundHDC, background);
	buffHDC = CreateCompatibleDC(currHDC);
	buffer = CreateCompatibleBitmap(currHDC, wndWidth, wndHeight);
	SelectObject(buffHDC, buffer);
	CreateMapTemplate(mapInfo);
	EndPaint(hWND, &ps);
}

void Drawer::StartPaint()
{
	currHDC = BeginPaint(hWND, &ps);
	BitBlt(buffHDC, 0, 0, wndWidth, wndHeight, backgroundHDC, 0, 0, SRCCOPY);
}

void Drawer::FinishPaint()
{
	EndPaint(hWND, &ps);
}

void Drawer::DrawSprite(HDC hDC, HANDLE hBitmap, short x, short y, COLORREF cTransparentColor)
{
	BITMAP     bm;
	COLORREF   cColor;
	HBITMAP    bmAndBack, bmAndObject, bmAndMem, bmSave;
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
	
	SelectObject(hdcBack, bmAndBack);
	SelectObject(hdcObject, bmAndObject);
	SelectObject(hdcMem, bmAndMem);
	SelectObject(hdcSave, bmSave);
	
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

	DeleteObject(bmAndBack);
	DeleteObject(bmAndObject);
	DeleteObject(bmAndMem);
	DeleteObject(bmSave);

	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}

void Drawer::CreateMapTemplate(MapInfo mapInfo)
{
	SpriteInfo currSprite;
	RECT rect;
	HDC MapHDC;
	HANDLE bmp;
	HBRUSH brush = CreateSolidBrush(transparentColor);
	SpriteDrawInfo* spritesDrawInfo = GetSpritesDrawInfo(mapInfo.spritesInfo, mapInfo.spritesCount);

	rect.top = 0;
	rect.left = 0;
	rect.right = mapInfo.jCount * blockSize;
	rect.bottom = mapInfo.iCount * blockSize;

	if (mapTemplate != NULL)
		DeleteObject(mapTemplate);
	mapTemplate = CreateCompatibleBitmap(currHDC, rect.right, rect.bottom);
	MapHDC = CreateCompatibleDC(currHDC);
	SelectObject(MapHDC, mapTemplate);
	FillRect(MapHDC,&rect, brush);

	for (int i = 0; i < mapInfo.iCount; i++)
	{
		for (int j = 0; j < mapInfo.jCount; j++)
		{
			bool isFound = false;
			HBITMAP currBitmap;
			for (int x = 0; x < mapInfo.spritesCount; x++)
			{
				if (mapInfo.Map[i][j] == spritesDrawInfo[x].code)
				{
					currBitmap = spritesDrawInfo[x].bitmap;
					isFound = true;
					break;
				}
			}

			if (isFound)
				DrawSprite(MapHDC, currBitmap, j * blockSize, i * blockSize, transparentColor);
		}
	}
	DeleteSpritesDrawInfo(spritesDrawInfo, mapInfo.spritesCount);
	DeleteDC(MapHDC);
	DeleteObject(brush);
}

void Drawer::DrawMap()
{	
	DrawSprite(buffHDC, mapTemplate, 0, 0, transparentColor);
}

void Drawer::TestDraw(HANDLE hBitmap, short x, short y)
{
	DrawSprite(buffHDC, hBitmap, x, y, transparentColor);
}

void Drawer::DrawFromBuff()
{
	BitBlt(currHDC, 0, 0, wndWidth, wndHeight, buffHDC, 0, 0, SRCCOPY);
}

SpriteDrawInfo* Drawer::GetSpritesDrawInfo(SpriteInfo* spritesInfo, int count)
{
	SpriteDrawInfo* result = (SpriteDrawInfo*)malloc(sizeof(SpriteDrawInfo)*count);
	for (int i = 0; i < count; i++)
	{
		result[i].code = spritesInfo[i].code;
		result[i].bitmap = (HBITMAP)LoadImage(NULL, CA2W(spritesInfo[i].pathToImg), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}
	return result;
}

void Drawer::DeleteSpritesDrawInfo(SpriteDrawInfo* spritesInfo, int count)
{
	for (int i = 0; i < count; i++)
	{
		DeleteObject(spritesInfo[i].bitmap);
	}
	delete spritesInfo;
}

Drawer::~Drawer()
{
	if (background != NULL)
		DeleteObject(background);
	if (backgroundHDC != NULL)
		DeleteDC(backgroundHDC);
	if (buffer != NULL)
		DeleteObject(buffer);
	if (buffHDC != NULL)
		DeleteDC(buffHDC);
	if (mapTemplate != NULL)
		DeleteObject(mapTemplate);
}
