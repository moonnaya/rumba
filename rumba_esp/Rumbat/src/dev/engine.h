// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins



// Motor.h
#ifndef PLAYER_MIN_KILLABLE
#define PLAYER_MIN_KILLABLE 0
#endif

#if defined PLAYER_GENITAL || defined PLAYER_CUSTOM_ANIMATION
	// right: 0 + frame
	// left: 2 + frame
	// up: 4 + frame
	// down: 6 + frame
	const unsigned char *player_cells [] = {
		sprite_1_a, sprite_2_a, sprite_3_a, sprite_4_a,
		sprite_5_a, sprite_6_a, sprite_7_a, sprite_8_a
	};
#elif defined PLAYER_BOOTEE
	// vy = 0: 0 + facing
	// vy < 0: 1 + facing
	// vy > 0: 2 + facing
	const unsigned char *player_cells [] = {
		sprite_5_a, sprite_6_a, sprite_7_a, sprite_8_a,
		sprite_1_a, sprite_2_a, sprite_3_a, sprite_4_a
	};
#else
	// Normal animation:
	// 0 1 2 3 + facing: walk, 1 = stand. 8 + facing = jump/fall
	const unsigned char *player_cells [] = {
		sprite_5_a, sprite_6_a, sprite_7_a, sprite_6_a,
		sprite_1_a, sprite_2_a, sprite_3_a, sprite_2_a,
		sprite_8_a, sprite_4_a
	};
#endif

const unsigned char *enem_cells [] = {
	sprite_9_a, sprite_10_a, sprite_11_a, sprite_12_a, 
	sprite_13_a, sprite_14_a, sprite_15_a, sprite_16_a
};

#include "my/fixed_screens.h"

signed int addsign (signed int n, signed int value) {
	if (n >= 0) return value; else return -value;
}

#ifdef ENABLE_TILANIMS
	#include "tilanim.h"
#endif

void espera_activa (int espera) {
	while (sp_GetKey ());
	do {
		#ifndef MODE_128K
			gpjt = 250; do { gpit = 1; } while (--gpjt);
		#else
			#asm
				halt
			#endasm
		#endif
		//if (sp_GetKey ()) break;
	} while (--espera);
}

#ifndef COMPRESSED_LEVELS
	#if !defined DEACTIVATE_KEYS && MAX_CERROJOS > 0
		void locks_init (void) {
			for (gpit = 0; gpit < MAX_CERROJOS; ++ gpit) cerrojos [gpit].st = 1;	
		}
	#endif
#endif

#ifdef ENABLE_RANDOM_RESPAWN
	char player_hidden (void) {
		gpxx = gpx >> 4;
		gpyy = gpy >> 4;
		if ( (gpy & 15) == 0 && p_vx == 0 ) {
			if (attr (gpxx, gpyy) == 2 || (attr (1 + gpxx, gpyy) == 2 && (gpx & 15)) ) {
				return 1;
			}
		}			
		return 0;
	}
#endif

#ifdef ACTIVATE_SCRIPTING
	void run_fire_script (void) {
		run_script (2 * MAP_W * MAP_H + 2); 		// PRESS_FIRE AT ANY
		run_script ((n_pant << 1) + 1); 			// PRESS_FIRE AT SCREEN n
	}
#endif

