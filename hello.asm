
hello.elf:     file format elf64-littleriscv


Disassembly of section .text.init:

0000000000000000 <_start>:
   0:	06c0006f          	j	6c <main>

Disassembly of section .text.getpid:

0000000000000004 <getpid>:
   4:	fe010113          	addi	sp,sp,-32
   8:	00813c23          	sd	s0,24(sp)
   c:	02010413          	addi	s0,sp,32
  10:	fe843783          	ld	a5,-24(s0)
  14:	0ac00893          	li	a7,172
  18:	00000073          	ecall
  1c:	00050793          	mv	a5,a0
  20:	fef43423          	sd	a5,-24(s0)
  24:	fe843783          	ld	a5,-24(s0)
  28:	00078513          	mv	a0,a5
  2c:	01813403          	ld	s0,24(sp)
  30:	02010113          	addi	sp,sp,32
  34:	00008067          	ret

Disassembly of section .text.fork:

0000000000000038 <fork>:
  38:	fe010113          	addi	sp,sp,-32
  3c:	00813c23          	sd	s0,24(sp)
  40:	02010413          	addi	s0,sp,32
  44:	fe843783          	ld	a5,-24(s0)
  48:	0dc00893          	li	a7,220
  4c:	00000073          	ecall
  50:	00050793          	mv	a5,a0
  54:	fef43423          	sd	a5,-24(s0)
  58:	fe843783          	ld	a5,-24(s0)
  5c:	00078513          	mv	a0,a5
  60:	01813403          	ld	s0,24(sp)
  64:	02010113          	addi	sp,sp,32
  68:	00008067          	ret

Disassembly of section .text.main:

000000000000006c <main>:
  6c:	fe010113          	addi	sp,sp,-32
  70:	00113c23          	sd	ra,24(sp)
  74:	00813823          	sd	s0,16(sp)
  78:	02010413          	addi	s0,sp,32
  7c:	fbdff0ef          	jal	ra,38 <fork>
  80:	fea43023          	sd	a0,-32(s0)
  84:	fb5ff0ef          	jal	ra,38 <fork>
  88:	fea43023          	sd	a0,-32(s0)
    8c:	f79ff0ef          	jal	ra,4 <getpid>
    90:	00050793          	mv	a5,a0
    94:	00010713          	mv	a4,sp
    98:	00070613          	mv	a2,a4
    9c:	00078593          	mv	a1,a5
    a0:	73000513          	li	a0,1840
    a4:	600000ef          	jal	ra,6a4 <printf>
    a8:	fe042623          	sw	zero,-20(s0)
    ac:	0100006f          	j	bc <main+0x50>
    b0:	fec42783          	lw	a5,-20(s0)
    b4:	0017879b          	addiw	a5,a5,1
    b8:	fef42623          	sw	a5,-20(s0)
    bc:	fec42783          	lw	a5,-20(s0)
    c0:	0007871b          	sext.w	a4,a5
    c4:	fff00793          	li	a5,-1
    c8:	fef714e3          	bne	a4,a5,b0 <main+0x44>
    cc:	fc1ff06f          	j	8c <main+0x20>

Disassembly of section .text.putchar:

00000000000000d0 <putchar>:
  d0:	fe010113          	addi	sp,sp,-32
  d4:	00813c23          	sd	s0,24(sp)
  d8:	02010413          	addi	s0,sp,32
  dc:	00050793          	mv	a5,a0
  e0:	fef42623          	sw	a5,-20(s0)
  e4:	7a400793          	li	a5,1956
  e8:	0007a783          	lw	a5,0(a5)
  ec:	0017871b          	addiw	a4,a5,1
  f0:	0007069b          	sext.w	a3,a4
  f4:	7a400713          	li	a4,1956
  f8:	00d72023          	sw	a3,0(a4)
  fc:	fec42703          	lw	a4,-20(s0)
 100:	0ff77713          	andi	a4,a4,255
 104:	7a800693          	li	a3,1960
 108:	00f687b3          	add	a5,a3,a5
 10c:	00e78023          	sb	a4,0(a5)
 110:	00000793          	li	a5,0
 114:	00078513          	mv	a0,a5
 118:	01813403          	ld	s0,24(sp)
 11c:	02010113          	addi	sp,sp,32
 120:	00008067          	ret

