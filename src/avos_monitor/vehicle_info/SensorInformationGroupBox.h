#ifndef SENSORINFORMATIONGROUPBOX_H
#define SENSORINFORMATIONGROUPBOX_H

#include "SensorItem.h"
#include <QGroupBox>
#include <string>
#include <unordered_map>

class QComboBox;
class QStatusIndicator;
class QLineEdit;
class InstallParameterGroupBox;

class SensorInformationGroupBox : public QGroupBox {
    Q_OBJECT
public:
    using Sensors = std::map<Amass::Sensor::Type, std::string>;
    SensorInformationGroupBox(QWidget *parent = nullptr);
    void setSensors(const Sensors &sensors);
    void setSensorData(const Amass::Sensor &sensor);
signals:
    void currentSensorChanged(Amass::Sensor::Type type);

protected slots:
    void onSensorChanged(int index);

private:
    Sensors m_sensors;
    QComboBox *m_comboBox{nullptr};
    QStatusIndicator *m_indicator{nullptr};
    QLineEdit *m_timestamp{nullptr};
    InstallParameterGroupBox *m_installParameter{nullptr};
};

#endif // SENSORINFORMATIONGROUPBOX_H
