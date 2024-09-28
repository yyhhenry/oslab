!
! SYS_SIZE is the number of clicks (16 bytes) to be loaded.
! 0x3000 is 0x30000 bytes = 196kB, more than enough for current
! versions of linux
!
SYSSIZE = 0x3000
!
!	setup.s		(C) 1991 Linus Torvalds
!
! setup.s is responsible for getting the system data from the BIOS,
! and putting them into the appropriate places in system memory.
! both setup.s and system has been loaded by the bootblock.
!
! This code asks the bios for memory/disk/other parameters, and
! puts them in a "safe" place: 0x90000-0x901FF, ie where the
! boot-block used to be. It is then up to the protected mode
! system to read them from there before the area is overwritten
! for buffer-blocks.
!

.globl begtext, begdata, begbss, endtext, enddata, endbss
.text
begtext:
.data
begdata:
.bss
begbss:
.text

SETUPLEN = 4				! nr of setup-sectors
BOOTSEG  = 0x07c0			! original address of boot-sector
INITSEG  = 0x9000			! we move boot here - out of the way
SETUPSEG = 0x9020			! setup starts here
SYSSEG   = 0x1000			! system loaded at 0x10000 (65536).
ENDSEG   = SYSSEG + SYSSIZE		! where to stop loading

! ROOT_DEV:	0x000 - same type of floppy as boot.
!		0x301 - first partition on first drive etc
ROOT_DEV = 0x306

entry start
start:

! Print Hello message
	mov	ax,#SETUPSEG
	mov	es,ax

	call before_print
	mov	cx,#25
	mov	bp,#setup_msg
	call after_print

! Read cursor
	mov	ax,#INITSEG	
	mov	ds,ax 		!设置ds=0x9000
	mov	ah,#0x03	!读入光标位置
	xor	bh,bh
	int	0x10		!调用0x10中断
	mov	[0],dx		!将光标位置写入0x90000.

! Read memory
	mov	ah,#0x88
	int	0x15
	mov	[2],ax

! Read Others
	mov	ax,#0x0000
	mov	ds,ax
	lds	si,[4*0x41]
	mov	ax,#INITSEG
	mov	es,ax
	mov	di,#0x0004
	mov	cx,#0x10
	rep			!重复16次
	movsb

	mov	ax,#SETUPSEG
	mov	es,ax  		

	mov ax,#INITSEG
	mov ss,ax
	sub sp,sp


! Print Cursor
	call 	print_dividing_line

	call 	before_print
	mov	bp, #msg_cur
	call	after_print
	mov bp, #0x00
	call 	print_hex

! Print Memory
	call 	before_print
	mov	bp, #msg_mem
	call	after_print
	mov bp, #0x02
	call 	print_hex

! Print Cyls
	call 	before_print
	mov	bp, #msg_cyls
	call	after_print
	mov bp, #0x04
	call 	print_hex

! Print Heads
	call 	before_print
	mov	bp, #msg_heads
	call	after_print
	mov bp, #0x06
	call 	print_hex

! Print Sector
	call 	before_print
	mov	bp, #msg_sector
	call	after_print
	mov bp, #0x12
	call 	print_hex

	call 	print_dividing_line
	jmp 	os_exit

print_dividing_line:
	call 	before_print
	mov	bp, #msg_stars
	mov	cx, #30
	call	after_print
	ret

! void print_hex(bp) {
!     // bp is the address of the byte to print
print_hex:
    	mov	cx,#4 		! 4个十六进制数字
    	mov	dx,(bp) 	! 将(bp)所指的值放入dx中，如果bp是指向栈顶的话
	print_digit:
    	rol	dx,#4		! 循环以使低4比特用上 !! 取dx的高4比特移到低4比特处。
    	mov	ax,#0xe0f 	! ah = 请求的功能值，al = 半字节(4个比特)掩码。
    	and	al,dl 		! 取dl的低4比特值。
    	add	al,#0x30 	! 给al数字加上十六进制0x30
    	cmp	al,#0x3a
    	jl	outp  		!是一个不大于十的数字
    	add	al,#0x07  	!是a～f，要多加7
    	outp: 
    	int	0x10
    	loop	print_digit

    print_nl:
   		mov	ax,#0xe0d 	! CR
    	int	0x10
    	mov	al,#0xa 	! LF
    	int	0x10
		ret

! } // print_hex

before_print:
	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10
	
	mov	cx,#16			! Default length: 16
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	ret
	

after_print:
	mov	ax,#0x1301		! write string, move cursor
	int	0x10
	ret

os_exit:
	

sectors:
	.word 0

! 16 bytes (= default)
msg_cur:	.ascii "*      Cursor : "
msg_mem:	.ascii "*      Memory : "
msg_cyls:	.ascii "*      Cyls   : "
msg_heads:	.ascii "*      Heads  : "
msg_sector:	.ascii "*      Sectors: "

setup_msg: ! 25 bytes
	.byte 13,10
	.ascii "Now we are in SETUP"
	.byte 13,10,13,10
msg_stars: ! 30 bytes
	.ascii "****************************"
	.byte 13,10

.org 508
root_dev:
	.word ROOT_DEV
boot_flag:
	.word 0xAA55

.text
endtext:
.data
enddata:
.bss
endbss:
