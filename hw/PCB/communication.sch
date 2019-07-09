EESchema Schematic File Version 5
LIBS:Femtosat-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 6
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
L RF_Module:RFM69HCW U4
U 1 1 5BD13EFB
P 5600 3550
AR Path="/5BD13EFB" Ref="U4"  Part="1" 
AR Path="/5BD13DD6/5BD13EFB" Ref="U3"  Part="1" 
F 0 "U3" H 5250 4000 50  0000 C CNN
F 1 "RFM69HCW" H 5800 4000 50  0000 C CNN
F 2 "Transceptor:RFM69HCW" H 2300 5200 50  0001 C CNN
F 3 "http://www.hoperf.com/upload/rf/RFM69HCW-V1.1.pdf" H 2300 5200 50  0001 C CNN
	1    5600 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 4150 5500 4200
Wire Wire Line
	5500 4200 5600 4200
Wire Wire Line
	5700 4200 5700 4150
Wire Wire Line
	5600 4150 5600 4200
Connection ~ 5600 4200
Wire Wire Line
	5600 4200 5700 4200
Wire Wire Line
	5600 4200 5600 4250
$Comp
L power:GND #PWR0109
U 1 1 5BD16858
P 5600 4250
F 0 "#PWR0109" H 5600 4000 50  0001 C CNN
F 1 "GND" H 5605 4077 50  0000 C CNN
F 2 "" H 5600 4250 50  0001 C CNN
F 3 "" H 5600 4250 50  0001 C CNN
	1    5600 4250
	1    0    0    -1  
$EndComp
Text Label 4850 3750 0    50   ~ 0
RESET
Text Label 4850 3550 0    50   ~ 0
NSS
Text Label 4850 3450 0    50   ~ 0
MISO
Text Label 4850 3350 0    50   ~ 0
MOSI
Text Label 4850 3250 0    50   ~ 0
SCK
Text Label 6300 3950 2    50   ~ 0
DIO0
Text Label 6300 3850 2    50   ~ 0
DIO1
Text Label 6300 3750 2    50   ~ 0
DIO2
Text Label 6300 3650 2    50   ~ 0
DIO3
Text Label 6300 3550 2    50   ~ 0
DIO4
Text Label 6300 3450 2    50   ~ 0
DIO5
Wire Wire Line
	5100 3250 4850 3250
Wire Wire Line
	4850 3350 5100 3350
Wire Wire Line
	5100 3450 4850 3450
Wire Wire Line
	4850 3550 5100 3550
Wire Wire Line
	4850 3750 5100 3750
Wire Wire Line
	6100 3950 6300 3950
Wire Wire Line
	6300 3850 6100 3850
Wire Wire Line
	6100 3750 6300 3750
Wire Wire Line
	6300 3650 6100 3650
Wire Wire Line
	6100 3550 6300 3550
Wire Wire Line
	6300 3450 6100 3450
Wire Wire Line
	5600 3050 5600 2950
Text HLabel 4150 3000 0    50   Input ~ 0
3.3V
Text HLabel 4150 3100 0    50   Input ~ 0
SCK
Text HLabel 4150 3200 0    50   Input ~ 0
MOSI
Text HLabel 4150 3300 0    50   Input ~ 0
MISO
Text HLabel 4150 3400 0    50   Input ~ 0
NSS
Text HLabel 4150 3500 0    50   Input ~ 0
RESET
Text HLabel 4150 3600 0    50   Input ~ 0
INT
Text HLabel 4150 3700 0    50   Input ~ 0
GND
Text Label 4400 3500 2    50   ~ 0
RESET
Wire Wire Line
	4150 3500 4400 3500
Wire Wire Line
	4150 3000 4400 3000
Wire Wire Line
	4400 3100 4150 3100
Wire Wire Line
	4400 3200 4150 3200
Wire Wire Line
	4400 3300 4150 3300
Wire Wire Line
	4400 3400 4150 3400
Wire Wire Line
	4400 3600 4150 3600
Wire Wire Line
	4400 3700 4150 3700
