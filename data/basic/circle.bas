100 rem graphics 
101 rem draw a sixteen point on circle 
102 rem connect each point with line
103 gosub 1000
105 pi = 3.14159
110 dim x(16),y(16)
200 r = 200
210 for i = 1 to 16
220 x(i) = r* cos(i*2*PI/16)+320
230 y(i) = r* sin(i*2*PI/16)+240
240 next i
300 for i = 1 to 16
310 for j= i+1 to 16
320 gosub 600
330 line x(i),y(i),x(j),y(j)
340 next j
350 next i
360 end
600 REM TRUECOLOR
620 R=INT(RND(1)*255)
630 G=INT(RND(1)*255)
640 B=INT(RND(1)*255)
650 COLOR R,G,B
660 RETURN
1000 color 15
1010 frect 0,0,639,479
1020 return
