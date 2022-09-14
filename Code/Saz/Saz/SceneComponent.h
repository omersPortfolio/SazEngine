#pragma once

#include "Core/String.h"
#include "glm/glm.hpp"

enum class SceneState
{
	Editor,
	Play,
	Simulate,

};

namespace component
{
	struct LoadedSceneComponent
	{
		String Name = {};
		String Path = {};
		glm::vec2 SceneSize;
		bool IsFocused;
		bool IsHovered;
		SceneState SceneState;
	};

	struct NewSceneRequestOneFrameComponent
	{
		String Path = {};
	};

	struct LoadSceneRequestOneFrameComponent
	{
		String Path = {};
	};

	struct SaveSceneRequestOneFrameComponent
	{
		String Path = {};
	};

	struct SceneStateChangedOneFrameComponent
	{
		SceneState SceneState;
	};

	struct SceneEntityComponent
	{
		String ScenePath{};
	};
}
