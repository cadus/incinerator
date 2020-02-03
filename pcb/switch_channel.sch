EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 7
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
L Device:Q_NMOS_GDS Q?
U 1 1 5E3E0615
P 6350 3750
AR Path="/5E3E0615" Ref="Q?"  Part="1" 
AR Path="/5E3D49A4/5E3E0615" Ref="Q1"  Part="1" 
AR Path="/5E3E825D/5E3E0615" Ref="Q?"  Part="1" 
AR Path="/5E3F787D/5E3E0615" Ref="Q2"  Part="1" 
AR Path="/5E3F9D41/5E3E0615" Ref="Q3"  Part="1" 
AR Path="/5E3FD547/5E3E0615" Ref="Q4"  Part="1" 
AR Path="/5E3FFB75/5E3E0615" Ref="Q5"  Part="1" 
AR Path="/5E401B95/5E3E0615" Ref="Q6"  Part="1" 
F 0 "Q2" H 6555 3796 50  0000 L CNN
F 1 "IRLU120N" H 6555 3705 50  0000 L CNN
F 2 "incinerator:SIPAK_Vertical_Custom" H 6550 3850 50  0001 C CNN
F 3 "~" H 6350 3750 50  0001 C CNN
	1    6350 3750
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 5E3E061B
P 7050 3750
AR Path="/5E3E061B" Ref="R?"  Part="1" 
AR Path="/5E3D49A4/5E3E061B" Ref="R2"  Part="1" 
AR Path="/5E3E825D/5E3E061B" Ref="R?"  Part="1" 
AR Path="/5E3F787D/5E3E061B" Ref="R5"  Part="1" 
AR Path="/5E3F9D41/5E3E061B" Ref="R8"  Part="1" 
AR Path="/5E3FD547/5E3E061B" Ref="R11"  Part="1" 
AR Path="/5E3FFB75/5E3E061B" Ref="R14"  Part="1" 
AR Path="/5E401B95/5E3E061B" Ref="R17"  Part="1" 
F 0 "R5" V 7257 3750 50  0000 C CNN
F 1 "100" V 7166 3750 50  0000 C CNN
F 2 "incinerator:R_THT_D2.5mm_P7.62mm_Custom" V 6980 3750 50  0001 C CNN
F 3 "~" H 7050 3750 50  0001 C CNN
	1    7050 3750
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5E3E0621
P 6600 3950
AR Path="/5E3E0621" Ref="R?"  Part="1" 
AR Path="/5E3D49A4/5E3E0621" Ref="R3"  Part="1" 
AR Path="/5E3E825D/5E3E0621" Ref="R?"  Part="1" 
AR Path="/5E3F787D/5E3E0621" Ref="R6"  Part="1" 
AR Path="/5E3F9D41/5E3E0621" Ref="R9"  Part="1" 
AR Path="/5E3FD547/5E3E0621" Ref="R12"  Part="1" 
AR Path="/5E3FFB75/5E3E0621" Ref="R15"  Part="1" 
AR Path="/5E401B95/5E3E0621" Ref="R18"  Part="1" 
F 0 "R6" V 6807 3950 50  0000 C CNN
F 1 "15k" V 6716 3950 50  0000 C CNN
F 2 "incinerator:R_THT_D2.5mm_P7.62mm_Custom" V 6530 3950 50  0001 C CNN
F 3 "~" H 6600 3950 50  0001 C CNN
	1    6600 3950
	0    -1   1    0   
$EndComp
Wire Wire Line
	4700 3850 4900 3850
Wire Wire Line
	4900 3950 5000 3950
Wire Wire Line
	4900 3850 4900 3950
Connection ~ 5000 3650
Wire Wire Line
	4900 3650 5000 3650
Wire Wire Line
	4900 3750 4900 3650
Wire Wire Line
	4700 3750 4900 3750
$Comp
L power:+12V #PWR?
U 1 1 5E3E062E
P 5000 3650
AR Path="/5E3E062E" Ref="#PWR?"  Part="1" 
AR Path="/5E3D49A4/5E3E062E" Ref="#PWR015"  Part="1" 
AR Path="/5E3E825D/5E3E062E" Ref="#PWR?"  Part="1" 
AR Path="/5E3F787D/5E3E062E" Ref="#PWR017"  Part="1" 
AR Path="/5E3F9D41/5E3E062E" Ref="#PWR019"  Part="1" 
AR Path="/5E3FD547/5E3E062E" Ref="#PWR021"  Part="1" 
AR Path="/5E3FFB75/5E3E062E" Ref="#PWR023"  Part="1" 
AR Path="/5E401B95/5E3E062E" Ref="#PWR025"  Part="1" 
F 0 "#PWR017" H 5000 3500 50  0001 C CNN
F 1 "+12V" H 5015 3823 50  0000 C CNN
F 2 "" H 5000 3650 50  0001 C CNN
F 3 "" H 5000 3650 50  0001 C CNN
	1    5000 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:D D?
U 1 1 5E3E0634
P 5000 3800
AR Path="/5E3E0634" Ref="D?"  Part="1" 
AR Path="/5E3D49A4/5E3E0634" Ref="D1"  Part="1" 
AR Path="/5E3E825D/5E3E0634" Ref="D?"  Part="1" 
AR Path="/5E3F787D/5E3E0634" Ref="D3"  Part="1" 
AR Path="/5E3F9D41/5E3E0634" Ref="D5"  Part="1" 
AR Path="/5E3FD547/5E3E0634" Ref="D7"  Part="1" 
AR Path="/5E3FFB75/5E3E0634" Ref="D9"  Part="1" 
AR Path="/5E401B95/5E3E0634" Ref="D11"  Part="1" 
F 0 "D3" V 4954 3879 50  0000 L CNN
F 1 "D" V 5045 3879 50  0000 L CNN
F 2 "incinerator:D_DO-41_P7.62mm_Custom" H 5000 3800 50  0001 C CNN
F 3 "~" H 5000 3800 50  0001 C CNN
	1    5000 3800
	0    1    1    0   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J?
