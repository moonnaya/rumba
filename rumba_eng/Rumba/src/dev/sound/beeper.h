// MTE MK1 (la Churrera) v5.0
// Copyleft 2010-2014, 2020 by the Mojon Twins


#asm
	;org 60000

;BeepFX player by Shiru
;You are free to do whatever you want with this code



.playBasic
	ld a,0
.play
	ld hl,sfxData	;address of sound effects data

	;di
	push ix
	push iy

	ld b,0
	ld c,a
	add hl,bc
	add hl,bc
	ld e,(hl)
	inc hl
	ld d,(hl)
	push de
	pop ix			;put it into ix

	ld a,(23624)	;get border color from BASIC vars to keep it unchanged
	rra
	rra
	rra
	and 7
	ld (sfxRoutineToneBorder  +1),a
	ld (sfxRoutineNoiseBorder +1),a
	ld (sfxRoutineSampleBorder+1),a


.readData
	ld a,(ix+0)		;read block type
	ld c,(ix+1)		;read duration 1
	ld b,(ix+2)
	ld e,(ix+3)		;read duration 2
	ld d,(ix+4)
	push de
	pop iy

	dec a
	jr z,sfxRoutineTone
	dec a
	jr z,sfxRoutineNoise
	dec a
	jr z,sfxRoutineSample
	pop iy
	pop ix
	;ei
	ret

	

;play sample

.sfxRoutineSample
	ex de,hl
.sfxRS0
	ld e,8
	ld d,(hl)
	inc hl
.sfxRS1
	ld a,(ix+5)
.sfxRS2
	dec a
	jr nz,sfxRS2
	rl d
	sbc a,a
	and 16
.sfxRoutineSampleBorder
	or 0
	out (254),a
	dec e
	jr nz,sfxRS1
	dec bc
	ld a,b
	or c
	jr nz,sfxRS0

	ld c,6
	
.nextData
	add ix,bc		;skip to the next block
	jr readData



;generate tone with many parameters

.sfxRoutineTone
	ld e,(ix+5)		;freq
	ld d,(ix+6)
	ld a,(ix+9)		;duty
	ld (sfxRoutineToneDuty+1),a
	ld hl,0

.sfxRT0
	push bc
	push iy
	pop bc
.sfxRT1
	add hl,de
	ld a,h
.sfxRoutineToneDuty
	cp 0
	sbc a,a
	and 16
.sfxRoutineToneBorder
	or 0
	out (254),a

	dec bc
	ld a,b
	or c
	jr nz,sfxRT1

	ld a,(sfxRoutineToneDuty+1)	 ;duty change
	add a,(ix+10)
	ld (sfxRoutineToneDuty+1),a

	ld c,(ix+7)		;slide
	ld b,(ix+8)
	ex de,hl
	add hl,bc
	ex de,hl

	pop bc
	dec bc
	ld a,b
	or c
	jr nz,sfxRT0

	ld c,11
	jr nextData



;generate noise with two parameters

.sfxRoutineNoise
	ld e,(ix+5)		;pitch

	ld d,1
	ld h,d
	ld l,d
.sfxRN0
	push bc
	push iy
	pop bc
.sfxRN1
	ld a,(hl)
	and 16
.sfxRoutineNoiseBorder
	or 0
	out (254),a
	dec d
	jr nz,sfxRN2
	ld d,e
	inc hl
	ld a,h
	and 31
	ld h,a
.sfxRN2
	dec bc
	ld a,b
	or c
	jr nz,sfxRN1

	ld a,e
	add a,(ix+6)	;slide
	ld e,a

	pop bc
	dec bc
	ld a,b
	or c
	jr nz,sfxRN0

	ld c,7
	jr nextData


.sfxData

.SoundEffectsData
	defw SoundEffect0Data
	defw SoundEffect1Data
	defw SoundEffect2Data
	defw SoundEffect3Data
	defw SoundEffect4Data
	defw SoundEffect5Data
	defw SoundEffect6Data
	defw SoundEffect7Data
	defw SoundEffect8Data
	defw SoundEffect9Data
	defw SoundEffect10Data
	defw SoundEffect11Data
	defw SoundEffect12Data
	defw SoundEffect13Data

