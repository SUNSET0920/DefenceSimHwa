// maprender.h
#pragma once
#include <windows.h>
#include <gdiplus.h>

// 호출 성공 시 true 반환
bool InitRenderer(HINSTANCE hInst, int nCmdShow);
// 윈도우 메시지 루프 실행, 리턴값은 메시지 펌프의 wParam
int  RunRenderer();
// 리소스 해제
void CleanupRenderer();
