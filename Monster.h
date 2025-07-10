#pragma once
#include "Entity.h"
using namespace std;

class Path {
public:
	vector<Vec2>waypoints;
};

class Monster : public Entity {
public:
	float speed;
	float hp;
	size_t pathIndex; // 현재 경로 인덱스
	shared_ptr<Path> path;
	Rank rank;

	Monster(shared_ptr<Path> pt);
	void update(float deltaTime) override;
	void render(Graphics& g)override;
};