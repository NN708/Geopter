#ifndef GAP_H
#define GAP_H

#include <memory>
#include <string>

#include "Material/material.h"
#include "Material/material_library.h"


namespace geopter {


/**
 * @brief Gap container class
 *
 * The gap class represents the space between 2 surfaces. It contains the
    media definition for the space and a (z) displacement between the
    adjacent surfaces.

    The most common use case is an optical system with surfaces centered on a
    common axis. The Gap structure implements this case in the simplest manner.
    More complicated transformations between surfaces are implemented using
    transformations associated with the surfaces themselves.
 */
class Gap
{
public:
    Gap();
    Gap(double t, Material* m);
    ~Gap();

    inline double thi() const;
    inline void set_thi(double t);

    inline Material* material() const;

    void set_material(Material* m);

private:
    double thi_;
    Material* material_;
};


double Gap::thi() const
{
    return thi_;
}

void Gap::set_thi(double t)
{
    thi_ = t;
}


Material* Gap::material() const
{
    return material_;
}


} //namespace geopter

#endif // GAP_H
