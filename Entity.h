#pragma once
#include "mainheader.h"
#include <utility>
#include <vector>
#include <memory>


struct Vec2 { // 좌표
	int x, y;
	//Vec2(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

enum class Rank { Bronze, Silver, Gold };

class Entity {
public:
	Vec2 pos;
	virtual void update(float deltaTime) = 0; 
	// deltaTime = 이전 프레임 끝나고 현재 프레임 시작될 때까지 걸린 시간
	virtual void render(Graphics& g) = 0;
	virtual ~Entity() = default;
};