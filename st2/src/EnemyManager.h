#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics/Texture.hpp>

struct KeyHasher
{
	std::size_t operator()(const sf::Vector2f& key) const;
};


#include "Player.h"


class Enemy;
class Expbar;
class EnemyManager
{
	std::vector<std::shared_ptr<Enemy>> m_enemies;
	std::unordered_map<sf::Vector2f,std::vector<std::weak_ptr<Enemy>>, KeyHasher> m_cells_enemies;
	sf::Texture m_texture;
	sf::VertexArray m_vertex_array;
public:
	EnemyManager();
	std::shared_ptr<Enemy> get_closest_enemy(sf::Vector2f position);
	std::vector<std::shared_ptr<Enemy>> all_intersections(sf::FloatRect bounding_box);
	void spawn_enemy(Player* player);
	void update(std::shared_ptr<Eventsystem>& eventsystem, std::shared_ptr<Soundsystem>& soundsystem, double deltatime, Player* player, std::shared_ptr<Expbar>& expbar);
	void add_enemy();
	void draw(sf::RenderTarget& target) const;
};

