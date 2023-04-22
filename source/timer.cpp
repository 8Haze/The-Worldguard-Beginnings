#include "timer.h"

// ----------------------------------------------------------------
// Timer
// ----------------------------------------------------------------

Timer::Timer(float arg_interval) : interval(arg_interval) {}

bool Timer::is_ready() const noexcept 
{ 
	return ready; 
}

void Timer::update(const sf::Time & dt)
{
	timer += dt.asSeconds();
	if (timer >= interval)
	{
		ready = true;
		timer = 0.f;
	}
	else
		ready = false;
}

void Timer::restart() noexcept
{
	timer = 0.f;
	ready = false;
}

// ----------------------------------------------------------------
// Countdown
// ----------------------------------------------------------------

Countdown::Countdown(float arg_starting_time) : starting_time(arg_starting_time) {}

bool Countdown::is_done() const noexcept
{ 
	return done;
}

void Countdown::update(const sf::Time & dt)
{
	if (done)
		return;

	timer -= dt.asSeconds();
	if (timer <= 0.f)
	{
		timer = 0.f;
		done = true;
	}
	else
		done = false;
}

void Countdown::restart() noexcept
{
	timer = starting_time;
	done = false;
}

void Countdown::stop() noexcept
{
	timer = 0.f;
	done = true;
}