Disassembly of section .text.vprintfmt:

0000000000000124 <vprintfmt>:
 124:	f1010113          	addi	sp,sp,-240
 128:	0e113423          	sd	ra,232(sp)
 12c:	0e813023          	sd	s0,224(sp)
 130:	0f010413          	addi	s0,sp,240
 134:	f2a43423          	sd	a0,-216(s0)
 138:	f2b43023          	sd	a1,-224(s0)
 13c:	f0c43c23          	sd	a2,-232(s0)
 140:	fe042623          	sw	zero,-20(s0)
 144:	fe042423          	sw	zero,-24(s0)
 148:	fe043023          	sd	zero,-32(s0)
 14c:	4d40006f          	j	620 <vprintfmt+0x4fc>
 150:	fec42783          	lw	a5,-20(s0)
 154:	0007879b          	sext.w	a5,a5
 158:	46078863          	beqz	a5,5c8 <vprintfmt+0x4a4>
 15c:	f2043783          	ld	a5,-224(s0)
 160:	0007c783          	lbu	a5,0(a5)
 164:	0007879b          	sext.w	a5,a5
 168:	f9d7869b          	addiw	a3,a5,-99
 16c:	0006871b          	sext.w	a4,a3
 170:	01500793          	li	a5,21
 174:	48e7ee63          	bltu	a5,a4,610 <vprintfmt+0x4ec>
 178:	02069793          	slli	a5,a3,0x20
 17c:	0207d793          	srli	a5,a5,0x20
 180:	00279713          	slli	a4,a5,0x2
 184:	74c00793          	li	a5,1868
 188:	00f707b3          	add	a5,a4,a5
 18c:	0007a783          	lw	a5,0(a5)
 190:	0007871b          	sext.w	a4,a5
 194:	74c00793          	li	a5,1868
 198:	00f707b3          	add	a5,a4,a5
 19c:	00078067          	jr	a5
 1a0:	00100793          	li	a5,1
 1a4:	fef42423          	sw	a5,-24(s0)
 1a8:	46c0006f          	j	614 <vprintfmt+0x4f0>
 1ac:	fe842783          	lw	a5,-24(s0)
 1b0:	0007879b          	sext.w	a5,a5
 1b4:	00078c63          	beqz	a5,1cc <vprintfmt+0xa8>
 1b8:	f1843783          	ld	a5,-232(s0)
 1bc:	00878713          	addi	a4,a5,8
 1c0:	f0e43c23          	sd	a4,-232(s0)
 1c4:	0007b783          	ld	a5,0(a5)
 1c8:	0140006f          	j	1dc <vprintfmt+0xb8>
 1cc:	f1843783          	ld	a5,-232(s0)
 1d0:	00878713          	addi	a4,a5,8
 1d4:	f0e43c23          	sd	a4,-232(s0)
 1d8:	0007a783          	lw	a5,0(a5)
 1dc:	f8f43423          	sd	a5,-120(s0)
 1e0:	fe842783          	lw	a5,-24(s0)
 1e4:	0007879b          	sext.w	a5,a5
 1e8:	00078663          	beqz	a5,1f4 <vprintfmt+0xd0>
 1ec:	00f00793          	li	a5,15
 1f0:	0080006f          	j	1f8 <vprintfmt+0xd4>
 1f4:	00700793          	li	a5,7
 1f8:	f8f42223          	sw	a5,-124(s0)
 1fc:	f8442783          	lw	a5,-124(s0)
 200:	fcf42e23          	sw	a5,-36(s0)
 204:	0880006f          	j	28c <vprintfmt+0x168>
 208:	fdc42783          	lw	a5,-36(s0)
 20c:	0027979b          	slliw	a5,a5,0x2
 210:	0007879b          	sext.w	a5,a5
 214:	f8843703          	ld	a4,-120(s0)
 218:	40f757b3          	sra	a5,a4,a5
 21c:	0007879b          	sext.w	a5,a5
 220:	00f7f793          	andi	a5,a5,15
 224:	f8f42023          	sw	a5,-128(s0)
 228:	f8042783          	lw	a5,-128(s0)
 22c:	0007871b          	sext.w	a4,a5
 230:	00900793          	li	a5,9
 234:	00e7cc63          	blt	a5,a4,24c <vprintfmt+0x128>
 238:	f8042783          	lw	a5,-128(s0)
 23c:	0ff7f793          	andi	a5,a5,255
 240:	0307879b          	addiw	a5,a5,48
 244:	0ff7f793          	andi	a5,a5,255
 248:	0140006f          	j	25c <vprintfmt+0x138>
 24c:	f8042783          	lw	a5,-128(s0)
 250:	0ff7f793          	andi	a5,a5,255
 254:	0577879b          	addiw	a5,a5,87
 258:	0ff7f793          	andi	a5,a5,255
 25c:	f6f40fa3          	sb	a5,-129(s0)
 260:	f7f44783          	lbu	a5,-129(s0)
 264:	0007879b          	sext.w	a5,a5
 268:	f2843703          	ld	a4,-216(s0)
 26c:	00078513          	mv	a0,a5
 270:	000700e7          	jalr	a4
 274:	fe043783          	ld	a5,-32(s0)
 278:	00178793          	addi	a5,a5,1
 27c:	fef43023          	sd	a5,-32(s0)
 280:	fdc42783          	lw	a5,-36(s0)
 284:	fff7879b          	addiw	a5,a5,-1
 288:	fcf42e23          	sw	a5,-36(s0)
 28c:	fdc42783          	lw	a5,-36(s0)
 290:	0007879b          	sext.w	a5,a5
 294:	f607dae3          	bgez	a5,208 <vprintfmt+0xe4>
 298:	fe042423          	sw	zero,-24(s0)
 29c:	fe042623          	sw	zero,-20(s0)
 2a0:	3740006f          	j	614 <vprintfmt+0x4f0>
 2a4:	fe842783          	lw	a5,-24(s0)
 2a8:	0007879b          	sext.w	a5,a5
 2ac:	00078c63          	beqz	a5,2c4 <vprintfmt+0x1a0>
 2b0:	f1843783          	ld	a5,-232(s0)
 2b4:	00878713          	addi	a4,a5,8
 2b8:	f0e43c23          	sd	a4,-232(s0)
 2bc:	0007b783          	ld	a5,0(a5)
 2c0:	0140006f          	j	2d4 <vprintfmt+0x1b0>
 2c4:	f1843783          	ld	a5,-232(s0)
 2c8:	00878713          	addi	a4,a5,8
 2cc:	f0e43c23          	sd	a4,-232(s0)
 2d0:	0007a783          	lw	a5,0(a5)
 2d4:	fcf43823          	sd	a5,-48(s0)
 2d8:	fd043783          	ld	a5,-48(s0)
 2dc:	0207d463          	bgez	a5,304 <vprintfmt+0x1e0>
 2e0:	fd043783          	ld	a5,-48(s0)
 2e4:	40f007b3          	neg	a5,a5
 2e8:	fcf43823          	sd	a5,-48(s0)
 2ec:	f2843783          	ld	a5,-216(s0)
 2f0:	02d00513          	li	a0,45
 2f4:	000780e7          	jalr	a5
 2f8:	fe043783          	ld	a5,-32(s0)
 2fc:	00178793          	addi	a5,a5,1
 300:	fef43023          	sd	a5,-32(s0)
 304:	fc042623          	sw	zero,-52(s0)
 308:	03000793          	li	a5,48
 30c:	f4f43823          	sd	a5,-176(s0)
 310:	f4043c23          	sd	zero,-168(s0)
 314:	f6043023          	sd	zero,-160(s0)
 318:	f6040423          	sb	zero,-152(s0)
 31c:	fd043783          	ld	a5,-48(s0)
 320:	fcf43023          	sd	a5,-64(s0)
 324:	0480006f          	j	36c <vprintfmt+0x248>
 328:	fc043703          	ld	a4,-64(s0)
 32c:	00a00793          	li	a5,10
 330:	02f767b3          	rem	a5,a4,a5
 334:	0ff7f793          	andi	a5,a5,255
 338:	0307879b          	addiw	a5,a5,48
 33c:	0ff7f713          	andi	a4,a5,255
 340:	fcc42783          	lw	a5,-52(s0)
 344:	ff040693          	addi	a3,s0,-16
 348:	00f687b3          	add	a5,a3,a5
 34c:	f6e78023          	sb	a4,-160(a5)
 350:	fc043703          	ld	a4,-64(s0)
 354:	00a00793          	li	a5,10
 358:	02f747b3          	div	a5,a4,a5
 35c:	fcf43023          	sd	a5,-64(s0)
 360:	fcc42783          	lw	a5,-52(s0)
 364:	0017879b          	addiw	a5,a5,1
 368:	fcf42623          	sw	a5,-52(s0)
 36c:	fc043783          	ld	a5,-64(s0)
 370:	fa079ce3          	bnez	a5,328 <vprintfmt+0x204>
 374:	fcc42783          	lw	a5,-52(s0)
 378:	0007879b          	sext.w	a5,a5
 37c:	00079863          	bnez	a5,38c <vprintfmt+0x268>
 380:	fcc42783          	lw	a5,-52(s0)
 384:	0017879b          	addiw	a5,a5,1
 388:	fcf42623          	sw	a5,-52(s0)
 38c:	fcc42783          	lw	a5,-52(s0)
 390:	fff7879b          	addiw	a5,a5,-1
 394:	faf42e23          	sw	a5,-68(s0)
 398:	0300006f          	j	3c8 <vprintfmt+0x2a4>
 39c:	fbc42783          	lw	a5,-68(s0)
 3a0:	ff040713          	addi	a4,s0,-16
 3a4:	00f707b3          	add	a5,a4,a5
 3a8:	f607c783          	lbu	a5,-160(a5)
 3ac:	0007879b          	sext.w	a5,a5
 3b0:	f2843703          	ld	a4,-216(s0)
 3b4:	00078513          	mv	a0,a5
 3b8:	000700e7          	jalr	a4
 3bc:	fbc42783          	lw	a5,-68(s0)
 3c0:	fff7879b          	addiw	a5,a5,-1
 3c4:	faf42e23          	sw	a5,-68(s0)
 3c8:	fbc42783          	lw	a5,-68(s0)
 3cc:	0007879b          	sext.w	a5,a5
 3d0:	fc07d6e3          	bgez	a5,39c <vprintfmt+0x278>
 3d4:	fcc42783          	lw	a5,-52(s0)
 3d8:	0017879b          	addiw	a5,a5,1
 3dc:	0007879b          	sext.w	a5,a5
 3e0:	00078713          	mv	a4,a5
 3e4:	fe043783          	ld	a5,-32(s0)
 3e8:	00e787b3          	add	a5,a5,a4
 3ec:	fef43023          	sd	a5,-32(s0)
 3f0:	fe042423          	sw	zero,-24(s0)
 3f4:	fe042623          	sw	zero,-20(s0)
 3f8:	21c0006f          	j	614 <vprintfmt+0x4f0>
 3fc:	fe842783          	lw	a5,-24(s0)
 400:	0007879b          	sext.w	a5,a5
 404:	00078c63          	beqz	a5,41c <vprintfmt+0x2f8>
 408:	f1843783          	ld	a5,-232(s0)
 40c:	00878713          	addi	a4,a5,8
 410:	f0e43c23          	sd	a4,-232(s0)
 414:	0007b783          	ld	a5,0(a5)
 418:	0140006f          	j	42c <vprintfmt+0x308>
 41c:	f1843783          	ld	a5,-232(s0)
 420:	00878713          	addi	a4,a5,8
 424:	f0e43c23          	sd	a4,-232(s0)
 428:	0007a783          	lw	a5,0(a5)
 42c:	f6f43823          	sd	a5,-144(s0)
 430:	fa042c23          	sw	zero,-72(s0)
 434:	03000793          	li	a5,48
 438:	f2f43823          	sd	a5,-208(s0)
 43c:	f2043c23          	sd	zero,-200(s0)
 440:	f4043023          	sd	zero,-192(s0)
 444:	f4040423          	sb	zero,-184(s0)
 448:	f7043783          	ld	a5,-144(s0)
 44c:	faf43823          	sd	a5,-80(s0)
 450:	0480006f          	j	498 <vprintfmt+0x374>
 454:	fb043703          	ld	a4,-80(s0)
 458:	00a00793          	li	a5,10
 45c:	02f767b3          	rem	a5,a4,a5
 460:	0ff7f793          	andi	a5,a5,255
 464:	0307879b          	addiw	a5,a5,48
 468:	0ff7f713          	andi	a4,a5,255
 46c:	fb842783          	lw	a5,-72(s0)
 470:	ff040693          	addi	a3,s0,-16
 474:	00f687b3          	add	a5,a3,a5
 478:	f4e78023          	sb	a4,-192(a5)
 47c:	fb043703          	ld	a4,-80(s0)
 480:	00a00793          	li	a5,10
 484:	02f747b3          	div	a5,a4,a5
 488:	faf43823          	sd	a5,-80(s0)
 48c:	fb842783          	lw	a5,-72(s0)
 490:	0017879b          	addiw	a5,a5,1
 494:	faf42c23          	sw	a5,-72(s0)
 498:	fb043783          	ld	a5,-80(s0)
 49c:	fa079ce3          	bnez	a5,454 <vprintfmt+0x330>
 4a0:	fb842783          	lw	a5,-72(s0)
 4a4:	0007879b          	sext.w	a5,a5
 4a8:	00079863          	bnez	a5,4b8 <vprintfmt+0x394>
 4ac:	fb842783          	lw	a5,-72(s0)
 4b0:	0017879b          	addiw	a5,a5,1
 4b4:	faf42c23          	sw	a5,-72(s0)
 4b8:	fb842783          	lw	a5,-72(s0)
 4bc:	fff7879b          	addiw	a5,a5,-1
 4c0:	faf42623          	sw	a5,-84(s0)
 4c4:	0300006f          	j	4f4 <vprintfmt+0x3d0>
 4c8:	fac42783          	lw	a5,-84(s0)
 4cc:	ff040713          	addi	a4,s0,-16
 4d0:	00f707b3          	add	a5,a4,a5
 4d4:	f407c783          	lbu	a5,-192(a5)
 4d8:	0007879b          	sext.w	a5,a5
 4dc:	f2843703          	ld	a4,-216(s0)
 4e0:	00078513          	mv	a0,a5
 4e4:	000700e7          	jalr	a4
 4e8:	fac42783          	lw	a5,-84(s0)
 4ec:	fff7879b          	addiw	a5,a5,-1
 4f0:	faf42623          	sw	a5,-84(s0)
 4f4:	fac42783          	lw	a5,-84(s0)
 4f8:	0007879b          	sext.w	a5,a5
 4fc:	fc07d6e3          	bgez	a5,4c8 <vprintfmt+0x3a4>
 500:	fb842783          	lw	a5,-72(s0)
 504:	fff7879b          	addiw	a5,a5,-1
 508:	0007879b          	sext.w	a5,a5
 50c:	00078713          	mv	a4,a5
 510:	fe043783          	ld	a5,-32(s0)
 514:	00e787b3          	add	a5,a5,a4
 518:	fef43023          	sd	a5,-32(s0)
 51c:	fe042423          	sw	zero,-24(s0)
 520:	fe042623          	sw	zero,-20(s0)
 524:	0f00006f          	j	614 <vprintfmt+0x4f0>
 528:	f1843783          	ld	a5,-232(s0)
 52c:	00878713          	addi	a4,a5,8
 530:	f0e43c23          	sd	a4,-232(s0)
 534:	0007b783          	ld	a5,0(a5)
 538:	faf43023          	sd	a5,-96(s0)
 53c:	0340006f          	j	570 <vprintfmt+0x44c>
 540:	fa043783          	ld	a5,-96(s0)
 544:	0007c783          	lbu	a5,0(a5)
 548:	0007879b          	sext.w	a5,a5
 54c:	f2843703          	ld	a4,-216(s0)
 550:	00078513          	mv	a0,a5
 554:	000700e7          	jalr	a4
 558:	fe043783          	ld	a5,-32(s0)
 55c:	00178793          	addi	a5,a5,1
 560:	fef43023          	sd	a5,-32(s0)
 564:	fa043783          	ld	a5,-96(s0)
 568:	00178793          	addi	a5,a5,1
 56c:	faf43023          	sd	a5,-96(s0)
 570:	fa043783          	ld	a5,-96(s0)
 574:	0007c783          	lbu	a5,0(a5)
 578:	fc0794e3          	bnez	a5,540 <vprintfmt+0x41c>
 57c:	fe042423          	sw	zero,-24(s0)
 580:	fe042623          	sw	zero,-20(s0)
 584:	0900006f          	j	614 <vprintfmt+0x4f0>
 588:	f1843783          	ld	a5,-232(s0)
 58c:	00878713          	addi	a4,a5,8
 590:	f0e43c23          	sd	a4,-232(s0)
 594:	0007a783          	lw	a5,0(a5)
 598:	f6f407a3          	sb	a5,-145(s0)
 59c:	f6f44783          	lbu	a5,-145(s0)
 5a0:	0007879b          	sext.w	a5,a5
 5a4:	f2843703          	ld	a4,-216(s0)
 5a8:	00078513          	mv	a0,a5
 5ac:	000700e7          	jalr	a4
 5b0:	fe043783          	ld	a5,-32(s0)
 5b4:	00178793          	addi	a5,a5,1
 5b8:	fef43023          	sd	a5,-32(s0)
 5bc:	fe042423          	sw	zero,-24(s0)
 5c0:	fe042623          	sw	zero,-20(s0)
 5c4:	0500006f          	j	614 <vprintfmt+0x4f0>
 5c8:	f2043783          	ld	a5,-224(s0)
 5cc:	0007c783          	lbu	a5,0(a5)
 5d0:	00078713          	mv	a4,a5
 5d4:	02500793          	li	a5,37
 5d8:	00f71863          	bne	a4,a5,5e8 <vprintfmt+0x4c4>
 5dc:	00100793          	li	a5,1
 5e0:	fef42623          	sw	a5,-20(s0)
 5e4:	0300006f          	j	614 <vprintfmt+0x4f0>
 5e8:	f2043783          	ld	a5,-224(s0)
 5ec:	0007c783          	lbu	a5,0(a5)
 5f0:	0007879b          	sext.w	a5,a5
 5f4:	f2843703          	ld	a4,-216(s0)
 5f8:	00078513          	mv	a0,a5
 5fc:	000700e7          	jalr	a4
 600:	fe043783          	ld	a5,-32(s0)
 604:	00178793          	addi	a5,a5,1
 608:	fef43023          	sd	a5,-32(s0)
 60c:	0080006f          	j	614 <vprintfmt+0x4f0>
 610:	00000013          	nop
 614:	f2043783          	ld	a5,-224(s0)
 618:	00178793          	addi	a5,a5,1
 61c:	f2f43023          	sd	a5,-224(s0)
 620:	f2043783          	ld	a5,-224(s0)
 624:	0007c783          	lbu	a5,0(a5)
 628:	b20794e3          	bnez	a5,150 <vprintfmt+0x2c>
 62c:	00100793          	li	a5,1
 630:	f8f43c23          	sd	a5,-104(s0)
 634:	7a400793          	li	a5,1956
 638:	0007a783          	lw	a5,0(a5)
 63c:	0017871b          	addiw	a4,a5,1
 640:	0007069b          	sext.w	a3,a4
 644:	7a400713          	li	a4,1956
 648:	00d72023          	sw	a3,0(a4)
 64c:	7a800713          	li	a4,1960
 650:	00f707b3          	add	a5,a4,a5
 654:	00078023          	sb	zero,0(a5)
 658:	7a400793          	li	a5,1956
 65c:	0007a603          	lw	a2,0(a5)
 660:	f9843703          	ld	a4,-104(s0)
 664:	7a800693          	li	a3,1960
 668:	f9043783          	ld	a5,-112(s0)
 66c:	04000893          	li	a7,64
 670:	00070513          	mv	a0,a4
 674:	00068593          	mv	a1,a3
 678:	00060613          	mv	a2,a2
 67c:	00000073          	ecall
 680:	00050793          	mv	a5,a0
 684:	f8f43823          	sd	a5,-112(s0)
 688:	f9043783          	ld	a5,-112(s0)
 68c:	0007879b          	sext.w	a5,a5
 690:	00078513          	mv	a0,a5
 694:	0e813083          	ld	ra,232(sp)
 698:	0e013403          	ld	s0,224(sp)
 69c:	0f010113          	addi	sp,sp,240
 6a0:	00008067          	ret

