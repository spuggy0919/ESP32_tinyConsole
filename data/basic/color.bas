1  REM COLOR PALLETE demo
5 GOSUB 1000 :REM CLS
10 FOR I=0 TO 15
20 COLOR I
30 X=80*INT(I%8)
40 Y=60*INT(I/8)
50 FRECT X,Y,80,60
60 NEXT I
65 REM color rgb
70 COLOR 0,0,0
80 FRECT 0,120,80,60
90 COLOR 128,0,0
100 FRECT 80,120,80,60
110 COLOR 0,128,0
120 FRECT 160,120,80,60
130 COLOR 128,128,0
140 FRECT 240,120,80,60
150 COLOR 0,0,128
160 FRECT 320,120,80,60
170 COLOR 128,0,128
180 FRECT 400,120,80,60
190 COLOR 0,128,128
200 FRECT 480,120,80,60
210 COLOR 192,192,192
220 FRECT 560,120,80,60
230 COLOR 128,128,128
240 FRECT 0,180,80,60
250 COLOR 255,0,0
260 FRECT 80,180,80,60
270 COLOR 0,255,0
280 FRECT 160,180,80,60
290 COLOR 255,255,0
300 FRECT 240,180,80,60
310 COLOR 0,0,255
320 FRECT 320,180,80,60
330 COLOR 255,0,255
340 FRECT 400,180,80,60
350 COLOR 0,255,255
360 FRECT 480,180,80,60
370 COLOR 255,255,255
380 FRECT 560,180,80,60
390 REM RGBA
400 COLOR 128,0,0,128
410 FRECT 80,240,80,60
420 COLOR 0,128,0,128
430 FRECT 160,240,80,60
440 COLOR 128,128,0,128
450 FRECT 240,240,80,60
460 COLOR 0,0,128,128
470 FRECT 320,240,80,60
480 COLOR 128,0,128,128
490 FRECT 400,240,80,60
500 COLOR 0,128,128,128
510 FRECT 480,240,80,60
520 COLOR 192,192,192,128
530 FRECT 560,240,80,60
540 COLOR 128,128,128,128
550 FRECT 0,300,80,60
560 COLOR 255,0,0,128
570 FRECT 80,300,80,60
580 COLOR 0,255,0,128
590 FRECT 160,300,80,60
600 COLOR 255,255,0,128
610 FRECT 240,300,80,60
620 COLOR 0,0,255,128
630 FRECT 320,300,80,60
640 COLOR 255,0,255,128
650 FRECT 400,300,80,60
660 COLOR 0,255,255,128
670 FRECT 480,300,80,60
680 COLOR 255,255,255,128
690 FRECT 560,300,80,60
700 STOP :REM use "CONT" to continue
705 GOSUB 1000
710 l= POW(640.0,2.0)+POW(480.0,2)
715 T=20
720 FOR i=0 TO 640 STEP T
730 FOR j=0 TO 480 STEP T
740 if ((i/640.0-j/480.0)>0) then R = int((i/640.0-j/480.0)*255.0) ELSE R=0
750 if ((j/480.0-i/640.0)>0) then B = int((j/480.0-i/640.0)*255.0) ELSE B=0 
760 d = POW(i,2.0)+POW(j,2)
762 h = (sqr(d)-sqr(l)/2.0)
770 if (h>0) then g= int(h/(sqr(l)/2.0)*255.0) else g=0.0 
780 COLOR r,g,B
790 FRECT i,j,T,T
800 NEXT j
810 NEXT i
820 END
1000 COLOR 0:REM BLACK
1010 FRECT 0,0, 640,480
1020 RETURN 

