#pragma once

namespace graphic
{
namespace ps
{

template<class P, class<class> Emitter, class<class> Keeper>
class particle_system
{
public:
	typedef P particle_t;
	typedef Emitter<particle_t> emitter_t;
	typedef Keeper<particle_t> keeper_t;

	emitter_t &get_emitter() { return emitter_; }
	keeper_t &get_keeper() { return keeper_; }

private:
	emitter_t emitter_;
	keeper_t keeper_;
};

}
}
