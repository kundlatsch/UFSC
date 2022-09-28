;==============================================================================
; EPOS BOOT STRAP
;
; Desc: Reads system and applications from disk, initializes ix86
;	protected mode, get the size of memory from BIOS and jump into
;	SETUP. The boot strap is loaded by BIOS at 0x07c00
;==============================================================================

; CONSTANTS                                                            
;==============================================================================
; PHYSICAL MEMORY MAP 
; 0x0000 0000 -+-----------------------+ BOOT_IDT 
;              | IDT (4 K)             | 
; 0x0000 1000 -+-----------------------+ BOOT_GDT 
;              | GDT (4 K)             | 
; 0x0000 2000 -+-----------------------+ 
;              | SMP TRAMP. STACK (4 K)| 
; 0x0000 3000 -+-----------------------+ TRAMPOLINE_STACK, TRAMPOLINE_CODE
;              | SMP TRAMPOLINE (4 K)  | 
; 0x0000 4000 -+-----------------------+ 
;              :                       : 
;              | BOOT STACK (15 K)     | 
; 0x0000 7c00 -+-----------------------+ BOOTSTRAP_STACK, BOOTSTRAP_CODE 
;              | BOOT CODE (512 b)     | 
; 0x0000 7e00 -+-----------------------+ 
;              | RESERVED (512 b)      | 
; 0x0000 8000 -+-----------------------+ DISK_IMAGE 
;              | DISK IMAGE (608 K)    | 
;              :                       : 
;              |                       | 
; 0x000a 0000 -+-----------------------+ 
;              | UNUSED (380K)         |
;              :                       : 
;              |                       | 
; 0x000f f000 -+-----------------------+ 
BOOT_IDT         = 0x0000
BOOT_GDT         = 0x1000
TRAMPOLINE_STACK = 0x3000 ; SMP trampoline stack (descendent, 4K)
TRAMPOLINE_CODE  = 0x3000 ; SMP trampoline code (4K) 
BOOTSTRAP_STACK  = 0x7c00 ; Bootstrap stack (descendent, 15K)
BOOTSTRAP_CODE   = 0x7c00 ; Bootstrap code (512 bytes)        
DISK_IMAGE 	 = 0x8000 ; = BOOT_IMAGE_PHY_ADDR from memory_map 

; The size of a disk sector in bytes 
DISK_SECT_SIZE =         512     

; The size of ELF object's header in bytes (GCC dependent)
; Can be determined with objdump -p $(EPOS)/src/setup/pc_setup | sed -n -e 's/^ *LOAD off *\(0x.*\) vaddr.*$/\1/ p' | head -1
; It is automatically adjusted by make (see makefile)
ELF_HDR_SIZE =           XXX

; DISK IMAGE LAYOUT 
; -+-----------------------+ DISK_IMAGE_SYS_INFO 
;  | SYS_INFO (512 bytes)  | 
; -+-----------------------+ DISK_IMAGE_SETUP 
;  | SETUP                 | 
;  :                       : 
; -+-----------------------+ 
;  | SYSTEM                | 
;  :                       : 
; -+-----------------------+ 
;  | INIT                  | 
;  :                       : 
; -+-----------------------+ 
;  | LOADER/APP1           | 
;  :                       : 
; -+-----------------------+ 
;  | APP1                  | 
;  :                       : 
; -+-----------------------+ 
;  :                       : 
; -+-----------------------+ 
;  | APPn                  | 
;  :                       : 
; -+-----------------------+ 
; BOOT IMAGE LAYOUT 
; System Information 
DISK_IMAGE_SYS_INFO =    DISK_IMAGE

; System Information 
DISK_IMAGE_SETUP =       (DISK_IMAGE + DISK_SECT_SIZE)

; SETUP entry point 
SETUP_ENTRY =            (DISK_IMAGE_SETUP + ELF_HDR_SIZE)

;==============================================================================
; DEFINITIONS
;==============================================================================
; 8086 segment addresses
IMAGE_SEG       = DISK_IMAGE >> 4
INFO_SEG        = DISK_IMAGE_SYS_INFO >> 4
	
.text
     
;==============================================================================
; MAIN
;==============================================================================
entry main
main:
		cli			; disable interrupts
		xor	ax,ax		; data segment base = 0x00000
		mov	ds,ax
		mov	es,ax
		mov	ss,ax
                mov     sp,#BOOTSTRAP_STACK	; set stack pointer

; Prepare a trampoline for SMP initialization
		call	setup_trampoline

; Load the boot image from the disk into "DISK_IMAGE"
                mov     si,#msg_loading
		call	print_msg
		push	es
                mov     ax,#IMAGE_SEG
		mov	es,ax		; don't try to load es directly;
                mov     bx,#0		; set es:bx to DISK_IMAGE
                mov     ax,[n_sec_image]
                mov     cx,#0x0002	; starts at track #0, sector #2,
                mov     dx,#0x0000	;  side #0, first drive
                call    load_image
		pop	es
                mov     si,#msg_done
		call	print_msg
		
