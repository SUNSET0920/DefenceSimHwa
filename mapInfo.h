// �� ������ ����
// �ش� ��ġ�� ���� �̵��������, ���� ��ġ ���������� �� �� ����.
#include <iostream>
#include <vector>

using namespace std;

int Width = 11;
int Height = 7;



// enum ���� ������ UP
enum class TileType : int {
    Path = 0,   // ���� �̵� ���
    Build = 1    // ���� ��ġ ����
};

class MapInfo {
public:
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
};