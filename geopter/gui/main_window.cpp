#include <iostream>
#include <limits>
#include <sstream>
#include <fstream>

#include "main_window.h"
#include "./ui_main_window.h"

#include "lens_data_manager_dock.h"
#include "text_view_dock.h"
#include "plot_view_dock.h"
#include "general_configuration_dialog.h"
#include "layout_dialog.h"
#include "paraxial_trace_dialog.h"
#include "single_ray_trace_dialog.h"
#include "transverse_ray_fan_dialog.h"
#include "longitudinal_setting_dialog.h"
#include "renderer_qcp.h"

#include "qdebugstream.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QTextEdit>
#include <QDebug>

using namespace ads;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Geopter");

    // File menu
    QObject::connect(ui->actionNew,  SIGNAL(triggered()), this, SLOT(newFile()));
    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveAs()));
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    QObject::connect(ui->actionConfig, SIGNAL(triggered()), this, SLOT(showConfig()));

    // Analysis menu
    QObject::connect(ui->actionReport,             SIGNAL(triggered()), this, SLOT(showReport()));
    QObject::connect(ui->actionLayout,             SIGNAL(triggered()), this, SLOT(showLayout()));
    QObject::connect(ui->actionFirst_Order_Data,   SIGNAL(triggered()), this, SLOT(showFirstOrderData()));
    QObject::connect(ui->actionSingle_Ray_Trace,   SIGNAL(triggered()), this, SLOT(showSingleRayTrace()));
    QObject::connect(ui->actionParaxial_Ray_Trace, SIGNAL(triggered()), this, SLOT(showParaxialRayTrace()));
    QObject::connect(ui->actionTransverse_Ray_Fan, SIGNAL(triggered()), this, SLOT(showRayFan()));
    QObject::connect(ui->actionLongitudinal_Aberration, SIGNAL(triggered()), this, SLOT(showLongitudinal()));

    // Help menu
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));

    // Lens Data Manager
    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);


    // create optical system
    opt_sys_ = std::make_shared<OpticalSystem>();

    agf_dir_path_ = QApplication::applicationDirPath().toStdString() + "/AGF";
    loadAgfsFromDir(QString().fromStdString(agf_dir_path_));

    opt_sys_->create_minimum_system();

    // set lens data manager as central dock
    dockManager_ = new CDockManager(this);
    lens_data_manager_ = new LensDataManagerDock(opt_sys_, "Lens Data");
    lens_data_manager_->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    auto* CentralDockArea = dockManager_->setCentralWidget(lens_data_manager_);
    CentralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);

    lens_data_manager_->initialize();
    lens_data_manager_->setOpticalSystem(opt_sys_);
    lens_data_manager_->syncTableWithModel();
}

MainWindow::~MainWindow()
{
    opt_sys_.reset();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Delete dock manager here to delete all floating widgets. This ensures
    // that all top level windows of the dock manager are properly closed
    dockManager_->deleteLater();
    QMainWindow::closeEvent(event);
}


void MainWindow::loadAgfsFromDir(QString agfDir)
{
    std::vector< std::string > agf_paths;

    QStringList nameFilters;
    nameFilters.append("*.agf");
    nameFilters.append("*.AGF");

    QDir dir(agfDir);
    QStringList entry = dir.entryList(nameFilters, QDir::Files);
    for (QString file : entry) {
        qDebug() << dir.filePath(file);
        agf_paths.push_back(dir.filePath(file).toStdString());
    }

    bool ret = opt_sys_->material_lib()->load_agf_files(agf_paths);

}


/*********************************************************************************************************************************
 *
 * File menu
 *
 * ********************************************************************************************************************************/
void MainWindow::newFile()
{
    opt_sys_.reset();
    opt_sys_ = std::make_shared<OpticalSystem>();

    opt_sys_->create_minimum_system();
    opt_sys_->update_model();

    lens_data_manager_->setOpticalSystem(opt_sys_);
    lens_data_manager_->syncTableWithModel();

    QMessageBox::information(this,tr("Info"), tr("Created new lens"));
}