; Stop the drive motor
                call	stop_drive   

; Get extended memory size (in K)
;		xor	dx,dx
;		mov	ah,#0x88
;		int	0x15		; what if memory size > 64 Mb?
;		push	ds
;		push	#INFO_SEG
;		pop	ds
;		mov	[0],ax
;		mov	[2],dx
;		pop	ds

; Say hello;
		mov	si,#msg_hello
		call	print_msg

; Enable A20 bus line
		call	enable_a20
		
; Zero IDT and GDT
                cld
                xor	ax,ax
                mov	cx,#0x1000	; IDT + GDT = 8K (4K WORDS)
                mov	di,#BOOT_IDT	; initial address (relative to ES)
		rep	   		; zero IDT and GDT with AX
		stosw

; Set GDT
                mov	si,#GDT_CODE	; Set GDT[1]=GDT_CODE and
                mov	di,#BOOT_GDT	; GDT[2]=GDT_DATA
                add	di,#8		; offset GDT[1] = 8
                mov	cx,#8		; sizeof GDT[1] + GDT[2] = 8 WORDS 
		rep			; move WORDS
		movsw

; Set GDTR
                lgdt	GDTR
                   
; Enable Protected Mode
                mov	eax,cr0
                or	al,#0x01	; set PE flag and MP flag
                mov	cr0,eax
     
; Adjust selectors
                mov	bx,#2 * 8	; adjust data selectors to use
                mov	ds,bx		; GDT[2] (DATA) with RPL = 00
                mov	es,bx
                mov     fs,bx
                mov     gs,bx
                mov	ss,bx

; As Linux as86 can't generate 32 bit instructions, we have to code it by hand.
; The instruction below is a inter segment jump to GDT[GDT_CODE]:SETUP.
; Jump into "SETUP" (actually ix86 Protected Mode starts here)
;		jmp	0x0008:#SETUP_ENTRY
		.byte	0x66
		.byte	0xEA
		.long	SETUP_ENTRY
		.word	0x0008
		
;==============================================================================
; PRINT_MSG
;
; Desc: Print a \0 terminated string on the screen using the BIOS
;       Message must end with 00h
;
; Parm: si  -> pointer to the string
;==============================================================================
print_msg:
                pushf
                push	ax
                push	bx
                push	bp
                cld

print_char:   
                lodsb
                cmp	al,#0
                jz	end_print
                mov	ah,#0x0E       
                mov	bx,#0x0007
                int	0x10
                jmp	print_char

end_print:              
		pop	bp
                pop	bx
                pop	ax
                popf
                ret

;==============================================================================
; SAY_Z
;
; Desc: Print 'z' on the screen.
;==============================================================================
say_z:
                pushf
                push	si
                mov	si,#msg_z
                call	print_msg
                pop	si
                popf
                ret

;==============================================================================
; STOP_DRIVE
;
; Desc: Stops the drive motor.
;==============================================================================
stop_drive:
                pushf
                push	ax
                push	dx
                mov	dx,#0x03F2
                xor	al,al
                out	dx,al
                pop	dx
                pop	ax
                popf
                ret

;==============================================================================
; LOAD_ONE_SECTOR
;
; Desc: Load a single sector from disk using the BIOS.
;
; Parm: es:bx  -> buffer
;       cx     -> track (ch) and sector number (cl)
;       dx     -> side (dh) and drive number (dl)
;==============================================================================
load_sector:
		pushf
		push	ax

                mov     ax,#0x0201      ; function #2, load 1 sector
                int     0x13
                cli                     ; int 0x13 sets IF
                jc      ls_disk_error	; if CF=1, error on reading

		pop	ax
		popf
                ret

ls_disk_error:
                mov     si,#msg_disk_error
                call    print_msg      ; print error msg if disk is bad
                call    stop_drive
ls_disk_halt:            
                jmp     ls_disk_halt   ; halt 

;========================================================================
; LOAD_IMAGE
;
; Desc: Load the the image from disk into a buffer.
;
; Parm: es:bx  -> buffer
;       ax     -> number of sectors to load
;       cx     -> initial track (ch) and sector number (cl)
;       dx     -> inital side (dh) and drive number (dl)
;========================================================================
load_image:	
		pushf
		push	ax
		push	bx
		push	cx
		push	dx
		push	es

li_check:
                cmp     ax,#0
                jz      li_done
                call    load_sector
		push	ax
		mov	ax,es
		add	ax,#0x20	; get next buffer position
		mov	es,ax
		pop	ax
		dec	ax
                inc     cl              ; get next sector
                cmp     cl,[n_sec_track]; was this last sector?
                jnz     li_next
                mov     cl,#1
                inc     dh              ; get next side
                cmp     dh,#2           ; read both?
                jnz     li_next
                mov     dh,#0
                inc     ch              ; get next track
                call    say_z