U 1 1 5E3E063A
P 4500 3750
AR Path="/5E3E063A" Ref="J?"  Part="1" 
AR Path="/5E3D49A4/5E3E063A" Ref="J4"  Part="1" 
AR Path="/5E3E825D/5E3E063A" Ref="J?"  Part="1" 
AR Path="/5E3F787D/5E3E063A" Ref="J5"  Part="1" 
AR Path="/5E3F9D41/5E3E063A" Ref="J6"  Part="1" 
AR Path="/5E3FD547/5E3E063A" Ref="J7"  Part="1" 
AR Path="/5E3FFB75/5E3E063A" Ref="J8"  Part="1" 
AR Path="/5E401B95/5E3E063A" Ref="J9"  Part="1" 
F 0 "J5" H 4418 3967 50  0000 C CNN
F 1 "SWITCH_OUT" H 4418 3876 50  0000 C CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 4500 3750 50  0001 C CNN
F 3 "~" H 4500 3750 50  0001 C CNN
	1    4500 3750
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6250 3550 5750 3550
Wire Wire Line
	5750 3550 5750 3950
Connection ~ 5000 3950
Wire Wire Line
	6250 3950 6450 3950
Wire Wire Line
	6550 3750 6800 3750
Wire Wire Line
	6750 3950 6800 3950
Wire Wire Line
	6800 3950 6800 3750
Connection ~ 6800 3750
Wire Wire Line
	6800 3750 6900 3750
Wire Wire Line
	6250 3950 6250 4050
Connection ~ 6250 3950
Wire Wire Line
	7500 3750 7200 3750
Connection ~ 5450 3950
Wire Wire Line
	5000 3950 5450 3950
Wire Wire Line
	5450 3950 5750 3950
$Comp
L Device:LED D?
U 1 1 5E3E064F
P 5450 3800
AR Path="/5E3E064F" Ref="D?"  Part="1" 
AR Path="/5E3D49A4/5E3E064F" Ref="D2"  Part="1" 
AR Path="/5E3E825D/5E3E064F" Ref="D?"  Part="1" 
AR Path="/5E3F787D/5E3E064F" Ref="D4"  Part="1" 
AR Path="/5E3F9D41/5E3E064F" Ref="D6"  Part="1" 
AR Path="/5E3FD547/5E3E064F" Ref="D8"  Part="1" 
AR Path="/5E3FFB75/5E3E064F" Ref="D10"  Part="1" 
AR Path="/5E401B95/5E3E064F" Ref="D12"  Part="1" 
F 0 "D4" V 5489 3682 50  0000 R CNN
F 1 "RED" V 5398 3682 50  0000 R CNN
F 2 "incinerator:LED_D3.0mm_Custom" H 5450 3800 50  0001 C CNN
F 3 "~" H 5450 3800 50  0001 C CNN
	1    5450 3800
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R?
U 1 1 5E3E0655
P 5300 3650
AR Path="/5E3E0655" Ref="R?"  Part="1" 
AR Path="/5E3D49A4/5E3E0655" Ref="R1"  Part="1" 
AR Path="/5E3E825D/5E3E0655" Ref="R?"  Part="1" 
AR Path="/5E3F787D/5E3E0655" Ref="R4"  Part="1" 
AR Path="/5E3F9D41/5E3E0655" Ref="R7"  Part="1" 
AR Path="/5E3FD547/5E3E0655" Ref="R10"  Part="1" 
AR Path="/5E3FFB75/5E3E0655" Ref="R13"  Part="1" 
AR Path="/5E401B95/5E3E0655" Ref="R16"  Part="1" 
F 0 "R4" V 5093 3650 50  0000 C CNN
F 1 "1k" V 5184 3650 50  0000 C CNN
F 2 "incinerator:R_THT_D2.5mm_P7.62mm_Custom" V 5230 3650 50  0001 C CNN
F 3 "~" H 5300 3650 50  0001 C CNN
	1    5300 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	5000 3650 5150 3650
$Comp
L power:Earth #PWR?
U 1 1 5E3E065C
P 6250 4050
AR Path="/5E3E065C" Ref="#PWR?"  Part="1" 
AR Path="/5E3D49A4/5E3E065C" Ref="#PWR016"  Part="1" 
AR Path="/5E3E825D/5E3E065C" Ref="#PWR?"  Part="1" 
AR Path="/5E3F787D/5E3E065C" Ref="#PWR018"  Part="1" 
AR Path="/5E3F9D41/5E3E065C" Ref="#PWR020"  Part="1" 
AR Path="/5E3FD547/5E3E065C" Ref="#PWR022"  Part="1" 
AR Path="/5E3FFB75/5E3E065C" Ref="#PWR024"  Part="1" 
AR Path="/5E401B95/5E3E065C" Ref="#PWR026"  Part="1" 
F 0 "#PWR018" H 6250 3800 50  0001 C CNN
F 1 "Earth" H 6250 3900 50  0001 C CNN
F 2 "" H 6250 4050 50  0001 C CNN
F 3 "~" H 6250 4050 50  0001 C CNN
	1    6250 4050
	1    0    0    -1  
$EndComp
Text HLabel 7500 3750 2    50   Input ~ 0
SIG_IN
$EndSCHEMATC
