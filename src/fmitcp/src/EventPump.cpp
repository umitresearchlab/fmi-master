#include "EventPump.h"

using namespace fmitcp;

EventPump::EventPump(){
    m_pump = lw_eventpump_new();
    m_exiting = false;
}

EventPump::~EventPump(){
    //lw_pump_delete(m_pump);
}

lw_pump EventPump::getPump(){
    return m_pump;
}

void EventPump::startEventLoop(){
    if(!m_exiting){
        lw_eventpump_start_eventloop(m_pump);
    }
}

void EventPump::exitEventLoop(){
    if(!m_exiting){
        printf("exitEventLoop()\n");
        lw_eventpump_post_eventloop_exit(m_pump);
        m_exiting = true;
    }
    //lw_pump_delete(m_pump);
}
