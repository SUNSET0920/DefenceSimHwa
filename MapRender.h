// maprender.h
#pragma once
#include <windows.h>
#include <gdiplus.h>

// ȣ�� ���� �� true ��ȯ
bool InitRenderer(HINSTANCE hInst, int nCmdShow);
// ������ �޽��� ���� ����, ���ϰ��� �޽��� ������ wParam
int  RunRenderer();
// ���ҽ� ����
void CleanupRenderer();
