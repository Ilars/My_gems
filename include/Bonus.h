#pragma once
#include "Board.h"
#include "Gem.h"


class Brush : public Gem {
private:
	static const unsigned int PAINT_TARGETS;
	static const unsigned int PAINT_RADIUS;
protected:
	int _num_affected_gems;
public:
	Brush(float x, float y, float size, COLORS color = COLORS::NUM_COLORS);

	int GetNumTargets() const { return _num_affected_gems; };
	virtual void Activate(Board& board, const sf::Vector2i& coord) override;
	~Brush() {};
};


class Bomb : public Gem {
private:
	static const unsigned int BOMB_TARGETS;

protected:
	int _num_affected_gems;
public:
	Bomb(float x, float y, float size, COLORS color = COLORS::NUM_COLORS);
	int GetNumTargets() const { return _num_affected_gems; };
	virtual void Activate(Board& board, const sf::Vector2i& coord) override;
	~Bomb() {};

};