#if defined(PLAYER_PUSH_BOXES) || !defined(DEACTIVATE_KEYS)
	void process_tile (void) {
		#ifdef PLAYER_PUSH_BOXES
			#ifdef FIRE_TO_PUSH
				if ((pad0 & sp_FIRE) == 0)				
			#endif		
			{ 
				#ifdef FIRE_TO_PUSH			
					// Para no disparar...
					//pushed_any = 1;
					p_disparando = 1;
				#endif

				if (qtile (x0, y0) == 14 && attr (x1, y1) == 0 && x1 < 15 && y1 < 10) {
					rda = map_buff [COORDS(x1,y1)];
					
					#if defined(ACTIVATE_SCRIPTING) && defined(ENABLE_PUSHED_SCRIPTING)
						flags [MOVED_TILE_FLAG] = rda; 
						flags [MOVED_X_FLAG] = x1;
						flags [MOVED_Y_FLAG] = y1;
					#endif			
					
					// Pintar
					_x = x0; _y = y0; _t = 0; _n = 0; update_tile ();
					_x = x1; _y = y1; _t = 14; _n = 10; update_tile ();
					
					// Sonido
					#ifdef MODE_128K
						PLAY_SOUND (SFX_PUSH_BOX);
					#else			
						beep_fx (2);	
					#endif
				
					#if defined(ACTIVATE_SCRIPTING) && defined(ENABLE_PUSHED_SCRIPTING) && defined(PUSHING_ACTION)
						// Call scripting
						just_pushed = 1;
						run_fire_script ();
						just_pushed = 0;
					#endif

					#include "my/ci/on_tile_pushed.h"
				} 
			}			
		#endif

		#if !defined DEACTIVATE_KEYS && MAX_CERROJOS > 0
			if (qtile (x0, y0) == 15 && p_keys) {
				for (gpit = 0; gpit < MAX_CERROJOS; ++ gpit) {
					if (cerrojos [gpit].x == x0 && cerrojos [gpit].y == y0 && cerrojos [gpit].np == n_pant) {
						cerrojos [gpit].st = 0;
						break;
					}
				}
				_x = x0; _y = y0; _t = 0; _n = 0; update_tile ();
				-- p_keys;
		
				#ifdef MODE_128K
					PLAY_SOUND (SFX_OPEN_LOCK);
				#else
					beep_fx (8);
				#endif

				#include "my/ci/on_unlocked_bolt.h"
			}
		#endif
	}
#endif

void draw_scr_background (void) {
	#ifdef UNPACKED_MAP
		map_pointer = mapa + (n_pant * 150);
	#else
		map_pointer = mapa + (n_pant * 75);
	#endif
		
	seed = n_pant;

	_x = VIEWPORT_X; _y = VIEWPORT_Y;

	// Draw 150 tiles
	
	#ifdef USE_MAP_CUSTOM_DECODER
		#include "my/map_custom_decoder.h"
	#else
		for (gpit = 0; gpit < 150; ++ gpit) {	
			#ifdef UNPACKED_MAP
				// Mapa tipo UNPACKED
				/*
				_t = *map_pointer ++;
				map_attr [gpit] = behs [_t];
				map_buff [gpit] = _t;
				*/
				#asm
						ld  hl, (_map_pointer)
						ld  a, (hl)
						ld  (__t), a
						inc hl
						ld  (_map_pointer), hl

						ld  b, 0
						ld  c, a
						ld  hl, _behs
						add hl, bc
						ld  a, (hl)

						ld  bc, (_gpit)
						ld  b, 0
						ld  hl, _map_attr
						add hl, bc
						ld  (hl), a

						ld  hl, _map_buff
						add hl, bc
						ld  a, (__t)
						ld  (hl), a
				#endasm
			#else
				// Mapa tipo PACKED
				/*
				if (!(gpit & 1)) {
					gpc = *map_pointer ++;
					_t = gpc >> 4;
				} else {
					_t = gpc & 15;
				}
				map_attr [gpit] = behs [_t];
				if (_t == 0 && (rand () & 15) == 1) _t = 19;
				map_buff [gpit] = _t;
				*/
				#asm
						ld  a, (_gpit)
						and 1
						jr  nz, _draw_scr_packed_existing
					._draw_scr_packed_new
						ld  hl, (_map_pointer)
						ld  a, (hl)
						ld  (_gpc), a
						inc hl
						ld  (_map_pointer), hl

						srl a
						srl a
						srl a
						srl a
						jr  _draw_scr_packed_done

					._draw_scr_packed_existing
						ld  a, (_gpc)
						and 15

					._draw_scr_packed_done
						ld  (__t), a
						
						ld  b, 0
						ld  c, a
						ld  hl, _behs
						add hl, bc
						ld  a, (hl)

						ld  bc, (_gpit)
						ld  b, 0
						ld  hl, _map_attr
						add hl, bc
						ld  (hl), a

				#ifdef PACKED_MAP_ALT_TILE
						ld  a, (__t)
						or  a
						jr  nz, _draw_scr_packed_noalt

					._draw_scr_packed_alt
						call _rand
						ld  a, l
						and 15
						cp  1
						jr  z, _draw_scr_packed_alt_subst

						ld  a, (__t)
						jr  _draw_scr_packed_noalt

					._draw_scr_packed_alt_subst
						ld  a, PACKED_MAP_ALT_TILE
						ld  (__t), a

					._draw_scr_packed_noalt
				#endif

						ld  hl, _map_buff
						add hl, bc
						
						ld  (hl), a
				#endasm
			#endif	

			#ifdef BREAKABLE_WALLS
				// brk_buff [gpit] = 0;
				#asm
						ld  hl, _brk_buff
						add hl, bc
						xor a
						ld  (hl), a
				#endasm
			#endif

			#ifdef ENABLE_TILANIMS
				if (_t >= ENABLE_TILANIMS) {
					_n = (((_x - VIEWPORT_X) << 3) & 0xf0) | ((_y - VIEWPORT_Y) >> 1);
					tilanims_add ();	
				}
			#endif

			draw_coloured_tile ();

			//_x += 2; if (_x == VIEWPORT_X + 30) { _x = VIEWPORT_X; _y += 2; }
			#asm
					ld  a, (__x)
					add 2
					cp  30 + VIEWPORT_X
					jr  c, _advance_worm_no_inc_y

					ld  a, (__y)
					add 2
					ld  (__y), a

					ld  a, VIEWPORT_X

				._advance_worm_no_inc_y
					ld  (__x), a
			#endasm
		}
	#endif
}



