// maprender.h
#pragma once
#include <windows.h>
#include <gdiplus.h>
#include "mapInfo.h"    // ���ϸ� ���� ��� ����


using namespace Gdiplus;
//MapInfo g_map;
// ȣ�� ���� �� true ��ȯ
bool InitRenderer(HINSTANCE hInst, int nCmdShow);
// ������ �޽��� ���� ����, ���ϰ��� �޽��� ������ wParam
int  RunRenderer();
// ���ҽ� ����
void CleanupRenderer();
//�ʸ� �׷��ִ� �Լ�
void RenderMap(Graphics& g, MapInfo g_map);