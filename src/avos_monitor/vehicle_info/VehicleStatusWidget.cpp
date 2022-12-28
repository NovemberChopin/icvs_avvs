#include "VehicleStatusWidget.h"
#include "Backend.h"
#include "CarFrontSketchItem.h"
#include "CarLeftSketchItem.h"
#include "CarTopSketchItem.h"
#include <QTableWidget>
#include <QVBoxLayout>

VehicleStatusWidget::VehicleStatusWidget(QWidget *parent) : QGroupBox(parent) {

    auto backend = Backend::instance();

    m_frontView = new CarFrontSketchItem();
    // m_frontView->setCarPixmap(":/images/front.png", 1735, 1899);
    m_frontView->setCarPixmap(":/images/front.png", 1830, 2050);
    m_frontView->setOffset(0, -1000);
    m_frontView->setRange(-2200, 2200, -2200, 2200, 200);

    m_leftView = new CarLeftSketchItem();
    // m_leftView->setCarPixmap(":/images/left.png", 4000, 1906);
    m_leftView->setCarPixmap(":/images/left.png", 3970, 2050);
    m_leftView->setOffset(-1437, -1037);
    m_leftView->setRange(-800, 3600, -2200, 2200, 200);

    m_topView = new CarTopSketchItem();
    // m_topView->setCarPixmap(":/images/top.png", 4000, 1735);
    m_topView->setCarPixmap(":/images/top.png", 3970, 1830);
    m_topView->setOffset(-1500, 0);
    m_topView->setRange(-800, 3600, -2200, 2200, 200);

    auto tabWidget = new QTabWidget();
    tabWidget->addTab(m_frontView, "正视图"); // y z
    tabWidget->addTab(m_leftView, "左视图");  // x z
    tabWidget->addTab(m_topView, "俯视图");   // x y

    tabWidget->setStyleSheet("QTabWidget{color:white;background-color:#555555;font-size:14px;}\
    QTabBar::tab{border-bottom-color: #C2C7CB;border-top-left-radius: 2px; border-top-right-radius: 2px;\
    max-width: 90px; min-width:90px; min-height:30px;padding: 0px;font:16px Times New Roman;}\
    QTabBar::tab:selected{margin-left: 5;margin-right: 5;color: white;font-weight:bold;\
    background-color: qlineargradient(spread:pad, x1:0.0618557, y1:1, x2:0.948454, y2:0.057, stop:0 rgba(50, 50, 50, 255), stop:0.618557 rgba(100, 100, 100, 255));}\
    QTabBar::tab:!selected{color: rgb(200, 200, 200);margin-left: 5; margin-right: 0;\
    background-color: qlineargradient(spread:pad, x1:0.0618557, y1:1, x2:0.948454, y2:0.057, stop:0 rgba(50, 50, 50, 255), stop:0.618557 rgba(100, 100, 100, 255));}\
    QTabBar::tab:hover:!selected {color: white;margin-left: 5; margin-right: 5;}");


    auto layout = new QVBoxLayout(this);
    layout->addWidget(tabWidget);

    connect(backend, &Backend::sensorDataChanged, m_frontView, &CarFrontSketchItem::updateSensor);
    connect(backend, &Backend::sensorDataChanged, m_leftView, &CarLeftSketchItem::updateSensor);
    connect(backend, &Backend::sensorDataChanged, m_topView, &CarTopSketchItem::updateSensor);
    connect(backend, &Backend::initialized, this, [this]() {
        auto backend = Backend::instance();
        auto sensors = backend->sensors();
        m_frontView->setSensors(sensors);
        m_leftView->setSensors(sensors);
        m_topView->setSensors(sensors);
    });
}