Disassembly of section .text.printf:

00000000000006a4 <printf>:
 6a4:	f9010113          	addi	sp,sp,-112
 6a8:	02113423          	sd	ra,40(sp)
 6ac:	02813023          	sd	s0,32(sp)
 6b0:	03010413          	addi	s0,sp,48
 6b4:	fca43c23          	sd	a0,-40(s0)
 6b8:	00b43423          	sd	a1,8(s0)
 6bc:	00c43823          	sd	a2,16(s0)
 6c0:	00d43c23          	sd	a3,24(s0)
 6c4:	02e43023          	sd	a4,32(s0)
 6c8:	02f43423          	sd	a5,40(s0)
 6cc:	03043823          	sd	a6,48(s0)
 6d0:	03143c23          	sd	a7,56(s0)
 6d4:	fe042623          	sw	zero,-20(s0)
 6d8:	04040793          	addi	a5,s0,64
 6dc:	fcf43823          	sd	a5,-48(s0)
 6e0:	fd043783          	ld	a5,-48(s0)
 6e4:	fc878793          	addi	a5,a5,-56
 6e8:	fef43023          	sd	a5,-32(s0)
 6ec:	7a400793          	li	a5,1956
 6f0:	0007a023          	sw	zero,0(a5)
 6f4:	fe043783          	ld	a5,-32(s0)
 6f8:	00078613          	mv	a2,a5
 6fc:	fd843583          	ld	a1,-40(s0)
 700:	00000517          	auipc	a0,0x0
 704:	9d050513          	addi	a0,a0,-1584 # d0 <putchar>
 708:	a1dff0ef          	jal	ra,124 <vprintfmt>
 70c:	00050793          	mv	a5,a0
 710:	fef42623          	sw	a5,-20(s0)
 714:	fec42783          	lw	a5,-20(s0)
 718:	00078513          	mv	a0,a5
 71c:	02813083          	ld	ra,40(sp)
 720:	02013403          	ld	s0,32(sp)
 724:	07010113          	addi	sp,sp,112
 728:	00008067          	ret
