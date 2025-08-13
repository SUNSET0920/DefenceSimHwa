#include "Monster.h"
#include "mainheader.h"  // Gdiplus ����
#include "mapInfo.h"
#include <cmath>

using namespace Gdiplus;

static constexpr float RANK_MULTIPLIER[] = { 1.0f, 1.5f, 2.0f };


Monster::Monster(shared_ptr<Path> pt, Rank r): speed(80.0f), hp(100.0f), pathIndex(0), path(pt), rank(r)
{
    if (path && !path->waypoints.empty()) {
        Vec2 start = path->waypoints[0];
        pos.x = OFFSET_X + start.x * dstW + dstW / 2;
        pos.y = OFFSET_Y + start.y * dstH + dstH / 2;
    }

    float coef = RANK_MULTIPLIER[static_cast<int>(rank)];
    hp *= coef;
    speed /= coef;
}

void Monster::update(float deltaTime) {
    if (!path || path->waypoints.empty()) return;

    // ���� ��� ����
    size_t nextIndex = (pathIndex + 1) % path->waypoints.size();
    Vec2 nextTile = path->waypoints[nextIndex];
    float targetX = OFFSET_X + nextTile.x * dstW + dstW / 2;
    float targetY = OFFSET_Y + nextTile.y * dstH + dstH / 2;

    // ���� �� �Ÿ� ���
    float dx = targetX - pos.x;
    float dy = targetY - pos.y;
    float dist = sqrt(dx * dx + dy * dy);

    if (dist < speed * deltaTime) {
        pos.x = targetX;
        pos.y = targetY;
        pathIndex = nextIndex;
    }
    else {

        float moveval = speed * deltaTime;

        int vx = dx / dist * moveval;
        int vy = dy / dist * moveval;
        
        pos.x += vx;
        pos.y += vy;
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
