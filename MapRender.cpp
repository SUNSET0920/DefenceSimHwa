// MapRender.cpp
#include "maprender.h"
#include "resource.h"
#include "GameManager.h"

#include "mainheader.h"

static GameManager gameManager(Width, Height, dstW, dstH);

static HINSTANCE  g_hInst = nullptr;

//static MapInfo             g_map; // 출력할때 이동구간인지, 영웅 설치 구간인지 정보 받아오기 위함

ULONG_PTR gdiplusToken;

MapInfo g_map;

// 영웅 설치구간 L - 왼쪽 R - 오른쪽 U - 위 D - 아래
Image* g_LUimg = nullptr;
Image* g_Limg = nullptr;
Image* g_LDimg = nullptr;
Image* g_RUimg = nullptr;
Image* g_Rimg = nullptr;
Image* g_RDimg = nullptr;
Image* g_Uimg = nullptr;
Image* g_Dimg = nullptr;
Image* g_Mimg = nullptr;

// 몬스터 이동경로
Image* g_Grassimg = nullptr;
Image* g_Flowerimg = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc, MemDC;
    PAINTSTRUCT ps;
    HBITMAP MyBitmap, OldBitmap;

    g_map.setMap();
    switch (message)
    {
    case WM_CREATE:
    {
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
        
        // 이미지 로드
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
        gameManager.init(hWnd, g_hInst);
    }
    break;
    case WM_COMMAND:
        gameManager.cmd(hWnd, LOWORD(wParam));
        return 0;
    case WM_LBUTTONDOWN:
        gameManager.mouseDown(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_MOUSEMOVE:
        gameManager.mouseMove(LOWORD(lParam), HIWORD(lParam));
        return 0;
    case WM_LBUTTONUP:
        gameManager.mouseUp(LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_TIMER:
        // dt = 16ms
        gameManager.timer(wParam, 0.016f);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // 1) 메모리 DC, 비트맵 생성
        RECT rc;
        GetClientRect(hWnd, &rc);
        HDC memDC = CreateCompatibleDC(hdc);
        HBITMAP backBmp = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
        HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, backBmp);

        // 2) 메모리 DC에서 GDI+ 그리기
        Graphics gMem(memDC);
        RenderMap(gMem, g_map);
        gameManager.render(gMem);

        // 3) 메모리 DC → 실제 화면 복사
        BitBlt(hdc, 0, 0, rc.right, rc.bottom, memDC, 0, 0, SRCCOPY);

        // 4) 리소스 정리
        SelectObject(memDC, oldBmp);
        DeleteObject(backBmp);
        DeleteDC(memDC);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
    {
        // 정리
        PostQuitMessage(0);
        // 메모리 해제
        GdiplusShutdown(gdiplusToken);
    }
    break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// GDI+ 초기화 및 윈도우 생성
bool InitRenderer(HINSTANCE hInst, int nCmdShow) {
    
    g_hInst = hInst;
    LPCWSTR lpszClass = L"심화";

    // 2) 윈도우 클래스 등록
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

// 메시지 루프 실행
int RunRenderer() {
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

// (리소스 정리는 WM_DESTROY에서 처리하므로 비어 있어도 무방)
void CleanupRenderer() {}

void RenderMap(Graphics& g,MapInfo g_map) {
    
    // 바탕 하얀색으로 초기화
    g.Clear(Color::White);

    g.SetSmoothingMode(SmoothingModeHighQuality);
    // 픽셀 단위 확대 / 축소 시 보간을 하지 않아 경계가 또렷
    g.SetInterpolationMode(InterpolationModeNearestNeighbor);
    // 좌표 반 픽셀 단위 절삭을 보정
    g.SetPixelOffsetMode(PixelOffsetModeHalf);
    g.SetCompositingQuality(CompositingQualityHighSpeed);

    for (int i = 0; i < Height; ++i) {
        for (int j = 0; j < Width; ++j) {
            Image* selimg = nullptr;
            if (g_map.mapinfo[i][j].type == TileType::Build) {
                if (i == 1 && j == 1)                        selimg = g_LUimg;
                else if (i == 1 && j == Width - 2)           selimg = g_RUimg;
                else if (i == Height - 2 && j == 1)          selimg = g_LDimg;
                else if (i == Height - 2 && j == Width - 2)  selimg = g_RDimg;
                else if (j == 1)                             selimg = g_Limg;
                else if (j == Width - 2)                       selimg = g_Rimg;
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
                //g.DrawImage(selimg, OFFSET_X + j * dstW, OFFSET_Y + i * dstH, dstW, dstH);
                g.DrawImage(selimg, g_map.mapinfo[j][i].pos.x, g_map.mapinfo[j][i].pos.y, dstW, dstH);
            }
        }
    }
    g.ResetTransform();
}