// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins

case 5:
    // status 0: perro dormido
    // status 1: se despierta
    // status 2: camina
    // status 3: se para a hacer cositas pipi
    // status 4: se para a hacer cositas popo

    unsigned char enem_i = game_level[n_pant].enem_i;
    
    if (dog_loaded.status == 0) {
        active = 0;
        if (dog_loaded.time_sleeping <= dog_loaded.current_time_sleep) {
            dog_coords_index = 0;
            dog_loaded.status = 1;
            _x = dog_coords[enem_i][0].x;
	        _y = dog_coords[enem_i][0].y;
            _t = 2;
            _n = 0;
            update_tile();
        } else {
            dog_sleep(enem_i);
            dog_loaded.current_time_sleep++;
        }
    } else if (dog_loaded.status == 1) {
        active = 1;
        dog_loaded.status = 2;
        update_dog_target(enem_i);
        _en_x = dog_coords[enem_i][0].x << 4;
        _en_y = dog_coords[enem_i][0].y << 4;
    } else if (dog_loaded.status == 2) {
        active = 1;
        move_dog(enem_i);

        if (isDogInTarget(enem_i)) {
            update_dog_target(enem_i);
        }

        //cambio de tile? que este alineado a tile
        if (_en_x >> 4 != dog_loaded.t_x || _en_y >> 4 != dog_loaded.t_y 
        && (_en_x & 16 && _en_y & 16)) {
            dog_loaded.t_x = _en_x >> 4;
            dog_loaded.t_y = _en_y >> 4;
            dirty(dog_loaded.percentaje_dirty);
        }
    } else if (dog_loaded.status == 3 || dog_loaded.status == 4) {
        if (dog_loaded.current_time_waiting == 0) {
            dog_loaded.last_x = _en_x;
            dog_loaded.last_y = _en_y;
            _en_x = 255;
            _en_y = 255;
            _x = dog_loaded.last_x >> 4;
            _y = dog_loaded.last_y >> 4;
            if (_en_direction == 1 || _en_direction == 3) {
                _t = 11;
            } else if (_en_direction == 2) {
                _t = 12;
            } else if (_en_direction == 4) {
                _t = 14;
            }
            _n = 8;
            update_tile();
            dog_loaded.current_time_waiting++;
        } else if (dog_loaded.time_waiting <= dog_loaded.current_time_waiting) {
            dog_loaded.current_time_waiting = 0;
            _en_x = dog_loaded.last_x;
            _en_y = dog_loaded.last_y;
           
	        if (dog_loaded.status == 3) {
                ooops(42);
            } else if (dog_loaded.status == 4) {
                ooops(1);
            }
            dog_loaded.status = 2;
        } else {
            dog_loaded.current_time_waiting++;
        }
        
    }
    break;


    