void MainWindow::saveAs()
{
    // open file selection dialog
    QString filePath = QFileDialog::getSaveFileName(this,
                                                          tr("select JSON"),
                                                          QApplication::applicationDirPath(),
                                                          tr("JSON files(*.json);;All Files(*.*)"));
    if(filePath.isEmpty()){
        return;
    }

    std::string json_path = filePath.toStdString();

    FileIO::save_to_json(*opt_sys_, json_path);

    std::ostringstream oss;
    opt_sys_->optical_assembly()->print(oss);
    std::fstream fout;
    fout.open("assembly.txt", std::ios::out);
    fout << oss.str() << std::endl;
    fout.close();


    QMessageBox::information(this,tr("Info"), tr("Saved to JSON file"));
}

void MainWindow::openFile()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(this,
                                                          tr("Open JSON"),
                                                          QApplication::applicationDirPath(),
                                                          tr("JSON files(*.json)"));
    if(filePaths.empty()){
        //QMessageBox::warning(this,tr("Canceled"), tr("Canceled"));
        return;
    }

    std::string json_path = filePaths.first().toStdString();
    FileIO::load_from_json(*opt_sys_,json_path);

    opt_sys_->update_model();
    //opt_sys_->update_model(); // FIXME: must be twice updated

    lens_data_manager_->setOpticalSystem(opt_sys_);
    lens_data_manager_->syncTableWithModel();

    QMessageBox::information(this,tr("Info"), tr("OpticalSystem newly loaded"));
}

void MainWindow::showConfig()
{
    GeneralConfigurationDialog* dlg = new GeneralConfigurationDialog(opt_sys_, this);
    dlg->exec();
    delete dlg;
}

/*********************************************************************************************************************************
 *
 * Analysis menu
 *
 * ********************************************************************************************************************************/
void MainWindow::showReport()
{
    TextViewDock *dock = new TextViewDock("Report");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    std::ostringstream oss;
    oss << "Title: " << opt_sys_->title() << std::endl;
    oss << std::endl;

    opt_sys_->optical_spec()->print(oss);
    oss << std::endl;

    oss << "Assembly:" << std::endl;
    opt_sys_->optical_assembly()->print(oss);

    // This analysis does not have setting dialog so just shows the result.
    dock->setStringStreamToText(oss);
}


void MainWindow::showLayout()
{
    PlotViewDock *dock = new PlotViewDock("Layout");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<LayoutDialog>(opt_sys_.get(), dock));
    dock->showSettingDlg();
}


void MainWindow::showFirstOrderData()
{
    std::ostringstream ss;

    opt_sys_->update_model();
    opt_sys_->parax_data()->first_order_data().print(ss);

    TextViewDock *dock = new TextViewDock("First Order Data");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    // This analysis does not have setting dialog so just shows the result.
    dock->setStringStreamToText(ss);
}


void MainWindow::showParaxialRayTrace()
{
    opt_sys_->parax_data()->update_model();

    TextViewDock *dock = new TextViewDock("Paraxial Ray Trace");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<ParaxialTraceDialog>(opt_sys_.get(), dock));
    dock->showSettingDlg();

}

void MainWindow::showSingleRayTrace()
{
    TextViewDock *dock = new TextViewDock("Single Ray Trace");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<SingleRayTraceDialog>(opt_sys_.get(), dock));
    dock->showSettingDlg();
}


void MainWindow::showRayFan()
{
    PlotViewDock *dock = new PlotViewDock("Transverse Ray Fan");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<TransverseRayFanDialog>(opt_sys_.get(), dock));
    dock->showSettingDlg();   
}

void MainWindow::showLongitudinal()
{
    PlotViewDock *dock = new PlotViewDock("Longitudinal Aberration");
    dockManager_->addDockWidgetFloating(dock);
    dock->resize(300,200);

    dock->possessDlg(std::make_unique<LongitudinalSettingDialog>(opt_sys_.get(), dock));
    dock->showSettingDlg();
}



/*********************************************************************************************************************************
 *
 * Tool menu
 *
 * ********************************************************************************************************************************/
void MainWindow::showDebugStream()
{

}



/*********************************************************************************************************************************
 *
 * Help menu
 *
 * ********************************************************************************************************************************/
void MainWindow::showAbout()
{
    // TODO: Dialog to show description and licence notice should be implemented

    QMessageBox::information(this,tr("About"), tr("Geopter v0.1.0"));
}
