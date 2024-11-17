#pragma once
#include <memory>
#include <stack>

#include "Eventsystem.h"
#include "Layer.h"

class Layer;

class LayerManager
{
public:
	std::shared_ptr<Layer>& get_top();

	void push_layer(const std::shared_ptr<Layer>& layer);

	void pop_layer();

	[[nodiscard]] bool is_empty() const;

	void close_till_layer(LayerID targetLayerID);
private:
	std::stack<std::shared_ptr<Layer>> m_layers;
};

