#include "TopicBrowser.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>

TopicBrowser::TopicBrowser(QWidget *parent) : QWidget(parent) {
    m_browser = new QTextBrowser();
    auto clearButton = new QPushButton("清除");
    connect(clearButton, &QPushButton::clicked, this, &TopicBrowser::clear);

    auto buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch(5);
    buttonLayout->addWidget(clearButton);

    auto layout = new QVBoxLayout(this);
    layout->addWidget(m_browser);
    layout->addLayout(buttonLayout);
}

void TopicBrowser::appendTopic(const QString &topic) { m_browser->append(topic); }

void TopicBrowser::clear() { m_browser->clear(); }
