#ifndef _RNG_H_
#define _RNG_H_

#include <array>
#include <random>
#include <functional>

namespace hct
{

    class RNG
    {
    private:

        std::mt19937 gen;
        std::uniform_int_distribution<> distr;

        RNG() : distr(0, std::numeric_limits<int>::max())
        {
            std::random_device rd;
            auto seed_data = std::array<int, std::mt19937::state_size> {};
            std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
            std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
            gen.seed(seq);
        }

    public:

        static RNG& instance()
        {
            static RNG rng;
            return rng;
        }

        int operator()(int min, int max)
        {
            return (distr(gen) % (max - min + 1)) + min;
        }

        int operator()()
        {
            return distr(gen);
        }

    };

}

#endif // _RNG_H_