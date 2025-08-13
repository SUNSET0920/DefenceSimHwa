#pragma once
#include "Hero.h"
#define TILE_SIZE 50.0f


class Warrior : public Hero {
private:
	Image* idleSheet;
	Image* attackSheet;
	int idleFrameCount = 4;   // Idle.png�� ������ ����
	int attackFrameCount = 6; // Attack.png�� ������ ����
public:
	Warrior() : Hero(Rank::Bronze, { 0,0 }){}

	Warrior(Rank r, Vec2 p) : Hero(r, p) {
		type = HeroType::Warrior;
		attackSpeed = 0.5f; // 0.5�ʿ� �� ��
		attackDamage = 30.0f; //
		detectRange = TILE_SIZE * 1.0f; // ���� 1Ÿ��
		
		float coef = RANK_MULTIPLIER[static_cast<int>(r)];
		attackDamage *= coef;
		attackSpeed /= coef;

		attackCooldown = 0.0f;
		// ���⼭�� �̹��� �ε�
		Image img(L"Asserts/Images/Hero/Pink__Idle_.png");
		if (img.GetLastStatus() != Ok) {
			MessageBox(NULL, L"�̹��� �ε� ����", L"����", MB_ICONERROR);
		}
		idleSheet = new Image(L"Asserts/Images/Hero/Pink__Idle_.png");
		attackSheet = new Image(L"Asserts/Images/Hero/Pink_attack.png");		
	};

	void render(Gdiplus::Graphics& g) override {
		if (!idleSheet || !attackSheet) return;

		auto p = pixelPos();
		int frameWidth = idleSheet->GetWidth() / idleFrameCount;
		int frameHeight = idleSheet->GetHeight();

		int frame = curFrame % (attacking ? attackFrameCount : idleFrameCount);
		Image* sheet = attacking ? attackSheet : idleSheet;
		int enW = 40;
		int enH = 40;
		g.DrawImage(
			sheet,
			Rect(int(p.X) - enW / 2, int(p.Y) - enH / 2, enW, enH),
			frame * frameWidth, 0, frameWidth, frameHeight,
			UnitPixel
		);
	};
};

class Archer :public Hero {
public:
	Archer() :Hero(Rank::Bronze, { 0,0 }) {};
	Archer(Rank r, Vec2 p) : Hero(r, p) {
		type = HeroType::Archer;
		attackSpeed = 1.0f; // 1�ʿ� �� ��
		attackDamage = 20.0f; // 
		detectRange = TILE_SIZE * 2.5f; // ���� 2.5Ÿ��

		float coef = RANK_MULTIPLIER[static_cast<int>(r)];
		attackDamage *= coef;
		attackSpeed /= coef;

		attackCooldown = 0.0f;
	}
};

class Mage : public Hero {
public:
	Mage() :Hero(Rank::Bronze, { 0,0 }) {};
	Mage(Rank r, Vec2 p) : Hero(r, p) {
		type = HeroType::Mage;
		attackSpeed = 1.5f; // 1.5�ʿ� �� ��
		attackDamage = 25.0f; // 
		detectRange = TILE_SIZE * 2.0f; // ���� 2Ÿ��
		
		float coef = RANK_MULTIPLIER[static_cast<int>(r)];
		attackDamage *= coef;
		attackSpeed /= coef;

		attackCooldown = 0.0f;
	}
};