#ifndef ASYNCEVENT_H
#define ASYNCEVENT_H

#include <QCoreApplication>
#include <QEvent>
#include <QThread>
#include <tuple>

template <typename Functor>
class AsyncEvent : public QEvent {
public:
    AsyncEvent(Functor &&functor)
        : QEvent(static_cast<QEvent::Type>(QEvent::registerEventType())), m_functor(std::forward<Functor>(functor)) {}

    ~AsyncEvent() { m_functor(); }

private:
    Functor m_functor;
};

using ForceAsync = std::true_type;
template <typename T = std::false_type, typename Functor>
void executeAtObjectThread(QObject *target, Functor &&functor) {
    if (!T::value && target->thread() == QThread::currentThread()) {
        functor();
        return;
    }
    auto event = new AsyncEvent<Functor>(std::forward<Functor>(functor));
    QCoreApplication::postEvent(target, event, Qt::LowEventPriority);
}

#endif // ASYNCEVENT_H
