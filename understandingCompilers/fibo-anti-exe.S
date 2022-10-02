
fibo:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 2f 00 00 	mov    0x2fd9(%rip),%rax        # 3fe8 <__gmon_start__>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	callq  *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	retq   

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 6a 2f 00 00    	pushq  0x2f6a(%rip)        # 3f90 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 6b 2f 00 00 	bnd jmpq *0x2f6b(%rip)        # 3f98 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nopl   (%rax)
    1030:	f3 0f 1e fa          	endbr64 
    1034:	68 00 00 00 00       	pushq  $0x0
    1039:	f2 e9 e1 ff ff ff    	bnd jmpq 1020 <.plt>
    103f:	90                   	nop
    1040:	f3 0f 1e fa          	endbr64 
    1044:	68 01 00 00 00       	pushq  $0x1
    1049:	f2 e9 d1 ff ff ff    	bnd jmpq 1020 <.plt>
    104f:	90                   	nop
    1050:	f3 0f 1e fa          	endbr64 
    1054:	68 02 00 00 00       	pushq  $0x2
    1059:	f2 e9 c1 ff ff ff    	bnd jmpq 1020 <.plt>
    105f:	90                   	nop
    1060:	f3 0f 1e fa          	endbr64 
    1064:	68 03 00 00 00       	pushq  $0x3
    1069:	f2 e9 b1 ff ff ff    	bnd jmpq 1020 <.plt>
    106f:	90                   	nop
    1070:	f3 0f 1e fa          	endbr64 
    1074:	68 04 00 00 00       	pushq  $0x4
    1079:	f2 e9 a1 ff ff ff    	bnd jmpq 1020 <.plt>
    107f:	90                   	nop

Disassembly of section .plt.got:

0000000000001080 <__cxa_finalize@plt>:
    1080:	f3 0f 1e fa          	endbr64 
    1084:	f2 ff 25 3d 2f 00 00 	bnd jmpq *0x2f3d(%rip)        # 3fc8 <__cxa_finalize@GLIBC_2.2.5>
    108b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .plt.sec:

0000000000001090 <_ZNSirsERi@plt>:
    1090:	f3 0f 1e fa          	endbr64 
    1094:	f2 ff 25 05 2f 00 00 	bnd jmpq *0x2f05(%rip)        # 3fa0 <_ZNSirsERi@GLIBCXX_3.4>
    109b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000010a0 <__cxa_atexit@plt>:
    10a0:	f3 0f 1e fa          	endbr64 
    10a4:	f2 ff 25 fd 2e 00 00 	bnd jmpq *0x2efd(%rip)        # 3fa8 <__cxa_atexit@GLIBC_2.2.5>
    10ab:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000010b0 <_ZNSolsEPFRSoS_E@plt>:
    10b0:	f3 0f 1e fa          	endbr64 
    10b4:	f2 ff 25 f5 2e 00 00 	bnd jmpq *0x2ef5(%rip)        # 3fb0 <_ZNSolsEPFRSoS_E@GLIBCXX_3.4>
    10bb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000010c0 <_ZNSt8ios_base4InitC1Ev@plt>:
    10c0:	f3 0f 1e fa          	endbr64 
    10c4:	f2 ff 25 ed 2e 00 00 	bnd jmpq *0x2eed(%rip)        # 3fb8 <_ZNSt8ios_base4InitC1Ev@GLIBCXX_3.4>
    10cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000010d0 <_ZNSolsEi@plt>:
    10d0:	f3 0f 1e fa          	endbr64 
    10d4:	f2 ff 25 e5 2e 00 00 	bnd jmpq *0x2ee5(%rip)        # 3fc0 <_ZNSolsEi@GLIBCXX_3.4>
    10db:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .text:

00000000000010e0 <_start>:
    10e0:	f3 0f 1e fa          	endbr64 
    10e4:	31 ed                	xor    %ebp,%ebp
    10e6:	49 89 d1             	mov    %rdx,%r9
    10e9:	5e                   	pop    %rsi
    10ea:	48 89 e2             	mov    %rsp,%rdx
    10ed:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    10f1:	50                   	push   %rax
    10f2:	54                   	push   %rsp
    10f3:	4c 8d 05 a6 02 00 00 	lea    0x2a6(%rip),%r8        # 13a0 <__libc_csu_fini>
    10fa:	48 8d 0d 2f 02 00 00 	lea    0x22f(%rip),%rcx        # 1330 <__libc_csu_init>
    1101:	48 8d 3d d9 00 00 00 	lea    0xd9(%rip),%rdi        # 11e1 <main>
    1108:	ff 15 d2 2e 00 00    	callq  *0x2ed2(%rip)        # 3fe0 <__libc_start_main@GLIBC_2.2.5>
    110e:	f4                   	hlt    
    110f:	90                   	nop

0000000000001110 <deregister_tm_clones>:
    1110:	48 8d 3d f9 2e 00 00 	lea    0x2ef9(%rip),%rdi        # 4010 <__TMC_END__>
    1117:	48 8d 05 f2 2e 00 00 	lea    0x2ef2(%rip),%rax        # 4010 <__TMC_END__>
    111e:	48 39 f8             	cmp    %rdi,%rax
    1121:	74 15                	je     1138 <deregister_tm_clones+0x28>
    1123:	48 8b 05 ae 2e 00 00 	mov    0x2eae(%rip),%rax        # 3fd8 <_ITM_deregisterTMCloneTable>
    112a:	48 85 c0             	test   %rax,%rax
    112d:	74 09                	je     1138 <deregister_tm_clones+0x28>
    112f:	ff e0                	jmpq   *%rax
    1131:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1138:	c3                   	retq   
    1139:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001140 <register_tm_clones>:
    1140:	48 8d 3d c9 2e 00 00 	lea    0x2ec9(%rip),%rdi        # 4010 <__TMC_END__>
    1147:	48 8d 35 c2 2e 00 00 	lea    0x2ec2(%rip),%rsi        # 4010 <__TMC_END__>
    114e:	48 29 fe             	sub    %rdi,%rsi
    1151:	48 89 f0             	mov    %rsi,%rax
    1154:	48 c1 ee 3f          	shr    $0x3f,%rsi
    1158:	48 c1 f8 03          	sar    $0x3,%rax
    115c:	48 01 c6             	add    %rax,%rsi
    115f:	48 d1 fe             	sar    %rsi
    1162:	74 14                	je     1178 <register_tm_clones+0x38>
    1164:	48 8b 05 85 2e 00 00 	mov    0x2e85(%rip),%rax        # 3ff0 <_ITM_registerTMCloneTable>
    116b:	48 85 c0             	test   %rax,%rax
    116e:	74 08                	je     1178 <register_tm_clones+0x38>
    1170:	ff e0                	jmpq   *%rax
    1172:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    1178:	c3                   	retq   
    1179:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001180 <__do_global_dtors_aux>:
    1180:	f3 0f 1e fa          	endbr64 
    1184:	80 3d ed 30 00 00 00 	cmpb   $0x0,0x30ed(%rip)        # 4278 <completed.8061>
    118b:	75 2b                	jne    11b8 <__do_global_dtors_aux+0x38>
    118d:	55                   	push   %rbp
    118e:	48 83 3d 32 2e 00 00 	cmpq   $0x0,0x2e32(%rip)        # 3fc8 <__cxa_finalize@GLIBC_2.2.5>
    1195:	00 
    1196:	48 89 e5             	mov    %rsp,%rbp
    1199:	74 0c                	je     11a7 <__do_global_dtors_aux+0x27>
    119b:	48 8b 3d 66 2e 00 00 	mov    0x2e66(%rip),%rdi        # 4008 <__dso_handle>
    11a2:	e8 d9 fe ff ff       	callq  1080 <__cxa_finalize@plt>
    11a7:	e8 64 ff ff ff       	callq  1110 <deregister_tm_clones>
    11ac:	c6 05 c5 30 00 00 01 	movb   $0x1,0x30c5(%rip)        # 4278 <completed.8061>
    11b3:	5d                   	pop    %rbp
    11b4:	c3                   	retq   
    11b5:	0f 1f 00             	nopl   (%rax)
    11b8:	c3                   	retq   
    11b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000011c0 <frame_dummy>:
    11c0:	f3 0f 1e fa          	endbr64 
    11c4:	e9 77 ff ff ff       	jmpq   1140 <register_tm_clones>

