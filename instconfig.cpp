#include "instconfig.h"
InstConfig::InstConfig()
{
}
InstConfig::InstConfig(QString suite,const InstConfig& that):
suiteName(suite),
CHANnel1_Status(that.CHANnel1_Status),
CHANnel2_Status(that.CHANnel2_Status),
CHANnel3_Status(that.CHANnel3_Status),
CHANnel4_Status(that.CHANnel4_Status),
CHANnel1_Name(that.CHANnel1_Name),
CHANnel2_Name(that.CHANnel2_Name),
CHANnel3_Name(that.CHANnel3_Name),
CHANnel4_Name(that.CHANnel4_Name),
Channel1_Scale(that.Channel1_Scale),
Channel2_Scale(that.Channel2_Scale),
Channel3_Scale(that.Channel3_Scale),
Channel4_Scale(that.Channel4_Scale),
CHANnel1_Coupling(that.CHANnel1_Coupling),
CHANnel2_Coupling(that.CHANnel2_Coupling),
CHANnel3_Coupling(that.CHANnel3_Coupling),
CHANnel4_Coupling(that.CHANnel4_Coupling),
channel1_bandwidth(that.channel1_bandwidth),
channel2_bandwidth(that.channel2_bandwidth),
channel3_bandwidth(that.channel3_bandwidth),
channel4_bandwidth(that.channel4_bandwidth),
channel1_Offset(that.channel1_Offset),
channel2_Offset(that.channel2_Offset),
channel3_Offset(that.channel3_Offset),
channel4_Offset(that.channel4_Offset),
ch1_measure_type1(that.ch1_measure_type1),
ch1_measure_type2(that.ch1_measure_type2),
ch1_measure_type3(that.ch1_measure_type3),
ch1_measure_type4(that.ch1_measure_type4),
ch1_measure_hold(that.ch1_measure_hold),
ch2_measure_type1(that.ch2_measure_type1),
ch2_measure_type2(that.ch2_measure_type2),
ch2_measure_type3(that.ch2_measure_type3),
ch2_measure_type4(that.ch2_measure_type4),
ch2_measure_hold(that.ch2_measure_hold),
ch3_measure_type1(that.ch3_measure_type1),
ch3_measure_type2(that.ch3_measure_type2),
ch3_measure_type3(that.ch3_measure_type3),
ch3_measure_type4(that.ch3_measure_type4),
ch3_measure_hold(that.ch3_measure_hold),
ch4_measure_type1(that.ch4_measure_type1),
ch4_measure_type2(that.ch4_measure_type2),
ch4_measure_type3(that.ch4_measure_type3),
ch4_measure_type4(that.ch4_measure_type4),
ch4_measure_hold(that.ch4_measure_hold),
Source1(that.Source1),
Source2(that.Source2),
Source3(that.Source3),
Source4(that.Source4),
Function1_Scale(that.Function1_Scale),
Function2_Scale(that.Function2_Scale),
Function1_Status(that.Function1_Status),
Function2_Status(that.Function2_Status),
DisplayArea(that.DisplayArea),
Slect_Model(that.Slect_Model),
HorizontalPosition(that.HorizontalPosition),
HorizontalScale(that.HorizontalScale),
Trigger_CHANnel(that.Trigger_CHANnel),
Trigger_Model(that.Trigger_Model),
Trigger_Edge(that.Trigger_Edge),
Trigger_Level(that.Trigger_Level),
Vec_time(that.Vec_time)
{
}