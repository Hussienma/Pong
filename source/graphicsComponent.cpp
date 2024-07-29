#include "GraphicsComponent.hpp"
#include "GameObject.hpp"
#include "Game.hpp"

void GraphicsComponent::update(GameObject& obj){
	if(!texture)
		Game::window->render(obj.position);
	else
		Game::window->render(texture, obj.position);
}
