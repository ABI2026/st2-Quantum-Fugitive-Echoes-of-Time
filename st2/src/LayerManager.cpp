#include "LayerManager.h"

std::shared_ptr<Layer>& LayerManager::get_top()
{
#ifndef DIST
	if (m_layers.empty())
	{
		throw std::runtime_error("Layer stack is empty, cannot get top layer.");
	}
#endif
	return m_layers.top();
}

void LayerManager::push_layer(const std::shared_ptr<Layer>& layer)
{
	m_layers.push(layer);
}

void LayerManager::pop_layer()
{
	if (!m_layers.empty()) 
	{
		m_layers.top()->on_close();
		m_layers.pop();
	}
}

bool LayerManager::is_empty() const
{
	return m_layers.empty();
}

void LayerManager::close_till_layer(LayerID targetLayerID)
{
	while (!m_layers.empty())
	{
		Layer* topLayer = m_layers.top().get();

		if (topLayer->get_layer_id() == targetLayerID) 
		{
			break;  
		}

		topLayer->on_close();
		m_layers.pop();
	}
}
