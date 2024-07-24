#ifndef MYEVENT_H
#define MYEVENT_H

#include <QEvent>
#include <QObject>

enum EventType {UpdateEv=QEvent::User+200, SelectEv=QEvent::User+201};

class MyEvent : public QEvent
{

public:
    MyEvent(int k):QEvent ((Type)(QEvent::User+k)){}
  //  QString info() { return "CustomEvent"; }
};



#endif // MYEVENT_H
