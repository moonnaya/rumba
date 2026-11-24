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
		
	}

	cortina();
	sp_UpdateNow ();
	blackout ();

	difficulty = -1;
	while (difficulty == -1) {
		if (color_diff > 7) {
			color_diff = 1;
		}

		_x = 10; _y = 9; _t =  color_diff;
		_gp_gen = "1  EASY   ";
		print_str ();
		_x = 10; _y = 10; _t =  color_diff;
		_gp_gen = "2  NORMAL ";
		print_str ();
		_x = 10; _y = 11; _t =  color_diff;
		_gp_gen = "3  WOF WOF ";
		print_str ();
		

		sp_UpdateNow ();
		color_diff++;
		espera_activa(5);
		gpjt = sp_GetKey ();
		if (gpjt == '1') {
			difficulty = 0;
		} else if (gpjt == '2') {
			difficulty = 1;
		} else if (gpjt == '3') {
			difficulty = 2;
		}

	}
	//beep_fx(6);
	cortina(); 
	sp_UpdateNow ();
	blackout ();
//1: cambio modo
//2: ??
//3: start
//4: salto? recarga electrica
//5: golpe
//6: puntos
//7: agua?
//8: perfect?
// 11: good job
}