void draw_scr (void) {
	is_rendering = 1;

	#ifdef ENABLE_TILANIMS
		tilanims_reset ();
	#endif

	#ifdef ENABLE_FIRE_ZONE
		f_zone_ac = 0;
	#endif	

	draw_scr_background ();

	// Object setup

	enems_load ();

	#ifdef ACTIVATE_SCRIPTING
		#if defined LINE_OF_TEXT && !defined LINE_OF_TEXT_NO_AUTOERASE
			_x = LINE_OF_TEXT_X; _y = LINE_OF_TEXT; _t = LINE_OF_TEXT_ATTR; _gp_gen = "                              "; print_str ();
		#endif
		// Ejecutamos los scripts de entrar en pantalla:
		run_script (2 * MAP_W * MAP_H + 1); 	// ENTERING ANY
		run_script (n_pant << 1); 				// ENTERING SCREEN n
	#endif

	#include "my/ci/entering_screen.h"

	//draw_scr_hotspots_locks ();

	#ifdef PLAYER_CAN_FIRE
		bullets_init ();
	#endif

	#ifdef ENABLE_SIMPLE_COCOS
		simple_coco_init ();
	#endif

	invalidate_viewport ();
	is_rendering = 0;
}

unsigned char difficulty = 0;
unsigned char show_menu = 1;
unsigned char color_diff = 3;

unsigned char select_joyfunc (void) {
	

		#ifdef MODE_128K
		#else
			#asm
				; Music generated by beepola
				call musicstart
			#endasm
		#endif

	
	while (1) {

		gpjt = sp_GetKey ();
		if (gpjt >= '1' && gpjt <= '3') {
			joyfunc = joyfuncs [gpjt - '1'];

			#ifdef USE_TWO_BUTTONS
				isJoy = gpjt > '1';
			#endif
			return 0;
		} else if (gpjt == 'r') {
			joyfunc = joyfuncs[0];
			return 0;
		} else if (gpjt == '4') {
			return 2;
		}
		
	}
	return 1;
	/*#ifdef MODE_128K
		STOP_SOUND ();
		PLAY_SOUND (SFX_START);
		sp_WaitForNoKey ();
	#endif*/
}

#ifdef WALLS_STOP_ENEMIES
	unsigned char mons_col_sc_x (void) {
		cx1 = cx2 = (_en_mx > 0 ? _en_x + 15 : _en_x) >> 4;
		cy1 = _en_y >> 4; cy2 = (_en_y + 15) >> 4;
		cm_two_points ();
		#ifdef EVERYTHING_IS_A_WALL
			return (at1 || at2);
		#else
			return ((at1 & 8) || (at2 & 8));
		#endif
	}
		
	unsigned char mons_col_sc_y (void) {
		cy1 = cy2 = (_en_my > 0 ? _en_y + 15 : _en_y) >> 4;
		cx1 = _en_x >> 4; cx2 = (_en_x + 15) >> 4;
		cm_two_points ();
		#ifdef EVERYTHING_IS_A_WALL
			return (at1 || at2);
		#else
			return ((at1 & 8) || (at2 & 8));
		#endif
	}
#endif

#if defined(SLOW_DRAIN) && defined(PLAYER_BOUNCES)
	unsigned char lasttimehit;
#endif

#if defined (ENABLE_FANTIES) && defined (FANTIES_TYPE_HOMING)
	// Funciones auxiliares custom
	unsigned char distance (void) {
		rdx = abs (cx2 - gpx);
		rdy = abs (cy2 - gpy);
		rdn = rdx < rdy ? rdx : rdy;
		return (rdx + rdy - (rdn >> 1) - (rdn >> 2) + (rdn >> 4));
	}
