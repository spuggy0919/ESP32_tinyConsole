10 for i=0 to 20
20 r=int(rnd(1)*255)
30 g=int(rnd(1)*255)
40 b=int(rnd(1)*255)
50 color r,g,b
60 w=int(rnd(1)*320)
70 h=int(rnd(1)*240)
80 REM FRECT 320-w/2, 240-h/2,w,h
85 FCIRCLE 320, 240,h
90 delay 30
100 next I
110 END
