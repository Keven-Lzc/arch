main:
label1:
    addiu $5, $5, 88         # 增加寄存器$5的值88
    bgez $zero, label4          # 如果$zero寄存器大于等于零，跳转到label4

label2:
    addiu $5, $5, 52         # 增加寄存器$5的值52
    blez $3, label1             # 如果$3寄存器小于等于零，跳转到label1

label3:
    addiu $5, $5, 21         # 增加寄存器$5的值21
    bgtz $3, label1             # 如果$3寄存器大于零，跳转到label1

label4:
    addiu $5, $5, 123        # 增加寄存器$5的值123
    jr $ra                   # 跳转回到函数调用的地址

label5:
    addiu $5, $5, 444        # 增加寄存器$5的值444
    syscall                  # 执行系统调用

label6:
    addiu $5, $5, 111        # 增加寄存器$5的值111
    jal label9                  # 跳转并链接到label9标签，同时保存返回地址
    addiu $5, $5, 222        # 增加寄存器$5的值222
    syscall                  # 执行系统调用

label7:
    addu $5, $5, $6          # 将寄存器$5和$6的值相加
    bltzal $4, label10          # 如果$4寄存器小于零，跳转并链接到label10标签

label8:
    addiu $5, $5, 666        # 增加寄存器$5的值666
    syscall                  # 执行系统调用

label9:
    addu $5, $5, $6          # 将寄存器$5和$6的值相加
    bgezal $4, label8           # 如果$4寄存器大于等于零，跳转并链接到label8标签

label10:
    addiu $5, $5, 0xc0decafe  # 增加寄存器$5的值为0xc0decafe
    syscall                  # 执行系统调用
