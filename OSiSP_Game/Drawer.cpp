#include "Drawer.h"

Drawer::Drawer(HWND hWND, PAINTSTRUCT ps)
{
	transparentColor = 0x00FFFFFF;
	background = NULL;
	backgroundHDC = NULL;
	buffer = NULL;
	buffHDC = NULL;
	mapTemplate = NULL;
	invMap = NULL;
	this->hWND = hWND;
	this->ps = ps;
}
 
void Drawer::SetDrawInfo(MapInfo mapInfo)
{
	RECT rect;
	if (background != NULL)
		DeleteObject(background);
	if (backgroundHDC != NULL)
		DeleteDC(backgroundHDC);
	if (buffer != NULL)
		DeleteObject(buffer);
	if (buffHDC != NULL)
		DeleteDC(buffHDC);
	currHDC = BeginPaint(hWND, &ps);
	GetClientRect(hWND, (LPRECT)&rect);
	wnd.x = rect.right;
	wnd.y = rect.bottom;
	camera.x = 0;
	camera.y = 0;
	background = LoadImage(NULL, CA2W(mapInfo.backgroundPath), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	backgroundHDC = CreateCompatibleDC(currHDC);
	SelectObject(backgroundHDC, background);
	buffHDC = CreateCompatibleDC(currHDC);
	buffer = CreateCompatibleBitmap(currHDC, wnd.x, wnd.y);
	SelectObject(buffHDC, buffer);
	CreateMapTemplate(mapInfo);

	EndPaint(hWND, &ps);
}

void Drawer::StartPaint()
{
	currHDC = BeginPaint(hWND, &ps);
	BitBlt(buffHDC, 0, 0, wnd.x, wnd.y, backgroundHDC, 0, 0, SRCCOPY);
}

void Drawer::FinishPaint()
{
	EndPaint(hWND, &ps);
}

void Drawer::DrawSprite(HDC hDC, HANDLE hBitmap, short x, short y, COLORREF cTransparentColor)
{
	BITMAP     bm;
	COLORREF   cColor;
	HBITMAP    bmAndBack, bmAndObject, bmSave;
	HDC        hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT      ptSize;

	hdcTemp = CreateCompatibleDC(hDC);
	SelectObject(hdcTemp, hBitmap);

	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdcTemp, &ptSize, 1);

	hdcBack = CreateCompatibleDC(hDC);
	hdcObject = CreateCompatibleDC(hDC);
	hdcSave = CreateCompatibleDC(hDC);

	bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

	bmSave = CreateCompatibleBitmap(hDC, ptSize.x, ptSize.y);
	
	SelectObject(hdcBack, bmAndBack);
	SelectObject(hdcObject, bmAndObject);
	SelectObject(hdcSave, bmSave);
	
	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);
	BitBlt(hDC, x, y, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	BitBlt(hDC, x, y, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

	DeleteObject(bmAndBack);
	DeleteObject(bmAndObject);
	DeleteObject(bmSave);

	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}

void Drawer::CreateMapTemplate(MapInfo mapInfo)
{
	SpriteInfo currSprite;
	RECT rect;
	HDC mapHDC, invMapHDC, backHDC;
	HBITMAP back;
	HBRUSH brush = CreateSolidBrush(transparentColor);
	SpriteDrawInfo* spritesDrawInfo = GetSpritesDrawInfo(mapInfo.spritesInfo, mapInfo.spritesCount);
	mapHDC = CreateCompatibleDC(currHDC);
	invMapHDC = CreateCompatibleDC(currHDC);
	backHDC = CreateCompatibleDC(currHDC);

	rect.top = 0;
	rect.left = 0;
	rect.right = mapInfo.jCount * blockSize;
	rect.bottom = mapInfo.iCount * blockSize;
	map.y = rect.bottom;
	map.x = rect.right;

	if (mapTemplate != NULL)
		DeleteObject(mapTemplate);
	if (invMap != NULL)
		DeleteObject(invMap);

	back = CreateBitmap(rect.right, rect.bottom, 1, 1, NULL);
	invMap = CreateBitmap(rect.right, rect.bottom, 1, 1, NULL);
	mapTemplate = CreateCompatibleBitmap(currHDC, rect.right, rect.bottom);
	SelectObject(mapHDC, mapTemplate);
	SelectObject(invMapHDC, invMap);
	SelectObject(backHDC, back);
	FillRect(mapHDC,&rect, brush);

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
				DrawSprite(mapHDC, currBitmap, j * blockSize, i * blockSize, transparentColor);
		}
	}

	BitBlt(invMapHDC, 0, 0, rect.right, rect.bottom, mapHDC, 0, 0, SRCCOPY);
	BitBlt(backHDC, 0, 0, rect.right, rect.bottom, invMapHDC, 0, 0, NOTSRCCOPY);
	BitBlt(mapHDC, 0, 0, rect.right, rect.bottom, backHDC, 0, 0, SRCAND);

	DeleteSpritesDrawInfo(spritesDrawInfo, mapInfo.spritesCount);
	DeleteObject(back);
	DeleteDC(mapHDC);
	DeleteDC(backHDC);
	DeleteDC(invMapHDC);
	DeleteObject(brush);
}

