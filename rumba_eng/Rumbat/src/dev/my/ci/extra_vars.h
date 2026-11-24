// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins
#include "plugins/textbox.h"

unsigned char player_direction = 0;


// control azulejos
unsigned char tile;
unsigned char p_txp = 0;
unsigned char p_typ = 0;
unsigned char change_tile = 0;

// acciones 
unsigned char action = 0;  //0: nothing, 1 aspire, 2 wash
unsigned char exit = 0;
unsigned char _en_direction = 1;


// energia
unsigned char battery = 0;
unsigned char water_level = 0;
unsigned char cont_down_battery = 0;
unsigned char cont_down_water = 0;
unsigned char is_charging = 0;
unsigned char is_filling = 0;
unsigned char cont_charge_battery = 0;
unsigned char cont_filling_water = 0;
unsigned char has_rag = 0;  //0: no tiene, 1: tiene, 2: usado

unsigned char shit_tile_acum = 0;

// tiempo
unsigned char cuentafaps = 0;
unsigned char segundos = 0;
unsigned char cont_change_action = 0;
