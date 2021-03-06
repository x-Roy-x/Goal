# THIS FILE IS AUTOMATICALLY GENERATED
# Project: D:\bachelor_work\program\Goal\SmartBoy.cydsn\SmartBoy.cyprj
# Date: Tue, 04 May 2021 21:45:48 GMT
#set_units -time ns
create_clock -name {ADC_intClock(FFB)} -period 208.33333333333331 -waveform {0 104.166666666667} [list [get_pins {ClockBlock/ff_div_6}]]
create_clock -name {Clock_2(FFB)} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ff_div_7}]]
create_clock -name {Clock_1(FFB)} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ff_div_8}]]
create_clock -name {CyRouted1} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/dsi_in_0}]]
create_clock -name {CyILO} -period 25000 -waveform {0 12500} [list [get_pins {ClockBlock/ilo}]]
create_clock -name {CyLFClk} -period 25000 -waveform {0 12500} [list [get_pins {ClockBlock/lfclk}]]
create_clock -name {CyECO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/eco}]]
create_clock -name {CyIMO} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyHFClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/hfclk}]]
create_clock -name {CySysClk} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/sysclk}]]
create_generated_clock -name {ADC_intClock} -source [get_pins {ClockBlock/hfclk}] -edges {1 5 11} -nominal_period 208.33333333333331 [list]
create_generated_clock -name {Clock_2} -source [get_pins {ClockBlock/hfclk}] -edges {1 24001 48001} [list]
create_generated_clock -name {Clock_1} -source [get_pins {ClockBlock/hfclk}] -edges {1 24001 48001} [list]
create_generated_clock -name {Debouncer_Clock} -source [get_pins {ClockBlock/hfclk}] -edges {1 62993 125985} -nominal_period 2624666.6666666665 [list [get_pins {ClockBlock/udb_div_0}]]


# Component constraints for D:\bachelor_work\program\Goal\SmartBoy.cydsn\TopDesign\TopDesign.cysch
# Project: D:\bachelor_work\program\Goal\SmartBoy.cydsn\SmartBoy.cyprj
# Date: Tue, 04 May 2021 21:45:44 GMT
