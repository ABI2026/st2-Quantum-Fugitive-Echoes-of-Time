#pragma once
#include <memory>

#include "ButtonBehaviour.h"
#include "ButtonLayout.h"

class Buttons
{
private:
	std::shared_ptr<ButtonLayout> m_layout;
	std::shared_ptr<ButtonBehaviour> m_button_behaviour;
public:
	Buttons(){}

	void set_layout(const std::shared_ptr<ButtonLayout>& i_layout)
	{
		m_layout = i_layout;
	}

	void set_behaviour(const std::shared_ptr<ButtonBehaviour>& i_behaviour)
	{
		m_button_behaviour = i_behaviour;
	}

	bool is_clicked() const
	{
		return m_layout->is_clicked();
	}
	bool is_hovered() const
	{
		return m_layout->is_hovered();
	}

	void update(sf::Vector2f mouse_position, bool mouse_pressed) const
	{
		m_layout->update(mouse_position,mouse_pressed);
	}

	bool on_click(std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window) const
	{
		return m_button_behaviour->on_click(layer_manager, soundsystem, window);
	}

	void set_position(sf::Vector2f position) const
	{
		m_layout->set_position(position);
	}

	void render(sf::RenderWindow& window) const
	{
		m_layout->render(window);
	}

	void set_is_hovered(bool hovered) const
	{
		m_layout->set_is_hovered(hovered);
	}

	void set_text(std::string ini_string) {
		m_layout->set_text(ini_string);
	}
};

