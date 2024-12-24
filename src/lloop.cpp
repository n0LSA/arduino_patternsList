#include "../include/lloop.h"

ListLoop::ListLoop(){
  _timer = new adri_timer(_delayCurrent*1000, false);
}
ListLoop::~ListLoop(){
  Serial.printf("ListLoop::destructor\n");  
  if(_timer) delete _timer;
}

void ListLoop::get_remainingTime(uint32_t & v){_timer->get_remainingTime(v);}
boolean ListLoop::isPlaying()     {return _play;}
boolean ListLoop::isPause()       {return _pause;}
boolean ListLoop::isRndPlaying()  {return _random;}

void ListLoop::set_delayCurrent(const uint32_t & v) {_delayCurrent = v;}
// void ListLoop::set_delay(const uint32_t & v)        {_delay = v;}
void ListLoop::set_delayManual(boolean v)           {_delayManual = v;}
void ListLoop::set_delayMin(boolean v)              {_delayMin = v;}
void ListLoop::set_delayMin()                       {_delayMin = !_delayMin;}
void ListLoop::set_rnd()                            {_random = !_random;}
void ListLoop::set_pos(apListLoopFlag_t * flag, const uint16_t & v) {reset(flag, v);}

void ListLoop::loop_next(apListLoopFlag_t * flag){
  if (_random)  rnd(flag);
  else          next(flag);
}
void ListLoop::loop_prev(apListLoopFlag_t * flag){
  if (_random)  rnd(flag);
  else          prev(flag);
}
void ListLoop::next(apListLoopFlag_t * flag){
  flag->_posOld = flag->_pos;
  flag->_pos++;
  if (flag->_pos > flag->_posMax) flag->_pos = flag->_posMin;
}
void ListLoop::prev(apListLoopFlag_t * flag){
  flag->_posOld = flag->_pos;
  flag->_pos--;
  if (flag->_pos < flag->_posMin) flag->_pos = flag->_posMax;
}
void ListLoop::rnd(apListLoopFlag_t * flag){
  flag->_posOld = flag->_pos;
  flag->_pos = random(flag->_posMin, flag->_posMax);
  if (flag->_pos > flag->_posMax) flag->_pos = flag->_posMin;
}
void ListLoop::setup(apListLoopFlag_t * flag, const uint16_t & v){
  flag->_pos        = 0;
  flag->_posMax     = v;
  flag->_posOld     = flag->_pos;
}
void ListLoop::updatePos(apListLoopFlag_t * flag, const uint16_t & v){
  flag->_posMax = v;
  if (flag->_pos > flag->_posMax) flag->_pos = flag->_posMax;
}
void ListLoop::reset(apListLoopFlag_t * flag, int v){
  flag->_pos    = v;
  flag->_posOld = flag->_pos;
  _timer->reset();    
}
void ListLoop::reset(){
  _timer->reset();    
}
void ListLoop::start(){
  if(_pause)  _timer->resume();
  else        _timer->activate();
  _play   = true;
  _pause  = false;
}
void ListLoop::stop(){
  _timer->activate(false);
  _play   = false;    
  _pause  = false;
}
void ListLoop::pause(){
  if (_pause) return;
  _timer->pause();
  _play   = false;
  _pause  = true;
}