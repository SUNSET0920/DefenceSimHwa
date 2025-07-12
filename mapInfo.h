// �� ������ ����
// �ش� ��ġ�� ���� �̵��������, ���� ��ġ ���������� �� �� ����.
#pragma once
#include <iostream>
#include <vector>
#include "Entity.h"

using namespace std;

static const int Width = 11;
static const int Height = 7;

constexpr int OFFSET_X = 100;
constexpr int OFFSET_Y = 300;
constexpr int dstW = 50;
constexpr int dstH = 50;


// enum ���� ������ UP
enum class TileType : int {
    Path = 0,   // ���� �̵� ���
    Build = 1    // ���� ��ġ ����
};

struct MapForHero {
    Vec2 pos;
    bool inHero;

};

class MapInfo {
public:
    vector<vector<MapForHero>> MFH;
    vector<vector<TileType>> grid;
    MapInfo() : grid(Height, vector<TileType>(Width, TileType::Build)) {};
    void setMap() {
        // �ʱ�ȭ ����
        for (int i = 0; i < Height; ++i) {
            for (int j = 0; j < Width; ++j) {
                if (i == 0 || i == Height-1 || j == 0 || j == Width-1)
                    grid[i][j] = TileType::Path;
                else
                    grid[i][j] = TileType::Build;
            }
        }
    };
    void setMFH() {
        for (int i = 0; i < Height - 1; ++i) {
            for (int j = 0; j < Width - 1; ++j) {
                MFH[i][j].inHero = false;
                MFH[i][j].pos.x = OFFSET_X + dstH * (j + 1);
                MFH[i][j].pos.y = OFFSET_Y + dstW * (i + 1);
            }
        }
    };
};