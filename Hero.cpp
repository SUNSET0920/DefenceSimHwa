#include "Hero.h"
#include "Monster.h"
#include "MapRender.h"

static constexpr float TILE_SIZE = 50.f;
static constexpr float RANK_MULTIPLIER[] = { 1.0f, 1.5f, 2.0f };

Hero::Hero(HeroType t, Rank r, Vec2 p):type(t),rank(r), gridPos(p) {

    bool ishero = false;
    for (int i = 1; i < Height-1; ++i) {
        for (int j = 1; j < Width-1; ++j) {
            auto& cell = g_map.mapinfo[i][j];
            if (cell.inHero == false) {
                pos.x = cell.pos.x;
                pos.y = cell.pos.y;
                cell.inHero = true;
                ishero = true;
                break;
            }
        }
        if (ishero)
            break;
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
    shared_ptr<Monster> target = nullptr;
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

PointF Hero::pixelPos() const {
    return {
        float(OFFSET_X + gridPos.x * dstW + dstW / 2), float(OFFSET_Y + gridPos.y * dstH + dstH / 2)
    };
}

void Hero::render(Gdiplus::Graphics& g) {
    // �̹����� �߾� �������� �׸� ���:  
    auto p = pixelPos();
    int halfW = dstW / 2, halfH = dstH / 2;
    // e.g. g.DrawImage(heroBitmap, p.X-halfW, p.Y-halfH, dstW, dstH);  
    // �׽�Ʈ��: �Ķ� ������ ��ü  
    SolidBrush blueBrush(Color(255, 0, 0, 255));
    g.FillEllipse(&blueBrush,
        int(p.X) - halfW,
        int(p.Y) - halfH,
        dstW, dstH
    );
}