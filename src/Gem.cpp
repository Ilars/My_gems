#include "Gem.h"
#include "Resources.h"

using namespace std;

const float Gem::SCALE_RATE = 0.8;

const vector<sf::Color> Gem::AVAILABLE_COLORS = { 
	  sf::Color::Blue, sf::Color::Green, sf::Color::Yellow,
	  sf::Color::Cyan, sf::Color::Magenta, sf::Color::White,
	  sf::Color::Red
};


void Gem::SetColor(COLORS color) {
	_color = color;
	setColor(AVAILABLE_COLORS[static_cast<int>(color)]);
	
}

void Gem::SetTexture() {
	sf::Image img;
	img.loadFromFile(_texturePath);
	sf::Texture tex;
	tex.loadFromImage(img);
	_texture = tex;
	setTexture(_texture);

}

Gem::Gem(Board& board, float x, float y, float size, COLORS color): _board(board) {
	setPosition(x,y);
	setScale(sf::Vector2f(SCALE_RATE, SCALE_RATE));
	_color = color;
	_texturePath = GEM_TEXTURE_PATH;
}

