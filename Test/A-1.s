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
  li $15, 1
  sw $15, -12($30)
  lw $15, -16($30)
  lw $14, -12($30)
  move $15, $14
  sw $15, -16($30)
  lw $14, -20($30)
  li $14, 11
  sw $14, -20($30)
  lw $14, -24($30)
  lw $15, -20($30)
  move $14, $15
  sw $14, -24($30)
  lw $15, -28($30)
  li $15, 39
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
  lw $14, -16($30)
  move $15, $14
  sw $15, -44($30)
  lw $14, -48($30)
  li $2, 0
  lw $15, -44($30)
  sub $14, $2, $15
  sw $14, -48($30)
  lw $14, -52($30)
  lw $15, -32($30)
  move $14, $15
  sw $14, -52($30)
  lw $15, -56($30)
  lw $14, -48($30)
  lw $13, -52($30)
  add $15, $14, $13
  sw $15, -56($30)
  lw $14, -60($30)
  li $14, 17
  sw $14, -60($30)
  lw $14, -64($30)
  lw $15, -16($30)
  move $14, $15
  sw $14, -64($30)
  lw $15, -68($30)
  lw $14, -60($30)
  lw $13, -64($30)
  mul $15, $14, $13
  sw $15, -68($30)
  lw $13, -72($30)
  li $2, 0
  lw $14, -68($30)
  sub $13, $2, $14
  sw $13, -72($30)
  lw $13, -76($30)
  lw $14, -56($30)
  lw $15, -72($30)
  add $13, $14, $15
  sw $13, -76($30)
  lw $14, -16($30)
  lw $13, -76($30)
  move $14, $13
  sw $14, -16($30)
  lw $13, -80($30)
  lw $14, -16($30)
  move $13, $14
  sw $13, -80($30)
  lw $14, -80($30)
  move $4, $14
  jal write
  lw $13, -84($30)
  li $13, 11
  sw $13, -84($30)
  lw $13, -88($30)
  lw $15, -24($30)
  move $13, $15
  sw $13, -88($30)
  lw $15, -92($30)
  lw $13, -84($30)
  lw $12, -88($30)
  mul $15, $13, $12
  sw $15, -92($30)
  lw $12, -96($30)
  lw $13, -32($30)
  move $12, $13
  sw $12, -96($30)
  lw $13, -100($30)
  lw $12, -16($30)
  move $13, $12
  sw $13, -100($30)
  lw $12, -104($30)
  lw $13, -16($30)
  move $12, $13
  sw $12, -104($30)
  lw $13, -108($30)
  lw $12, -100($30)
  lw $15, -104($30)
  mul $13, $12, $15
  sw $13, -108($30)
  lw $15, -112($30)
  lw $12, -96($30)
  lw $13, -108($30)
  add $15, $12, $13
  sw $15, -112($30)
  lw $12, -116($30)
  lw $15, -16($30)
  move $12, $15
  sw $12, -116($30)
  lw $15, -120($30)
  lw $12, -112($30)
  lw $13, -116($30)
  mul $15, $12, $13
  sw $15, -120($30)
  lw $13, -124($30)
  lw $12, -92($30)
  lw $15, -120($30)
  sub $13, $12, $15
  sw $13, -124($30)
  lw $12, -24($30)
  lw $13, -124($30)
  move $12, $13
  sw $12, -24($30)
  lw $13, -128($30)
  lw $12, -24($30)
  move $13, $12
  sw $13, -128($30)
  lw $12, -128($30)
  move $4, $12
  jal write
  lw $13, -132($30)
  lw $15, -16($30)
  move $13, $15
  sw $13, -132($30)
  lw $15, -136($30)
  lw $13, -16($30)
  move $15, $13
  sw $15, -136($30)
  lw $13, -140($30)
  lw $15, -24($30)
  move $13, $15
  sw $13, -140($30)
  lw $15, -144($30)
  lw $13, -136($30)
  lw $11, -140($30)
  div $13, $11
  mflo $15
  sw $15, -144($30)
  lw $11, -148($30)
  lw $13, -132($30)
  lw $15, -144($30)
  mul $11, $13, $15
  sw $11, -148($30)
  lw $15, -152($30)
  lw $13, -32($30)
  move $15, $13
  sw $15, -152($30)
  lw $13, -156($30)
  lw $15, -148($30)
  lw $11, -152($30)
  add $13, $15, $11
  sw $13, -156($30)
  lw $15, -32($30)
  lw $13, -156($30)
  move $15, $13
  sw $15, -32($30)
  lw $13, -160($30)
  lw $15, -32($30)
  move $13, $15
  sw $13, -160($30)
  lw $15, -160($30)
  move $4, $15
  jal write
  lw $13, -164($30)
  li $13, 4
  sw $13, -164($30)
  lw $13, -168($30)
  lw $11, -16($30)
  move $13, $11
  sw $13, -168($30)
  lw $11, -172($30)
  lw $13, -164($30)
  lw $10, -168($30)
  mul $11, $13, $10
  sw $11, -172($30)
  lw $10, -176($30)
  lw $13, -24($30)
  move $10, $13
  sw $10, -176($30)
  lw $13, -180($30)
  li $13, 17
  sw $13, -180($30)
  lw $13, -184($30)
  lw $10, -176($30)
  lw $11, -180($30)
  div $10, $11
  mflo $13
  sw $13, -184($30)
  lw $11, -188($30)
  lw $10, -172($30)
  lw $13, -184($30)
  add $11, $10, $13
  sw $11, -188($30)
  lw $10, -192($30)
  lw $11, -16($30)
  move $10, $11
  sw $10, -192($30)
  lw $11, -196($30)
  lw $10, -32($30)
  move $11, $10
  sw $11, -196($30)
  lw $10, -200($30)
  lw $11, -192($30)
  lw $13, -196($30)
  mul $10, $11, $13
  sw $10, -200($30)
  lw $13, -204($30)
  lw $11, -188($30)
  lw $10, -200($30)
  add $13, $11, $10
  sw $13, -204($30)
  lw $11, -40($30)
  lw $13, -204($30)
  move $11, $13
  sw $11, -40($30)
  lw $13, -208($30)
  lw $11, -40($30)
  move $13, $11
  sw $13, -208($30)
  lw $11, -208($30)
  move $4, $11
  jal write
  lw $13, -212($30)
  li $13, 0
  sw $13, -212($30)
  lw $13, -212($30)
  lw $31, -4($30)
  move $29, $30
  lw $30, -8($30)
  move $2, $13
  jr $ra
