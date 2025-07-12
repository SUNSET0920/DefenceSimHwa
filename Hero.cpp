#include "Hero.h"
#include "Monster.h"

MapInfo map;

static constexpr float TILE_SIZE = 50.f;
static constexpr float RANK_MULTIPLIER[] = { 1.0f, 1.5f, 2.0f };

Hero::Hero(HeroType t, Rank r, Vec2 p):type(t),rank(r) {
    for (int i = 0; i < Height - 1; ++i) {
        for (int j = 0; j < Width - 1; ++j) {
            if (map.MFH[i][j].inHero == false) {
                pos = map.MFH[i][j].pos;
                map.MFH[i][j].inHero = true;
                break;
            }
        }
    }

	switch (type) {
	case HeroType::Warrior:
		attackSpeed = 0.5f; // 0.5�ʿ� �� ��
		attackDamage = 30.0f; // 
		detectRange = TILE_SIZE * 1.0f; // ���� 1Ÿ��
		break;
	case HeroType::Archer:
		attackSpeed = 1.0f; // 1�ʿ� �� ��
		attackDamage = 20.0f; // 
		detectRange = TILE_SIZE * 2.5f; // ���� 2.5Ÿ��
		break;
	case HeroType::Mage:
		attackSpeed = 1.5f; // 1.5�ʿ� �� ��
		attackDamage = 25.0f; // 
		detectRange = TILE_SIZE * 2.0f; // ���� 2Ÿ��
		break;
	}

	// ��� ����
	float coef = RANK_MULTIPLIER[static_cast<int>(rank)];
	attackDamage *= coef;
	attackSpeed /= coef;

	attackCooldown = 0.0f;
}

void Hero::update(float dt) {
    // ��Ÿ�� ����
    if (attackCooldown > 0.0f)
        attackCooldown -= dt;

    // ��Ÿ� �� ���� Ž�� �� ����
    // (GameManager���� monsters ���͸� �Ѱ� �ްų�, ���� ȹ��)
    //findAndAttack(/*monsters from GM*/);
}

void Hero::findAndAttack(const vector<shared_ptr<Monster>>& monsters) {
    if (attackCooldown > 0.0f) return;

    // ���� ����� ���� ã��
    std::shared_ptr<Monster> target = nullptr;
    float bestDist2 = detectRange * detectRange;

    for (auto& m : monsters) {
        float dx = m->pos.x - pos.x;
        float dy = m->pos.y - pos.y;
        float dist2 = dx * dx + dy * dy;
        if (dist2 <= bestDist2) {
            bestDist2 = dist2;
            target = m;
        }
    }

    if (target) {
        // ���� ���� ó�� (hp ���� ��)
        target->hp -= static_cast<int>(attackDamage);
        attackCooldown = attackSpeed;
    }
}

void Hero::render(Gdiplus::Graphics& g) {
    // pos�� �߽����� ���� �̹��� �׸���
    // ��: g.DrawImage(warriorImg, pos.x - TILE_SIZE/2, pos.y - TILE_SIZE/2);
}