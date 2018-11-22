EESchema Schematic File Version 4
LIBS:Femtosat-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 5 5
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
L Connector:Conn_01x04_Male J2
U 1 1 5BFFAA36
P 4750 3100
F 0 "J2" H 4722 2982 50  0000 R CNN
F 1 "Programmer" H 4722 3073 50  0000 R CNN
F 2 "" H 4750 3100 50  0001 C CNN
F 3 "~" H 4750 3100 50  0001 C CNN
	1    4750 3100
	-1   0    0    1   
$EndComp
Text HLabel 4200 2700 0    50   Input ~ 0
3V3
Text HLabel 4200 2900 0    50   Input ~ 0
SWDIO
Text HLabel 4200 3000 0    50   Input ~ 0
SWCLK
Text HLabel 4200 3100 0    50   Input ~ 0
RESET
Text HLabel 4200 3200 0    50   Input ~ 0
GND
$Comp
L Device:R R1
U 1 1 5BFFC4E5
P 4450 2700
F 0 "R1" V 4657 2700 50  0000 C CNN
F 1 "1k" V 4566 2700 50  0000 C CNN
F 2 "" V 4380 2700 50  0001 C CNN
F 3 "~" H 4450 2700 50  0001 C CNN
	1    4450 2700
	0    -1   -1   0   
$EndComp
Text Label 4900 2700 2    50   ~ 0
SWCLK
Wire Wire Line
	4900 2700 4600 2700
Wire Wire Line
	4300 2700 4200 2700
Text Label 4250 3000 0    50   ~ 0
SWCLK
Wire Wire Line
	4550 2900 4200 2900
Wire Wire Line
	4200 3000 4550 3000
Wire Wire Line
	4550 3100 4200 3100
Wire Wire Line
	4200 3200 4550 3200
$EndSCHEMATC
