#pragma once

namespace graphic
{
namespace ps
{

template
<
	class P,
	template<class> class Emitter,
	template<class> class LifespanController,
	template<class> class CountController,
	template<class> class Keeper
>
class ParticleSystem
{
public:
	typedef P particle_t;
	typedef Emitter<particle_t> emitter_t;
	typedef LifespanController<particle_t> lifespan_controller_t;
	typedef CountController<particle_t> count_controller_t;
	typedef Keeper<particle_t> keeper_t;

	emitter_t &get_emitter() { return emitter_; }
	lifespan_controller_t const &get_lifespan_controller() { return lifespan_controller_; }
	count_controller_t const &get_count_controller() { return count_controller_; }
	keeper_t &get_keeper() { return keeper_; }

private:
	emitter_t emitter_;
	lifespan_controller_t lifespan_controller_;
	count_controller_t count_controller_;
	keeper_t keeper_;
};

}
}
