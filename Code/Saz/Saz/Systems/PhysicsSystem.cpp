#include "SazPCH.h"
#include "PhysicsSystem.h"

#include "Saz/ProjectSettings/PhysicsSettings.h"
#include "Saz/PhysicsComponents.h"
#include "Saz/TransformComponent.h"
#include "Saz/SceneComponent.h"

#include <box2d/box2d.h>
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"

namespace
{
	constexpr int32_t velocityIterations = 6;
	constexpr int32_t positionIterations = 2;

	b2BodyType ConvertSazPhysicsTypeToBox2D(component::Rigidbody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case component::Rigidbody2DComponent::BodyType::Static:
			return b2_staticBody;
		case component::Rigidbody2DComponent::BodyType::Dynamic:
			return b2_dynamicBody;
		case component::Rigidbody2DComponent::BodyType::Kinematic:
			return b2_kinematicBody;
		}

		SAZ_CORE_ASSERT(false, "Unknown physics type!");
		return b2_staticBody;
	}
}

namespace ecs
{
	void PhysicsSystem::Init()
	{
		m_World->m_Registry.on_update<component::SceneStateChangeRequestOneFrameComponent>().connect<&PhysicsSystem::OnSceneStateChanged>(this);
	}

	void PhysicsSystem::Update(const Saz::GameTime& gameTime)
	{
		auto& registry = m_World->m_Registry;

		auto sceneStateChangeView = m_World->GetAllEntitiesWith<component::SceneStateChangeRequestOneFrameComponent>();
		for (auto& entity : sceneStateChangeView)
		{
			auto& sceneStateChangedComp = m_World->GetComponent<component::SceneStateChangeRequestOneFrameComponent>(entity);
			if (sceneStateChangedComp.SceneState == SceneState::Play)
				OnRuntimeStart();
			else if (sceneStateChangedComp.SceneState == SceneState::Editor)
				OnRuntimeStop();
		}

		if (m_World->GetSingleComponent<component::LoadedSceneComponent>().SceneState != SceneState::Play)
			return;

		m_PhysicsWorld->Step(gameTime.GetDeltaTime(), velocityIterations, positionIterations);

		auto view = m_World->GetAllEntitiesWith<component::Rigidbody2DComponent, component::TransformComponent>();
		for (auto& entity : view)
		{
			auto& transform = m_World->GetComponent<component::TransformComponent>(entity);
			auto& rigidbody2D = m_World->GetComponent<component::Rigidbody2DComponent>(entity);

			b2Body* body = (b2Body*)rigidbody2D.RuntimeBody;
			const auto& position = body->GetPosition();
			transform.Position.x = position.x;
			transform.Position.y = position.y;
			transform.Rotation.z = body->GetAngle();
		}
	}

	void PhysicsSystem::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ PhysicsSettings::Gravity[0], PhysicsSettings::Gravity[1] });
		auto view = m_World->GetAllEntitiesWith<component::Rigidbody2DComponent>();
		for (auto entity : view)
		{
			auto& transform = m_World->GetComponent<component::TransformComponent>(entity);
			auto& rigidbody2D = m_World->GetComponent<component::Rigidbody2DComponent>(entity);

			b2BodyDef bodyDef;
			bodyDef.type = ConvertSazPhysicsTypeToBox2D(rigidbody2D.Type);
			bodyDef.position.Set(transform.Position.x, transform.Position.y);
			bodyDef.angle = transform.Rotation.z;
			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rigidbody2D.FixedRotation);
			rigidbody2D.RuntimeBody = body;

			if (m_World->HasComponent<component::BoxCollider2DComponent>(entity))
			{
				auto& boxCollider = m_World->GetComponent<component::BoxCollider2DComponent>(entity);

				b2PolygonShape boxShape;
				boxShape.SetAsBox(transform.Scale.x * boxCollider.Size.x, transform.Scale.y * boxCollider.Size.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = boxCollider.Density;
				fixtureDef.friction = boxCollider.Friction;
				fixtureDef.restitution = boxCollider.Restitution;
				fixtureDef.restitutionThreshold = boxCollider.RestitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void PhysicsSystem::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void PhysicsSystem::OnSceneStateChanged(entt::registry& registry, entt::entity entity)
	{
		component::SceneStateChangeRequestOneFrameComponent& comp = m_World->GetComponent<component::SceneStateChangeRequestOneFrameComponent>(entity);
		if (comp.SceneState == SceneState::Play)
			OnRuntimeStart();
		else if (comp.SceneState == SceneState::Editor)
			OnRuntimeStop();
	}
}