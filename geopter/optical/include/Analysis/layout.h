#ifndef SYSTEM_LAYOUT_H
#define SYSTEM_LAYOUT_H

#include "Renderer/rgb.h"
#include "Sequential/ray.h"

namespace geopter{

class OpticalSystem;
class Renderer;
class Lens;
class Surface;
class DummyInterface;
class Stop;

/** Layout class support 2d drawing of optical system including optical elements, rays and other components. */
class Layout
{
public:
    Layout(OpticalSystem* sys, Renderer* renderer);
    ~Layout();

    void draw_elements();

    /** Draw chief ray, upper/lower marginal rays for one field */
    void draw_reference_rays();

    void draw_fan_rays(int nrd= 10);

    /** Draw a single ray */
    void draw_single_ray(const Ray& ray, const Rgb& color);

    void update();

private:
    OpticalSystem* opt_sys_;
    Renderer* renderer_;

    void draw_lens(Lens* lens, const Rgb& color);
    void draw_surface(Surface* srf, double max_y, const Rgb& color);
    void draw_stop(Stop* stop_elem, const Rgb &color);
    void draw_dummy_surface(DummyInterface *dummy_srf, const Rgb &color);
    void draw_flat(Surface* srf, double min_y, double max_y, const Rgb& color);
};

} //namespace geopter

#endif // LAYOUT_H
