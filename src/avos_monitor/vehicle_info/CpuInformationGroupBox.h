#ifndef CPUINFORMATIONGROUPBOX_H
#define CPUINFORMATIONGROUPBOX_H

#include "SensorItem.h"
#include <QGroupBox>

class QLineEdit;

class CpuInformationGroupBox : public QGroupBox {
    Q_OBJECT
public:
    CpuInformationGroupBox(QWidget *parent = nullptr);
    void setStatus(const CpuStatus &status);

private:
    QLineEdit *m_perception{nullptr};

    QLineEdit *m_localization{nullptr};

    QLineEdit *m_decision{nullptr};

    QLineEdit *m_control{nullptr};

    QLineEdit *m_navigation{nullptr};
    QLineEdit *m_timestamp{nullptr};
};

#endif // CPUINFORMATIONGROUPBOX_H
