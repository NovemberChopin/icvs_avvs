#ifndef SINGLESENSORINFORMATIONGROUPBOX_H
#define SINGLESENSORINFORMATIONGROUPBOX_H

#include "SensorItem.h"
#include <QGroupBox>

class QStatusIndicator;
class QLineEdit;
class InstallParameterGroupBox;

class SingleSensorInformationGroupBox : public QGroupBox {
    Q_OBJECT
public:
    explicit SingleSensorInformationGroupBox(QWidget *parent = nullptr);

    void setSensorData(const Amass::Sensor &sensor);

private:
    QStatusIndicator *m_indicator{nullptr};
    QLineEdit *m_timestamp{nullptr};
    InstallParameterGroupBox *m_installParameter{nullptr};
};

#endif // SINGLESENSORINFORMATIONGROUPBOX_H
