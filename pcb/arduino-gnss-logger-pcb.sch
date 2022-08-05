EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Module:Arduino_Nano_v2.x A1
U 1 1 60F55BF8
P 4200 3100
F 0 "A1" H 4200 2011 50  0000 C CNN
F 1 "Arduino_Nano_v2.x" H 4200 1920 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 4200 3100 50  0001 C CIN
F 3 "https://www.arduino.cc/en/uploads/Main/ArduinoNanoManual23.pdf" H 4200 3100 50  0001 C CNN
	1    4200 3100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x07 J6
U 1 1 60F8B7A3
P 2400 4550
F 0 "J6" H 2318 5067 50  0000 C CNN
F 1 "OledScreen" H 2318 4976 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x07_P2.54mm_Vertical" H 2400 4550 50  0001 C CNN
F 3 "~" H 2400 4550 50  0001 C CNN
	1    2400 4550
	-1   0    0    1   
$EndComp
Wire Wire Line
	3450 3800 3700 3800
Wire Wire Line
	2600 4250 3500 4250
Wire Wire Line
	3500 4250 3500 3500
Wire Wire Line
	3500 3500 3700 3500
Wire Wire Line
	2600 4350 3550 4350
Wire Wire Line
	3550 4350 3550 3400
Wire Wire Line
	3550 3400 3700 3400
Wire Wire Line
	2600 4450 3600 4450
Wire Wire Line
	3600 4450 3600 3300
Wire Wire Line
	3600 3300 3700 3300
Wire Wire Line
	2600 4550 3400 4550
Wire Wire Line
	2600 4650 3450 4650
Wire Wire Line
	3450 4650 3450 3800
Wire Wire Line
	3400 3600 3700 3600
Wire Wire Line
	3400 4550 3400 3600
Wire Wire Line
	2600 3600 3400 3600
Wire Wire Line
	2600 3800 3100 3800
Connection ~ 3400 3600
Wire Wire Line
	3700 3200 2800 3200
Wire Wire Line
	2800 3200 2800 3400
Wire Wire Line
	2800 3400 2600 3400
Wire Wire Line
	2600 3700 3700 3700
Wire Wire Line
	3450 3800 3450 3500
Wire Wire Line
	3450 3500 2600 3500
Connection ~ 3450 3800
Wire Wire Line
	2600 4750 3150 4750
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 60FD593D
P 2200 1150
F 0 "J3" V 2164 962 50  0000 R CNN
F 1 "GNSSUARTOut" V 2073 962 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2200 1150 50  0001 C CNN
F 3 "~" H 2200 1150 50  0001 C CNN
	1    2200 1150
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J7
U 1 1 60FEE048
P 2550 1400
F 0 "J7" V 2514 1212 50  0000 R CNN
F 1 "UARTJumper" V 2423 1212 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 2550 1400 50  0001 C CNN
F 3 "~" H 2550 1400 50  0001 C CNN
	1    2550 1400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2550 1700 2450 1700
$Comp
L Connector_Generic:Conn_01x08 J10
U 1 1 61882574
P 5050 3400
F 0 "J10" H 5130 3392 50  0000 L CNN
F 1 "GPIO2" H 5130 3301 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 5050 3400 50  0001 C CNN
F 3 "~" H 5050 3400 50  0001 C CNN
	1    5050 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	4700 3100 4850 3100
Wire Wire Line
	4850 3200 4700 3200
Wire Wire Line
	4700 3300 4850 3300
Wire Wire Line
	4850 3400 4700 3400
Wire Wire Line
	4700 3500 4750 3500
Wire Wire Line
	4850 3600 4800 3600
Wire Wire Line
	4700 3700 4850 3700
Wire Wire Line
	4850 3800 4700 3800
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 619056F3
P 2400 2600
F 0 "J4" H 2318 2275 50  0000 C CNN
F 1 "UART" H 2318 2366 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2400 2600 50  0001 C CNN
F 3 "~" H 2400 2600 50  0001 C CNN
	1    2400 2600
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J2
U 1 1 61906131
P 1250 2900
F 0 "J2" H 1168 2475 50  0000 C CNN
F 1 "GPIO1" H 1168 2566 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 1250 2900 50  0001 C CNN
F 3 "~" H 1250 2900 50  0001 C CNN
	1    1250 2900
	-1   0    0    1   
