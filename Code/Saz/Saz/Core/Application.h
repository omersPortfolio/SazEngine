#pragma once

#include <Saz/Core/EntityWorld.h>
#include <Saz/Core/LayerStack.h>
#include <Saz/Core/WindowBase.h>
#include "Saz/Events/ApplicationEvent.h"
#include "Saz/ImGui/ImGuiLayer.h"


namespace Saz
{
	class ResourceManager;
	class GameTime;
	class OrthographicCamera;
}

namespace imgui
{
	class Log;
}

namespace Saz::imgui
{
	class Log;
}

namespace Saz
{

	class SAZ_API Application
	{

	public:
		Application();
		virtual ~Application();

		void Run();

		inline static Application& Get() { return *s_Instance; }
		inline WindowBase& GetWindow() { return *m_Window; }
		const ecs::EntityWorld& GetWorld();

	protected:
		virtual void Init();
		virtual void PostInit();
		virtual void Register();
		virtual void Destroy();
		virtual void Update(const Saz::GameTime& gameTime);
		void OnEvent(Event& e);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		float m_LastFrameTime = 0.0f;

	protected:
		ecs::EntityWorld m_EntityWorld;
		std::unique_ptr<WindowBase> m_Window = nullptr;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;

		bool m_Running = true;
		bool m_Minimized = false;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
