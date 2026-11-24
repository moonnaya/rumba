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

unsigned char isDogInTarget(unsigned char enem_i) {
	if (_en_x == dog_loaded.target_x && _en_y == dog_loaded.target_y) {
		return 1;
	}
	return 0;
}

void dog_sleep(unsigned char enem_i) {
	_en_x = 255;
	_en_y = 255;
	move_dog(enem_i);
	_x = dog_coords[enem_i][0].x;
	_y = dog_coords[enem_i][0].y;
	_t = 11;
	_n = 8;
	update_tile();
}

void dirty(unsigned char percentaje) {
	rdx = _en_x >> 4;
	rdy = _en_y >> 4;
	_x = rdx;
	_y = rdy;
	_n = 0;
	
	rda = rand () % 100;
	if (rda < percentaje) {

		tile = qtile(rdx, rdy);
		if (rda < dog_loaded.percentaje_pee) {
			dog_loaded.status = 3;
		} else if (rda < dog_loaded.percentaje_poo) {
			dog_loaded.status = 4;
		} else if (tile == 0) {
			_t = 2;
			update_tile();
		} else if (tile == 2) {
			_t = 3;
			update_tile();
		}
	}

}

void ooops(unsigned char tile) {
	_x = _en_x >> 4;
	_y = _en_y >> 4;
	_t = tile;
	_n = 0;
	update_tile();
}

void move_dog(unsigned char enem_i) {
   
	if (_en_x < dog_loaded.target_x) {
		_en_direction = 1;
		_en_x++;
		en_an_base_frame [enit] = 2;
	} else if (_en_x > dog_loaded.target_x) {
		_en_direction = 2;
		_en_x--;
		en_an_base_frame [enit] = 0; 
	} else if (_en_y > dog_loaded.target_y) {
		_en_direction = 4;
		_en_y--;
		en_an_base_frame [enit] = 4;
	} else if (_en_y < dog_loaded.target_y) {
		_en_direction = 3;
		_en_y++;
		en_an_base_frame [enit] = 6;
	} 

}

void update_dog_target(unsigned char enem_i) {
	if (dog_coords[enem_i][dog_coords_index].x == -1) {
		dog_loaded.status = 0;
		dog_loaded.current_time_sleep = 0;
		return;
	}
	dog_loaded.target_x = dog_coords[enem_i][dog_coords_index].x << 4;
	dog_loaded.target_y = dog_coords[enem_i][dog_coords_index].y << 4;
    dog_coords_index++;
}

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

void calculate_percent_clean(void) {
	rdc = 0;
	for (rda = 0; rda < 150; rda++) {
		rdb = map_buff [rda];
		if (rdb == 0) {
			rdc++;
		}
	} 
	percentaje_done = (rdc * 100) / tiles_to_clean_level;
}

void calculate_penalties(void) {
	rdc = 0;
	for (rda = 0; rda < 150; rda++) {
		rdb = map_buff [rda];
		if (rdb == 3) {
		    percentaje_penalties += 1;
		}
		if (rdb == 42 || rdb == 20) {
			percentaje_penalties += 4;
		}
		if (rdb == 1) {
			percentaje_penalties += 12;
		}
	} 
}

void draw_percent_level(void) {
	
	calculate_percent_clean();

	//print
	_x = 7;
	_y = 0;
	_t = percentaje_done;

	if (percentaje_done >= 100) {
		_x = 7; _y = 0; _t = _n;
		_gp_gen = "WOF";
		print_str ();
	} else {
		_n = 2;
		if (percentaje_done >= game_level[n_pant].percentaje_to_completed) {
			_n = 4;
		} 

		print_number_wan();

		//pintar porcentaje
		_x = 9; _y = 0; _t = _n;
		_gp_gen = "@";
		print_str ();
	}

}

