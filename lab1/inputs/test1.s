main:
    # 初始化寄存器 $2 和 $8
    li $2, 0x1000
    li $8, 0x1234

    # 存储字节、半字、字
    sb $9, 1($2)      # 存储字节
    sh $10, 2($2)     # 存储半字
    sw $12, 4($2)     # 存储字

    # 从内存中加载数据
    lb $9, 1($2)      # 加载字节
    lbu $11, 1($2)    # 加载无符号字节
    lh $10, 2($2)     # 加载半字
    lhu $13, 2($2)    # 加载无符号半字
    lw $12, 4($2)     # 加载字

    # 条件分支
    bltzal $1, label5 # 如果 $1 < 0，则跳转到 label5
    addi $1, $1, 0x13

label1:
    addi $1, $1, 0x0a

    # 条件判断
    sltiu $4, $1, 0x05
    beq $4, $0, label3

label2:
    andi $5, $1, 0x02
    beq $5, $0, label1

label3:
    ori $6, $1, 1
    xor $7, $6, $1
    bne $7, $0, label5

label4:
    li $1, 0x11
    li $2, 0x23

    # 条件判断
    slti $3, $1, 0x05
    bne $3, $0, label2

label5:

    li $v0, 0x0a
    syscall

