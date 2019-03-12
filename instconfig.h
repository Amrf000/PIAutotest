#ifndef INSTCONFIG_H
#define INSTCONFIG_H
#include <QString>

class InstConfig
{
public:
    InstConfig();
    InstConfig(QString suite,const InstConfig& that);
 //----------------------
    QString  suiteName;

    bool CHANnel1_Status;//"ON"
    bool CHANnel2_Status;//"ON"
    bool CHANnel3_Status;//"ON"
    bool CHANnel4_Status;//"OFF"

    QString CHANnel1_Name;//"CPUB_BUMP"
    QString CHANnel2_Name;//"CPUB_CAP"
    QString CHANnel3_Name;//"TRI"
    QString CHANnel4_Name;//"Current"

    QString Channel1_Scale;//"0.05"
    QString Channel2_Scale;//"0.02"
    QString Channel3_Scale;//"1"
    QString Channel4_Scale;//"0.2"

    //===========
     QString CHANnel1_Coupling;//DC50
     QString CHANnel2_Coupling;//DC50
     QString CHANnel3_Coupling;//DC50
     QString CHANnel4_Coupling;//DC50

     QString channel1_bandwidth;
     QString channel2_bandwidth;
     QString channel3_bandwidth;
     QString channel4_bandwidth;

     QString channel1_Offset;
     QString channel2_Offset;
     QString channel3_Offset;
     QString channel4_Offset;

     QString ch1_measure_type1;
     QString ch1_measure_type2;
     QString ch1_measure_type3;
     QString ch1_measure_type4;
     bool ch1_measure_hold;

     QString ch2_measure_type1;
     QString ch2_measure_type2;
     QString ch2_measure_type3;
     QString ch2_measure_type4;
     bool ch2_measure_hold;

     QString ch3_measure_type1;
     QString ch3_measure_type2;
     QString ch3_measure_type3;
     QString ch3_measure_type4;
     bool ch3_measure_hold;

     QString ch4_measure_type1;
     QString ch4_measure_type2;
     QString ch4_measure_type3;
     QString ch4_measure_type4;
     bool ch4_measure_hold;
   //========================
    QString Source1;//"Channel1"
    QString Source2;//"Channel2"
    QString Source3;//"Channel3"
    QString Source4;//"Channel4"

    QString Function1_Scale;//"0.05"
    QString Function2_Scale;//"0.05"
    QString Function1_Status;//"ON"
    QString Function2_Status;//"OFF"

    QString DisplayArea;//30  #屏幕显示，20->6 lane'； 30->9 lane； 40->12 lane

    QString Slect_Model;//"Volt"  # {Volt | Current}
    double HorizontalPosition;//1000E-6 #横坐标左移
    double HorizontalScale;//5000E-6
    QString Trigger_CHANnel;//"Channel3"
    QString Trigger_Model;//"TRIGgered"      # {AUTO | TRIGgered | SINGle}
    QString Trigger_Edge;//"POSitive"        # {POSitive | NEGative | EITHer}
    uint Trigger_Level;//"1"
    //========================
    uint Vec_time;
};
#endif // INSTCONFIG_H