00000000000011c9 <_Z6myplusii>:
    11c9:	f3 0f 1e fa          	endbr64 
    11cd:	55                   	push   %rbp
    11ce:	48 89 e5             	mov    %rsp,%rbp
    11d1:	89 7d fc             	mov    %edi,-0x4(%rbp)
    11d4:	89 75 f8             	mov    %esi,-0x8(%rbp)
    11d7:	8b 55 fc             	mov    -0x4(%rbp),%edx
    11da:	8b 45 f8             	mov    -0x8(%rbp),%eax
    11dd:	01 d0                	add    %edx,%eax
    11df:	5d                   	pop    %rbp
    11e0:	c3                   	retq   

00000000000011e1 <main>:
    11e1:	f3 0f 1e fa          	endbr64 
    11e5:	55                   	push   %rbp
    11e6:	48 89 e5             	mov    %rsp,%rbp
    11e9:	48 83 ec 20          	sub    $0x20,%rsp
    11ed:	c7 45 ec 00 00 00 00 	movl   $0x0,-0x14(%rbp)
    11f4:	c7 45 f0 01 00 00 00 	movl   $0x1,-0x10(%rbp)
    11fb:	c7 45 f4 01 00 00 00 	movl   $0x1,-0xc(%rbp)
    1202:	c7 45 f8 01 00 00 00 	movl   $0x1,-0x8(%rbp)
    1209:	48 8d 35 6c 30 00 00 	lea    0x306c(%rip),%rsi        # 427c <globe_n>
    1210:	48 8d 3d 49 2f 00 00 	lea    0x2f49(%rip),%rdi        # 4160 <_ZSt3cin@@GLIBCXX_3.4>
    1217:	e8 74 fe ff ff       	callq  1090 <_ZNSirsERi@plt>
    121c:	8b 45 ec             	mov    -0x14(%rbp),%eax
    121f:	89 c6                	mov    %eax,%esi
    1221:	48 8d 3d 18 2e 00 00 	lea    0x2e18(%rip),%rdi        # 4040 <_ZSt4cout@@GLIBCXX_3.4>
    1228:	e8 a3 fe ff ff       	callq  10d0 <_ZNSolsEi@plt>
    122d:	48 89 c2             	mov    %rax,%rdx
    1230:	48 8b 05 99 2d 00 00 	mov    0x2d99(%rip),%rax        # 3fd0 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GLIBCXX_3.4>
    1237:	48 89 c6             	mov    %rax,%rsi
    123a:	48 89 d7             	mov    %rdx,%rdi
    123d:	e8 6e fe ff ff       	callq  10b0 <_ZNSolsEPFRSoS_E@plt>
    1242:	8b 45 f0             	mov    -0x10(%rbp),%eax
    1245:	89 c6                	mov    %eax,%esi
    1247:	48 8d 3d f2 2d 00 00 	lea    0x2df2(%rip),%rdi        # 4040 <_ZSt4cout@@GLIBCXX_3.4>
    124e:	e8 7d fe ff ff       	callq  10d0 <_ZNSolsEi@plt>
    1253:	48 89 c2             	mov    %rax,%rdx
    1256:	48 8b 05 73 2d 00 00 	mov    0x2d73(%rip),%rax        # 3fd0 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GLIBCXX_3.4>
    125d:	48 89 c6             	mov    %rax,%rsi
    1260:	48 89 d7             	mov    %rdx,%rdi
    1263:	e8 48 fe ff ff       	callq  10b0 <_ZNSolsEPFRSoS_E@plt>
    1268:	8b 05 0e 30 00 00    	mov    0x300e(%rip),%eax        # 427c <globe_n>
    126e:	39 45 f4             	cmp    %eax,-0xc(%rbp)
    1271:	7d 4a                	jge    12bd <main+0xdc>
    1273:	8b 45 f0             	mov    -0x10(%rbp),%eax
    1276:	89 45 fc             	mov    %eax,-0x4(%rbp)
    1279:	8b 55 f0             	mov    -0x10(%rbp),%edx
    127c:	8b 45 ec             	mov    -0x14(%rbp),%eax
    127f:	89 d6                	mov    %edx,%esi
    1281:	89 c7                	mov    %eax,%edi
    1283:	e8 41 ff ff ff       	callq  11c9 <_Z6myplusii>
    1288:	89 45 f0             	mov    %eax,-0x10(%rbp)
    128b:	8b 45 f0             	mov    -0x10(%rbp),%eax
    128e:	89 c6                	mov    %eax,%esi
    1290:	48 8d 3d a9 2d 00 00 	lea    0x2da9(%rip),%rdi        # 4040 <_ZSt4cout@@GLIBCXX_3.4>
    1297:	e8 34 fe ff ff       	callq  10d0 <_ZNSolsEi@plt>
    129c:	48 89 c2             	mov    %rax,%rdx
    129f:	48 8b 05 2a 2d 00 00 	mov    0x2d2a(%rip),%rax        # 3fd0 <_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GLIBCXX_3.4>
    12a6:	48 89 c6             	mov    %rax,%rsi
    12a9:	48 89 d7             	mov    %rdx,%rdi
    12ac:	e8 ff fd ff ff       	callq  10b0 <_ZNSolsEPFRSoS_E@plt>
    12b1:	8b 45 fc             	mov    -0x4(%rbp),%eax
    12b4:	89 45 ec             	mov    %eax,-0x14(%rbp)
    12b7:	83 45 f4 01          	addl   $0x1,-0xc(%rbp)
    12bb:	eb ab                	jmp    1268 <main+0x87>
    12bd:	b8 00 00 00 00       	mov    $0x0,%eax
    12c2:	c9                   	leaveq 
    12c3:	c3                   	retq   

