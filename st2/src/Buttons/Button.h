#pragma once
#include <memory>

#include "ButtonBehaviour.h"
#include "ButtonLayout.h"

class Button
{
public:
	Button()
		: m_layout(std::make_shared<EmptyLayout>()), m_button_behaviour(std::make_shared<EmptyBehaviour>())
	{}

	void set_layout(const std::shared_ptr<ButtonLayout>& i_layout)
	{
		m_layout = i_layout;
	}

	void set_behaviour(const std::shared_ptr<ButtonBehaviour>& i_behaviour)
	{
		m_button_behaviour = i_behaviour;
	}

	std::shared_ptr<ButtonLayout> get_layout() const
	{
		return m_layout;
	}

	std::shared_ptr<ButtonBehaviour> get_behaviour() const
	{
		return m_button_behaviour;
	}


	[[nodiscard]] bool is_clicked() const
	{
		return m_layout->is_clicked();
	}

	[[nodiscard]] bool is_hovered() const
	{
		return m_layout->is_hovered();
	}

	void update(const sf::Vector2f& mouse_position, const bool mouse_pressed) const
	{
		m_layout->update(mouse_position,mouse_pressed);
	}

	[[nodiscard]] bool on_click(std::shared_ptr<LayerManager>& layer_manager, std::shared_ptr<Soundsystem>& soundsystem,
		sf::RenderWindow& window) const
	{

		return m_button_behaviour->on_click(layer_manager, soundsystem, window);
	}

	void set_position(const sf::Vector2f& position) const
	{
		m_layout->set_position(position);
	}

	void render(sf::RenderWindow& window) const
	{
		m_layout->render(window);
	}

	void set_is_hovered(const bool hovered) const
	{
		m_layout->set_is_hovered(hovered);
	}

	void set_text(const std::string& i_string) const
	{
		m_layout->set_text(i_string);
	}

private:
	std::shared_ptr<ButtonLayout> m_layout;
	std::shared_ptr<ButtonBehaviour> m_button_behaviour;
};

