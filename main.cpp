#include <windows.h>
#include "maprender.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    // 렌더러 초기화 → 윈도우 생성, GDI+ 초기화까지 여기서
    if (!InitRenderer(hInstance,nCmdShow)) {
        MessageBox(nullptr, TEXT("Renderer 초기화 실패"), TEXT("Error"), MB_OK | MB_ICONERROR);
        return -1;
    }

    // 메시지 루프 실행 (닫기 전까지 이 함수에서 계속 블록됨)
    int ret = RunRenderer();

    // 리소스 정리는 WM_DESTROY 에서 이미 처리
    CleanupRenderer();
    return ret;
}