00000000000012c4 <_Z41__static_initialization_and_destruction_0ii>:
    12c4:	f3 0f 1e fa          	endbr64 
    12c8:	55                   	push   %rbp
    12c9:	48 89 e5             	mov    %rsp,%rbp
    12cc:	48 83 ec 10          	sub    $0x10,%rsp
    12d0:	89 7d fc             	mov    %edi,-0x4(%rbp)
    12d3:	89 75 f8             	mov    %esi,-0x8(%rbp)
    12d6:	83 7d fc 01          	cmpl   $0x1,-0x4(%rbp)
    12da:	75 32                	jne    130e <_Z41__static_initialization_and_destruction_0ii+0x4a>
    12dc:	81 7d f8 ff ff 00 00 	cmpl   $0xffff,-0x8(%rbp)
    12e3:	75 29                	jne    130e <_Z41__static_initialization_and_destruction_0ii+0x4a>
    12e5:	48 8d 3d 94 2f 00 00 	lea    0x2f94(%rip),%rdi        # 4280 <_ZStL8__ioinit>
    12ec:	e8 cf fd ff ff       	callq  10c0 <_ZNSt8ios_base4InitC1Ev@plt>
    12f1:	48 8d 15 10 2d 00 00 	lea    0x2d10(%rip),%rdx        # 4008 <__dso_handle>
    12f8:	48 8d 35 81 2f 00 00 	lea    0x2f81(%rip),%rsi        # 4280 <_ZStL8__ioinit>
    12ff:	48 8b 05 f2 2c 00 00 	mov    0x2cf2(%rip),%rax        # 3ff8 <_ZNSt8ios_base4InitD1Ev@GLIBCXX_3.4>
    1306:	48 89 c7             	mov    %rax,%rdi
    1309:	e8 92 fd ff ff       	callq  10a0 <__cxa_atexit@plt>
    130e:	90                   	nop
    130f:	c9                   	leaveq 
    1310:	c3                   	retq   

