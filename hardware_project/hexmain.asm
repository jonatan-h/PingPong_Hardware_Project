  # hexmain.asm
  # Written 2015-09-04 by F Lundevall
  # Copyright abandonded - this file is in the public domain.

	.text
main:
	li	$a0,17	# change this to test different values

	jal	hexasc		# call hexasc
	nop			# delay slot filler (just in case)	

	move	$a0,$v0		# copy return value to argument register

	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window
	
stop:	j	stop		# stop after one run
	nop			# delay slot filler (just in case)

  # You can write your own code for hexasc here
  #
  hexasc:
  	 and $t0, $a0, 0xf
  	 sltiu $t1, $t0, 10 #0xa
  	 
  	 beq $t1, $0, else
  	 nop
  	 addiu $v0, $t0, 48 #0x30
  	 j end
  	 
  	 else:
  	 addiu $v0, $t0, 55 #0x37
  	 
  	 end:	
	jr $ra
	nop