void update_time(void) {
	cuentafaps ++; if (cuentafaps == 25 && segundos >= 0) { cuentafaps = 0; segundos --; }
	//pintar segundos
	_x = 28;
	_y = 0;
	_t = segundos;
	_n = 3;
	print_number_wan();
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

void clear_gamezone() {
	for (rda = 0; rda < 10; rda++) {
		for (rdb = 0; rdb < 15; rdb ++) {
			_x = rdb;
			_y = rda;
			_t = 0;
			draw_invalidate_coloured_tile_gamearea ();
			sp_UpdateNow ();
		}
	}
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


	clear_gamezone();
	calculate_penalties();
	PLAY_MUSIC (9);
	_x = 7; _y = 7; _t = 4;
    _gp_gen = "LEVEL REPORT";
    print_str ();
	sp_UpdateNow ();
	//beep_fx(5);
	espera_activa(90);

	_x = 7; _y = 9; _t = 3;
    _gp_gen = "CLEAN        @";
    print_str ();
	_t = percentaje_done;
	if (percentaje_done >= 100) {
		_x = 18; _y = 9; _t = 132;
		_gp_gen = "PERFECT!";
		print_str ();
		sp_UpdateNow ();
		PLAY_SOUND(8); 
		//beep_fx(13);
	} else {
		_x = 18; _y = 9; _n = 4;
		print_number_wan();
		//beep_fx(5);
		PLAY_SOUND(6); 
	}
	sp_UpdateNow ();
	espera_activa(50);

	_x = 7; _y = 10; _t = 3;
    _gp_gen = "PENALTIES    @";
	print_str ();
	if (percentaje_done >= 100) {
		_x = 18; _y = 10; _t = 132;
		_gp_gen = "PERFECT!";
		total_perfect = total_perfect + 1;
		print_str ();
		sp_UpdateNow ();
		//beep_fx(13);
		PLAY_SOUND(8); 
	} else {
		_t = percentaje_penalties;
		_x = 18; _y = 10; _n = 4;
		print_number_wan();
		//beep_fx(5);
		PLAY_SOUND(6); 
	}
	sp_UpdateNow ();
	espera_activa(50);

	_x = 7; _y = 11; _t = 3;
    _gp_gen = "TOTAL        @";
	print_str ();

	if (percentaje_done >= 100) {
		_x = 18; _y = 11; _t = 132;
		_gp_gen = "PERFECT!";
		print_str ();
		sp_UpdateNow ();
		//beep_fx(13);
		PLAY_SOUND(8); 
	} else {
		_t = percentaje_done - percentaje_penalties;
		if (_t >= game_level[n_pant].percentaje_to_completed) {
			_n = 4;
		} else {
			_n = 2;
		}
		_x = 18; _y = 11;
		print_number_wan();
		sp_UpdateNow ();
		//beep_fx(5);
		PLAY_SOUND(6); 
	}	
	espera_activa(50);
	

	_x = 7; _y = 12; _t = 3;
    _gp_gen = "LIFES    ";
	print_str ();
	_t = lifes;
	_x = 18; _y = 12; _n = 2;
	print_number_wan();
	sp_UpdateNow ();
	//beep_fx(5);
	PLAY_SOUND(6); 
	espera_activa(50);

	_x = 7; _y = 13; _t = 3;
    _gp_gen = "PERFECTS    ";
	print_str ();
	_t = total_perfect;
	_x = 18; _y = 13; _n = 4;
	print_number_wan();
	sp_UpdateNow ();
	//beep_fx(5);
	PLAY_SOUND(6); 
	espera_activa(50);

	if (total_perfect >= 3) {
		lifes++;
		total_perfect = 0;
		_t = lifes;
		_x = 18; _y = 12; _n = 2;
		//beep_fx(13);
		PLAY_SOUND(8); 
		print_number_wan();
		sp_UpdateNow ();
		_t = 0;
		_x = 18; _y = 13; _n = 4;
		print_number_wan();
		sp_UpdateNow ();
		espera_activa(50);
	}
	
	if ((percentaje_done - percentaje_penalties) >= game_level[n_pant].percentaje_to_completed) {
		_x = 7; _y = 15; _t = 132;
    	_gp_gen = "GOOD JOB!";
		print_str ();
		//beep_fx(12);
		PLAY_SOUND(11); 
		sp_UpdateNow ();
	} else  {
		_t = lifes - 1;
		_x = 18; _y = 12; _n = 2;
		print_number_wan();
		sp_UpdateNow ();
		if (lifes - 1 > 0) {
			_x = 7; _y = 15; _t = 130;
			_gp_gen = "TRY AGAIN...";
			print_str ();
			//beep_fx(11);
			sp_UpdateNow ();
			PLAY_SOUND(12); 
		}
		
	}

	espera_activa(250);

}


void respawn_level() {
	tiles_to_clean_level = 0;
	for (rda = 0; rda < 150; rda++) {
		rdb = map_buff [rda];
		if (rdb == 0 || rdb == 1 || rdb == 2 || rdb == 3 || rdb == 20 || rdb == 42) {
			tiles_to_clean_level++;
		}
	} 

	percentaje_penalties = 0;
	percentaje_done = 0;
	cuentafaps = 0;
	segundos = game_level[n_pant].time;
	battery = game_level[n_pant].battery;
	water_level = game_level[n_pant].water_level;
	gpx = game_level[n_pant].player_x << 4; p_x = gpx << FIXBITS;
	gpy = game_level[n_pant].player_y << 4; p_y = gpy << FIXBITS;

	cont_down_battery = 0;
	cont_down_water = 0;
	action = 1;
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

	dog_loaded.target_x = dog_loaded.target_y = 0;
	dog_loaded.percentaje_sleep = dogs[game_level[n_pant].enem_i].percentaje_sleep;
	dog_loaded.t_x = dogs[game_level[n_pant].enem_i].t_x;
	dog_loaded.t_y = dogs[game_level[n_pant].enem_i].t_y;
	dog_loaded.time_sleeping = dogs[game_level[n_pant].enem_i].time_sleeping;
	dog_loaded.percentaje_pee = dogs[game_level[n_pant].enem_i].percentaje_pee + difficulty;
	dog_loaded.percentaje_poo = dogs[game_level[n_pant].enem_i].percentaje_poo + difficulty;
	dog_loaded.percentaje_dirty = dogs[game_level[n_pant].enem_i].percentaje_dirty + (difficulty * 4);
	dog_loaded.time_waiting = dogs[game_level[n_pant].enem_i].time_waiting;
	dog_loaded.status = dogs[game_level[n_pant].enem_i].status;
	dog_loaded.current_time_sleep = dogs[game_level[n_pant].enem_i].current_time_sleep;
	dog_loaded.current_time_waiting = dogs[game_level[n_pant].enem_i].current_time_waiting;
	dog_loaded.last_x = dogs[game_level[n_pant].enem_i].last_x;
	dog_loaded.last_y = dogs[game_level[n_pant].enem_i].last_y;

	PLAY_MUSIC (game_level[n_pant].level + 2);
} 

void draw_rag_status(unsigned char color) {
	_gp_gen = ";";
	_n = color; _x = 13; _y = 22; _t = _n;
    print_str ();
	sp_UpdateNow ();
}

void draw_level_text(void) {
	blackout_area();
	PLAY_MUSIC (2);
	_x = 9; _y = 9; _t = 4;
    _gp_gen = "LEVEL ";
    print_str ();

	_x = 15;
	_y = 9;
	_t = game_level[n_pant].level; _n = 4;
	print_number_wan();

	_x = 17; _y = 9; _t = _n;
    _gp_gen = " -";  
    print_str ();

	_x = 20;
	_y = 9;
	_t = game_level[n_pant].stage; _n = 4;
	print_number_wan();

	_x = 14;
	_y = 11;
	_t = game_level[n_pant].percentaje_to_completed; _n = 4;
	print_number_wan();

	_x = 16; _y = 11; _t = _n;
    _gp_gen = "@";  
    print_str ();

	sp_UpdateNow ();
	//beep_fx(10);
	espera_activa(200);
	blackout_area();
}

void draw_level_text_start() {
	signed int v = 7;
	while (v > -1)  {
		blackout_area();
		_n = v;
		_x = 9; _y = 9; _t = v;
		_gp_gen = "LEVEL ";
		print_str ();

		_x = 15;
		_y = 9;
		_t = game_level[n_pant].level; 
		print_number_wan();

		_x = 17; _y = 9; _n = v;  _t = v;
		_gp_gen = " -";  
		print_str ();

		_x = 20;
		_y = 9;
		_t = game_level[n_pant].stage; 
		print_number_wan();

		_x = 14;
		_y = 11;
		_t = game_level[n_pant].percentaje_to_completed;
		print_number_wan();

		_x = 16; _y = 11; _t= v;
		_gp_gen = "@";  
		print_str ();

		sp_UpdateNow ();
		espera_activa(2);
		v = v - 1;
	}
	espera_activa(5);
	
	blackout_area();
}


