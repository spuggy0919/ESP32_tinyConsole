10 REM Test http websock touch input
20 REM iTouch Stream is &3
100 FOR i=0 To 10
190 REm get a tuple(E,X,Y), when E>=128
195  REM TOUCH: 128: START,129,MOVE, 130 STOP 132,  MOUSE >128 BUTTON bit 0,1,2 
200 INPUT &3, E
210 IF E=0 THEN GOTO 200
220 INPUT &3, X,Y
230 PRINT E;"(";X;",";Y;")\n"
240 IF E=128 THEN GOTO 200
300 GOSUB 600
310 FCIRCLE X,Y, 30
320 NEXT
330 END
600 REM random Color
620 r=int(rnd(1)*255)
630 g=int(rnd(1)*255)
640 b=int(rnd(1)*255)
650 color r,g,b
660 RETURN

