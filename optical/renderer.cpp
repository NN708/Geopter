#include "renderer.h"

using namespace geopter;

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::update()
{

}

void Renderer::draw_text(std::string str, const Rgb& color)
{

}

void Renderer::draw_line(Eigen::Vector2d p1, Eigen::Vector2d p2, const Rgb& color)
{

}

void Renderer::draw_polyline(std::vector<Eigen::Vector2d>& pts, const Rgb& color)
{

}

void Renderer::draw_polyline(std::vector<double>& x, std::vector<double>& y, const Rgb& color)
{

}

void Renderer::set_x_axis_range(double xmin, double xmax)
{

}

void Renderer::set_y_axis_range(double ymin, double ymax)
{

}


void Renderer::set_x_axis_label(std::string label)
{

}

void Renderer::set_y_axis_label(std::string label)
{

}

void Renderer::set_aspect_ratio(double h_per_v)
{

}

void Renderer::set_grid_layout(int rows, int cols)
{

}

void Renderer::set_current_cell(int row, int col)
{

}

void Renderer::clear()
{

}
