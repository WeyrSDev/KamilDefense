/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include "Damagable.hpp"

namespace kd
{
	void Damagable::SetHealth( uint8_t val, bool ignoreLimit )
	{
		if ( val > kd::settings_t::GetInstance().GAMEPLAY.MAX_HEALTH && !ignoreLimit )
			this->health = kd::settings_t::GetInstance().GAMEPLAY.MAX_HEALTH;
		else
			this->health = val;
	}

	void Damagable::SetArmor( uint8_t val, bool ignoreLimit )
	{
		if ( val > kd::settings_t::GetInstance().GAMEPLAY.MAX_ARMOR && !ignoreLimit )
			this->armor = kd::settings_t::GetInstance().GAMEPLAY.MAX_ARMOR;
		else
			this->armor = val;
	}

	void Damagable::AddDamage( uint8_t val )
	{
		if ( this->damageBlockTime == 0 )
		{
			this->pendingDamage = val;
			this->damageBlockTime = kd::settings_t::GetInstance().GAMEPLAY.DAMAGE_BLOCK_TIME;
		}
	}

	void Damagable::updateDamage( seconds_t dt )
	{
		if ( this->damageBlockTime > 0.0f )
			this->damageBlockTime -= dt;
		if ( this->damageBlockTime <= 0.0f )
			this->damageBlockTime = 0.0f;

		this->addPendingDamage();
		this->updateLifeState();
	}

	void Damagable::updateLifeState()
	{
		if ( this->health > 0 )
			this->lifeState = lifeState_t::ALIVE;
		else if ( this->health <= 0 && this->lifeState != lifeState_t::DEAD )
			this->lifeState = lifeState_t::DYING;
	}

	void Damagable::addPendingDamage()
	{
		if ( this->pendingDamage > this->armor )
		{
			this->pendingDamage -= this->armor;
			this->armor = 0;
		}
		else
		{
			this->armor -= this->pendingDamage;
			this->pendingDamage = 0;
		}

		if ( this->pendingDamage )
		{
			if ( this->pendingDamage > this->health )
				this->health = 0;
			else
				this->health -= this->pendingDamage;
		}

		this->pendingDamage = 0;
	}
}