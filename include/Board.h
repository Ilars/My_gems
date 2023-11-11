#pragma once

#include <cmath>
#include <vector>
#include <set>
#include "SFML/Graphics.hpp"



class Gem;

class Board : public sf::IntRect {

private:
	static const unsigned int BONUS_DROP_RADIUS;
	static const unsigned int DROP_THRESHOLD;
	static const unsigned int BONUS_CHANCE;
	static const unsigned int COMBO_SIZE_FOR_BONUS;

	std::vector<std::vector<std::shared_ptr<Gem>>> _gems;
	sf::RenderWindow* _window;

	std::vector<std::set<int>> _for_dropping;
	int _dimension; 
	int _space;
	float _block_size;

	void LoadBoard();

	bool IsColorMatch(const sf::Vector2i& gem_pos, const sf::Vector2i& neighbor) const;
	void SearchSequence(sf::Vector2i& block_pos, std::set<int>& seq_elems);
	void DestroySequence(std::set<int>& seq_elems);
	bool IfSequence(std::set<int>& seq_elems);


	void DropStep(std::vector<int>& for_step);
	void MoveToTop();
	void CheckFallen();
	void Dropping();

	sf::Vector2i ChooseRandInRadius(int radius, sf::Vector2i pos);
	bool InSequence(int bonus_pos_key, std::set<int>& seq);
	void SetRandomBonus(int key, std::set<int>& seq);


public:
	Board(int dimension, int space, float block_size, sf::RenderWindow* window);
	int GetDimension() { return _dimension; };
	void AddForDropping(std::vector<sf::Vector2i> targets);
	void Recolor(std::vector<sf::Vector2i> targets, sf::Vector2i position);

	std::vector<std::vector<std::shared_ptr<Gem>>> GetGems() { return _gems; };
	std::vector<std::set<int>> GetDrops() { return _for_dropping; };

	void Draw();
	void Swap(sf::Vector2i block1, sf::Vector2i block2);
	void IfDropped(std::vector<sf::Vector2i>& for_check);
};
