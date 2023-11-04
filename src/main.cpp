#include <iostream>
#include "Gem.h"
#include "Board.h"
using namespace sf;

int main() {
	
	srand(time(nullptr));
	sf::RenderWindow window(sf::VideoMode(780, 800), "Gems");
	int field_space = 10;
	int dimension = 10;
	float block_size = 70.f;

	auto new_board = std::make_unique<Board>(dimension, field_space, block_size, &window);
	new_board->Draw();
	std::vector<sf::Vector2i> for_check;
	sf::Vector2i first_block_pos = { -1,-1 };
	sf::Vector2i second_block_pos = { -1,-1 };
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
				if (new_board->contains(mousePosition)) {
					if (first_block_pos.x == -1) {
						first_block_pos.x = 
							(mousePosition.x - field_space) / (int)block_size;
						first_block_pos.y = 
							(mousePosition.y - field_space) / (int)block_size;
						for_check.push_back(first_block_pos);
					}
					else {
						second_block_pos.x = 
							(mousePosition.x - field_space) / (int)block_size;
						second_block_pos.y = 
							(mousePosition.y - field_space) / (int)block_size;
						for_check.push_back(second_block_pos);

						new_board->Swap(first_block_pos, second_block_pos);
						new_board->IfDropped(for_check);
						for_check.clear();
						first_block_pos.x = -1;
					}
				}
			}
		}
	}
	return 0;
}