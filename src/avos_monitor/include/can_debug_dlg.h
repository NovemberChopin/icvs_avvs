#ifndef CAN_DEBUG_DLG_H
#define CAN_DEBUG_DLG_H

#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include "data_pool.h"

namespace Ui {
class CanDebugDlg;
}

class CanDebugDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CanDebugDlg(QWidget *parent = 0);
    ~CanDebugDlg();

protected:
    void timerEvent(QTimerEvent *event);

private Q_SLOTS:
    void on_connect_can_btn_clicked();

    void on_start_recieve_btn_clicked();

    void on_send_can_btn_clicked();

    void on_cycle_send_can_btn_clicked();

private:
    Ui::CanDebugDlg *ui;
    bool can_connect_status_ = false;
    bool can_send_status_ = false;
    bool can_recieve_status_ = false;

    int can_send_rate_ = 50;
    QString can_port_ = "CAN0";
    QString can_id_ = "";

    QString can_recieve_ = "";
    QString can_send_ = "";

    int m_nTimerId = 0;
private:
	bool OnConnectCan();
	bool OnDisconnectCan();

    void RecieveCanDataThread();
};

#endif // CAN_DEBUG_DLG_H
