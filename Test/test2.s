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
fact:
  sw $30, -8($29)
  move $30, $29
  sw $31, -4($30)
  lw $15, -12($30)
  lw $14, 0($30)
  move $15, $14
  sw $15, -12($30)
  lw $14, -16($30)
  li $14, 1
  sw $14, -16($30)
  lw $14, -12($30)
  lw $15, -16($30)
  beq $14, $15, label1
  j label2
label1:
  lw $13, -20($30)
  lw $12, 0($30)
  move $13, $12
  sw $13, -20($30)
  lw $12, -20($30)
  lw $31, -4($30)
  addi $29, $30, 256
  lw $30, -8($30)
  move $2, $12
  jr $ra
  j label3
label2:
  lw $13, -24($30)
  lw $11, 0($30)
  move $13, $11
  sw $13, -24($30)
  lw $11, -28($30)
  lw $13, 0($30)
  move $11, $13
  sw $11, -28($30)
  lw $13, -32($30)
  li $13, 1
  sw $13, -32($30)
  lw $13, -36($30)
  lw $11, -28($30)
  lw $10, -32($30)
  sub $13, $11, $10
  sw $13, -36($30)
  lw $11, -36($30)
  sw $11, -292($30)
  addi $29, $30, -292
  jal fact
  lw $13, -40($30)
  move $13, $2
  sw $13, -40($30)
  lw $13, -44($30)
  lw $10, -24($30)
  lw $9, -40($30)
  mul $13, $10, $9
  sw $13, -44($30)
  lw $9, -44($30)
  lw $31, -4($30)
  addi $29, $30, 256
  lw $30, -8($30)
  move $2, $9
  jr $ra
label3:
main:
  sw $30, -8($29)
  move $30, $29
  sw $31, -4($30)
  jal read
  lw $10, -12($30)
  move $10, $2
  sw $10, -12($30)
  lw $10, -16($30)
  lw $13, -12($30)
  move $10, $13
  sw $10, -16($30)
  lw $13, -20($30)
  lw $10, -16($30)
  move $13, $10
  sw $13, -20($30)
  lw $10, -24($30)
  li $10, 1
  sw $10, -24($30)
  lw $10, -20($30)
  lw $13, -24($30)
  bgt $10, $13, label4
  j label5
label4:
  lw $8, -28($30)
  lw $9, -16($30)
  move $8, $9
  sw $8, -28($30)
  lw $9, -28($30)
  sw $9, -284($30)
  addi $29, $30, -284
  jal fact
  lw $8, -32($30)
  move $8, $2
  sw $8, -32($30)
  lw $8, -36($30)
  lw $9, -32($30)
  move $8, $9
  sw $8, -36($30)
  j label6
label5:
  lw $9, -40($30)
  li $9, 1
  sw $9, -40($30)
  lw $9, -36($30)
  lw $8, -40($30)
  move $9, $8
  sw $9, -36($30)
label6:
  lw $8, -44($30)
  lw $9, -36($30)
  move $8, $9
  sw $8, -44($30)
  lw $9, -44($30)
  move $4, $9
  jal write
  lw $8, -48($30)
  li $8, 0
  sw $8, -48($30)
  lw $8, -48($30)
  lw $31, -4($30)
  move $29, $30
  lw $30, -8($30)
  move $2, $8
  jr $ra
