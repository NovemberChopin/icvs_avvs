#ifndef CHASSISINFORMATIONGROUPBOX_H
#define CHASSISINFORMATIONGROUPBOX_H

#include <QGroupBox>

namespace Amass {
class Chassis;
}

class QLineEdit;

class ChassisInformationGroupBox : public QGroupBox {
    Q_OBJECT
public:
    ChassisInformationGroupBox(QWidget *parent = nullptr);
    void setChassisData(const Amass::Chassis &chassis);

private:
    QLineEdit *m_eps{nullptr};
    QLineEdit *m_vcu{nullptr};
    QLineEdit *m_nboost{nullptr};
    QLineEdit *m_timestamp{nullptr};
};

#endif // CHASSISINFORMATIONGROUPBOX_H
