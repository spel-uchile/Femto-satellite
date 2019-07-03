EESchema Schematic File Version 5
LIBS:Femtosat-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 6
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
L Connector:Micro_SD_Card_Det J4
U 1 1 5D1D232F
P 5600 3500
F 0 "J4" H 5550 4317 50  0000 C CNN
F 1 "Micro_SD_Card_Det" H 5550 4226 50  0000 C CNN
F 2 "" H 7650 4200 50  0001 C CNN
F 3 "https://www.hirose.com/product/en/download_file/key_name/DM3/category/Catalog/doc_file_id/49662/?file_category_id=4&item_id=195&is_series=1" H 5600 3600 50  0001 C CNN
	1    5600 3500
	1    0    0    -1  
$EndComp
Text HLabel 4550 3200 0    50   Input ~ 0
NSS
Text HLabel 4550 3300 0    50   Input ~ 0
MOSI
Text HLabel 4550 3500 0    50   Input ~ 0
SCK
Text HLabel 4550 3700 0    50   Input ~ 0
MISO
Text HLabel 4550 3400 0    50   Input ~ 0
3V3
Text HLabel 4550 3900 0    50   Input ~ 0
CD
Text HLabel 4550 4000 0    50   Input ~ 0
GND
NoConn ~ 4700 3100
NoConn ~ 4700 3800
Wire Wire Line
	4550 3200 4700 3200
Wire Wire Line
	4700 3300 4550 3300
Wire Wire Line
	4550 3400 4700 3400
Wire Wire Line
	4700 3500 4550 3500
Wire Wire Line
	4550 3700 4700 3700
Wire Wire Line
	4700 3900 4550 3900
Wire Wire Line
	4550 4000 4650 4000
Wire Wire Line
	4700 3600 4650 3600
Wire Wire Line
	4650 3600 4650 4000
Connection ~ 4650 4000
Wire Wire Line
	4650 4000 4700 4000
Wire Wire Line
	6400 4000 6450 4000
Wire Wire Line
	6450 4000 6450 4050
$Comp
L power:GND #PWR?
U 1 1 5D1D4624
P 6450 4050
F 0 "#PWR?" H 6450 3800 50  0001 C CNN
F 1 "GND" H 6455 3877 50  0000 C CNN
F 2 "" H 6450 4050 50  0001 C CNN
F 3 "" H 6450 4050 50  0001 C CNN
	1    6450 4050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D1D4B1C
P 4650 4050
F 0 "#PWR?" H 4650 3800 50  0001 C CNN
F 1 "GND" H 4655 3877 50  0000 C CNN
F 2 "" H 4650 4050 50  0001 C CNN
F 3 "" H 4650 4050 50  0001 C CNN
	1    4650 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4650 4050 4650 4000
$Comp
L power:+3V3 #PWR?
U 1 1 5D1F34BB
P 6800 2950
F 0 "#PWR?" H 6800 2800 50  0001 C CNN
F 1 "+3V3" H 6815 3123 50  0000 C CNN
F 2 "" H 6800 2950 50  0001 C CNN
F 3 "" H 6800 2950 50  0001 C CNN
	1    6800 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:C Csd1
U 1 1 5D1F37B2
P 6600 3200
F 0 "Csd1" H 6715 3246 50  0000 L CNN
F 1 "10uF" H 6715 3155 50  0000 L CNN
F 2 "" H 6638 3050 50  0001 C CNN
F 3 "~" H 6600 3200 50  0001 C CNN
	1    6600 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:C Csd2
U 1 1 5D1F3A08
P 7000 3200
F 0 "Csd2" H 7115 3246 50  0000 L CNN
F 1 "100nF" H 7115 3155 50  0000 L CNN
F 2 "" H 7038 3050 50  0001 C CNN
F 3 "~" H 7000 3200 50  0001 C CNN
	1    7000 3200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5D1F43D1
P 6800 3450
F 0 "#PWR?" H 6800 3200 50  0001 C CNN
F 1 "GND" H 6805 3277 50  0000 C CNN
F 2 "" H 6800 3450 50  0001 C CNN
F 3 "" H 6800 3450 50  0001 C CNN
	1    6800 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 3050 6600 3000
Wire Wire Line
	6600 3000 6800 3000
Wire Wire Line
	7000 3000 7000 3050
Wire Wire Line
	6800 3000 6800 2950
Connection ~ 6800 3000
Wire Wire Line
	6800 3000 7000 3000
Wire Wire Line
	6600 3350 6600 3400
Wire Wire Line
	6600 3400 6800 3400
Wire Wire Line
	7000 3400 7000 3350
Wire Wire Line
	6800 3400 6800 3450
Connection ~ 6800 3400
Wire Wire Line
	6800 3400 7000 3400
Text Label 4550 3400 0    50   ~ 0
3V3
Text Label 6800 3000 0    50   ~ 0
3V3
$EndSCHEMATC
