#include "VehicleInformationWidget.h"
#include "VehiclePartsStatusWidget.h"
#include "VehicleStatusWidget.h"
#include <QHBoxLayout>

VehicleInformationWidget::VehicleInformationWidget(QWidget *parent) : QWidget(parent) {
    auto vehicleStatusWidget = new VehicleStatusWidget();
    vehicleStatusWidget->setTitle("整车各部件状态");

    auto vehiclePartsStatusWidget = new VehiclePartsStatusWidget();
    vehiclePartsStatusWidget->setTitle("各部件详情");

    auto layout = new QHBoxLayout(this);
    layout->addWidget(vehicleStatusWidget, 3);

    layout->addWidget(vehiclePartsStatusWidget, 2);
}
// Autonomous DrivingOperation
