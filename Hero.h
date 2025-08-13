#pragma once
#include "Entity.h"
#include "mapInfo.h"
#include "mainheader.h"
using namespace std;

enum class HeroType {Warrior, Archer, Mage};

class Monster; // ���漱��
// ������ϳ��� ���� include �ϸ� �����Ϸ��� � Ŭ������ ���� ���ǵǴ��� ���� �� �ְ�,
// Ư�� �� Ŭ������ ���θ� �����ϴ� ��Ȳ������ ��ȯ ���� ������ ����Ŵ.

class Hero : public Entity {
public:
	Hero();
	Hero(Rank r, Vec2 p);
	
	void update(float dt) override;
	void render(Graphics& g) override;
	PointF pixelPos() const;
	Vec2 gridPos;  // �̰ɷ� ������ ��ġ�� ���
	void findAndAttack(const vector<shared_ptr<Monster>>& monsters);

	HeroType type;
	Rank rank;
	
	float attackCooldown; // ���� ������ �ð�
	float attackSpeed; // �ʴ� ���� Ƚ��
	float attackDamage; // Ÿ�ݴ� ������
	float detectRange; // ����/��Ÿ� (�ȼ� ����)
	
	static constexpr float RANK_MULTIPLIER[] = { 1.0f, 1.5f, 2.0f };
	bool attacking = false;          // ���� ���� ������
	float attackAnimTime = 0.0f;     // ���� ���� ���� �ð�
	const float attackAnimDuration = 0.2f; // ���� ���� ���� (��)
	int curFrame = 0;
	float frameTime = 0.0f;
	float frameDuration = 0.1f;
};