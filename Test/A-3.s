.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra
write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra
main:
  sw $30, -8($29)
  move $30, $29
  sw $31, -4($30)
  lw $15, -12($30)
  li $15, 0
  sw $15, -12($30)
  lw $15, -16($30)
  lw $14, -12($30)
  move $15, $14
  sw $15, -16($30)
  lw $14, -20($30)
  li $14, 0
  sw $14, -20($30)
  lw $14, -24($30)
  lw $15, -20($30)
  move $14, $15
  sw $14, -24($30)
  lw $15, -28($30)
  li $15, 0
  sw $15, -28($30)
  lw $15, -32($30)
  lw $14, -28($30)
  move $15, $14
  sw $15, -32($30)
  lw $14, -36($30)
  li $14, 0
  sw $14, -36($30)
  lw $14, -40($30)
  lw $15, -36($30)
  move $14, $15
  sw $14, -40($30)
  lw $15, -44($30)
  li $15, 1
  sw $15, -44($30)
  lw $15, -48($30)
  lw $14, -44($30)
  move $15, $14
  sw $15, -48($30)
  lw $14, -52($30)
  li $14, 1
  sw $14, -52($30)
  lw $14, -56($30)
  lw $15, -52($30)
  move $14, $15
  sw $14, -56($30)
  jal read
  lw $15, -60($30)
  move $15, $2
  sw $15, -60($30)
  lw $15, -64($30)
  lw $14, -60($30)
  move $15, $14
  sw $15, -64($30)
  lw $14, -68($30)
  lw $15, -64($30)
  move $14, $15
  sw $14, -68($30)
  lw $15, -72($30)
  li $15, 0
  sw $15, -72($30)
  lw $15, -68($30)
  lw $14, -72($30)
  ble $15, $14, label1
  j label2
label1:
  lw $13, -76($30)
  li $13, 1
  sw $13, -76($30)
  lw $13, -80($30)
  li $2, 0
  lw $12, -76($30)
  sub $13, $2, $12
  sw $13, -80($30)
  lw $13, -80($30)
  move $4, $13
  jal write
  lw $12, -84($30)
  li $12, 0
  sw $12, -84($30)
  lw $12, -84($30)
  lw $31, -4($30)
  move $29, $30
  lw $30, -8($30)
  move $2, $12
  jr $ra
label2:
label3:
  lw $11, -88($30)
  lw $10, -24($30)
  move $11, $10
  sw $11, -88($30)
  lw $10, -92($30)
  lw $11, -64($30)
  move $10, $11
  sw $10, -92($30)
  lw $11, -88($30)
  lw $10, -92($30)
  blt $11, $10, label4
  j label5
label4:
  lw $9, -96($30)
  lw $8, -16($30)
  move $9, $8
  sw $9, -96($30)
  lw $8, -100($30)
  li $8, 1
  sw $8, -100($30)
  lw $8, -104($30)
  lw $9, -96($30)
  sw $10, -92($30)
  lw $10, -100($30)
  add $8, $9, $10
  sw $8, -104($30)
  lw $9, -16($30)
  lw $8, -104($30)
  move $9, $8
  sw $9, -16($30)
  lw $8, -108($30)
  lw $9, -24($30)
  move $8, $9
  sw $8, -108($30)
  lw $9, -112($30)
  lw $8, -16($30)
  move $9, $8
  sw $9, -112($30)
  lw $8, -116($30)
  lw $9, -108($30)
  lw $10, -112($30)
  add $8, $9, $10
  sw $8, -116($30)
  lw $9, -24($30)
  lw $8, -116($30)
  move $9, $8
  sw $9, -24($30)
  j label3
label5:
  lw $8, -120($30)
  lw $9, -64($30)
  move $8, $9
  sw $8, -120($30)
  lw $9, -124($30)
  lw $8, -24($30)
  move $9, $8
  sw $9, -124($30)
  lw $8, -128($30)
  lw $9, -16($30)
  move $8, $9
  sw $8, -128($30)
  lw $9, -132($30)
  lw $8, -124($30)
  lw $10, -128($30)
  sub $9, $8, $10
  sw $9, -132($30)
  lw $8, -136($30)
  lw $9, -120($30)
  lw $10, -132($30)
  sub $8, $9, $10
  sw $8, -136($30)
  lw $9, -32($30)
  lw $8, -136($30)
  move $9, $8
  sw $9, -32($30)
  lw $8, -140($30)
  lw $9, -16($30)
  move $8, $9
  sw $8, -140($30)
  lw $9, -40($30)
  lw $8, -140($30)
  move $9, $8
  sw $9, -40($30)
  lw $8, -144($30)
  lw $9, -40($30)
  move $8, $9
  sw $8, -144($30)
  lw $9, -144($30)
  move $4, $9
  jal write
  lw $8, -148($30)
  lw $10, -32($30)
  move $8, $10
  sw $8, -148($30)
  lw $10, -148($30)
  move $4, $10
  jal write
  lw $8, -152($30)
  li $8, 0
  sw $8, -152($30)
  lw $8, -24($30)
  sw $9, -144($30)
  lw $9, -152($30)
  move $8, $9
  sw $8, -24($30)