void Drawer::SetPlayer(Player* _player)
{
	player = _player;
}

void Drawer::DrawDynamicObjects()
{
	
	DrawDynamicObject(player->GetDrawInfo(), player->GetCoordAndSize());
	CoordAndSize coordAndSize = player->GetCoordAndSize();
	int offsetX = map.x - (map.x - coordAndSize.x - coordAndSize.width / 2) - wnd.x / 2;
	int offsetY = map.y - (map.y - coordAndSize.y - coordAndSize.height / 2) - wnd.y / 2;
	if (offsetX < 0)
		offsetX = 0;
	else if (offsetX + wnd.x > map.x)
		offsetX = map.x - wnd.x;
	if (offsetY < 0)
		offsetY = 0;
	else if (offsetY + wnd.y > map.y)
		offsetY = map.y - wnd.y;
	camera.x = offsetX;
	camera.y = offsetY;
}

void Drawer::DrawDynamicObject(ObjDrawInfo objDrawInfo, CoordAndSize coordAndSize)
{
	HDC imgHDC, spriteHDC;
	HBITMAP sprite;
	imgHDC = CreateCompatibleDC(buffHDC);
	spriteHDC = CreateCompatibleDC(buffHDC);
	SelectObject(imgHDC, objDrawInfo.bitmap);
	sprite = CreateCompatibleBitmap(buffHDC, coordAndSize.width, coordAndSize.height);
	SelectObject(spriteHDC, sprite);
	
	if (objDrawInfo.isInvert == true)
		StretchBlt(spriteHDC, 0, 0, coordAndSize.width, coordAndSize.height,
			imgHDC, (objDrawInfo.currFrame+1)*coordAndSize.width-1, 0, -coordAndSize.width, coordAndSize.height, SRCCOPY);
	else
	StretchBlt(spriteHDC, 0, 0, coordAndSize.width, coordAndSize.height, 
		imgHDC, objDrawInfo.currFrame*coordAndSize.width, 0, coordAndSize.width, coordAndSize.height, SRCCOPY);
	
	DeleteDC(spriteHDC);
	DeleteDC(imgHDC);
	DrawSprite(buffHDC, sprite, coordAndSize.x - camera.x, coordAndSize.y - camera.y, transparentColor);
	DeleteObject(sprite);
}

void Drawer::DrawMap()
{	
	HDC mapHDC, invMapHDC;
	mapHDC = CreateCompatibleDC(currHDC);
	invMapHDC = CreateCompatibleDC(currHDC);
	SelectObject(mapHDC, mapTemplate);
	SelectObject(invMapHDC, invMap);

	BitBlt(buffHDC, 0, 0, wnd.x, wnd.y, invMapHDC, 0 + camera.x, 0 + camera.y, SRCAND);
	BitBlt(buffHDC, 0, 0, wnd.x, wnd.y, mapHDC, 0 + camera.x, 0 + camera.y, SRCPAINT);

	DeleteDC(mapHDC);
	DeleteDC(invMapHDC);
}

void Drawer::DrawFromBuff()
{
	BitBlt(currHDC, 0, 0, wnd.x, wnd.y, buffHDC, 0, 0, SRCCOPY);
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

void Drawer::Invert(HDC hdc)
{
	XFORM xForm;

	xForm.eM11 = -1.0;
	xForm.eM12 = 0.0;
	xForm.eM21 = 0.0;
	xForm.eM22 = -1.0;
	xForm.eDx =  0.0;
	xForm.eDy =  0.0;

	SetWorldTransform(hdc, &xForm);
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
	if (invMap != NULL)
		DeleteObject(invMap);
}
