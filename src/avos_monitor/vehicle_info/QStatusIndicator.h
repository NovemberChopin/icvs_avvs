#ifndef QSTATUSINDICATOR_H
#define QSTATUSINDICATOR_H

#include <QWidget>

class QStatusIndicator : public QWidget {
    Q_OBJECT
public:
    QStatusIndicator(QWidget *parent = nullptr);
    void setColor(const QColor &active, const QColor &inactive=Qt::black);
    void setActive(bool active);
    void setAlignment(Qt::Alignment alignment);

protected:
    void paintEvent(QPaintEvent *event) final;

private:
    QColor m_activeColor{Qt::green};
    QColor m_inactiveColor{Qt::black};
    bool m_active{false};
    Qt::Alignment m_alignment{Qt::AlignHCenter};
};

#endif // QSTATUSINDICATOR_H