$EndComp
Connection ~ 2650 2500
Wire Wire Line
	2550 1600 2550 1700
Connection ~ 2450 1700
Wire Wire Line
	2450 1700 2450 1600
Wire Wire Line
	2600 2500 2650 2500
Wire Wire Line
	4100 1750 4100 2100
Wire Wire Line
	3100 1850 4400 1850
Connection ~ 4400 1850
Wire Wire Line
	4400 1850 4850 1850
Wire Wire Line
	3150 1950 4300 1950
Wire Wire Line
	4300 1950 4850 1950
Connection ~ 4300 1950
Wire Wire Line
	4300 1950 4300 2100
Wire Wire Line
	4850 2900 4850 2250
Wire Wire Line
	4700 2900 4850 2900
Wire Wire Line
	4850 2150 4750 2150
Wire Wire Line
	4750 2150 4750 2500
Wire Wire Line
	4750 2500 4700 2500
Wire Wire Line
	4700 2600 4750 2600
Wire Wire Line
	4750 2600 4750 2500
Connection ~ 4750 2500
Wire Wire Line
	2650 2500 3700 2500
Wire Wire Line
	2600 2600 3700 2600
Text Notes 2300 4800 2    50   ~ 0
CS\nDC\nRES\nD1\nD0\nVCC\nGND
Text Notes 2300 3850 2    50   ~ 0
CS\nSCK\nMOSI\nMISO\nVCC\nGND
Wire Wire Line
	2450 1700 2450 1950
Text Notes 1750 2150 2    50   ~ 0
VCC\nTX\nRX\nGND
Wire Wire Line
	2050 2050 2300 2050
Wire Wire Line
	2050 1850 3100 1850
$Comp
L Connector_Generic:Conn_01x04 J1
U 1 1 60F87827
P 1850 2050
F 0 "J1" H 1768 2367 50  0000 C CNN
F 1 "GNSSModule" H 1768 2276 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 1850 2050 50  0001 C CNN
F 3 "~" H 1850 2050 50  0001 C CNN
	1    1850 2050
	-1   0    0    1   
$EndComp
Connection ~ 3100 1850
Wire Wire Line
	3100 1850 3100 3800
Wire Wire Line
	2300 2050 2300 1350
Wire Wire Line
	2650 1600 2650 2500
Wire Wire Line
	2050 1950 2200 1950
Wire Wire Line
	2200 1350 2200 1950
Connection ~ 2200 1950
Wire Wire Line
	2200 1950 2450 1950
Connection ~ 4100 1750
$Comp
L power:PWR_FLAG #FLG01
U 1 1 619E563C
P 4100 1750
F 0 "#FLG01" H 4100 1825 50  0001 C CNN
F 1 "PWR_FLAG" H 4100 1923 50  0000 C CNN
F 2 "" H 4100 1750 50  0001 C CNN
F 3 "~" H 4100 1750 50  0001 C CNN
	1    4100 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 1750 3800 1750
Wire Wire Line
	6200 4250 6200 4150
$Comp
L power:GNDPWR #PWR01
U 1 1 619E78B2
P 6200 4250
F 0 "#PWR01" H 6200 4050 50  0001 C CNN
F 1 "GNDPWR" H 6204 4096 50  0000 C CNN
F 2 "" H 6200 4200 50  0001 C CNN
F 3 "" H 6200 4200 50  0001 C CNN
	1    6200 4250
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J8
U 1 1 60F8E411
P 3700 900
F 0 "J8" V 3664 712 50  0000 R CNN
F 1 "BatteryInput" V 3573 712 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 3700 900 50  0001 C CNN
F 3 "~" H 3700 900 50  0001 C CNN
	1    3700 900 
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J9
U 1 1 619B2455
P 5050 1950
F 0 "J9" H 5130 1942 50  0000 L CNN
F 1 "IO" H 5130 1851 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 5050 1950 50  0001 C CNN
F 3 "~" H 5050 1950 50  0001 C CNN
	1    5050 1950
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 61A9C7B9
P 6200 4150
F 0 "#FLG02" H 6200 4225 50  0001 C CNN
F 1 "PWR_FLAG" H 6200 4323 50  0000 C CNN
F 2 "" H 6200 4150 50  0001 C CNN
F 3 "~" H 6200 4150 50  0001 C CNN
	1    6200 4150
	1    0    0    -1  
