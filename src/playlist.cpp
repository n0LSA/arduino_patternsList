#include "../include/playlist.h"

#include <altoolslib.h>
#include "../include/constants.h"


// #define DEBUG
#ifndef DEBUG
  #ifdef DEBUG_PLAYLIST
    #define DEBUG
  #endif
#endif
#ifdef DEBUG
  #define LOG(func, ...) ALT_TRACEC(func, __VA_ARGS__)
#else
  #define LOG(func, ...) ;
#endif

void Playlist_list::set_listRef     (const char  * const & v1)  {_listRef = v1;}
void Playlist_list::set_pos         (uint8_t v1)                {_pos = v1;}
void Playlist_list::set_lbl         (const String & v1)         {_lbl = v1;}
void Playlist_list::set_items_max   (uint8_t v1)        {_items_max = v1;}
void Playlist_list::set_items_cnt   (uint8_t v1)        {_items_cnt = v1;}

void Playlist_list::get_listRef     (const char * & v1) {v1 = _listRef;}
void Playlist_list::get_pos         (uint8_t & v1)      {v1 = _pos;}
void Playlist_list::get_lbl         (String & v1)       {v1 = _lbl;}
void Playlist_list::get_items_max   (uint16_t & v1)     {v1 = _items_max;}
void Playlist_list::get_items_cnt   (uint16_t & v1)     {v1 = _items_cnt;} 

boolean Playlist_list::get_itemPosByName(const String & search, uint16_t & result){
  String iB;
  for (int i=0; i < _items_max; i++) {
    _Playlist_itemArray[i].get_itemBase(iB);
    if (iB == search) {
      result = i;
      return true;
    } 
  }
  return false;
}
boolean Playlist_list::get_itemPosById(uint8_t search, uint16_t & result){
  uint8_t id;
  for (int i=0; i < _items_max; i++) {
    _Playlist_itemArray[i].get_id(id);
    if (id == search) {
      result = i;
      return true;
    } 
  }
  return false;
}
boolean Playlist_list::get_itemPosByItemId(uint8_t search, uint16_t & result){
  uint8_t id;
  for (int i=0; i < _items_max; i++) {
    _Playlist_itemArray[i].get_itemId(id);
    if (id == search) {
      result = i;
      return true;
    } 
  }
  return false;
}
boolean Playlist_list::get_itemIdById(uint8_t search, uint16_t & result){
  uint8_t id;
  for (int i=0; i < _items_max; i++) {
    _Playlist_itemArray[i].get_id(id);
    if (id == search) {
      uint8_t conv;
      _Playlist_itemArray[i].get_itemId(conv);
      result = conv;
      return true;
    } 
  }
  return false;
}

boolean Playlist_list::get_itemIdItemByArrayPos(uint8_t aP, uint8_t & r) { 
  if (aP > _items_cnt-1) return false;
  _Playlist_itemArray[aP].get_itemId(r);
  return true;
}
boolean Playlist_list::get_itemIdByArrayPos(uint8_t aP, uint8_t & r) { 
  if (aP > _items_cnt-1) return false;
  _Playlist_itemArray[aP].get_id(r);
  return true;
}
boolean Playlist_list::get_itemBaseByArrayPos(uint8_t aP, String & r) { 
  if (aP > _items_cnt-1) return false;
  _Playlist_itemArray[aP].get_itemBase(r);
  return true;
}

