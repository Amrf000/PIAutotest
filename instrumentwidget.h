#ifndef INSTRUMENTWIDGET_H
#define INSTRUMENTWIDGET_H
#include <QWidget>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QString>
#include "instconfig.h"

namespace Ui {
class InstrumentWidget;
}

class InstrumentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InstrumentWidget(QWidget *parent = 0);
    ~InstrumentWidget();
    void setConfig(QStringList vecs,QVariantMap conf,InstConfig* config);
    void addComboItems(QComboBox* cb,const QStringList& texts);
    void setSpinValue(QDoubleSpinBox* cb,double val);
private:
    Ui::InstrumentWidget *ui;
private:
    InstConfig* m_config;
private slots:
    void on_lineEdit_channel1_label_textEdited(const QString &arg1);
    void on_lineEdit_channel2_label_textEdited(const QString &arg1);
    void on_lineEdit_channel3_label_textEdited(const QString &arg1);
    void on_lineEdit_channel4_label_textEdited(const QString &arg1);
    void on_lineEdit_channel1_offset_textEdited(const QString &arg1);
    void on_lineEdit_channel2_offset_textEdited(const QString &arg1);
    void on_lineEdit_channel3_offset_textEdited(const QString &arg1);
    void on_lineEdit_channel4_offset_textEdited(const QString &arg1);
    void on_lineEdit_channel1_scale_textEdited(const QString &arg1);
    void on_lineEdit_channel2_scale_textEdited(const QString &arg1);
    void on_lineEdit_channel3_scale_textEdited(const QString &arg1);
    void on_lineEdit_channel4_scale_textEdited(const QString &arg1);
    void on_comboBox_channel1_coupling_currentIndexChanged(const QString &arg1);
    void on_comboBox_channel2_coupling_currentIndexChanged(const QString &arg1);
    void on_comboBox_channel3_coupling_currentIndexChanged(const QString &arg1);
    void on_comboBox_channel4_coupling_currentIndexChanged(const QString &arg1);
    void on_comboBox_channel1_bandwidth_currentIndexChanged(const QString &arg1);
    void on_comboBox_channel2_bandwidth_currentIndexChanged(const QString &arg1);
    void on_comboBox_channel3_bandwidth_currentIndexChanged(const QString &arg1);
    void on_comboBox_channel4_bandwidth_currentIndexChanged(const QString &arg1);
    void on_checkBox_channel1_toggled(bool checked);
    void on_checkBox_channel2_toggled(bool checked);
    void on_checkBox_channel3_toggled(bool checked);
    void on_checkBox_channel4_toggled(bool checked);
    void on_comboBox_ch1_measure_type1_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch1_measure_type2_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch1_measure_type3_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch1_measure_type4_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch2_measure_type1_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch2_measure_type2_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch2_measure_type3_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch2_measure_type4_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch3_measure_type1_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch3_measure_type2_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch3_measure_type3_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch3_measure_type4_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch4_measure_type1_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch4_measure_type2_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch4_measure_type3_currentIndexChanged(const QString &arg1);
    void on_comboBox_ch4_measure_type4_currentIndexChanged(const QString &arg1);
    void on_checkBox_ch1_measure_hold_toggled(bool checked);
    void on_checkBox_ch2_measure_hold_toggled(bool checked);
    void on_checkBox_ch3_measure_hold_toggled(bool checked);
    void on_checkBox_ch4_measure_hold_toggled(bool checked);
    void on_comboBox_trigger_channel_currentIndexChanged(const QString &arg1);
    void on_doubleSpinBox_trigger_level_valueChanged(double arg1);
    void on_comboBox_trigger_edge_currentIndexChanged(const QString &arg1);
    void on_comboBox_trigger_mode_currentIndexChanged(const QString &arg1);
    void on_doubleSpinBox_horizontalScale_valueChanged(double arg1);
    void on_doubleSpinBox_horizontalPosition_valueChanged(double arg1);
    void on_lineEdit_vec_time_textEdited(const QString &arg1);
    void on_comboBox_Slect_Model_currentIndexChanged(const QString &arg1);
};

#endif // INSTRUMENTWIDGET_H
