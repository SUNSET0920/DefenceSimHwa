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
	Hero(HeroType t, Rank r, Vec2 p);
	
	void update(float dt) override;
	void render(Graphics& g) override;
	PointF pixelPos() const;
	Vec2 gridPos;  // �̰ɷ� ������ ��ġ�� ���

	HeroType type;
	Rank rank;

	float attackCooldown; // ���� ������ �ð�
	float attackSpeed; // �ʴ� ���� Ƚ��
	float attackDamage; // Ÿ�ݴ� ������
	float detectRange; // ����/��Ÿ� (�ȼ� ����)

private:
	void findAndAttack(const vector<shared_ptr<Monster>>& monsters);
};