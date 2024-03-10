10 REM DHTesp library is uesed
20 REM DHT22 attach to Pin17
100 REM SERIAL INPUT 
200 REM basic call sensor, should be port in hardware-arduino-HTTP.h sensor topic
210 REM check dht exampls
220 REM 1. turn on macro define
230 REM 2. write your sensor.begin 
240 REM 3. assign sensor number
250 REM 4. write sensorread
260 REM for DHTesp
270 REM sensor(sensor#,item)
280 REM DHT sensor is assign to sensor 1
290 REM item 0:pindef 1:humidity 2:temperature
300 d = SENSOR (1,0) 
310 GOSUB 900: REm getkey
320 h = SENSOR(1,1)
330 t = SENSOR(1,2)
340 print "humidity=";h;"% temperature=";t
360 DELAY(2000)
370 GOTO 310
900 C=0
910 if AVAIL(1)=0 Then RETURN
920 GET C:GET R: print C
930 REm is CTRL-C
940 if C='#' then END
940 RETURN

