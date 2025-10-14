#include "../Manager/ResourceManager.h"
#include "Scene.h"

Scene::Scene(SceneController& controller)
	:
	controller_(controller),
	resMng_(ResourceManager::GetInstance())
{
}
