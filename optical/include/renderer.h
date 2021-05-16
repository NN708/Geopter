#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "Eigen/Core"
#include "rgb.h"


namespace geopter {

class Renderer
{
public:
    Renderer();
    virtual ~Renderer();

    virtual void set_grid_layout(int rows, int cols);
    virtual void set_current_cell(int row, int col);

    virtual void draw_line(Eigen::Vector2d p1, Eigen::Vector2d p2, const Rgb& color);
    virtual void draw_polyline(std::vector<Eigen::Vector2d>& pts, const Rgb& color);
    virtual void draw_polyline(std::vector<double>& x, std::vector<double>& y, const Rgb& color);
    virtual void draw_text(std::string str, const Rgb& color);

    virtual void set_x_axis_range(double xmin, double xmax);
    virtual void set_y_axis_range(double ymin, double ymax);

    virtual void set_x_axis_label(std::string label);
    virtual void set_y_axis_label(std::string label);


    /**
     * @brief Set aspect ratio h:v
     * @param ratio aspect ratio h/v
     */
    virtual void set_aspect_ratio(double h_per_v);

    virtual void update();

    virtual void clear();

protected:
    int current_cell_index_;
};

} //namespace geopter

#endif // RENDERER_H