#endif

#if defined(ENABLE_FANTIES)
	int limit (int val, int min, int max) {
		if (val < min) return min;
		if (val > max) return max;
		return val;
	}
#endif


// ---------------------------------- CUSTOM 


void animate_overlay(void) {

	if (water_level <= 0) {
		return;
	}
	
	//animar solo hasta (p_life / 10) + 21
	rdb = water_level / 10;
	for (rdn = 0; rdn < rdb; rdn ++) {
		_y = 22;
		_x = 1 + rdn;
		rda = rand() % 3;
		
		if (rda == 0) {
			#asm
				ld  a, (__x)
				ld  c, a
				ld  a, (__y)
				ld  d, 15
				ld  e, 61
			#endasm
		} else if (rda == 1) {
			#asm
				ld  a, (__x)
				ld  c, a
				ld  a, (__y)
				ld  d, 15
				ld  e, 62
			#endasm
		} else {
			#asm
				ld  a, (__x)
				ld  c, a
				ld  a, (__y)
				ld  d, 15
				ld  e, 63
			#endasm
		}
		#asm
			call SPPrintAtInv
		#endasm
		
	}

}

void draw_battery_level(void) {

	rda = battery / 10;
	_y = 22;

	for (rdb = 0; rdb < rda; rdb++) {
		_x = 21 + rdb;
		#asm
            ld  a, (__x)
            ld  c, a
			ld  a, (__y)
            ld  d, 6
            ld  e, 59
            call SPPrintAtInv
    	#endasm
	}

	rda = 10 - rda;

	for (rdb = 0; rdb < rda; rdb++) {
		_x = 30 - rdb;
		
		#asm
            ld  a, (__x)
            ld  c, a
			ld  a, (__y)
            ld  d, 0
            ld  e, 0
            call SPPrintAtInv
    	#endasm
	}
	
}

void draw_watter_level(void) {

	rda = water_level / 10;
	_y = 22;
	
	rda = 10 - rda;

	for (rdb = 0; rdb < rda; rdb++) {
		_x = 10 - rdb;
		
		#asm
            ld  a, (__x)
            ld  c, a
			ld  a, (__y)
            ld  d, 0
            ld  e, 0
            call SPPrintAtInv
    	#endasm
	}
}

void draw_action(void) {
	_x = 15;
	_y = 22;
	if (action == 1) {
		_t = 43;
	} 
	if (action == 2) {
		_t = 44;
	}
	draw_coloured_tile ();
	invalidate_tile();
}

void draw_info_end_level(void) {

	#asm
			ld  ix, (_sp_player)
			ld  iy, vpClipStruct

			ld  hl, (_p_next_frame)
			ld  de, (_p_current_frame)
			or  a
			sbc hl, de
			ld  b, h
			ld  c, l

			ld  a, 255
			srl a
			srl a
			srl a
			add VIEWPORT_Y
			ld  h, a 

			ld  a, 255
			srl a
			srl a
			srl a
			add VIEWPORT_X
			ld  l, a 
			
			ld  a, 255
			and 7
			ld  d, a

			ld  a, 255
			and 7
			ld  e, a

			call SPMoveSprAbs
	#endasm


	for (rda = 0; rda < 10; rda++) {
		for (rdb = 0; rdb < 15; rdb ++) {
			_x = rdb;
			_y = rda;
			_t = 0;
			draw_invalidate_coloured_tile_gamearea ();
			sp_UpdateNow ();
			espera_activa(1);
		}
	}
}


void respawn_level() {
	cuentafaps = 0;
	battery = 100;
	water_level = 100;
	gpx = 5 << 4; p_x = gpx << FIXBITS;
	gpy = 5 << 4; p_y = gpy << FIXBITS;

	cont_down_battery = 0;
	cont_down_water = 0;
	action = 1;
	exit = 0;
	player_direction = -1;
	has_rag = 0;
	draw_rag_status(0);

	cont_charge_battery = 0;
	cont_filling_water = 0;

	p_vx = 0;
	p_vy = 0;

	shit_tile_acum = 0;

	draw_watter_level();
	draw_battery_level();
	animate_overlay();
	draw_action();
} 

void draw_rag_status(unsigned char color) {
	_gp_gen = ";";
	_n = color; _x = 13; _y = 22; _t = _n;
    print_str ();
	sp_UpdateNow ();
}



