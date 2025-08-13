#pragma once
#include "Entity.h"
#include "mapInfo.h"
#include "mainheader.h"
using namespace std;

enum class HeroType {Warrior, Archer, Mage};

class Monster; // 전방선언
// 헤더파일끼리 서로 include 하면 컴파일러가 어떤 클래스가 언제 정의되는지 꼬일 수 있고,
// 특히 두 클래스가 서로를 참조하는 상황에서는 순환 포함 문제를 일으킴.

class Hero : public Entity {
public:
	Hero();
	Hero(Rank r, Vec2 p);
	
	void update(float dt) override;
	void render(Graphics& g) override;
	PointF pixelPos() const;
	Vec2 gridPos;  // 이걸로 렌더링 위치를 계산
	void findAndAttack(const vector<shared_ptr<Monster>>& monsters);

	HeroType type;
	Rank rank;
	
	float attackCooldown; // 남은 재장전 시간
	float attackSpeed; // 초당 공격 횟수
	float attackDamage; // 타격당 데미지
	float detectRange; // 감지/사거리 (픽셀 단위)
	
	static constexpr float RANK_MULTIPLIER[] = { 1.0f, 1.5f, 2.0f };
	bool attacking = false;          // 현재 공격 중인지
	float attackAnimTime = 0.0f;     // 공격 상태 유지 시간
	const float attackAnimDuration = 0.2f; // 공격 상태 지속 (초)
	int curFrame = 0;
	float frameTime = 0.0f;
	float frameDuration = 0.1f;
};