$EndComp
Text Label 6200 4200 2    50   ~ 0
GND
Text Label 4200 4400 2    50   ~ 0
GND
Wire Wire Line
	4200 4100 4200 4400
Wire Wire Line
	4200 4400 4300 4400
Wire Wire Line
	4300 4400 4300 4100
Text Label 2600 4850 0    50   ~ 0
GND
Text Label 2600 3900 0    50   ~ 0
GND
Text Label 2050 2150 0    50   ~ 0
GND
Text Label 4850 2050 2    50   ~ 0
GND
Text Label 3700 1100 2    50   ~ 0
GND
Wire Wire Line
	4100 1750 4850 1750
$Comp
L Connector_Generic:Conn_01x06 J5
U 1 1 60F8ADBC
P 2400 3700
F 0 "J5" H 2318 4117 50  0000 C CNN
F 1 "SDCardReader" H 2318 4026 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 2400 3700 50  0001 C CNN
F 3 "~" H 2400 3700 50  0001 C CNN
	1    2400 3700
	-1   0    0    1   
$EndComp
$Comp
L Device:LED D2
U 1 1 62E86138
P 1900 3800
F 0 "D2" H 1893 4016 50  0000 C CNN
F 1 "LED" H 1893 3925 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 1900 3800 50  0001 C CNN
F 3 "~" H 1900 3800 50  0001 C CNN
	1    1900 3800
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D1
U 1 1 62E8679A
P 1550 3800
F 0 "D1" H 1543 4016 50  0000 C CNN
F 1 "LED" H 1543 3925 50  0000 C CNN
F 2 "LED_THT:LED_D3.0mm" H 1550 3800 50  0001 C CNN
F 3 "~" H 1550 3800 50  0001 C CNN
	1    1550 3800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R2
U 1 1 62E882EF
P 1900 3450
F 0 "R2" H 1970 3496 50  0000 L CNN
F 1 "R" H 1970 3405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1830 3450 50  0001 C CNN
F 3 "~" H 1900 3450 50  0001 C CNN
	1    1900 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 62E88916
P 1550 3450
F 0 "R1" H 1620 3496 50  0000 L CNN
F 1 "R" H 1620 3405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 1480 3450 50  0001 C CNN
F 3 "~" H 1550 3450 50  0001 C CNN
	1    1550 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3600 1900 3650
Wire Wire Line
	1550 3600 1550 3650
Wire Wire Line
	1900 4050 1550 4050
Wire Wire Line
	1550 3950 1550 4050
Wire Wire Line
	1900 3950 1900 4050
Text Label 1900 4200 0    50   ~ 0
GND
Wire Wire Line
	1450 3100 3700 3100
Wire Wire Line
	1450 3000 1550 3000
Wire Wire Line
	1450 2900 1900 2900
Wire Wire Line
	1450 2800 3700 2800
Wire Wire Line
	1450 2700 3700 2700
Wire Wire Line
	1900 3300 1900 2900
Connection ~ 1900 2900
Wire Wire Line
	1900 2900 3700 2900
Wire Wire Line
	1550 3300 1550 3000
Connection ~ 1550 3000
Wire Wire Line
	1550 3000 3700 3000
