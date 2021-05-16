#include "renderer_qcp.h"

#include <iostream>


RendererQCP::RendererQCP(QCustomPlot* customPlot)
{
    customPlot_ = customPlot;
    current_cell_index_ = 0;
}

RendererQCP::~RendererQCP()
{
    customPlot_ = nullptr;
}

void RendererQCP::clear()
{
    customPlot_->clearGraphs();
    customPlot_->clearItems();
    customPlot_->clearPlottables();
}

void RendererQCP::update()
{
    customPlot_->replot();
}

void RendererQCP::set_grid_layout(int rows, int cols)
{
    customPlot_->plotLayout()->clear();

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            QCPAxisRect *axisRect = new QCPAxisRect(customPlot_);
            axisRect->setObjectName("axis rect (" + QString::number(i) + "," + QString::number(j) + ")");
            customPlot_->plotLayout()->addElement(i, j, axisRect);
            axisRect->setLayer("axes");
        }
    }

}

void RendererQCP::set_current_cell(int row, int col)
{
    current_cell_index_ = customPlot_->plotLayout()->rowColToIndex(row, col);
}

void RendererQCP::draw_line(Eigen::Vector2d p1, Eigen::Vector2d p2, const Rgb& color)
{
    QCPAxisRect *axisRect = customPlot_->axisRect(current_cell_index_);

    //QCPCurve* line = new QCPCurve(_customPlot->xAxis, _customPlot->yAxis);
    QCPCurve* line = new QCPCurve(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));

    line->setPen(QPen(rgb_to_QColor(color)));

    QVector<double> x(2);
    QVector<double> y(2);

    x[0] = p1(0);
    x[1] = p2(0);
    y[0] = p1(1);
    y[1] = p2(1);

    line->setData(x,y);
    line->setVisible(true);
}

void RendererQCP::draw_polyline(std::vector<Eigen::Vector2d> &pts, const Rgb& color)
{
    QCPAxisRect *axisRect = customPlot_->axisRect(current_cell_index_);
    //QCPCurve* polyline = new QCPCurve(_customPlot->xAxis, _customPlot->yAxis);
    QCPCurve* polyline = new QCPCurve(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));

    polyline->setPen(QPen(rgb_to_QColor(color)));

    int pointCount = pts.size();
    QVector<QCPCurveData> curveData(pointCount);

    for(int i = 0; i < pointCount; i++){
        curveData[i] = QCPCurveData(i, pts[i](0), pts[i](1));
    }

    polyline->data()->set(curveData, true);
    polyline->setVisible(true);

}

void RendererQCP::draw_polyline(std::vector<double> &x, std::vector<double> &y, const Rgb& color)
{
    int pointCount = x.size();
    QVector<QCPCurveData> curveData(pointCount);

    for(int i = 0; i < pointCount; i++)
    {
        curveData[i] = QCPCurveData(i, x[i], y[i]);
    }

    QCPAxisRect *axisRect = customPlot_->axisRect(current_cell_index_);
    std::cout << "axisRect= " << axisRect->objectName().toStdString() << std::endl;
    //QCPCurve* polyline = new QCPCurve(_customPlot->xAxis, _customPlot->yAxis);
    QCPCurve* polyline = new QCPCurve(axisRect->axis(QCPAxis::atBottom), axisRect->axis(QCPAxis::atLeft));

    polyline->setPen(QPen(rgb_to_QColor(color)));

    polyline->data()->clear();
    polyline->data()->set(curveData, true);
    polyline->setVisible(true);
}

void RendererQCP::set_x_axis_range(double xmin, double xmax)
{
    //_customPlot->xAxis->setRange(xmin, xmax);
    customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atBottom)->setRange(xmin, xmax);
}

void RendererQCP::set_y_axis_range(double ymin, double ymax)
{
    //_customPlot->yAxis->setRange(ymin, ymax);
    customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atLeft)->setRange(ymin, ymax);
}


void RendererQCP::set_x_axis_label(std::string label)
{
    QString xLabel = QString().fromStdString(label);
    customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atBottom)->setLabel(xLabel);
}

void RendererQCP::set_y_axis_label(std::string label)
{
    QString yLabel = QString().fromStdString(label);
    customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atLeft)->setLabel(yLabel);
}

void RendererQCP::set_aspect_ratio(double h_per_v)
{
    //_customPlot->yAxis->setScaleRatio(_customPlot->xAxis, h_per_v);
    QCPAxis *xAxis = customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atBottom);
    customPlot_->axisRect(current_cell_index_)->axis(QCPAxis::atLeft)->setScaleRatio(xAxis, h_per_v);
}


