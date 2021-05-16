#include "transverse_ray_fan_dialog.h"
#include "ui_transverse_ray_fan_dialog.h"

#include <QLineEdit>
#include <QValidator>
#include "renderer_qcp.h"
#include "plot_view_dock.h"
using namespace geopter;

TransverseRayFanDialog::TransverseRayFanDialog(OpticalModel* opt_model, PlotViewDock *parent) :
    QDialog(parent),
    ui(new Ui::TransverseRayFanDialog),
    parentDock_(parent),
    opm_(opt_model)
{
    ui->setupUi(this);

    this->setWindowTitle("Transverse Ray Fan Setting");

    ui->lineEdit_Scale->setValidator(new QDoubleValidator(-1.0e+10, 1.0e+10, 2, this));
    ui->lineEdit_Scale->setText(QString::number(0.05));
    ui->lineEdit_NRD->setValidator(new QIntValidator(0, 100, this));
    ui->lineEdit_NRD->setText(QString::number(20));

    renderer_ = new RendererQCP(parentDock_->customPlot());

    QObject::connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));
}

TransverseRayFanDialog::~TransverseRayFanDialog()
{
    delete renderer_;
    delete ui;
}

void TransverseRayFanDialog::onAccept()
{
    this->plotRayFan();
    accept();
}

void TransverseRayFanDialog::plotRayFan()
{
    double scale = ui->lineEdit_Scale->text().toDouble();
    int nrd = ui->lineEdit_NRD->text().toInt();

    opm_->update_model();
    RayFan ray_fan(opm_, renderer_);

    ray_fan.plot(scale, nrd);
}
