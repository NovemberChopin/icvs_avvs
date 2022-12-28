#ifndef INSTALLPARAMETERGROUPBOX_H
#define INSTALLPARAMETERGROUPBOX_H

#include <QWidget>

class QLineEdit;

class InstallParameterGroupBox : public QWidget {
    Q_OBJECT
public:
    InstallParameterGroupBox(QWidget *parent = nullptr);
    void setPosition(float x, float y, float z);
    void setCalibrationParameter(float yaw, float pitch, float roll);

private:
    QLineEdit *m_x{nullptr};
    QLineEdit *m_y{nullptr};
    QLineEdit *m_z{nullptr};

    QLineEdit *m_roll{nullptr};
    QLineEdit *m_yaw{nullptr};
    QLineEdit *m_pitch{nullptr};
};

#endif // INSTALLPARAMETERGROUPBOX_H
