// MapRender.cpp
#include "maprender.h"
#include "mapInfo.h"    // ���ϸ� ���� ��� ����
#include "resource.h"


#include "mainheader.h"

using namespace Gdiplus;

static HINSTANCE  g_hInst = nullptr;

static MapInfo             g_map; // ����Ҷ� �̵���������, ���� ��ġ �������� ���� �޾ƿ��� ����

ULONG_PTR gdiplusToken;

// ���� ��ġ���� L - ���� R - ������ U - �� D - �Ʒ�
Image* g_LUimg = nullptr;
Image* g_Limg = nullptr;
Image* g_LDimg = nullptr;
Image* g_RUimg = nullptr;
Image* g_Rimg = nullptr;
Image* g_RDimg = nullptr;
Image* g_Uimg = nullptr;
Image* g_Dimg = nullptr;
Image* g_Mimg = nullptr;

// ���� �̵����
Image* g_Grassimg = nullptr;
Image* g_Flowerimg = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc, MemDC;
    PAINTSTRUCT ps;
    HBITMAP MyBitmap, OldBitmap;
    switch (message)
    {
    case WM_CREATE:
    {
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
        
        // �� ����
        g_map.setMap();
        
        // �̹��� �ε�
        {
            g_LUimg = new Image(L"Asserts/Images/Tile/LU.png");
            g_Limg = new Image(L"Asserts/Images/Tile/L.png");
            g_LDimg = new Image(L"Asserts/Images/Tile/LD.png");
            g_RUimg = new Image(L"Asserts/Images/Tile/RU.png");
            g_Rimg = new Image(L"Asserts/Images/Tile/R.png");
            g_RDimg = new Image(L"Asserts/Images/Tile/RD.png");
            g_Uimg = new Image(L"Asserts/Images/Tile/U.png");
            g_Dimg = new Image(L"Asserts/Images/Tile/D.png");
            g_Mimg = new Image(L"Asserts/Images/Tile/M.png");

            g_Grassimg = new Image(L"Asserts/Images/Tile/Grass.png");
            g_Flowerimg = new Image(L"Asserts/Images/Tile/Flower.png");
        }
        InvalidateRect(hWnd, nullptr, TRUE);
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        Graphics graphics(hdc);

        // ���� �Ͼ������ �ʱ�ȭ
        graphics.Clear(Color::White);

        graphics.SetSmoothingMode(SmoothingModeHighQuality);
        // �ȼ� ���� Ȯ�� / ��� �� ������ ���� �ʾ� ��谡 �Ƿ�
        graphics.SetInterpolationMode(InterpolationModeNearestNeighbor);
        // ��ǥ �� �ȼ� ���� ������ ����
        graphics.SetPixelOffsetMode(PixelOffsetModeHalf);
        graphics.SetCompositingQuality(CompositingQualityHighSpeed);
        
        for (int i = 0; i < Height; ++i) {
            for (int j = 0; j < Width; ++j) {
                Image* selimg = nullptr;
                if (g_map.grid[i][j] == TileType::Build) {
                    if (i == 1 && j == 1)                        selimg = g_LUimg;
                    else if (i == 1 && j == Width - 2)           selimg = g_RUimg;
                    else if (i == Height - 2 && j == 1)          selimg = g_LDimg;
                    else if (i == Height - 2 && j == Width - 2)  selimg = g_RDimg;
                    else if (j == 1)                             selimg = g_Limg;
                    else if (j == Width-2)                       selimg = g_Rimg;
                    else if (i == 1)                             selimg = g_Uimg;
                    else if (i == Height - 2)                    selimg = g_Dimg;
                    else                                         selimg = g_Mimg;
                }
                else {
                    bool checker = ((i + j) % 2) == 0;
                    selimg = checker ? g_Grassimg : g_Flowerimg;
                }
                if (selimg)
                {
                    const int dstW = 50;
                    const int dstH = 50;

                    int x = 100;
                    int y = 100;
                    graphics.DrawImage(selimg, x + (dstW * j), y + (dstH * i), dstW, dstH);
                }
            }
        }
        graphics.ResetTransform();
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
    {
        // ����
        PostQuitMessage(0);
        // �޸� ����
        GdiplusShutdown(gdiplusToken);
    }
    break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// GDI+ �ʱ�ȭ �� ������ ����
bool InitRenderer(HINSTANCE hInst, int nCmdShow) {
    
    g_hInst = hInst;
    LPCWSTR lpszClass = L"��ȭ";

    // 2) ������ Ŭ���� ���
    static const wchar_t CLASS_NAME[] = L"RenderWindowClass";
    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = lpszClass;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    if (!RegisterClassExW(&wc))
        return false;

    RECT rc = { 0, 0, 1000, 700 };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    int winW = rc.right - rc.left;
    int winH = rc.bottom - rc.top;

    HWND hWnd = CreateWindowW(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, winW, winH,
        NULL, (HMENU)NULL, hInst, NULL);
    if (!hWnd) return false;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return true;
}

// �޽��� ���� ����
int RunRenderer() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

// (���ҽ� ������ WM_DESTROY���� ó���ϹǷ� ��� �־ ����)
void CleanupRenderer() {}