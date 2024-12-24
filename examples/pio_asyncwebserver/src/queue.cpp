/**
 * @defgroup  queue.cpp
 *
 * @brief     Taitement de taches asycrone par system de fille d'attent.
 *
 * @author    Adrien Grellard
 * @date      17/03/20222
 */
#include "../include/queue.h"

// #define DEBUG
#ifndef DEBUG
  #ifdef DEBUG_QUEUE
    #define DEBUG
  #endif
#endif
#ifdef DEBUG
  #define LOG(func, ...) ALT_TRACEC(func, __VA_ARGS__)
#else
  #define LOG(func, ...) ;
#endif


QueueItem::QueueItem() {
}
QueueItem::~QueueItem(void)  { resizeBuff(0,&_mStr); }


void QueueItem::get_string(String & v1){v1 = String(_mStr);}

void QueueItem::add(char* inStr) {
   _mStr = NULL;                              // Defualt our dynamic memory pointer to NULL
   if (resizeBuff(strlen(inStr)+1,&_mStr)) {  // IF we were able to allocate enough RAM for holding our string..
      strcpy(_mStr,inStr);                    // Stuff a copy of what we got into our object.
   }
}

void QueueItemList::addString(String* inStr) {
   char buffer[255]; 
   sprintf(buffer, "%s", inStr->c_str()); 
   addString(buffer); 
   #ifdef DEBUG
      LOG(APPT_DEBUGREGION_QUEUE, "\n\t[listSize]: %d\n\t[queue item][%d]: %s\n", _list.size(), String(buffer).length(), buffer);   
   #endif
}
void QueueItemList::addString(char* inStr) {
  // LOG(APPT_DEBUGREGION_QUEUE, "size: %d\n", _list.size());
  _list.add(new QueueItem());
  uint8_t pos = _list.size()-1;
  _list[pos]->add(inStr);
}


uint8_t QueueItemList::get_size(){return _list.size();}
void QueueItemList::set_callback(_execute_callback_f v1){_execute_callback = std::move(v1);}
void QueueItemList::set_task(Task * v1){_task = v1;}

/**
 * @fn        void QueueItemList::execute_cbTask()
 * @brief     traitment de la fille d'attente
 * @author    Adrien Grellard
 * @date      17/03/20222
 */
void QueueItemList::execute_cbTask(){
  if (_list.size() == 0) return;

  String sT = "";
  
  _list[0]->get_string(sT);

  if (_execute_callback) {
    #ifdef DEBUG
      LOG(APPT_DEBUGREGION_QUEUE, "\n\t[s: %d][e: %d][f: %d]\n\t[listSize]: %d\n\t[queue item][%d]\n\t[CALLBACK]\n", _startHeap, ESP.getFreeHeap(), ESP.getFreeHeap()-_startHeap, _list.size(), sT.length());   
    #endif
    _execute_callback(sT);
  }

  delete _list.remove(0);

  if (_list.size() == 0) {

    // A DELETE
      #ifdef DEBUG
        LOG(APPT_DEBUGREGION_QUEUE, "&c:1&s:\t[QUEUEEND][%d][s: %d][e: %d][f: %d]\n", _id, _startHeap, ESP.getFreeHeap(), ESP.getFreeHeap()-_startHeap);  
      #endif  
    //

    return;
  }

  if (!_task) return;

  #ifdef DEBUG
    char time[12];
    _timeStamp(micros(), time);    
    LOG(APPT_DEBUGREGION_QUEUE, "&c:1&s:\t[TASK RUN][%s]\n", time);   
  #endif

  _task->set_callbackOstart([=](){execute_cbTask();});
  _task->set_iteration_max(0);
  _task->set_taskDelay(ETD::ETD_DELAY, true, _taskDelay, 1);
  _task->set_taskDelayEnabled(ETD::ETD_DELAY, true);
  _task->set_enabled(true); 
}