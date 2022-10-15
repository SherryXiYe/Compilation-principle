    .comm a,4           @全局变量a

    .align 2            @以2^2字节对齐
    .global isPrime     @将isPrime函数名添加到全局符号表中
    .type isPrime,%function     @声明isPrime类型为函数
isPrime:    @ function int isPrime(int n) 判断n是否为质数,若是则返回1,否则返回0
    push	{fp, lr}        @将fp和lr压入栈
	add	fp, sp, #4          @设置新的fp
	sub	sp, sp, #16         @ sp=sp-16,为该函数分配栈帧空间
    str	r0, [fp, #-16]      @参数n入栈
	cmp	r0, #1              @ r0-1,并对相应的状态位置位
    bne .L2                 @若n不等于1,则跳转到标签.L2
    mov r0,#0               @若n等于1,则返回值为0,放到r0中
    b isPrimeRn             @若n等于1,则跳转到标签isPrimeRn
.L2:
    mov r1,#2               @ r1=2,即变量int i=2
    str r1,[fp, #-8]        @将变量i的值存到fp-8处
LOOP:
    ldr	r1, [fp, #-8]       @取出变量i的值到r1
	ldr	r2, [fp, #-16]      @取出n的值到r2
	cmp	r1, r2              
	blt	.L3                 @若r1<r2(即i<n),就跳转到标签.L3
	mov	r0, #1              @若r1>=r2(即i>=n),就结束循环,返回值为1,存入r0
    b isPrimeRn             @跳转到标签isPrimeRn
.L3:
    mov r0,r2                   @ r0中存n的值,r1中存i的值
    bl	__aeabi_idivmod(PLT)    @模运算
    mov r3,r1
    cmp r3,#0                   @比较n%i的结果是否等于0
    bne .L4                     @如果不等于0,就跳转到标签.L4
    mov r0,#0
    b isPrimeRn             @跳转到标签isPrimeRn
.L4:
    ldr	r1, [fp, #-8]       @ 令i++
	add	r1, r1, #1
	str	r1, [fp, #-8]
    b LOOP                  @继续循环
isPrimeRn:
    sub sp, fp, #4
    pop	{fp, pc}

    .section    .rodata
    .align	2
_str0:
	.ascii	"%d\000"
	.align	2
_str1:
	.ascii	"Prime number\n\000"
	.align	2
_str2:
	.ascii	"Composite number\n\000"

    .text
	.align	2
    .global	main
    .type	main, %function

main:
    push	{fp, lr}
	add	fp, sp, #4
	ldr r1, _bridge             @ r1=&a
	ldr r0, _bridge+4           @ *r0="%d\000"
	bl	__isoc99_scanf          @  scanf("%d", &a);
	ldr r2, _bridge             @ r2=&a  
    ldr r0, [r2]                @ r0=a   
	bl	isPrime                 @调用函数isPrime,判断a是否为质数
	cmp	r0, #0                  @比较r0中的isPrime返回值是否为0
	beq	.L5                     @若返回值为0,则跳转到标签.L5
	ldr r0, _bridge+8           @ *r0="Prime number\000"
    bl printf
    b END
.L5:
    ldr r0,_bridge+12           @ *r0="Composite number\000"
    bl printf
END:
	mov	r0, #0
	pop	{fp, pc}

_bridge:
    .word a
    .word _str0
    .word _str1
    .word _str2

    .section	.note.GNU-stack,"",%progbits
