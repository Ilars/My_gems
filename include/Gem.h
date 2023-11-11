#pragma once

#include <vector>
#include "Board.h"
#include "Resources.h"
#include "iostream"

enum class COLORS {
	BLUE,
	GREEN,
	YELLOW,
	CYAN,
	MAGENTA,
	RED,
	White,
	NUM_COLORS
};

class Gem: public sf::Sprite{

protected:
	COLORS _color;
	sf::Texture _texture;
	std::string _texturePath;
	static  const float SCALE_RATE;
	bool _is_dead = false;
	static const std::vector<sf::Color> AVAILABLE_COLORS;
protected:
	Board& _board;

public:
	Gem(Board& board, float x, float y, float size, COLORS color = COLORS::NUM_COLORS);
	COLORS GetColor() const { return _color; };
	void SetColor(COLORS new_color);

	void SetTexture();

	void Move(sf::Vector2f speed);
	
	bool IsDead() { return _is_dead; };
	void Dead() { _is_dead = true; };
	
	virtual void Activate(const sf::Vector2i& coord) {};

	virtual ~Gem() {};
};

