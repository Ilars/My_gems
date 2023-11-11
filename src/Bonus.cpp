#include "Bonus.h"
#include <set>
#include <iostream>
using namespace std;

const unsigned int Brush::PAINT_TARGETS = 3;
const unsigned int Brush::PAINT_RADIUS = 3;

Brush::Brush(Board& board, float x, float y, float size, COLORS color): Gem(board, x, y, size, color) {
	_texturePath = BRUSH_TEXTURE_PATH;
	_num_affected_gems = PAINT_TARGETS;
}

const unsigned int Bomb::BOMB_TARGETS = 5;


Bomb::Bomb(Board& board, float x, float y, float size, COLORS color): Gem(board, x, y, size, color) {
	_texturePath = BOMB_TEXTURE_PATH;
	_num_affected_gems = BOMB_TARGETS;
}


void Gem::Move(sf::Vector2f speed) {
	move(speed);
}

void Brush::Activate(const sf::Vector2i& coord) {
	const sf::Vector2i& board_size = { _board.GetDimension(), _board.GetDimension() };
	vector<sf::Vector2i> targets;
	for (int i = 0; i < _num_affected_gems; i++) {
		while (true) {
			sf::Vector2i targ = { coord.x, coord.y };

			if (rand() % 2)
				targ.x += rand() % PAINT_RADIUS;
			else
				targ.x -= rand() % PAINT_RADIUS;

			if (rand() % 3)
				targ.y -= rand() % PAINT_RADIUS;
			else
				targ.y += rand() % PAINT_RADIUS;


			// check if inside board and not a neighbor
			if (!(targ == coord || (targ.x == coord.x && targ.y == coord.y + 1)
				|| (targ.x == coord.x && targ.y == coord.y - 1)
				|| (targ.y == coord.y && targ.x == coord.x - 1)
				|| (targ.y == coord.y && targ.x == coord.x + 1))
				&& 0 <= targ.x && targ.x < board_size.x
				&& 0 <= targ.y && targ.y < board_size.y) {

				targets.push_back(targ);
				break;

			}
		}
	}

	_board.Recolor(targets, coord);
}


void Bomb::Activate(const sf::Vector2i& coord) {
	const sf::Vector2i& board_size = { _board.GetDimension(), _board.GetDimension() };
	vector<sf::Vector2i> targets;
	set<int> unique_targets;
	unique_targets.insert(coord.x * board_size.x + coord.y);
	int max_coord = board_size.x * board_size.y;
	while (unique_targets.size() < _num_affected_gems) {
		unique_targets.insert(rand() % max_coord);
	}
	for (const auto& c : unique_targets) {
		targets.push_back({ c / board_size.x, c % board_size.x });
	}

	_board.AddForDropping(targets);
}