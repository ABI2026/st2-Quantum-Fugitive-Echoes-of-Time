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
	Buttons();
	void set_layour(std::shared_ptr<ButtonLayout>& i_layout)
	{
		m_layout = i_layout;
	}


};

