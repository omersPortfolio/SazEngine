#pragma once

#include "glm/glm.hpp"

namespace Saz
{
	class FrameBuffer;
}


namespace component
{
	struct SpriteComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct FrameBufferComponent
	{
		Saz::Ref<Saz::FrameBuffer> FrameBuffer;
	};
}


