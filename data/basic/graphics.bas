10 REM "demo print for HTTPWSVGA"
20 GOTO 240 
100 GOSUB 500
200 COLOR 2
210 FOR I=0 TO 24
215 GOSUB 600
220 PRINT &2,"Hello World!"
230 NEXT
235 GOSUB 500
240 FOR X=240 TO 20 STEP -10
245 GOSUB 600
250 FCIRCLE 320,240,X
260 NEXT
300 FOR I=0 TO 100 STEP 1
310 GOSUB 600
320 X=int(rnd(1)*639)
330 Y=int(rnd(1)*479)
340 rem if (i%2=0) then FCIRCLE X,Y,40 ELSE FRECT X,Y,80,80
345 FCIRCLE X,Y,40 
350 NEXT
400 FOR I=0 TO 100 STEP 1
410 GOSUB 600
420 X=int(rnd(1)*320)
430 Y=int(rnd(1)*240)
440 X1=int(rnd(1)*319)+320
450 Y1=int(rnd(1)*239)+240
460 FRECT X,Y,X1-X,Y1-Y
470 NEXT
480 END
500 COLOR 2
510 FOR I=0 TO 12
520 PRINT &2,"                   "
530 NEXT
540 RETURN
600 REM true color
620 r=int(rnd(1)*255)
630 g=int(rnd(1)*255)
640 b=int(rnd(1)*255)
650 color r,g,b
660 RETURN
700 REM VGA COLOR 16 PALLETES
710 r=int(rnd(1)*16)
720 color r
730 RETURN
