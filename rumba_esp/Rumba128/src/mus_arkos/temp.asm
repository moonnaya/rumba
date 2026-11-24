; temp.asm
; Automaticly generated using build_mus_bin.exe by The Mojon Twins
 
org #C000

arkos:
    include "arkos_player_zx.asm"

song_pool:

SONG_0:
    ; Address is 0xC759
    incbin Rumba_Main_00.aks.bin

SONG_1:
    ; Address is 0xC759
    incbin Rumba_Menu_00.aks.bin

SONG_2:
    ; Address is 0xCC1B
    incbin Rumba_Level_00.aks.bin

SONG_3:
    ; Address is 0xCDDD
    incbin Rumba_Ingame_A_00.aks.bin

SONG_4:
    ; Address is 0xD353
    incbin Rumba_Ingame_B_00.aks.bin

SONG_5:
    ; Address is 0xD62D
    incbin Rumba_Ingame_C_00.aks.bin

SONG_6:
    ; Address is 0xD84A
    incbin Rumba_Ingame_D_00.aks.bin

SONG_7:
    ; Address is 0xDB4F
    incbin Rumba_Ingame_E_00.aks.bin

SONG_8:
    ; Address is 0xDCDC
    incbin Rumba_Ingame_F_00.aks.bin

SONG_9:
    ; Address is 0xDF17
    incbin Rumba_Stage_Clear_00.aks.bin

SONG_10:
    ; Address is 0xDF17
    incbin Rumba_Game_Over_00.aks.bin

SONG_11:
    ; Address is 0xE0C7
    incbin Rumba_Ending_00.aks.bin

SFXS_SONG:
SONG_12:
    ; Address is 0xE3C3
    incbin sfx.aks.bin

song_list:
    defw SONG_0, SONG_1, SONG_2, SONG_3, SONG_4, SONG_5, SONG_6, SONG_7, SONG_8, SONG_9, SONG_10, SONG_11, SONG_12

