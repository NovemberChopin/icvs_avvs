#ifndef BACKEND_H
#define BACKEND_H

#include "SensorItem.h"
#include <QObject>
#include <QSharedPointer>
#include <map>

class Backend : public QObject {
    Q_OBJECT
public:
    using Sensors = std::map<Amass::Sensor::Type, SensorItem>;
    using ChassisList = std::map<Amass::Chassis::Type, Amass::Chassis>;
    static Backend *instance();
    Sensors sensors() const;
    void updateSensor(const Amass::Sensor &sensor);
    void updateChassis(const Amass::Chassis &chassis);
    void updateTopic(TopicType type, const QString &topic);

signals:
    void sensorDataChanged(const Amass::Sensor &sensor);
    void chassisDataChanged(const Amass::Chassis &chassis);
    void cpuDataChanged(const CpuStatus &cpu);
    void initialized();
    void incommingTopic(TopicType type, const QString &topic);

protected:
    Backend(QObject *parent = nullptr);
    void initializeSensorItem();
    void initializeChassis();
    void timerEvent(QTimerEvent *event) final;


private:
    static QSharedPointer<Backend> m_instance;
    Sensors m_sensors;
    ChassisList m_chassis;
};

#endif // BACKEND_H
