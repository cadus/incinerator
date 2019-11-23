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
L Connector_Generic:Conn_01x15 J?
U 1 1 5DD971B9
P 4750 2800
F 0 "J?" H 4668 1875 50  0000 C CNN
F 1 "ESP32 NodeMCU" H 4668 1966 50  0000 C CNN
F 2 "" H 4750 2800 50  0001 C CNN
F 3 "~" H 4750 2800 50  0001 C CNN
	1    4750 2800
	1    0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x15 J?
U 1 1 5DD97FE0
P 5400 2800
F 0 "J?" H 5318 1875 50  0000 C CNN
F 1 "ESP32 NodeMCU" H 5318 1966 50  0000 C CNN
F 2 "" H 5400 2800 50  0001 C CNN
F 3 "~" H 5400 2800 50  0001 C CNN
	1    5400 2800
	-1   0    0    1   
$EndComp
Text Label 4350 2100 0    50   ~ 0
EN
Text Label 4350 2200 0    50   ~ 0
VP
Text Label 4350 2300 0    50   ~ 0
VN
Text Label 4350 2400 0    50   ~ 0
D34
Text Label 4350 2500 0    50   ~ 0
D35
Text Label 4350 2600 0    50   ~ 0
D32
Text Label 4350 2700 0    50   ~ 0
D33
Text Label 4350 2800 0    50   ~ 0
D25
Text Label 4350 2900 0    50   ~ 0
D26
Text Label 4350 3000 0    50   ~ 0
D27
Text Label 4350 3100 0    50   ~ 0
D14
Text Label 4350 3200 0    50   ~ 0
D12
Text Label 4350 3300 0    50   ~ 0
D13
Text Label 4350 3400 0    50   ~ 0
GND
Text Label 4350 3500 0    50   ~ 0
VIN
Text Label 5650 2100 0    50   ~ 0
D23
Text Label 5650 2200 0    50   ~ 0
D22
Text Label 5650 2300 0    50   ~ 0
TXD
Text Label 5650 2400 0    50   ~ 0
RXD
Text Label 5650 2500 0    50   ~ 0
D21
Text Label 5650 2600 0    50   ~ 0
D19
Text Label 5650 2700 0    50   ~ 0
D18
Text Label 5650 2800 0    50   ~ 0
D5
Text Label 5650 2900 0    50   ~ 0
TX2
Text Label 5650 3000 0    50   ~ 0
RX2
Text Label 5650 3100 0    50   ~ 0
D4
Text Label 5650 3200 0    50   ~ 0
D2
Text Label 5650 3300 0    50   ~ 0
D15
Text Label 5650 3400 0    50   ~ 0
GND
Text Label 5650 3500 0    50   ~ 0
3V3
Text Label 8200 2300 0    50   ~ 0
BUSY
Text Label 8200 2400 0    50   ~ 0
RST
Text Label 8200 2500 0    50   ~ 0
DC
Text Label 8200 2600 0    50   ~ 0
CS
Text Label 8200 2700 0    50   ~ 0
CLK
Text Label 8200 2800 0    50   ~ 0
DIN
Text Label 8200 2900 0    50   ~ 0
GND
Text Label 8200 3000 0    50   ~ 0
VCC
Wire Wire Line
	5600 2100 7050 2100
Wire Wire Line
	7950 1700 3950 1700
Wire Wire Line
	3950 1700 3950 2200
Wire Wire Line
	3950 2200 4550 2200
Wire Wire Line
	7950 2300 7950 1700
$Comp
L Connector:Conn_01x08_Male J?
U 1 1 5DDAE1A1
P 8150 2600
F 0 "J?" H 8258 2067 50  0000 C CNN
F 1 "Waveshare 4.2\" e-paper" H 8258 1976 50  0000 C CNN
F 2 "" H 8150 2600 50  0001 C CNN
F 3 "~" H 8150 2600 50  0001 C CNN
	1    8150 2600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7050 2800 7950 2800
Wire Wire Line
	7050 2100 7050 2800
Wire Wire Line
	5600 2700 7950 2700
Wire Wire Line
	5600 2800 6950 2800
Wire Wire Line
	6950 2800 6950 2600
Wire Wire Line
	6950 2600 7950 2600
Wire Wire Line
	5600 2600 6850 2600
Wire Wire Line
	6850 2600 6850 2500
Wire Wire Line
	6850 2500 7950 2500
Wire Wire Line
	5600 2500 6750 2500
Wire Wire Line
	6750 2500 6750 2400
Wire Wire Line
	6750 2400 7950 2400
Wire Wire Line
	5600 3500 7950 3500
Wire Wire Line
	7950 3500 7950 3000
Wire Wire Line
	5600 3400 7850 3400
Wire Wire Line
	7850 3400 7850 2900
Wire Wire Line
	7850 2900 7950 2900
$EndSCHEMATC
