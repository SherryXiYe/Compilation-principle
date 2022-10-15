    .text
	.section	.rodata
	.align	2
_str0:
	.ascii	"%d\000"
	.align	2
_str1:
	.ascii	"Array after rotation:\n\000"
	.align	2
_str2:
	.ascii	"%d \000"
    .align	2
_str3:
	.ascii	"\n\000"
.data
    array:.word 0,0,0,0,0,0,0,0,0,0     @定义一个长度为10的数组

	.text
	.align	2
	.global	main
	.type	main, %function
main:
    stmfd sp!,{lr}
    mov r4,#0           @ r4保存变量i的值,初始化i=0
    ldr r5, =array      @ 将数组a的首地址读取到r5中
    b .L2
Loop1:
    ldr r0, _bridge     @ *r0="%d\000"
    mov r1,r5           @ 令r1为此刻指向的数组元素的地址,作为scanf的第二个参数
    add r5,#4           @ 给r5自增4,指向数组下一个元素对应的地址
    bl scanf
    add r4,#1           @ i++
.L2:
    cmp r4,#10          @将r4中存的i和10比较
    blt Loop1           @若r4<10,就跳转到标签Loop1,继续第一个for循环

    mov r4, #0          @ r4保存变量i的值,重置i=0
    mov r3, #9          @ r3保存变量j的值,初始化为9
    ldr r0, =array      @ r0保存a[i]的地址,初始化为数组首地址
    add r1,r0,#36       @ r1保存a[j]的地址,初始化为数组末尾地址
    b .L3
Loop2:
    ldr r2,[r0]         @ r2保存a[i]的值
    ldr r6,[r1]         @ r6保存a[j]的值
    str r2,[r1]         @ a[j]为原来a[i]的值
    str r6,[r0]         @ a[i]为原来a[j]的值
    add r0,#4           @ 给r0自增4,指向数组下一个元素对应的地址
    sub r1,#4           @ 给r1自增4,指向数组前一个元素对应的地址
    add r4,#1           @ i++
    sub r3,#1           @ j--
.L3:
    cmp r4,r3           @ 比较r4和r3,即比较i和j的大小
    blt Loop2           @若r4<r3(即i<j),则跳转到标签Loop2,继续while循环
    
    ldr r0,_bridge+4    @ *r0="Array after rotation:\n\000"
    bl printf

    mov r4,#0           @ r4保存变量i的值,重置i=0
    ldr r5, =array      @ r5保存循环中当前数组元素的地址,重新赋值为数组a的首地址
    b .L4
Loop3:
    ldr r0,_bridge+8    @ *r0="%d \000"
    ldr r1,[r5],#4      @将数组的首地址读取到r5中,在循环中将r5的值传给r1,并向后取址
    bl printf  
    add r4,#1           @ i++
.L4:
    cmp r4,#10          @将r4中存的i和10比较
    blt Loop3           @若r4<10,就跳转到标签Loop3,继续第二个循环
    ldr r0,_bridge+12   @ *r0="\n\000"
    bl printf
    mov r0, #0
    ldmfd sp!,{lr}
    mov pc, lr

_bridge:
    .word _str0
    .word _str1
    .word _str2
    .word _str3
    .section	.note.GNU-stack,"",%progbits