void Playlist_list::item_df() {
  _items_cnt = 0;
  for (int i=0; i < _items_max; i++) {
    _Playlist_itemArray[i].set_id(0)                ;
    _Playlist_itemArray[i].set_itemId(0)            ;
    _Playlist_itemArray[i].set_itemBase("null")     ;
    _Playlist_itemArray[i].set_lbl("null")          ;
    _Playlist_itemArray[i].set_itemBaseCfg("null")  ;    
  }
}
void Playlist_list::item_restore(DynamicJsonDocument & doc) { 
    JsonArray arr = doc[FPSTR(APPT_REP_007)][F("items")].as<JsonArray>();
    for (size_t i = 0; i < arr.size(); i++) {
        JsonObject item = arr[i];
        if (i >= _items_max) {
          #ifdef DEBUG
            LOG(APPT_DEBUGREGION_PLAYLIST, "\t[LIMITE MAXIMUM ATTEINTE]\n"); 
          #endif     
          break;
        } else {
          _Playlist_itemArray[i].set_id(item[F("id")].as<uint8_t>());
          _Playlist_itemArray[i].set_itemId(item[F("itemid")].as<uint8_t>());
          _Playlist_itemArray[i].set_lbl(item[F("lbl")].as<String>());
          _Playlist_itemArray[i].set_itemBase(item[F("ib")].as<String>());
          _Playlist_itemArray[i].set_itemBaseCfg(item[F("ibcfg")].as<String>());
        }       
    }
    _items_cnt = arr.size();

}
void Playlist_list::item_remove(uint8_t id) {
    #ifdef DEBUG
      LOG(APPT_DEBUGREGION_PLAYLIST, "[Playlist_list::item_remove][START]\n");
    #endif

    Playlist_item * temp = new Playlist_item[_items_max];
    uint8_t cnt = 0;
    uint8_t sId;
    uint8_t sItemId;
    String sLbl, sIb, sIbCfg;
    for (int i=0; i < _items_max; i++) {
        _Playlist_itemArray[i].get_id(sId);
        if (sId == id) continue;
        _Playlist_itemArray[i].get_lbl(sLbl);
        _Playlist_itemArray[i].get_itemId(sItemId);
        _Playlist_itemArray[i].get_itemBase(sIb);
        _Playlist_itemArray[i].get_itemBaseCfg(sIbCfg);
        temp[cnt].set_id(sId);
        temp[cnt].set_itemId(sItemId);
        temp[cnt].set_itemBase(sIb);
        temp[cnt].set_lbl(sLbl);
        temp[cnt].set_itemBaseCfg(sIbCfg);
        cnt++;
    }

    uint8_t iC = _items_cnt - 1;
    item_df();

    for (int i=0; i < _items_max; i++) {
      temp[i].get_id(sId);
      temp[i].get_itemId(sItemId);
      temp[i].get_lbl(sLbl);
      temp[i].get_itemBase(sIb);
      temp[i].get_itemBaseCfg(sIbCfg);
      _Playlist_itemArray[i].set_id(sId);
      _Playlist_itemArray[i].set_itemId(sItemId);
      _Playlist_itemArray[i].set_itemBase(sIb);
      _Playlist_itemArray[i].set_lbl(sLbl);
      _Playlist_itemArray[i].set_itemBaseCfg(sIbCfg);     
    }
    _items_cnt = iC;

    delete[] temp;

    // item_restore();

    #ifdef DEBUG
      LOG(APPT_DEBUGREGION_PLAYLIST, "[Playlist_list::item_remove][DONE]\n");
    #endif
}

uint8_t Playlist_list::item_toArray(uint8_t iP, const String & lbl, const String & itemBase, const String & itemBaseCfg, uint8_t itemId) {
  #ifdef DEBUG
    LOG(APPT_DEBUGREGION_PLAYLIST, "[Playlist_list::item_toArray][START]\n");    
    LOG(APPT_DEBUGREGION_PLAYLIST, "&c:1&s:\t[iP: %d] ", iP);   
    LOG(APPT_DEBUGREGION_PLAYLIST, "&c:1&s:[lbl: %s] ", lbl.c_str());   
    LOG(APPT_DEBUGREGION_PLAYLIST, "&c:1&s:[itemBase : %s] ", itemBase.c_str());   
    LOG(APPT_DEBUGREGION_PLAYLIST, "&c:1&s:[itemBaseCfg : %s]\n", itemBaseCfg.c_str());  
   #endif 
  bool newSav = false;
  if (iP > _items_cnt) {
    if (_items_cnt >= _items_max) {
      #ifdef DEBUG
        LOG(APPT_DEBUGREGION_PLAYLIST, "\t[LIMITE MAXIMUM ATTEINTE][%d/%d]\n",_items_cnt, _items_max );
      #endif
      return 0;
    }   
    if (_items_cnt < 0) _items_cnt = 0;

    iP = _items_cnt;
    _items_cnt++;
    newSav = true;
    #ifdef DEBUG
      LOG(APPT_DEBUGREGION_PLAYLIST, "\t[new sav]\n"); 
    #endif
  }

    // String val = "compo_" +  String(iP);

  uint8_t newNbr = iP;
  uint8_t sId;
  if (newSav){
    if (newNbr>0){
      bool find = true;
      while (find) {
        for (uint8_t i=0; i < _items_cnt; i++) {
          _Playlist_itemArray[i].get_id(sId);
          if (sId == newNbr) {
            newNbr = random(0,255);
          }  
          else find = false;
        }
      }     
    }
  }
  #ifdef DEBUG
    LOG(APPT_DEBUGREGION_PLAYLIST, "\t[position : %d]\n", newNbr);
  #endif

  if (newSav) _Playlist_itemArray[iP].set_id(newNbr);
  
  _Playlist_itemArray[iP].set_itemBase(itemBase);
  _Playlist_itemArray[iP].set_lbl(lbl);
  _Playlist_itemArray[iP].set_itemBaseCfg(itemBaseCfg); 
  _Playlist_itemArray[iP].set_itemId(itemId); 

  // item_toTxt(); 

  #ifdef DEBUG
    LOG(APPT_DEBUGREGION_PLAYLIST, "[Playlist_list::item_toArray][DONE]\n");
  #endif

  return newNbr;
}
void Playlist_list::print(){
  Serial.printf_P(PSTR("[pos: %5d] [items_max: %5d] [items_cnt: %5d] [lbl: %-15s] [listRef: %-15s]\n"), 
    _pos,
    _items_max,
    _items_cnt,
    _lbl.c_str(),
    _listRef
  );
}
void Playlist_list::item_print(){
  Serial.printf_P(PSTR("\t[Playlist_list::printItems][%d/%d]\n"), _items_cnt, _items_max);
  for (int i = 0; i < _items_cnt; ++i) _Playlist_itemArray[i].print();
}
void Playlist_item::print(){
  Serial.printf_P(PSTR("\t[pos: %5d] [itemId: %5d] [lbl: %-45s] [itemBase: %-45s] [itemBaseCfg: %s]\n"), 
    _id,
    _itemId,
    _lbl.c_str(),
    _itemBase.c_str(),
    _itemBaseCfg.c_str()
  );
}

