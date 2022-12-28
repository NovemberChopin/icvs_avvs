#include "RosTopicWidget.h"
#include "Backend.h"
#include "TopicBrowser.h"

void RosTopicWidget::setTopic(TopicType type, const QString &topic) {
    if (type == TopicType::Lidar) {
        m_lidarTopicBrowser->appendTopic(topic);
    } else if (type == TopicType::MillRadar) {
        m_millTopicBrowser->appendTopic(topic);
    } else if (type == TopicType::Camera) {
        m_cameraTopicBrowser->appendTopic(topic);
    } else if (type == TopicType::Gnss) {
        m_rtkTopicBrowser->appendTopic(topic);
    }
}

RosTopicWidget::RosTopicWidget(QWidget *parent) : QTabWidget(parent) {
    m_lidarTopicBrowser = new TopicBrowser();
    m_millTopicBrowser = new TopicBrowser();
    m_cameraTopicBrowser = new TopicBrowser();
    m_rtkTopicBrowser = new TopicBrowser();

    addTab(m_lidarTopicBrowser, "激光雷达");
    addTab(m_millTopicBrowser, "毫米波雷达");
    addTab(m_cameraTopicBrowser, "相机");
    addTab(m_rtkTopicBrowser, "组合惯导");
    
    setStyleSheet("QTabWidget{color:white;background-color:#555555;font-size:14px;}\
    QTabBar::tab{border-bottom-color: #C2C7CB;border-top-left-radius: 2px; border-top-right-radius: 2px;\
    max-width: 90px; min-width:90px; min-height:30px;padding: 0px;font:16px Times New Roman;}\
    QTabBar::tab:selected{margin-left: 5;margin-right: 5;color: white;font-weight:bold;\
    background-color: qlineargradient(spread:pad, x1:0.0618557, y1:1, x2:0.948454, y2:0.057, stop:0 rgba(50, 50, 50, 255), stop:0.618557 rgba(100, 100, 100, 255));}\
    QTabBar::tab:!selected{color: rgb(200, 200, 200);margin-left: 5; margin-right: 0;\
    background-color: qlineargradient(spread:pad, x1:0.0618557, y1:1, x2:0.948454, y2:0.057, stop:0 rgba(50, 50, 50, 255), stop:0.618557 rgba(100, 100, 100, 255));}\
    QTabBar::tab:hover:!selected {color: white;margin-left: 5; margin-right: 5;}");

    setTabPosition(QTabWidget::South);

    auto backend = Backend::instance();
    connect(backend, &Backend::incommingTopic, this, &RosTopicWidget::setTopic);
}
