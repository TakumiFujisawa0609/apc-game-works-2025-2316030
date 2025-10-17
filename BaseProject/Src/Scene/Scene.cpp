#include <DxLib.h>
#include "../Application.h"
#include "../Manager/ResourceManager.h"
#include "Scene.h"

Scene::Scene(SceneController& controller)
	:
	controller_(controller),
	resMng_(ResourceManager::GetInstance())
{
}

int Scene::GetRenderTarget(void)
{
	return newRT_;
}

void Scene::SetRenderTarget(int handle)
{
	newRT_ = handle;
}
