#include "Board.h"
#include "Bonus.h"
#include "Gem.h"
#include "iostream"

using namespace std;

const unsigned int Board::BONUS_DROP_RADIUS = 3;
const unsigned int Board::DROP_THRESHOLD = 3;
const unsigned int Board::BONUS_CHANCE = 100;
const unsigned int Board::COMBO_SIZE_FOR_BONUS = 4;


void Board::LoadBoard() {
	float x = (float)_space;
	for (int i = 0; i < _dimension; i++) {
		float y = (float)_space;
		for (int j = 0; j < _dimension; j++) {
			auto gem = make_shared<Gem>(Gem(*this, x, y, _block_size));
			COLORS color;
			do {
				color = (static_cast<COLORS>
					(rand() % static_cast<int>(COLORS::NUM_COLORS)));
			} while ((i != 0 && _gems[i - 1][j]->GetColor() == color)
				|| (j != 0 && _gems[i][j - 1]->GetColor() == color));
			gem->SetColor(color);
			gem->SetTexture();
			_gems[i][j] = gem;
			y += _block_size;
		}
		x += _block_size;
	}
}

Board::Board(int dimension, int space, float block_size, sf::RenderWindow* window) {
	_window = window;
	_dimension = dimension;
	_space = space;
	_block_size = block_size;
	top = _space;
	left = _space;
	width = static_cast<int>(_dimension * _block_size);
	height = width;
	_for_dropping.resize(_dimension, set<int>());
	_gems.resize(_dimension, vector<std::shared_ptr<Gem>>(_dimension));
	
	LoadBoard();
}

void Board::SearchSequence(sf::Vector2i& block_pos, set<int>& seq_elems) {
	if (seq_elems.find(block_pos.x * _dimension + block_pos.y) != seq_elems.end())
		return;
	seq_elems.insert(block_pos.x * _dimension + block_pos.y);
	if (block_pos.x != 0 && IsColorMatch(block_pos, { block_pos.x - 1,block_pos.y })) {
		sf::Vector2i new_pos = { block_pos.x - 1, block_pos.y };
		SearchSequence(new_pos, seq_elems);
	}
	if (block_pos.x != _dimension - 1 && IsColorMatch(block_pos, { block_pos.x + 1,block_pos.y })) {
		sf::Vector2i new_pos = { block_pos.x + 1, block_pos.y };
		SearchSequence(new_pos, seq_elems);
	}
	if (block_pos.y != 0 && IsColorMatch(block_pos, { block_pos.x,block_pos.y - 1 })) {
		sf::Vector2i new_pos = { block_pos.x, block_pos.y - 1 };
		SearchSequence(new_pos, seq_elems);
	}
	if (block_pos.y != _dimension - 1 && IsColorMatch(block_pos, { block_pos.x ,block_pos.y + 1 })) {
		sf::Vector2i new_pos = { block_pos.x, block_pos.y + 1 };
		SearchSequence(new_pos, seq_elems);
	}
}

bool Board::IsColorMatch(const sf::Vector2i& gem_pos, const sf::Vector2i& neighbor) const {
	auto color = _gems[gem_pos.x][gem_pos.y]->GetColor();
	auto neigh_color = _gems[neighbor.x][neighbor.y]->GetColor();
	if (color == neigh_color)
		return true;
	return false;

}

sf::Vector2i Board::ChooseRandInRadius(int radius, sf::Vector2i pos) {
	int dx = pos.x + rand() % radius;
	int dy = pos.y + rand() % radius;
	while (dx < 1 || dy < 1 || dx > _dimension - 2 || dy > _dimension - 2) {
		dx = pos.x;
		dy = pos.y;
		if (rand() % 2)
			dx += rand() % radius;
		else
			dx -= rand() % radius;

		if (rand() % 2)
			dy -= rand() % radius;
		else
			dy += rand() % radius;
	}
	return { dx,dy };

}

