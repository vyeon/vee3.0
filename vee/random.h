#ifndef _VEE_RANDOM_H_
#define _VEE_RANDOM_H_

#include <random>
#include <chrono>

namespace vee {

template <typename RandomEngine = std::mt19937_64,
    typename ResultTy = uint64_t,
    typename Distribution = std::uniform_int_distribution<ResultTy> >
class random_engine
{
public:
    using engine_t = RandomEngine;
    using result_t = ResultTy;
    using dist_t   = Distribution;
    
    random_engine(result_t min, result_t max):
        engine{ std::chrono::high_resolution_clock::now().time_since_epoch().count() },
        distribution{ min, max }
    {
        
    }

    ~random_engine()
    {
        
    }

    template <typename SeedTy>
    void seed(SeedTy&& seed_)
    {
        engine.seed(std::forward<SeedTy>(seed_));
    }

    result_t min()
    {
        return distribution.min();
    }

    result_t max()
    {
        return distribution.max();
    }

    std::mt19937_64 engine;
    std::uniform_int_distribution<uint64_t> distribution;
};

} // !namespace vee

#endif // !_VEE_RANDOM_H_