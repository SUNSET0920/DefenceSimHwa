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
    // 쿨타임 갱신
    if (attackCooldown > 0.0f)
        attackCooldown -= dt;

    frameTime += dt;
    if (frameTime >= frameDuration) {
        frameTime = 0;
        curFrame++;
    }
    // 사거리 내 몬스터 탐색 및 공격
    // (GameManager에서 monsters 벡터를 넘겨 받거나, 참조 획득)
    //findAndAttack(/*monsters from GM*/);
}

void Hero::findAndAttack(const vector<shared_ptr<Monster>>& monsters) {
    if (attackCooldown > 0.0f) return;

    // 가장 가까운 몬스터 찾기
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
        // 실제 공격 처리 (hp 감소 등)
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
    // 이미지를 중앙 기준으로 그릴 경우:  
    auto p = pixelPos();
    int halfW = dstW / 2, halfH = dstH / 2;
    // e.g. g.DrawImage(heroBitmap, p.X-halfW, p.Y-halfH, dstW, dstH);  
    // 테스트용: 파란 원으로 대체  
    SolidBrush blueBrush(Color(255, 0, 0, 255));
    g.FillEllipse(&blueBrush,
        int(p.X) - halfW,
        int(p.Y) - halfH,
        dstW, dstH
    );
}