label6:
  lw $9, -156($30)
  lw $8, -24($30)
  move $9, $8
  sw $9, -156($30)
  lw $8, -160($30)
  lw $9, -32($30)
  move $8, $9
  sw $8, -160($30)
  lw $9, -156($30)
  lw $8, -160($30)
  blt $9, $8, label7
  j label8
label7:
  sw $8, -160($30)
  lw $8, -164($30)
  sw $9, -156($30)
  lw $9, -48($30)
  move $8, $9
  sw $8, -164($30)
  lw $9, -168($30)
  lw $8, -40($30)
  move $9, $8
  sw $9, -168($30)
  lw $8, -172($30)
  lw $9, -164($30)
  sw $10, -148($30)
  lw $10, -168($30)
  mul $8, $9, $10
  sw $8, -172($30)
  lw $10, -48($30)
  lw $9, -172($30)
  move $10, $9
  sw $10, -48($30)
  lw $9, -176($30)
  lw $10, -40($30)
  move $9, $10
  sw $9, -176($30)
  lw $10, -180($30)
  li $10, 1
  sw $10, -180($30)
  lw $10, -184($30)
  lw $9, -176($30)
  lw $8, -180($30)
  sub $10, $9, $8
  sw $10, -184($30)
  lw $9, -40($30)
  lw $10, -184($30)
  move $9, $10
  sw $9, -40($30)
  lw $10, -188($30)
  lw $9, -24($30)
  move $10, $9
  sw $10, -188($30)
  lw $9, -192($30)
  li $9, 1
  sw $9, -192($30)
  lw $9, -196($30)
  lw $10, -188($30)
  lw $8, -192($30)
  add $9, $10, $8
  sw $9, -196($30)
  lw $10, -24($30)
  lw $9, -196($30)
  move $10, $9
  sw $10, -24($30)
  j label6
label8:
label9:
  lw $9, -200($30)
  lw $10, -32($30)
  move $9, $10
  sw $9, -200($30)
  lw $10, -204($30)
  li $10, 0
  sw $10, -204($30)
  lw $10, -200($30)
  lw $9, -204($30)
  bgt $10, $9, label10
  j label11
label10:
  lw $8, -208($30)
  sw $9, -204($30)
  lw $9, -56($30)
  move $8, $9
  sw $8, -208($30)
  lw $9, -212($30)
  lw $8, -32($30)
  move $9, $8
  sw $9, -212($30)
  lw $8, -216($30)
  lw $9, -208($30)
  sw $10, -200($30)
  lw $10, -212($30)
  mul $8, $9, $10
  sw $8, -216($30)
  lw $10, -56($30)
  lw $9, -216($30)
  move $10, $9
  sw $10, -56($30)
  lw $9, -220($30)
  lw $10, -32($30)
  move $9, $10
  sw $9, -220($30)
  lw $10, -224($30)
  li $10, 1
  sw $10, -224($30)
  lw $10, -228($30)
  lw $9, -220($30)
  lw $8, -224($30)
  sub $10, $9, $8
  sw $10, -228($30)
  lw $9, -32($30)
  lw $10, -228($30)
  move $9, $10
  sw $9, -32($30)
  j label9
label11:
  lw $10, -232($30)
  lw $9, -48($30)
  move $10, $9
  sw $10, -232($30)
  lw $9, -236($30)
  lw $10, -56($30)
  move $9, $10
  sw $9, -236($30)
  lw $10, -240($30)
  lw $9, -232($30)
  lw $8, -236($30)
  div $9, $8
  mflo $10
  sw $10, -240($30)
  lw $8, -240($30)
  move $4, $8
  jal write
  lw $9, -244($30)
  li $9, 0
  sw $9, -244($30)
  lw $9, -244($30)
  lw $31, -4($30)
  addi $29, $30, 256
  lw $30, -8($30)
  move $2, $9
  jr $ra
