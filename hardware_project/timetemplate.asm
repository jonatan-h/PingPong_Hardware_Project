  # timetemplate.asm
  # Written 2015 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

.macro	PUSH (%reg)
	addi	$sp,$sp,-4
	sw	%reg,0($sp)
.end_macro

.macro	POP (%reg)
	lw	%reg,0($sp)
	addi	$sp,$sp,4
.end_macro

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
	.text
main:
	# print timstr
	la	$a0,timstr
	li	$v0,4
	syscall
	nop
	# wait a little
	li	$a0,1000
	jal	delay
	nop
	# call tick
	la	$a0,mytime
	jal	tick
	nop
	# call your function time2string
	la	$a0,timstr
	la	$t0,mytime
	lw	$a1,0($t0)
	jal	time2string
	nop
	# print a newline
	li	$a0,10
	li	$v0,11
	syscall
	nop
	# go back and do it all again
	j	main
	nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

  # you can write your code for subroutine "hexasc" below this line
  #
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
		
		addi $t2, $0, 20 #6 for 3000 milliesconds
		
		slt $t0, $0, $a0
		beq $t0, $0, endwhile
		while:
		addi $t0, $0, 0  #int i=0
		addi $a0, $a0, -1 #ms--
		for:
		slt $t1, $t0, $t2 #condition for for-loop #returns 1 if i<4711
		beq $t1, $0, endfor
		nop
		addi $t0, $t0, 1 #i++
		j for
		endfor:
		slt $t1, $0, $a0 #while 0<ms
		beq $t1, $0, endwhile
		nop
		j while
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
	
	and $t0, $a1, 0xff
	
	bne $t0, $0, timeprint
	nop
	
	addi $t1, $0, 68
	sb $t1, 0($s0)
	
	addi $t1, $0, 73
	sb $t1, 1($s0)
	
	addi $t1, $0, 78
	sb $t1, 2($s0)
	
	addi $t1, $0, 71
	sb $t1, 3($s0)
	
	addi $t1, $0, 0
	sb $t1, 4($s0)
	
	j exit
	nop
	timeprint:
	
	srl $a0, $a1, 12 #shift to get LSB
	jal hexasc
	nop
	sb, $v0, 0($s0) #store returnvalue in memorylocation refered to by s0 with offset 0 bytes
	
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