0000000000001311 <_GLOBAL__sub_I_globe_n>:
    1311:	f3 0f 1e fa          	endbr64 
    1315:	55                   	push   %rbp
    1316:	48 89 e5             	mov    %rsp,%rbp
    1319:	be ff ff 00 00       	mov    $0xffff,%esi
    131e:	bf 01 00 00 00       	mov    $0x1,%edi
    1323:	e8 9c ff ff ff       	callq  12c4 <_Z41__static_initialization_and_destruction_0ii>
    1328:	5d                   	pop    %rbp
    1329:	c3                   	retq   
    132a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000001330 <__libc_csu_init>:
    1330:	f3 0f 1e fa          	endbr64 
    1334:	41 57                	push   %r15
    1336:	4c 8d 3d 33 2a 00 00 	lea    0x2a33(%rip),%r15        # 3d70 <__frame_dummy_init_array_entry>
    133d:	41 56                	push   %r14
    133f:	49 89 d6             	mov    %rdx,%r14
    1342:	41 55                	push   %r13
    1344:	49 89 f5             	mov    %rsi,%r13
    1347:	41 54                	push   %r12
    1349:	41 89 fc             	mov    %edi,%r12d
    134c:	55                   	push   %rbp
    134d:	48 8d 2d 2c 2a 00 00 	lea    0x2a2c(%rip),%rbp        # 3d80 <__do_global_dtors_aux_fini_array_entry>
    1354:	53                   	push   %rbx
    1355:	4c 29 fd             	sub    %r15,%rbp
    1358:	48 83 ec 08          	sub    $0x8,%rsp
    135c:	e8 9f fc ff ff       	callq  1000 <_init>
    1361:	48 c1 fd 03          	sar    $0x3,%rbp
    1365:	74 1f                	je     1386 <__libc_csu_init+0x56>
    1367:	31 db                	xor    %ebx,%ebx
    1369:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    1370:	4c 89 f2             	mov    %r14,%rdx
    1373:	4c 89 ee             	mov    %r13,%rsi
    1376:	44 89 e7             	mov    %r12d,%edi
    1379:	41 ff 14 df          	callq  *(%r15,%rbx,8)
    137d:	48 83 c3 01          	add    $0x1,%rbx
    1381:	48 39 dd             	cmp    %rbx,%rbp
    1384:	75 ea                	jne    1370 <__libc_csu_init+0x40>
    1386:	48 83 c4 08          	add    $0x8,%rsp
    138a:	5b                   	pop    %rbx
    138b:	5d                   	pop    %rbp
    138c:	41 5c                	pop    %r12
    138e:	41 5d                	pop    %r13
    1390:	41 5e                	pop    %r14
    1392:	41 5f                	pop    %r15
    1394:	c3                   	retq   
    1395:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
    139c:	00 00 00 00 

00000000000013a0 <__libc_csu_fini>:
    13a0:	f3 0f 1e fa          	endbr64 
    13a4:	c3                   	retq   

Disassembly of section .fini:

00000000000013a8 <_fini>:
    13a8:	f3 0f 1e fa          	endbr64 
    13ac:	48 83 ec 08          	sub    $0x8,%rsp
    13b0:	48 83 c4 08          	add    $0x8,%rsp
    13b4:	c3                   	retq   
