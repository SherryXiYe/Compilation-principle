
fibo_x86.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <_Z6myplusii>:
   0:	f3 0f 1e fa          	endbr64 
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	89 7d fc             	mov    %edi,-0x4(%rbp)
   b:	89 75 f8             	mov    %esi,-0x8(%rbp)
   e:	8b 55 fc             	mov    -0x4(%rbp),%edx
  11:	8b 45 f8             	mov    -0x8(%rbp),%eax
  14:	01 d0                	add    %edx,%eax
  16:	5d                   	pop    %rbp
  17:	c3                   	retq   

0000000000000018 <main>:
  18:	f3 0f 1e fa          	endbr64 
  1c:	55                   	push   %rbp
  1d:	48 89 e5             	mov    %rsp,%rbp
  20:	48 83 ec 20          	sub    $0x20,%rsp
  24:	c7 45 ec 00 00 00 00 	movl   $0x0,-0x14(%rbp)
  2b:	c7 45 f0 01 00 00 00 	movl   $0x1,-0x10(%rbp)
  32:	c7 45 f4 01 00 00 00 	movl   $0x1,-0xc(%rbp)
  39:	c7 45 f8 01 00 00 00 	movl   $0x1,-0x8(%rbp)
  40:	48 8d 35 00 00 00 00 	lea    0x0(%rip),%rsi        # 47 <main+0x2f>
  47:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 4e <main+0x36>
  4e:	e8 00 00 00 00       	callq  53 <main+0x3b>
  53:	8b 45 ec             	mov    -0x14(%rbp),%eax
  56:	89 c6                	mov    %eax,%esi
  58:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 5f <main+0x47>
  5f:	e8 00 00 00 00       	callq  64 <main+0x4c>
  64:	48 89 c2             	mov    %rax,%rdx
  67:	48 8b 05 00 00 00 00 	mov    0x0(%rip),%rax        # 6e <main+0x56>
  6e:	48 89 c6             	mov    %rax,%rsi
  71:	48 89 d7             	mov    %rdx,%rdi
  74:	e8 00 00 00 00       	callq  79 <main+0x61>
  79:	8b 45 f0             	mov    -0x10(%rbp),%eax
  7c:	89 c6                	mov    %eax,%esi
  7e:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 85 <main+0x6d>
  85:	e8 00 00 00 00       	callq  8a <main+0x72>
  8a:	48 89 c2             	mov    %rax,%rdx
  8d:	48 8b 05 00 00 00 00 	mov    0x0(%rip),%rax        # 94 <main+0x7c>
  94:	48 89 c6             	mov    %rax,%rsi
  97:	48 89 d7             	mov    %rdx,%rdi
  9a:	e8 00 00 00 00       	callq  9f <main+0x87>
  9f:	8b 05 00 00 00 00    	mov    0x0(%rip),%eax        # a5 <main+0x8d>
  a5:	39 45 f4             	cmp    %eax,-0xc(%rbp)
  a8:	7d 4a                	jge    f4 <main+0xdc>
  aa:	8b 45 f0             	mov    -0x10(%rbp),%eax
  ad:	89 45 fc             	mov    %eax,-0x4(%rbp)
  b0:	8b 55 f0             	mov    -0x10(%rbp),%edx
  b3:	8b 45 ec             	mov    -0x14(%rbp),%eax
  b6:	89 d6                	mov    %edx,%esi
  b8:	89 c7                	mov    %eax,%edi
  ba:	e8 00 00 00 00       	callq  bf <main+0xa7>
  bf:	89 45 f0             	mov    %eax,-0x10(%rbp)
  c2:	8b 45 f0             	mov    -0x10(%rbp),%eax
  c5:	89 c6                	mov    %eax,%esi
  c7:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # ce <main+0xb6>
  ce:	e8 00 00 00 00       	callq  d3 <main+0xbb>
  d3:	48 89 c2             	mov    %rax,%rdx
  d6:	48 8b 05 00 00 00 00 	mov    0x0(%rip),%rax        # dd <main+0xc5>
  dd:	48 89 c6             	mov    %rax,%rsi
  e0:	48 89 d7             	mov    %rdx,%rdi
  e3:	e8 00 00 00 00       	callq  e8 <main+0xd0>
  e8:	8b 45 fc             	mov    -0x4(%rbp),%eax
  eb:	89 45 ec             	mov    %eax,-0x14(%rbp)
  ee:	83 45 f4 01          	addl   $0x1,-0xc(%rbp)
  f2:	eb ab                	jmp    9f <main+0x87>
  f4:	b8 00 00 00 00       	mov    $0x0,%eax
  f9:	c9                   	leaveq 
  fa:	c3                   	retq   

00000000000000fb <_Z41__static_initialization_and_destruction_0ii>:
  fb:	f3 0f 1e fa          	endbr64 
  ff:	55                   	push   %rbp
 100:	48 89 e5             	mov    %rsp,%rbp
 103:	48 83 ec 10          	sub    $0x10,%rsp
 107:	89 7d fc             	mov    %edi,-0x4(%rbp)
 10a:	89 75 f8             	mov    %esi,-0x8(%rbp)
 10d:	83 7d fc 01          	cmpl   $0x1,-0x4(%rbp)
 111:	75 32                	jne    145 <_Z41__static_initialization_and_destruction_0ii+0x4a>
 113:	81 7d f8 ff ff 00 00 	cmpl   $0xffff,-0x8(%rbp)
 11a:	75 29                	jne    145 <_Z41__static_initialization_and_destruction_0ii+0x4a>
 11c:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 123 <_Z41__static_initialization_and_destruction_0ii+0x28>
 123:	e8 00 00 00 00       	callq  128 <_Z41__static_initialization_and_destruction_0ii+0x2d>
 128:	48 8d 15 00 00 00 00 	lea    0x0(%rip),%rdx        # 12f <_Z41__static_initialization_and_destruction_0ii+0x34>
 12f:	48 8d 35 00 00 00 00 	lea    0x0(%rip),%rsi        # 136 <_Z41__static_initialization_and_destruction_0ii+0x3b>
 136:	48 8b 05 00 00 00 00 	mov    0x0(%rip),%rax        # 13d <_Z41__static_initialization_and_destruction_0ii+0x42>
 13d:	48 89 c7             	mov    %rax,%rdi
 140:	e8 00 00 00 00       	callq  145 <_Z41__static_initialization_and_destruction_0ii+0x4a>
 145:	90                   	nop
 146:	c9                   	leaveq 
 147:	c3                   	retq   

0000000000000148 <_GLOBAL__sub_I_globe_n>:
 148:	f3 0f 1e fa          	endbr64 
 14c:	55                   	push   %rbp
 14d:	48 89 e5             	mov    %rsp,%rbp
 150:	be ff ff 00 00       	mov    $0xffff,%esi
 155:	bf 01 00 00 00       	mov    $0x1,%edi
 15a:	e8 9c ff ff ff       	callq  fb <_Z41__static_initialization_and_destruction_0ii>
 15f:	5d                   	pop    %rbp
 160:	c3                   	retq   
