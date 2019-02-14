  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
  .global hexasc
  .global delay
  .global time2string
  .global enable_interrupt
  .global score2string


  enable_interrupt:
    ei
  jr $ra
  nop

  hexasc:
  	 and $t0, $a0, 0xf
  	 sltiu $t1, $t0, 10 #0xa

  	 beq $t1, $0, else
  	 nop
  	 addiu $v0, $t0, 48 #0x30
  	 j end
  	 nop
  	 else:
  	 addiu $v0, $t0, 55 #0x37

  	 end:
	jr $ra
	nop

	delay:

		addi $t2, $0, 5000 #3 for 3000 milliesconds

		slt $t0, $0, $a0
		beq $t0, $0, endwhile
		nop
		while:
		addi $t0, $0, 0  #int i=0
		addi $a0, $a0, -1 #ms--
		for:
		slt $t1, $t0, $t2 #condition for for-loop #returns 1 if i<4711
		beq $t1, $0, endfor
		nop
		addi $t0, $t0, 1 #i++
		j for
		nop
		endfor:
		slt $t1, $0, $a0 #while 0<ms
		beq $t1, $0, endwhile
		nop
		j while
		nop
		endwhile:


		jr $ra
		nop


	#li $v0, 32
	#li $a0, 1000

	#syscall

	#jr $ra
	#nop

	time2string:

	PUSH $ra
	PUSH $s0

	move $s0, $a0

	#and $t0, $a1, 0xff

	#bne $t0, $0, timeprint
	#nop

	#addi $t1, $0, 68
	#sb $t1, 0($s0)

	#addi $t1, $0, 73
	#sb $t1, 1($s0)

	#addi $t1, $0, 78
	#sb $t1, 2($s0)

	#addi $t1, $0, 71
	#sb $t1, 3($s0)

	#addi $t1, $0, 0
	#sb $t1, 4($s0)

	#j exit
	#nop
	#timeprint:

	srl $a0, $a1, 12 #shift to get LSB
	jal hexasc
	nop
	sb $v0, 0($s0) #store returnvalue in memorylocation refered to by s0 with offset 0 bytes

	srl $a0, $a1, 8 #shift to get LSB
	jal hexasc
	nop
	sb $v0, 1($s0) #store returnvalue in memorylocation refered to by s0 with offset 1 bytes

	addi $t0, $0, 58 #ascii-code for colon
	sb $t0, 2($s0) #store in memorylocation s0 with offset 2 bytes

	srl $a0, $a1, 4 #shift to get LSB
	jal hexasc
	nop
	sb $v0, 3($s0) #store in memorylocation s0 with offset 3 bytes

	move $a0, $a1 #move a1 to a0 since already LSB
	jal hexasc
	nop
	sb $v0, 4($s0) #store returnvalue in memorylocation s0 with offset 4 bytes

	addi $t0, $0, 0 # ascii-code for null-byte
	sb $t0, 5($s0) #store in memorylocation s0 with offset 5 bytes
	exit:

	POP $s0
	POP $ra

	jr $ra
	nop
