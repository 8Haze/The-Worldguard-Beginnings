#ifndef _TIMER_H_
#define _TIMER_H_

#include <SFML/System.hpp>

class Timer
{
private:

	float interval = 1.f;
	float timer = 0.f;
	bool ready = false;

public:

	Timer() = default;
	explicit Timer(float arg_interval);

	bool is_ready() const noexcept;

	void update(const sf::Time& dt);
	void restart() noexcept;

};

class Countdown
{
private:

	float starting_time = 1.f;
	float timer = 0.f;
	bool done = true;

public:

	Countdown() = default;
	explicit Countdown(float arg_starting_time);

	bool is_done() const noexcept;

	void update(const sf::Time& dt);
	void restart() noexcept;
	void stop() noexcept;

};

#endif // _TIMER_H_