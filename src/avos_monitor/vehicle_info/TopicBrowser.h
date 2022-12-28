#ifndef TOPICBROWSER_H
#define TOPICBROWSER_H

#include <QWidget>

class QTextBrowser;

class TopicBrowser : public QWidget {
    Q_OBJECT
public:
    TopicBrowser(QWidget *parent = nullptr);
    void appendTopic(const QString &topic);

protected:
    void clear();

private:
    QTextBrowser *m_browser{nullptr};
};

#endif // TOPICBROWSER_H
