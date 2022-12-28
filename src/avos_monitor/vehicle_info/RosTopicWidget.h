#ifndef ROSTOPICWIDGET_H
#define ROSTOPICWIDGET_H

#include "SensorItem.h"
#include <QTabWidget>

class TopicBrowser;

class RosTopicWidget : public QTabWidget {
    Q_OBJECT
public:
    void setTopic(TopicType type, const QString &topic);

    RosTopicWidget(QWidget *parent = nullptr);

private:
    TopicBrowser *m_lidarTopicBrowser{nullptr};
    TopicBrowser *m_millTopicBrowser{nullptr};
    TopicBrowser *m_cameraTopicBrowser{nullptr};
    TopicBrowser *m_rtkTopicBrowser{nullptr};
};

#endif // ROSTOPICWIDGET_H
