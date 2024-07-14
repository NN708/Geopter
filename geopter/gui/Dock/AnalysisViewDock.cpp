#include "AnalysisViewDock.h"

#include <iostream>
#include <sstream>

#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>


AnalysisViewDock::AnalysisViewDock(QString label, OpticalSystem* sys, bool textonly, QWidget *parent):
    ads::CDockWidget(label,parent),
    m_settingDlgPtr(nullptr),
    m_opticalSystem(sys),
    m_textOnly(textonly)
{
    this->setFeature(CDockWidget::DockWidgetDeleteOnClose, true);
    this->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    this->setMinimumSize(300,200);

    // Tab Widget
    m_tabWidget  = new QTabWidget;
    if(m_textOnly){
        m_textEdit   = new QTextEdit;
        m_tabWidget->addTab(m_textEdit, "Text");
    }else{
        m_customPlot = new QCustomPlot;
        m_textEdit   = new QTextEdit;
        m_tabWidget->addTab(m_textEdit, "Text");
        m_tabWidget->addTab(m_customPlot, "Plot");
        m_tabWidget->setCurrentIndex(1);
    }

    m_textEdit->setWordWrapMode(QTextOption::NoWrap);

    this->setWidget(m_tabWidget);

    // Tool Bar
    m_toolbar = new QToolBar(this);

    auto actionUpdate = m_toolbar->addAction("Update");
    actionUpdate->setIcon(QIcon(":/Resource/sync_icon.png"));
    auto actionSetting = m_toolbar->addAction("Setting");
    actionSetting->setIcon(QIcon(":/Resource/settings_icon.png"));
    auto actionSave = m_toolbar->addAction("Save");
    actionSave->setIcon(QIcon(":/Resource/save_icon.png"));
    auto actionCopy = m_toolbar->addAction("Copy");
    actionCopy->setIcon(QIcon(":/Resource/content_copy_icon.png"));


    m_toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setToolBar(m_toolbar);

    QObject::connect(actionUpdate,  SIGNAL(triggered()), this, SLOT(updateContent()));
    QObject::connect(actionSetting, SIGNAL(triggered()), this, SLOT(showSettingDlg()));
    QObject::connect(actionSave,    SIGNAL(triggered()), this, SLOT(saveToFile()));
    QObject::connect(actionCopy,    SIGNAL(triggered()), this, SLOT(copyToClipboard()));

    this->update();
}

AnalysisViewDock::~AnalysisViewDock()
{
    m_settingDlgPtr.reset();
    delete m_customPlot;
    delete m_toolbar;
}

void AnalysisViewDock::showSettingDlg()
{
    if(m_settingDlgPtr){
        if(m_settingDlgPtr->exec() == QDialog::Accepted){
            this->updateContent();
        }
    }
}


void AnalysisViewDock::saveToFile()
{
    int currentTab = m_tabWidget->currentIndex();

    if(currentTab == 0){
        QString filePath = QFileDialog::getSaveFileName(this, tr("Save to txt"),"",tr("Text file(*.txt);;All Files(*.*)"));
        if(filePath.isEmpty()){
            return;
        }

        QFile textFile(filePath);
        textFile.open(QIODevice::WriteOnly);

        QTextStream out(&textFile);
        out << m_textEdit->toPlainText() << Qt::endl;

        textFile.close();

    }else{
        QString filePath = QFileDialog::getSaveFileName(this, tr("Save to"),"",tr("PNG file(*.png);;All Files(*.*)"));
        if(filePath.isEmpty()){
            return;
        }

        m_customPlot->savePng(filePath);
    }

}

void AnalysisViewDock::copyToClipboard()
{
    QClipboard * clipboard = QApplication::clipboard();
    QPalette mypalette=this ->palette();
    mypalette.setColor(QPalette::Window,Qt::white);
    m_customPlot->setPalette(mypalette);
    //QPixmap pixmap= QPixmap::grabWidget(this);
    QPixmap pixmap = m_customPlot->toPixmap();
    clipboard->setPixmap(pixmap);
}

void AnalysisViewDock::updateContent()
{
    if(m_settingDlgPtr){
        m_settingDlgPtr->updateParentDockContent();
    }
}

