#include "projection.h"

using namespace BQ;

Projection::Projection()
{

}

Projection::Projection(float min, float max)
{
    this->min = min;
    this->max = max;
}

bool Projection::overlaps(const Projection & p)
{
    return(max > p.min && min < p.max);
}