bool Board::InSequence(int bonus_pos_key, std::set<int>& seq) {
	for (auto elem : seq) {
		if (bonus_pos_key == elem)
			return true;
	}
	return false;
}

void Board::SetRandomBonus(int key, std::set<int>& seq) {
	int bonus_threshold = rand() % 100;

	int x_bonus = key / _dimension;
	int y_bonus = key % _dimension;
	auto bonus_pos = ChooseRandInRadius(BONUS_DROP_RADIUS, { x_bonus, y_bonus });
	//	while (_gems[bonus_pos.x][bonus_pos.y]->IsDead() || _gems[bonus_pos.x][bonus_pos.y]->IsBonus() 
	//           || InSequence(bonus_pos.x * _dimension + bonus_pos.y, seq)) {

	while (_gems[bonus_pos.x][bonus_pos.y]->IsDead() || InSequence(bonus_pos.x * _dimension + bonus_pos.y, seq)) {
		bonus_pos = ChooseRandInRadius(BONUS_DROP_RADIUS, { x_bonus, y_bonus });
	};
	auto bonus_color = _gems[bonus_pos.x][bonus_pos.y]->GetColor();
	cout << bonus_pos.x << " " << bonus_pos.y <<"\n";
	sf::Vector2f move_to(_space /2+ bonus_pos.x * _block_size, _space + bonus_pos.y * _block_size);

	std::shared_ptr<Gem> bonus;

	if (bonus_threshold % 100 < BONUS_CHANCE) {
		if (bonus_threshold % 2) {
			bonus = std::make_shared<Bomb>(*this, bonus_pos.x, bonus_pos.y, _block_size);
		}
		else {
			bonus = std::make_shared<Brush>(*this, bonus_pos.x, bonus_pos.y, _block_size);
		}
		bonus->SetColor(bonus_color);
		bonus->SetTexture();
		_gems[bonus_pos.x][bonus_pos.y] = bonus;
		_gems[bonus_pos.x][bonus_pos.y]->Move(move_to);
	}
	
}

void Board::AddForDropping(std::vector<sf::Vector2i> targets) {
	for (auto& drop : targets) {
		_for_dropping[drop.x].insert(drop.y);
	}
}

void Board::Recolor(std::vector<sf::Vector2i> targets, sf::Vector2i position) {
	for (auto& recolor : targets) {
		_gems[recolor.x][recolor.y]->
			SetColor(_gems[position.x][position.y]->GetColor());
	}
}

void Board::DestroySequence(std::set<int>& seq_elems) {
	for (auto key : seq_elems) {
		sf::Vector2i position{ key / _dimension, key % _dimension };
		_gems[position.x][position.y]->Activate(position);
		_gems[position.x][position.y]->Dead();
		_for_dropping[position.x].insert(position.y);
	}
}

bool Board::IfSequence(std::set<int>& seq_elems) {
	if (seq_elems.size() >= DROP_THRESHOLD) {
		DestroySequence(seq_elems);
		if (seq_elems.size() >= COMBO_SIZE_FOR_BONUS)
		{
			int key = *seq_elems.begin();
			SetRandomBonus(key, seq_elems);
		}
		seq_elems.clear();
		return true;
	}

	seq_elems.clear();
	return false;
}


void Board::DropStep(vector<int>& for_step) {
	sf::Vector2f speed(0.f, 0.25f);
	for (int i = 0; i < int(_block_size * 4); i++) {
		for (int j = 0; j < _dimension; j++) {
			for (int k = 0; k <= for_step[j]; k++)
				_gems[j][k]->Move(speed);
		}
		Draw();
	}
	for (int j = 0; j < _dimension; j++) {
		if (for_step[j] != -1) {
			auto buf = _gems[j][for_step[j]];
			for (int k = for_step[j]; k > 0; k--) {
				_gems[j][k] = _gems[j][k - 1];
			}
			_gems[j][0] = buf;
		}
	}
}

