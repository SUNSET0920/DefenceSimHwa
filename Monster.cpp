#include "Monster.h"
#include "mainheader.h"  // Gdiplus 포함
#include <cmath>

using namespace Gdiplus;

static constexpr float RANK_MULTIPLIER[] = { 1.0f, 1.5f, 2.0f };

// 맵 렌더링에서 사용하는 타일 크기 및 오프셋
static const int dstW = 50;
static const int dstH = 50;
static const int offsetX = 100;
static const int offsetY = 100;

Monster::Monster(shared_ptr<Path> pt, Rank r): speed(80.0f), hp(100.0f), pathIndex(0), path(pt), rank(r)
{
    if (path && !path->waypoints.empty()) {
        Vec2 start = path->waypoints[0];
        pos.x = offsetX + start.x * dstW + dstW / 2;
        pos.y = offsetY + start.y * dstH + dstH / 2;
    }

    float coef = RANK_MULTIPLIER[static_cast<int>(rank)];
    hp *= coef;
    speed /= coef;
}

void Monster::update(float deltaTime) {
    if (!path || path->waypoints.empty()) return;

    // 다음 경로 지점
    size_t nextIndex = (pathIndex + 1) % path->waypoints.size();
    Vec2 nextTile = path->waypoints[nextIndex];
    float targetX = offsetX + nextTile.x * dstW + dstW / 2;
    float targetY = offsetY + nextTile.y * dstH + dstH / 2;

    // 방향 및 거리 계산
    float dx = targetX - pos.x;
    float dy = targetY - pos.y;
    float dist = sqrt(dx * dx + dy * dy);

    if (dist < speed * deltaTime) {
        pos.x = targetX;
        pos.y = targetY;
        pathIndex = nextIndex;
    }
    else {
        pos.x += dx / dist * speed * deltaTime;
        pos.y += dy / dist * speed * deltaTime;
    }
}

void Monster::render(Graphics& g) {
    Pen pen(Color(255, 255, 0, 0));
    g.DrawEllipse(&pen,
        pos.x - dstW / 2,
        pos.y - dstH / 2,
        dstW,
        dstH);
}
