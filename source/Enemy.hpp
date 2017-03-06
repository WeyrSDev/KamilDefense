/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include <Logger.hpp>

#include "BoxCollider.hpp"
#include "Damagable.hpp"
#include "Entity.hpp"
#include "MissileManager.hpp"

namespace kd
{
	class Enemy :
		public Damagable,
		public BoxCollider,
		public Entity
	{
	public:
		Enemy() :
			BoxCollider( this ),
			shootTime( ENEMY_SHOOT_COOLDOWN )
		{
			// temporary
			this->velocity = { 250.0f, 0.0f };
		}

		void SetTexture( std::shared_ptr<sf::Texture> tex );
		void SetPosition( const sf::Vector2f& pos ) override;

		void Update( seconds_t dt ) override;
		void Draw( sf::RenderTarget& target );

	private:
		std::weak_ptr<sf::Texture> texture;
		sf::Sprite sprite;

		seconds_t shootTime;

	private:
		void updateMovement( seconds_t dt );
		void shoot();
	};
}