.SoundEffect0Data
	defb 1 ;tone
	defw 10,5812,7940,0,35799
	defb 0
.SoundEffect1Data
	defb 2 ;noise
	defw 1,9396
	defb 0
.SoundEffect2Data
	defb 1 ;tone
	defw 10,1000,200,0,2688
	defb 1 ;tone
	defw 10,1000,400,0,2688
	defb 1 ;tone
	defw 10,1000,200,0,2688
	defb 1 ;tone
	defw 10,1000,400,0,128
	defb 1 ;tone
	defw 10,4000,100,0,128
	defb 1 ;tone
	defw 10,4000,200,0,128
	defb 1 ;tone
	defw 10,4000,400,0,128
	defb 1 ;tone
	defw 10,8000,100,0,128
	defb 0
.SoundEffect3Data
	defb 1 ;tone
	defw 8,2500,256,68,44221
	defb 0
.SoundEffect4Data
	defb 1 ;tone
	defw 10,250,200,5,128
    defb 0
.SoundEffect5Data
	defb 2 ;noise
	defw 10,500,10
	defb 2 ;noise
	defw 10,500,15
	defb 0
.SoundEffect6Data
	defb 1 ;tone
	defw 10,350,1000,0,128
	defb 1 ;tone
	defw 10,350,1500,0,128
	defb 0
.SoundEffect7Data
	defb 1 ;tone
	defw 10,500,500,0,128
	defb 1 ;tone
	defw 10,500,600,0,128
	defb 1 ;tone
	defw 10,500,750,0,128
	defb 0
.SoundEffect8Data
	defb 2 ;noise
	defw 10,250,0
	defb 2 ;noise
	defw 10,250,0
	defb 2 ;noise
	defw 10,250,0
	defb 2 ;noise
	defw 10,250,0
	defb 2 ;noise
	defw 10,250,0
	defb 2 ;noise
	defw 10,250,0
	defb 0
.SoundEffect9Data
	defb 1 ;tone
	defw 10,250,833,36
	defb 0
.SoundEffect10Data
	defb 1 ;tone
	defw 10,750,200,0,128
	defb 1 ;tone
	defw 10,750,500,0,128
	defb 1 ;tone
	defw 10,750,1000,0,128
	defb 1 ;tone
	defw 10,750,200,0,128
	defb 1 ;tone
	defw 10,750,500,0,128
	defb 1 ;tone
	defw 10,750,1000,0,128
	defb 1 ;tone
	defw 10,750,200,0,128
	defb 1 ;tone
	defw 10,750,500,0,128
	defb 1 ;tone
	defw 10,750,1000,0,128
	defb 0
.SoundEffect11Data
	defb 1 ;tone
	defw 10,1000,200,0,128
	defb 1 ;tone
	defw 10,1000,150,0,128
	defb 1 ;tone
	defw 10,1000,100,0,128
	defb 1 ;tone
	defw 10,1000,75,0,128
	defb 1 ;tone
	defw 10,2500,50,0,128
	defb 0
.SoundEffect12Data
	defb 1 ;tone
	defw 10,750,500,0,128
	defb 1 ;tone
	defw 10,750,500,0,128
	defb 1 ;tone
	defw 10,750,500,0,128
	defb 1 ;tone
	defw 10,3000,1000,0,128
	defb 0
.SoundEffect13Data
	defb 1 ;tone
	defw 10,750,500,0,128
	defb 1 ;tone
	defw 10,750,500,0,128
	defb 1 ;tone
	defw 10,750,500,0,128
	defb 1 ;tone
	defw 10,750,750,0,128
	defb 1 ;tone
	defw 10,750,750,0,128
	defb 1 ;tone
	defw 10,750,750,0,128
	defb 1 ;tone
	defw 10,3000,1000,0,128
	defb 0
#endasm

 void beep_fx (unsigned char n) {
        // Cargar en A el valor de n
        asm_int = n;
        #asm
            push ix
            push iy
            ld a, (_asm_int)
            call play
            pop ix
            pop iy
        #endasm
    }