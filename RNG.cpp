
#include "RNG.hpp"

RNG* RNG::get_instance()
{
        if (instance == 0)
            instance = new RNG;
        return instance;
}

std::size_t RNG::get_integer()
{
    return std::size_t();
}

RNG::~RNG()
{
    if (instance) {
        if (instance->prd) {
            delete prd;
            prd = NULL;
        }
        delete instance;
        instance = NULL;
    }
}

RNG::RNG() {
    {
        if (!RNG::instance) {
            RNG::instance = new RNG;
            RNG::instance->prd = new std::random_device;
        }
    }
}