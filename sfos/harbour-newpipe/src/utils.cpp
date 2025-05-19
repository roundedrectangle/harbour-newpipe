#include <sailfishapp.h>

#include "utils.h"

Utils* Utils::m_instance = nullptr;

Utils::Utils(QObject *parent) : QObject(parent)
{

}

void Utils::instantiate(QObject* parent) {
    if (m_instance == nullptr) {
        m_instance = new Utils(parent);
    }
}

Utils& Utils::getInstance() {
    return *m_instance;
}

QObject* Utils::provider(QQmlEngine* engine, QJSEngine* scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return m_instance;
}

QString Utils::millisecondsToTime(quint32 milliseconds)
{
    int remaining = milliseconds / 1000;
    int hours = remaining / 3600;
    int minutes = (remaining / 60) % 60;
    int seconds = (remaining % 60);

    return QString("%1:%2:%3").arg(hours).arg(minutes, 2, 'f', 0, '0').arg(seconds, 2, 'f', 0, '0');
}