li_next:
                jmp     li_check

li_done:
		pop	es
		pop	dx
		pop	cx
		pop	bx
		pop	ax
		popf
                ret

;========================================================================
; ENABLE_A20
;
; Desc: Enables the 20th address bus line by writing some bytes to the
;       keyboard port.
;========================================================================
enable_a20:
                pushf
                push	ax

 		call	keyb_flush	; empty keyb controller
		mov	al,#0xd1	; keyb->cmd = write
		out	#0x64,al
		call	keyb_flush
		mov	al,#0xdf	; keyb->data = 0xdf
		outb	#0x60,al
		call	keyb_flush

                pop	ax
                popf
                ret

;========================================================================
; FLUSH_KEYB
;
; Desc: Flushes the keyboard controler
;========================================================================
keyb_flush:
                pushf
                push	ax

kf_stat:	call    kf_delay
		in      al,#0x64        ; get keyb status
	        test    al,#1           ; output buffer full?
	        jz      kf_emptyt
	        call    kf_delay
	        in      al,#0x60        ; get data
	        jmp     kf_stat
kf_emptyt:      test    al,#2           ; input buffer full?
	        jnz     kf_stat

                pop	ax
                popf
                ret

kf_delay:	ret

;========================================================================
; SETUP_TRAMPOLINE
;
; Desc: copy the SMP trampoline code to a page-aligned address
;========================================================================
setup_trampoline:
		mov si,#trampoline
		mov di,#TRAMPOLINE_CODE
		cld
		mov cx,#100
		rep
		movsb
		ret

;========================================================================
; TRAMPOLINE
;
; Desc: "trampolines" additional CPUs into protected mode in SMP
;	configurations							=
;========================================================================
trampoline:
		cli ; disable interrupts
		xor ax,ax ; data segment base = 0x00000
		mov ds,ax
		mov es,ax
		mov ss,ax
                mov sp,#TRAMPOLINE_STACK ; set stack pointer
	;; 		mov [0xB8004], #(0x32 & 0xFF) ; 

; Set GDTR
                lgdt	GDTR
                   
; Enable Protected Mode
                mov	eax,cr0
                or	al,#0x01	; set PE flag and MP flag
                mov	cr0,eax
     
; Adjust selectors
                mov	bx,#2 * 8	; adjust data selectors to use
                mov	ds,bx		; GDT[2] (DATA) with RPL = 00
                mov	es,bx
                mov     fs,bx
                mov     gs,bx
                mov	ss,bx

; As Linux as86 can't generate 32 bit instructions, we have to code it by hand.
; The instruction below is a inter segment jump to GDT[GDT_CODE]:SETUP.
; Jump into "SETUP" (actually ix86 Protected Mode starts here)
;		jmp	0x0008:#SETUP_ENTRY
		.byte	0x66
		.byte	0xEA
		.long	SETUP_ENTRY
		.word	0x0008
			
;==============================================================================
; DATA SEGMENT
;==============================================================================
GDTR:
		.word	0x0FFF		; GDT limit - 4K
		.long	0x00001000	; GDT base address - also 4K

GDT_CODE:
		.word 0xFFFF           ; limit 15:00
                .word 0x0000           ; base 15:00
                .byte 0x00             ; base 23:16
                .byte 0x9A             ; 10011001 flags (p/dpl/s/code/c/w/a)
                .byte 0xCF             ; 11001111 (g/d/0/avl) & limit 19:16
                .byte 0x00             ; base 31:24

GDT_DATA:
		.word 0xFFFF           ; limit 15:00
                .word 0x0000           ; base 15:00
                .byte 0x00             ; base 23:16
                .byte 0x92             ; 10010011 flags (p/dpl/s/data/e/w/a)
                .byte 0xCF             ; 11001111 (g/d/0/avl) & limit 19:16
                .byte 0x00             ; base 31:24

msg_disk_error:
		.ascii "Disk error: system halted;"
		.word 0x0D0A
		.byte 0x00

msg_hello:
		.ascii  "This is EPOS!"
		.word 0x0D0A
		.byte 0x00

msg_z:
		.ascii "."
		.byte 0x00

msg_loading:
                .ascii "Loading EPOS "
		.byte 0x00

msg_done:      
		.ascii " done;"
		.word 0x0D0A
		.byte 0x00

; The following is to enable "sys" to define the actual size of the image.
; And the floppy geometry.
; The default values 19 and 360 will only be used if you don't use sys.
		.align 502
system_info:
                .word 0
                .word 0
n_sec_track:
                .word 19
n_sec_image:
		.word 360

; Tag the boot sector as "bootable"
		.word 0xAA55
