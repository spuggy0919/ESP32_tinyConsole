100 REm SERIAL #1, and it is linebuf so enter to sent
110 REM ESC can escape 
1000 C=0
1010 IF AVAIL(1)=0 THEN  GOTO 1010
1015 GET C:GET R :PUT C:PUT R
1020 IF C=65 THEN PRINT C : REM if A [enter] print 65 
1030 GOTO 1000