void Board::MoveToTop() {
	for (int colomn = 0; colomn < _dimension; colomn++) {
		if (_for_dropping[colomn].size() > 0) {
			auto row = _for_dropping[colomn].end();
			row--;
			for (unsigned int i = 1; i <= _for_dropping[colomn].size(); i++, row--) {
				auto ngem = make_shared<Gem>(Gem(*this, colomn, *row, _block_size));
				ngem->SetColor(static_cast<COLORS>
					(rand() % static_cast<int>(COLORS::NUM_COLORS)));
				ngem->SetTexture();
				_gems[colomn][*row] = ngem;
				_gems[colomn][*row]->
					Move(sf::Vector2f(_space/2 + colomn * _block_size, _space/2 + (*row) * _block_size - (_block_size * (*row + i))));

			}
		}
	}
}

void Board::CheckFallen() {
	vector<sf::Vector2i> for_check;
	for (int colomn = 0; colomn < _dimension; colomn++) {
		if (_for_dropping[colomn].size() > 0) {
			auto row = _for_dropping[colomn].end();
			row--;
			for (int i = 0; i <= *row; i++)
				for_check.push_back(sf::Vector2i{ colomn, i });
		}
		_for_dropping[colomn].clear();
	}
	IfDropped(for_check);
}

void Board::Dropping() {
	MoveToTop();

	vector<int> for_step(_dimension, -1);
	unsigned int step = 1;
	bool is_falling = true;
	while (is_falling) {
		is_falling = false;
		for (int colomn = 0; colomn < _dimension; colomn++) {
			if (_for_dropping[colomn].size() >= step) {
				is_falling = true;
				auto row = _for_dropping[colomn].end(); 
				for (unsigned int i = 0; i < step; i++, row--);
				for_step[colomn] = *row + step - 1;
			}
			else
				for_step[colomn] = -1;
		}
		DropStep(for_step);
		step++;
	}

	CheckFallen();
}


void Board::Draw() {
	_window->clear(sf::Color::White);
	for (int i = 0; i < _dimension; i++) {
		for (int j = 0; j < _dimension; j++) {
			_window->draw(*_gems[i][j]);
		}
	}
	_window->display();
}

void Board::Swap(sf::Vector2i block1, sf::Vector2i block2) {
	sf::Vector2f finish1 = _gems[block2.x][block2.y]->getPosition();
	sf::Vector2f finish2 = _gems[block1.x][block1.y]->getPosition();
	sf::Vector2f speed = finish2 - finish1;
	sf::Vector2f delt;
	float distance = sqrtf(speed.x * speed.x + speed.y * speed.y);
	if (distance > _block_size*1.2f)
		return;
	while (distance >= 0.03f) {
		_gems[block1.x][block1.y]->Move(-speed * 0.01f);
		_gems[block2.x][block2.y]->Move(speed * 0.01f);
		_window->clear();
		Draw();
		delt = _gems[block1.x][block1.y]->getPosition() - finish1;
		distance = sqrtf(delt.x * delt.x + delt.y * delt.y);
	}
	_gems[block2.x][block2.y]->Move(finish2 - _gems[block2.x][block2.y]->getPosition());
	_gems[block1.x][block1.y]->Move(finish1 - _gems[block1.x][block1.y]->getPosition());
	_window->clear();
	Draw();
	std::swap(_gems[block1.x][block1.y], _gems[block2.x][block2.y]);
}


void Board::IfDropped(vector<sf::Vector2i>& for_check) {
	bool if_seq = false;
	std::set<set<int>> seq_s;
	for (auto& pos : for_check) {
		set<int> seq_elems;
		SearchSequence(pos, seq_elems);
		seq_s.insert(seq_elems);
	}
	for (auto seq : seq_s) {
		if_seq = IfSequence(seq) || if_seq;
	}
	if (!if_seq && for_check.size() == 2)
		Swap(for_check[0], for_check[1]);
	for_check.clear();
	if (if_seq)
		Dropping();
}