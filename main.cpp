#include <windows.h>
#include "maprender.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
    ULONG_PTR gdiplusToken;
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // ������ �ʱ�ȭ �� ������ ����, GDI+ �ʱ�ȭ���� ���⼭
    if (!InitRenderer(hInstance,nCmdShow)) {
        MessageBox(nullptr, TEXT("Renderer �ʱ�ȭ ����"), TEXT("Error"), MB_OK | MB_ICONERROR);
        return -1;
    }

    // �޽��� ���� ���� (�ݱ� ������ �� �Լ����� ��� ��ϵ�)
    int ret = RunRenderer();

    // ���ҽ� ������ WM_DESTROY ���� �̹� ó��
    CleanupRenderer();
    return ret;
}
