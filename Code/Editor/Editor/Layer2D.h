#pragma once

#include "Saz/Core/Layer.h"
#include "Saz/Events/Event.h"
#include "glm/ext/vector_float3.hpp"
#include "Saz/OrthographicCameraController.h"
#include "Saz/Rendering/Shader.h"
#include "Saz/Rendering/VertexArray.h"
#include "Saz/Rendering/Texture.h"

class Layer2D : public Saz::Layer
{
public:
	Layer2D();
	virtual ~Layer2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(const Saz::GameTime& gameTime) override;
	void OnEvent(Saz::Event& event) override;
	void OnImGuiRender() override;
private:
	Saz::OrthographicCameraController m_CameraController;

	Saz::Ref<Saz::VertexArray> m_SquareVA;
	Saz::Ref<Saz::Shader> m_Shader;
	Saz::Ref<Saz::Texture2D> m_Texture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};