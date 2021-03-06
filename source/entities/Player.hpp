/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

#include <functional>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>

#include <framework/Logger.hpp>

#include "BoxCollider.hpp"
#include "Damagable.hpp"
#include "AnimatedDrawable.hpp"
#include "Entity.hpp" 
#include "Settings.hpp"
#include "poweUps/PowerUp.hpp"
#include "gameUtil/MissileManager.hpp"

namespace kd
{
	class Player final :
		public AnimatedDrawable,
		public BoxCollider,
		public Entity,
		public Damagable
	{
	public:
		Player() :
			BoxCollider( this ),
			shootingKeys( sf::Keyboard::Left, sf::Keyboard::Right ),
			movementKeys( sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::Space ),
			movementForces( -25.0f, 25.0f, 50.0f ),
			baseHealth( 0 )
		{
			this->setSpriteToApply( &this->sprite );
		}


		int8_t GetBaseHealth()
		{
			return this->baseHealth;
		}

		void SetPosition( const sf::Vector2f& pos ) override;
		void SetTexture( std::weak_ptr<sf::Texture> texture ) override;
		void SetMovementKeys( const movementKeys_t& keys )
		{
			this->movementKeys = keys;
		}
		void SetMovementForces( const movementForces_t& forces )
		{
			this->movementForces = forces;
		}
		void SetBaseHealth( int8_t val )
		{
			if ( val < 0 )
				cgf::Logger::Log( "Setting Player Base Health to value smaller than 0 (" + std::to_string( val ) + ")", cgf::Logger::WARNING );

			this->baseHealth = val;
		}
		void AddBaseDamage( int8_t val )
		{
			this->baseHealth -= val;
		}
		template<class T>
		typename std::enable_if<std::is_base_of<PowerUp, T>::value>::type
		AddPowerUp()
		{
			T t( this );
			powerUpID_t tID = t.GetID();
			if ( tID == powerUpID_t::UNITIALIZED )
			{
				cgf::Logger::Log( "Failed to add PowerUp: trying to add PoweUP of unitialized ID", cgf::Logger::ERROR, cgf::Logger::CONSOLE );
				return;
			}

			for ( const auto& ptr : this->powerUps )
				if ( ptr->GetID() == tID )
				{
					cgf::Logger::Log( "Failed to add PowerUp: trying to add same PoweUP (" + std::string( SHOW_REAL_NAME<T>() ), cgf::Logger::ERROR, cgf::Logger::CONSOLE );
					return;
				}

			this->powerUps.push_back( std::make_unique<T>( t ) );
			cgf::Logger::Log( "Added PowerUp to Player: " + std::string( SHOW_REAL_NAME<T>() ) );
		}
		/*
			Call it after adding first frame - it makes their bounds to be collision rect bounds.
		*/
		void UpdateCollider();

		void Update( seconds_t dt ) override;
		void Draw( sf::RenderTarget& target ) override;

	private:
		shootingKeys_t shootingKeys;
		movementKeys_t movementKeys;
		movementForces_t movementForces;
		int8_t baseHealth;

		sf::Sprite sprite;

		std::vector<std::unique_ptr<PowerUp>> powerUps;

		void checkMovementEvents();
		void checkShootingEvents();
		void checkEvents();

		void updateMovement( seconds_t dt );

		void updatePowerUps( seconds_t dt );
		void drawPowerUps( sf::RenderTarget& target );
	};
}
