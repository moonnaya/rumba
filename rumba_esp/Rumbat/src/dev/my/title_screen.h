// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

// You can change this function. To set level to anything different than 0.

{

	while (show_menu == 1) {
		
		if (show_menu == 1) {
			sp_UpdateNow ();
			blackout ();
			
			#ifdef MODE_128K
				get_resource (TITLE_BIN, 16384);
				PLAY_MUSIC (0);
			#else		
				#asm
					ld hl, _s_title
					ld de, 16384
					call depack
				#endasm
			#endif
		}
		
		show_menu = select_joyfunc ();
		
		if (show_menu == 2) {
			// mostrar instrucciones
			for (rdc = 0; rdc < 4; rdc = rdc + 1) {  // ponia 4
				sp_UpdateNow ();
				blackout ();
				textbox(8 + rdc, 0);
				espera_activa(200);
			}
			blackout ();

			// pintar objetos
				
			_x = 2;
			_y = 2;
			_t = 46;
			_n = 0;
			update_tile();

			_x = 2;
			_y = 4;
			_t = 47;
			_n = 0;
			update_tile();

			_x = 2;
			_y = 6;
			_t = 43;
			_n = 0;
			update_tile();

			_x = 2;
			_y = 8;
			_t = 44;
			_n = 0;
			update_tile();

			_gp_gen = "RECARGA DE ELECTRICIDAD";
			_n = 4; _x = 8; _y = 6; _t = _n;
			print_str ();

			_gp_gen = "RECARGA DE AGUA";
			_n = 4; _x = 8; _y = 10; _t = _n;
			print_str ();

			_gp_gen = "ACCION DE ASPIRAR";
			_n = 4; _x = 8; _y = 14; _t = _n;
			print_str ();

			_gp_gen = "ACCION DE FREGAR";
			_n = 4; _x = 8; _y = 18; _t = _n;
			print_str ();

			sp_UpdateNow ();
			espera_activa(1000);
			blackout ();

			_x = 2;
			_y = 2;
			_t = 16;
			_n = 0;
			update_tile();

			_x = 2;
			_y = 4;
			_t = 15;
			_n = 0;
			update_tile();

			_x = 2;
			_y = 6;
			_t = 2;
			_n = 0;
			update_tile();

			_x = 2;
			_y = 8;
			_t = 3;
			_n = 0;
			update_tile();

			_gp_gen = "MOPAS";
			_n = 4; _x = 8; _y = 6; _t = _n;
			print_str ();

			_gp_gen = "PAPELERA DE MOPAS";
			_n = 4; _x = 8; _y = 10; _t = _n;
			print_str ();

			_gp_gen = "LOSA SUCIA";
			_n = 4; _x = 8; _y = 14; _t = _n;
			print_str ();

			_gp_gen = "LOSA ASQUEROSA. RESTA 1@";
			_n = 4; _x = 8; _y = 18; _t = _n;
			print_str ();


			sp_UpdateNow ();
			espera_activa(1000);
			blackout ();

			_x = 2;
			_y = 2;
			_t = 42;
			_n = 0;
			update_tile();

			_x = 2;
			_y = 4;
			_t = 1;
			_n = 0;
			update_tile();

			_x = 2;
			_y = 6;
			_t = 20;
			_n = 0;
			update_tile();

			_gp_gen = "PIPI. RESTA 4@";
			_n = 4; _x = 8; _y = 6; _t = _n;
			print_str ();

			_gp_gen = "CACA. RESTA 12@";
			_n = 4; _x = 8; _y = 10; _t = _n;
			print_str ();

			_gp_gen = "PURRIA. RESTA 4@";
			_n = 4; _x = 8; _y = 14; _t = _n;
			print_str ();

			sp_UpdateNow ();
			espera_activa(1000);

			show_menu = 1;
			cortina();
		}
	}

	sp_UpdateNow ();
	blackout ();

	n_pant = -1;
	while (n_pant == -1) {
		
		if (color_diff > 7) {
			color_diff = 1;
		}

		_x = 10; _y = 7; _t =  color_diff;
		_gp_gen = "1  ASPIRAR";
		print_str ();

		_x = 10; _y = 8; _t =  color_diff;
		_gp_gen = "2  ASPIRAR Y FREGAR";
		print_str ();
		
		_x = 10; _y = 9; _t =  color_diff;
		_gp_gen = "3  PIPI ";
		print_str ();
		
		_x = 10; _y = 10; _t =  color_diff;
		_gp_gen = "4  CACA ";
		print_str ();
		
		_x = 10; _y = 11; _t =  color_diff;
		_gp_gen = "5  TODO ";
		print_str ();
		
		espera_activa(2);
		sp_UpdateNow ();
		color_diff++;
		
		gpjt = sp_GetKey ();
		if (gpjt == '1') {
			n_pant = 0;
		} else if (gpjt == '2') {
			n_pant = 1;
		} else if (gpjt == '3') {
			n_pant = 2;
		} else if (gpjt == '4') {
			n_pant = 3;
		} else if (gpjt == '5') {
			n_pant = 4;
		}
	}
	beep_fx(6);
	cortina();
	sp_UpdateNow ();
	blackout ();


}