void Playlist_list::item_json(JsonObject & root, boolean pItem){
  // Serial.printf_P(PSTR("[Playlist_list::item_json][%d]\n"),pItem); 
  // Serial.printf_P(PSTR("[cmax[%d]\n"),_items_max); 
  // Serial.printf_P(PSTR("[cnt[%d]\n"),_items_cnt); 
  // Serial.printf_P(PSTR("[pos[%d]\n"),_pos); 
  // Serial.printf_P(PSTR("[lbl[%s]\n"),_lbl.c_str()); 
  // Serial.printf_P(PSTR("[lref[%s]\n"),_listRef); 
  root[F("cmax")] = _items_max;
  root[F("cnt")]  = _items_cnt;
  root[F("pos")]  = _pos;
  root[F("lbl")]  = _lbl;
  root[F("lref")]  = _listRef;
  if (!pItem) return;
  JsonArray arr = root.createNestedArray(F("items")); 
  uint8_t sId, sItemId;
  String sLbl, sIb, sIbCfg;
  for (int i=0; i < _items_cnt; i++) {
    JsonObject var = arr.createNestedObject();
    _Playlist_itemArray[i].get_id(sId);
    _Playlist_itemArray[i].get_lbl(sLbl);
    _Playlist_itemArray[i].get_itemBase(sIb);
    _Playlist_itemArray[i].get_itemBaseCfg(sIbCfg);
    _Playlist_itemArray[i].get_itemId(sItemId);
    var[F("id")] = sId;
    var[F("lbl")] = sLbl;
    var[F("ib")] = sIb;
    var[F("ibcfg")] = sIbCfg;
    var[F("itemid")] = sItemId;
    
  }
}

void Playlist_item::set_id(uint8_t v1)                  {_id = v1;}
void Playlist_item::set_itemId(uint8_t v1)              {_itemId = v1;}
void Playlist_item::set_lbl(const String & v1)          {_lbl = v1;}
void Playlist_item::set_itemBase(const String & v1)     {_itemBase = v1;}
void Playlist_item::set_itemBaseCfg(const String & v1)  {_itemBaseCfg = v1;}

void Playlist_item::get_id(uint8_t & v1)          {v1 = _id;}
void Playlist_item::get_itemId(uint8_t & v1)      {v1 = _itemId;}
void Playlist_item::get_lbl(String & v1)          {v1 = _lbl;}
void Playlist_item::get_itemBase(String & v1)     {v1 = _itemBase;}
void Playlist_item::get_itemBaseCfg(String & v1)  {v1 = _itemBaseCfg;}   