Text Label 5700 4200 0    50   ~ 0
GND
Text Label 4400 3700 2    50   ~ 0
GND
Text Label 4400 3600 2    50   ~ 0
DIO0
Text Label 4400 3400 2    50   ~ 0
NSS
Text Label 4400 3300 2    50   ~ 0
MISO
Text Label 4400 3200 2    50   ~ 0
MOSI
Text Label 4400 3100 2    50   ~ 0
SCK
Text Label 4400 3000 2    50   ~ 0
3.3V
$Comp
L power:GND #PWR0110
U 1 1 5BD2EB6A
P 6600 3500
F 0 "#PWR0110" H 6600 3250 50  0001 C CNN
F 1 "GND" H 6605 3327 50  0000 C CNN
F 2 "" H 6600 3500 50  0001 C CNN
F 3 "" H 6600 3500 50  0001 C CNN
	1    6600 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 3450 6600 3500
$Comp
L Device:C Ct1
U 1 1 5BD8FE47
P 7350 3300
F 0 "Ct1" H 7450 3200 50  0000 C CNN
F 1 "0.1uF" H 7350 3400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7388 3150 50  0001 C CNN
F 3 "~" H 7350 3300 50  0001 C CNN
	1    7350 3300
	-1   0    0    1   
$EndComp
$Comp
L Device:C Ct2
U 1 1 5BD90136
P 7050 3300
F 0 "Ct2" H 7150 3200 50  0000 C CNN
F 1 "10uF" H 7050 3400 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7088 3150 50  0001 C CNN
F 3 "~" H 7050 3300 50  0001 C CNN
	1    7050 3300
	-1   0    0    1   
$EndComp
Wire Wire Line
	7350 3150 7350 3100
Wire Wire Line
	7050 3100 7050 3150
Wire Wire Line
	7350 3450 7350 3500
Wire Wire Line
	7050 3500 7050 3450
Text Label 6300 3250 2    50   ~ 0
ANT
$Comp
L power:+3V3 #PWR0117
U 1 1 5D1D8537
P 5600 2950
F 0 "#PWR0117" H 5600 2800 50  0001 C CNN
F 1 "+3V3" H 5615 3123 50  0000 C CNN
F 2 "" H 5600 2950 50  0001 C CNN
F 3 "" H 5600 2950 50  0001 C CNN
	1    5600 2950
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR0118
U 1 1 5D1D89F4
P 7200 3050
F 0 "#PWR0118" H 7200 2900 50  0001 C CNN
F 1 "+3V3" H 7215 3223 50  0000 C CNN
F 2 "" H 7200 3050 50  0001 C CNN
F 3 "" H 7200 3050 50  0001 C CNN
	1    7200 3050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 5D1D8ED6
P 7200 3550
F 0 "#PWR0119" H 7200 3300 50  0001 C CNN
F 1 "GND" H 7205 3377 50  0000 C CNN
F 2 "" H 7200 3550 50  0001 C CNN
F 3 "" H 7200 3550 50  0001 C CNN
	1    7200 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 3100 7200 3100
Wire Wire Line
	7050 3500 7200 3500
Wire Wire Line
	7200 3550 7200 3500
Connection ~ 7200 3500
Wire Wire Line
	7200 3500 7350 3500
Wire Wire Line
	7200 3100 7200 3050
Connection ~ 7200 3100
Wire Wire Line
	7200 3100 7350 3100
Text Label 7200 3100 2    50   ~ 0
3.3V
$Comp
L Connector:Conn_Coaxial J3
U 1 1 5D24BE5C
P 6600 3250
F 0 "J3" H 6700 3225 50  0000 L CNN
F 1 "U.FL" H 6700 3134 50  0000 L CNN
F 2 "Connector_Coaxial:U.FL_Hirose_U.FL-R-SMT-1_Vertical" H 6600 3250 50  0001 C CNN
F 3 " ~" H 6600 3250 50  0001 C CNN
	1    6600 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 3250 6400 3250
$EndSCHEMATC
