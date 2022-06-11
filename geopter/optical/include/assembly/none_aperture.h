#ifndef GEOPTER_NONE_APERTURE_H
#define GEOPTER_NONE_APERTURE_H

#include <string>

namespace geopter {

class NoneAperture
{
public:
    NoneAperture() : x_dimension_(0.0), y_dimension_(0.0), x_offset_(0.0), y_offset_(0.0), rotation_(0.0){}

    std::string shape_name() const{
        return "None";
    }

    double max_dimension() const{
        return 0.0;
    }

    bool point_inside(double x, double y) const{
        return true;
    }

protected:
    double x_dimension_;
    double y_dimension_;
    double x_offset_;
    double y_offset_;
    double rotation_;
};


} // namespace


#endif //GEOPTER_NONE_APERTURE_H
