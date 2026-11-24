// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

// control de modo de limpieza


/*

suciedad simple: aspirar. da igual si llevas mopa
sucidad fuerte: fregado sin mopa
pipi: hay que tener mopa y agua. La mopa se estropea y hay que cambiarla
caca: aspirar. Deja un solo rastro de mierda
      si mopa: arrastra mierda
      si modo fregado: arrastra mierda
rastro de mierda:  como suciedad fuerte.
*/
//draw_info(0, 0, has_rag);
rdn = pad0 & sp_FIRE;
if (rdn == 0 && cont_change_action == 0) {
    if (action == 1) {
        action = 2;
    } else {
        action = 1;
    }
    draw_action();
    cont_change_action = 1;
    beep_fx(6);
} 

if (rdn != 0) {
    cont_change_action = 0;
}


// control de cambio de tile
if (p_tx != p_txp || p_ty != p_typ) {
    change_tile = 1;
    p_txp = p_tx;
    p_typ = p_ty;
} else {
    change_tile = 0;
}


tile = qtile(p_tx, p_ty);

//control de recarga de recursos
if (tile == 46) {  //recarga de bateria
    is_charging = 1;
    cont_charge_battery++;
    if (cont_charge_battery >= 2 && battery < 100) {
        battery++;
        cont_charge_battery = 0;
        draw_battery_level();
        beep_fx(4);
    }
} else {
    is_charging = 0;
    cont_charge_battery = 0;
}

//coge trapo
if (tile == 16 && has_rag == 0) {
    has_rag = 1;
    draw_rag_status(7);
    beep_fx(7);
}
// tira trapo
if (tile == 15 && has_rag != 0) {
    has_rag = 0;
    draw_rag_status(0);
    beep_fx(5);
}

if (tile == 47) { //recarga de agua
    is_filling = 1;
    cont_filling_water++;
    if (cont_filling_water == 2 && water_level < 100) {
        water_level++;
        cont_filling_water = 0;
        beep_fx(9);
    }
} else {
    is_filling = 0;
    cont_filling_water = 0;
    cont_down_battery++;
}

//Control de bateria
if (battery > 0) {
    cont_down_battery++;
    if (cont_down_battery >= 25) {
        battery--;
        draw_battery_level();
        cont_down_battery = 0;
    }
}
if (action == 2 && water_level > 0) {
    cont_down_water++;
    if (cont_down_water == 25) {
        water_level--;
        draw_watter_level();
        cont_down_water = 0;
    }
}



// control de limpiado de losas
if (shit_tile_acum > 0) {  //lleva mierda
    //ensucia
    if (tile == 0 || tile == 2 || tile == 3) {
        _x = p_tx;
        _y = p_ty;
        _t = 20;
        _n = 0;
        update_tile();
        shit_tile_acum--;
        beep_fx(8);
    }
} else { //limpia
    if (water_level > 0 && tile == 3 && change_tile == 1 && action == 2) {
        _x = p_tx;
        _y = p_ty;
        _t = 2;
        _n = 0;
        update_tile();
    } 
    if (battery > 0 && tile == 2 && change_tile == 1 && action == 1) {
        _x = p_tx;
        _y = p_ty;
        _t = 0;
        _n = 0;
        update_tile();
    }
    //queda limpiar guarreria
    if (tile == 20 && water_level > 5 && action == 2 && change_tile == 1 && has_rag == 1) {
        _x = p_tx;
        _y = p_ty;
        _t = 3;
        _n = 0;
        update_tile();
        water_level = water_level - 5;
        draw_watter_level();
    }

    //caca
    if (tile == 1 && change_tile == 1) {
        _x = p_tx;
        _y = p_ty;
        _t = 20;
        _n = 0;
        update_tile();
        if (action == 2 || has_rag == 1 || has_rag == 2) {
            shit_tile_acum = 5;
            has_rag = 2;  
            draw_rag_status(2);
        } 
    }

    if (tile == 42 && has_rag == 1 && change_tile == 1  && action == 2  && water_level > 3) {
        _x = p_tx;
        _y = p_ty;
        _t = 0;
        _n = 0;
        has_rag = 2;
        update_tile();
        water_level = water_level - 3;
        draw_watter_level();
        draw_rag_status(6);
    }
}

//animations and update values percent
if (flags[0] == 1) {
    animate_overlay();
    draw_percent_level();
    flags[0] = 0;
} else {
    flags[0] = 1;
}

// time control
update_time();

//comprobar si fin de partida  == 0 || battery == 0 || 
if (segundos <= 0 || battery <= 0 || percentaje_done >= 100) {
    clear_sprites();
    sp_UpdateNow ();
    draw_percent_level();
    draw_info_end_level();
    dog_coords_index = 0;
    if (percentaje_done >= game_level[n_pant].percentaje_to_completed &&
    ((percentaje_done - percentaje_penalties) >= game_level[n_pant].percentaje_to_completed)) {
        if (n_pant == 29) {
            success = 1;
            playing = 0;
        } else {
            n_pant++;
            
        }
        
	} else {
        lifes--;
        if (lifes == 0) {
            game_over();
            playing = success = 0;
            show_menu = 1;
        }
        else {
            //mostrar vidas y mensaje de intentarlo de nuevo
            draw_level_text();
            sp_UpdateNow ();
            draw_scr_background ();
            invalidate_viewport ();
            respawn_level();
            sp_UpdateNow ();
        }
    }  
} 
