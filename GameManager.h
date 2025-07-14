// GameManager.h
#pragma once
#include <vector>
#include <memory>
#include <windows.h>
#include <gdiplus.h>
#include "Hero.h"
#include "Monster.h"
#include "mapInfo.h"

static const UINT ID_TIMER_SPAWN = 1;
static const UINT ID_TIMER_UPDATE = 2;
static const UINT ID_BTN_SUMMON = 1001;

class GameManager {
public:
    GameManager(int mapW, int mapH, int tileW, int tileH)
        : mapWidth(mapW), mapHeight(mapH),
        dstW(tileW), dstH(tileH),
        dragIndex(-1), dragging(false)
    {
        // ��� ����: �� �׵θ� ���� �ð����
        path = std::make_shared<Path>();
        for (int j = 0; j < mapW; ++j)     path->waypoints.push_back({ j, 0 });
        for (int i = 1; i < mapH; ++i)     path->waypoints.push_back({ mapW - 1, i });
        for (int j = mapW - 2; j >= 0; --j)   path->waypoints.push_back({ j, mapH - 1 });
        for (int i = mapH - 2; i > 0; --i)    path->waypoints.push_back({ 0, i });
    }

    void init(HWND hWnd, HINSTANCE hInst) {
        this->hWnd = hWnd;
        // ��ư
        CreateWindowW(L"BUTTON", L"���� ��ȯ",
            WS_CHILD | WS_VISIBLE,
            10, 10, 100, 30,
            hWnd, (HMENU)ID_BTN_SUMMON,
            hInst, nullptr);
        // Ÿ�̸�: ���� ���� 2��, ������Ʈ 16ms
        SetTimer(hWnd, ID_TIMER_SPAWN, 2000, nullptr);
        SetTimer(hWnd, ID_TIMER_UPDATE, 16, nullptr);
    }

    void cmd(HWND, int id) {
        if (id == ID_BTN_SUMMON) {
            // ù��° build Ÿ�� ��ġ ã�Ƽ� ��ȯ
            for (int i = 0; i < mapHeight; ++i)
                for (int j = 0; j < mapWidth; ++j)
                    if (map.mapinfo[i][j].type == TileType::Build) {
                        heroes.push_back(
                            make_shared<Hero>(HeroType::Warrior, Rank::Bronze, Vec2{ j,i })
                        );
                        return;
                    }
        }
    }

    void mouseDown(int mx, int my) {
        // Ŭ���� ������ ������ �ֳ�?
        for (int k = 0; k < (int)heroes.size(); ++k) {
            auto& h = heroes[k];
            float dx = mx - h->pos.x, dy = my - h->pos.y;
            if (dx * dx + dy * dy <= (dstW / 2) * (dstW / 2)) {
                dragging = true;
                dragIndex = k;
                return;
            }
        }
    }
    void mouseMove(int mx, int my) {
        if (dragging && dragIndex >= 0) {
            heroes[dragIndex]->pos.x = mx;
            heroes[dragIndex]->pos.y = my;
        }
    }
    void mouseUp(int, int) {
        if (dragging) {
            dragging = false;
            dragIndex = -1;
        }
    }

    void timer(UINT_PTR id, float dt) {
        if (id == ID_TIMER_SPAWN) {
            if (monsters.size() < 100) {
                monsters.push_back(
                    std::make_shared<Monster>(path, Rank::Bronze)
                );
            }
            else {
                KillTimer(hWnd, ID_TIMER_SPAWN);
                MessageBox(hWnd, L"���� 100���� ����! ���� ����", L"Game Over", MB_OK);
            }
        }
        if (id == ID_TIMER_UPDATE) {
            for (auto& m : monsters)  m->update(dt);
            for (auto& h : heroes)    h->update(dt);
            InvalidateRect(hWnd, nullptr, FALSE);
        }
    }

    void render(Gdiplus::Graphics& g) {
        // ����: �Ķ� Ÿ��
        Gdiplus::SolidBrush blueBrush(Gdiplus::Color(255, 0, 0, 255));
        for (auto& h : heroes) {
            g.FillEllipse(&blueBrush,
                h->pos.x - dstW / 2,
                h->pos.y - dstH / 2,
                dstW, dstH
            );
        }
        // ����: ���� Ÿ��
        Gdiplus::SolidBrush redBrush(Gdiplus::Color(255, 255, 0, 0));
        for (auto& m : monsters) {
            g.FillEllipse(&redBrush,
                m->pos.x - dstW / 2,
                m->pos.y - dstH / 2,
                dstW, dstH
            );
        }
    }

private:
    HWND hWnd;
    int  mapWidth, mapHeight;
    int  dstW, dstH;
    std::shared_ptr<Path> path;
    std::vector<std::shared_ptr<Hero>>    heroes;
    std::vector<std::shared_ptr<Monster>> monsters;
    bool dragging;
    int  dragIndex;
    MapInfo map;
};