$Comp
L Connector_Generic:Conn_01x03 J11
U 1 1 62F05EA4
P 3350 1650
F 0 "J11" V 3314 1462 50  0000 R CNN
F 1 "BatteryJumper" V 3223 1462 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 3350 1650 50  0001 C CNN
F 3 "~" H 3350 1650 50  0001 C CNN
	1    3350 1650
	-1   0    0    1   
$EndComp
Wire Wire Line
	3800 1550 3550 1550
Wire Wire Line
	3800 1100 3800 1550
Wire Wire Line
	3550 1650 3800 1650
Wire Wire Line
	3800 1650 3800 1750
Connection ~ 3800 1750
Wire Wire Line
	3800 1750 4100 1750
$Comp
L Connector_Generic:Conn_01x04 J12
U 1 1 62F5F152
P 5400 4850
F 0 "J12" H 5318 5167 50  0000 C CNN
F 1 "OledScreen_I2C" H 5318 5076 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 5400 4850 50  0001 C CNN
F 3 "~" H 5400 4850 50  0001 C CNN
	1    5400 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 4050 1900 4200
Connection ~ 1900 4050
Wire Wire Line
	3150 1950 3150 4750
Connection ~ 3150 4750
Wire Wire Line
	3150 4750 5200 4750
Wire Wire Line
	5200 4850 5100 4850
Wire Wire Line
	4800 4950 5200 4950
Wire Wire Line
	5200 5050 4750 5050
Wire Wire Line
	4800 3600 4800 4950
Connection ~ 4800 3600
Wire Wire Line
	4800 3600 4700 3600
Wire Wire Line
	4750 3500 4750 5050
Connection ~ 4750 3500
Wire Wire Line
	4750 3500 4850 3500
Text Label 5100 4850 2    50   ~ 0
GND
Text Notes 5500 5050 0    50   ~ 0
VCC\nGND\nSCL\nSDA
$Comp
L Connector_Generic:Conn_01x02 J13
U 1 1 62EC1DEE
P 4400 1150
F 0 "J13" V 4364 962 50  0000 R CNN
F 1 "5V Header" V 4273 962 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 4400 1150 50  0001 C CNN
F 3 "~" H 4400 1150 50  0001 C CNN
	1    4400 1150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4400 1850 4400 2100
Wire Wire Line
	4400 1850 4400 1600
Wire Wire Line
	4500 1350 4500 1450
Text Label 4500 1450 0    50   ~ 0
GND
$Comp
L Switch:SW_Push_Dual SW1
U 1 1 62ECA799
P 5950 2700
F 0 "SW1" H 5950 2985 50  0000 C CNN
F 1 "SW_Push_Dual" H 5950 2894 50  0000 C CNN
F 2 "Button_Switch_THT:SW_PUSH_6mm" H 5950 2900 50  0001 C CNN
F 3 "~" H 5950 2900 50  0001 C CNN
	1    5950 2700
	1    0    0    -1  
$EndComp
Text Label 2200 3100 0    50   ~ 0
D6
Wire Wire Line
	5450 2700 5650 2700
Wire Wire Line
	5750 2900 5650 2900
Wire Wire Line
	5650 2900 5650 2700
Connection ~ 5650 2700
Wire Wire Line
	5650 2700 5750 2700
Text Label 5450 2700 0    50   ~ 0
D6
Wire Wire Line
	6150 2900 6250 2900
Wire Wire Line
	6250 2900 6250 2700
Wire Wire Line
	6250 2700 6150 2700
Wire Wire Line
	6250 2700 6450 2700
Connection ~ 6250 2700
Text Label 6450 2700 0    50   ~ 0
GND
Wire Wire Line
	5650 2500 5650 2700
$Comp
L Device:R R3
U 1 1 62EF3538
P 5650 2350
F 0 "R3" H 5720 2396 50  0000 L CNN
F 1 "R" H 5720 2305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5580 2350 50  0001 C CNN
F 3 "~" H 5650 2350 50  0001 C CNN
	1    5650 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 1600 5650 1600
Wire Wire Line
	5650 1600 5650 2200
Connection ~ 4400 1600
Wire Wire Line
	4400 1600 4400 1350
$EndSCHEMATC
