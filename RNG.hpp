//random number generator RNG.hpp
//author : AntiMoron 
//Date   : 2014-11-27
#ifndef __RNG_HPP__
#define __RNG_HPP__

#include <memory>
#include <random>

class RNG {
public:
    static RNG& get_instance();
    //const static std::size_t max_rand = std::random_device::max();
    std::size_t get_integer();
    ~RNG();

private:
    static RNG* instance;
    std::random_device* prd=NULL;
    RNG();
};
#define RAND_INT(x)  (RNG::get_instance().get_integer() % x)

#endif