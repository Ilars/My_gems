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

public:
	Gem(float x = 0.f, float y = 0.f, float size=0.f, COLORS color = COLORS::NUM_COLORS);
	COLORS GetColor() const { return _color; };
	void SetColor(COLORS new_color);

	void SetTexture();

	void Move(sf::Vector2f speed);
	
	bool IsDead() { return _is_dead; };
	void Dead() { _is_dead = true; };
	
	virtual void Activate(Board& board, const sf::Vector2i& coord) {};

	virtual ~Gem() {};
};

