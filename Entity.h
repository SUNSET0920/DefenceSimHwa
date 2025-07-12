#pragma once
#include "mainheader.h"
#include <utility>
#include <vector>
#include <memory>


struct Vec2 { // ��ǥ
	int x, y;
	//Vec2(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

enum class Rank { Bronze, Silver, Gold };

class Entity {
public:
	Vec2 pos;
	virtual void update(float deltaTime) = 0; 
	// deltaTime = ���� ������ ������ ���� ������ ���۵� ������ �ɸ� �ð�
	virtual void render(Graphics& g) = 0;
	virtual ~Entity() = default;
};