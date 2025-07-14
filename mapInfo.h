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
    TileType type;
};

class MapInfo {
public:
    vector<vector<MapForHero>> mapinfo;
    //vector<vector<TileType>> grid;
    //MapInfo() : mapinfo(Height, vector<MapForHero>(Width)), grid(Height, vector<TileType>(Width, TileType::Build)) {};
    MapInfo() : mapinfo(Height, vector<MapForHero>(Width)) {};
    void setMap() {
        // �ʱ�ȭ ����
        for (int i = 0; i < Height; ++i) {
            for (int j = 0; j < Width; ++j) {
                if (i == 0 || i == Height - 1 || j == 0 || j == Width - 1) {
                    mapinfo[i][j].type = TileType::Path;
                }
                else {
                    mapinfo[i][j].type = TileType::Build;
                }
                mapinfo[i][j].inHero = false;
                mapinfo[i][j].pos.x = OFFSET_X + dstH * (j + 1);
                mapinfo[i][j].pos.y = OFFSET_Y + dstW * (i + 1);
            }
        }
    };
};