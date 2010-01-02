#pragma once

#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>

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

	ParticleSystem():
		max_nparticles_(1000)
	{
	}

	emitter_t &get_emitter() { return emitter_; }
	lifespan_controller_t const &get_lifespan_controller() { return lifespan_controller_; }
	count_controller_t const &get_count_controller() { return count_controller_; }
	keeper_t &get_keeper() { return keeper_; }

	void set_max_nparticles(size_t max_nparticles) { max_nparticles_ = max_nparticles; }

	void move(math::scalar dt)
	{
		size_t nparticles_to_emit = count_controller_.move(dt);

		for (size_t i = 0; i < nparticles_to_emit && max_nparticles_ < keeper_.get_particles_count(); i++)
		{
			particle_t particle;
			emitter_.init(particle);
			lifespan_controller_.init(particle);
			keeper_.add_particle(particle);
		}

		keeper_.move(dt);
	}

	static void bind(lua_State *L, char const *classname)
	{
		using namespace luabind;

		module_(L, "graphic.ps")
		[
			class_<ParticleSystem, boost::shared_ptr<ParticleSystem> >(classname)
			.def(constructor<>())
			.def("move", &ParticleSystem::move)
		];
	}

private:
	size_t max_nparticles_;

	emitter_t emitter_;
	lifespan_controller_t lifespan_controller_;
	count_controller_t count_controller_;
	keeper_t keeper_;
};

}
}
