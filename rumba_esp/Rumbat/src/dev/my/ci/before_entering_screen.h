// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

draw_action();
textbox(n_pant + 1, 0);
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
textbox(0,0);