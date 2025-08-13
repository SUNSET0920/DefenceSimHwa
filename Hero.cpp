#include "Hero.h"
#include "Monster.h"
#include "MapRender.h"


static constexpr float RANK_MULTIPLIER[] = { 1.0f, 1.5f, 2.0f };

Hero::Hero(Rank r, Vec2 p):rank(r), gridPos(p) {

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
}

void Hero::update(float dt) {
    // ��Ÿ�� ����
    if (attackCooldown > 0.0f)
        attackCooldown -= dt;

    frameTime += dt;
    if (frameTime >= frameDuration) {
        frameTime = 0;
        curFrame++;
    }
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