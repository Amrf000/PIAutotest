#include "instrumentwidget.h"
#include "ui_instrumentwidget.h"

InstrumentWidget::InstrumentWidget(QWidget *parent) :
    QWidget(parent),m_config(NULL),
    ui(new Ui::InstrumentWidget)
{
    ui->setupUi(this);
}

InstrumentWidget::~InstrumentWidget()
{
    delete ui;
}

void InstrumentWidget::addComboItems(QComboBox* cb,const QStringList& texts)
{
    bool oldState = cb->blockSignals(true);
    cb->clear();
    cb->addItems(texts);
    cb->blockSignals(oldState);
}
void InstrumentWidget::setSpinValue(QDoubleSpinBox* cb,double val)
{
    bool oldState = cb->blockSignals(true);
    cb->setValue(val);
    cb->blockSignals(oldState);
}
void InstrumentWidget::setConfig(QStringList vecs,QVariantMap conf,InstConfig* config)
{
   QStringList bandwidths = conf["bandwidth"].toStringList();
   addComboItems(ui->comboBox_channel1_bandwidth,bandwidths);
   addComboItems(ui->comboBox_channel2_bandwidth,bandwidths);
   addComboItems(ui->comboBox_channel3_bandwidth,bandwidths);
   addComboItems(ui->comboBox_channel4_bandwidth,bandwidths);
   QStringList Coupling = conf["Coupling"].toStringList();
   addComboItems(ui->comboBox_channel1_coupling,Coupling);
   addComboItems(ui->comboBox_channel2_coupling,Coupling);
   addComboItems(ui->comboBox_channel3_coupling,Coupling);
   addComboItems(ui->comboBox_channel4_coupling,Coupling);
   QStringList TestOptions = conf["TestOptions"].toStringList();
   addComboItems(ui->comboBox_ch1_measure_type1,TestOptions);
   addComboItems(ui->comboBox_ch1_measure_type2,TestOptions);
   addComboItems(ui->comboBox_ch1_measure_type3,TestOptions);
   addComboItems(ui->comboBox_ch1_measure_type4,TestOptions);

   addComboItems(ui->comboBox_ch2_measure_type1,TestOptions);
   addComboItems(ui->comboBox_ch2_measure_type2,TestOptions);
   addComboItems(ui->comboBox_ch2_measure_type3,TestOptions);
   addComboItems(ui->comboBox_ch2_measure_type4,TestOptions);

   addComboItems(ui->comboBox_ch3_measure_type1,TestOptions);
   addComboItems(ui->comboBox_ch3_measure_type2,TestOptions);
   addComboItems(ui->comboBox_ch3_measure_type3,TestOptions);
   addComboItems(ui->comboBox_ch3_measure_type4,TestOptions);

   addComboItems(ui->comboBox_ch4_measure_type1,TestOptions);
   addComboItems(ui->comboBox_ch4_measure_type2,TestOptions);
   addComboItems(ui->comboBox_ch4_measure_type3,TestOptions);
   addComboItems(ui->comboBox_ch4_measure_type4,TestOptions);

   QStringList Trigger_Edge = conf["Trigger_Edge"].toStringList();
   addComboItems(ui->comboBox_trigger_edge,Trigger_Edge);
   QStringList Trigger_Model = conf["Trigger_Model"].toStringList();
   addComboItems(ui->comboBox_trigger_mode,Trigger_Model);
   QStringList Trigger_CHANnel = conf["Trigger_CHANnel"].toStringList();
   addComboItems(ui->comboBox_trigger_channel,Trigger_CHANnel);
   QStringList Slect_Model = conf["Slect_Model"].toStringList();
   addComboItems(ui->comboBox_Slect_Model,Slect_Model);

   m_config = config;
   ui->lineEdit_vec_time->setText(QString::number(m_config->Vec_time));
   setSpinValue(ui->doubleSpinBox_horizontalScale,m_config->HorizontalScale);
   setSpinValue(ui->doubleSpinBox_horizontalPosition,m_config->HorizontalPosition);
   ui->comboBox_trigger_channel->setCurrentText(m_config->Trigger_CHANnel);
   setSpinValue(ui->doubleSpinBox_trigger_level,m_config->Trigger_Level);
   ui->comboBox_trigger_edge->setCurrentText(m_config->Trigger_Edge);
   ui->comboBox_trigger_mode->setCurrentText(m_config->Trigger_Model);
   //ui->comboBox_measure_channel;
   //ui->comboBox_measure_type1;
   //ui->comboBox_measure_type2;
   //ui->comboBox_measure_type3;
   //ui->comboBox_measure_type4;
   //ui->checkBox_measure_hold;

   ui->comboBox_ch1_measure_type1->setCurrentText(m_config->ch1_measure_type1);
   ui->comboBox_ch1_measure_type2->setCurrentText(m_config->ch1_measure_type2);
   ui->comboBox_ch1_measure_type3->setCurrentText(m_config->ch1_measure_type3);
   ui->comboBox_ch1_measure_type4->setCurrentText(m_config->ch1_measure_type4);
   ui->checkBox_ch1_measure_hold->setChecked(m_config->ch1_measure_hold);

   ui->comboBox_ch2_measure_type1->setCurrentText(m_config->ch2_measure_type1);
   ui->comboBox_ch2_measure_type2->setCurrentText(m_config->ch2_measure_type2);
   ui->comboBox_ch2_measure_type3->setCurrentText(m_config->ch2_measure_type3);
   ui->comboBox_ch2_measure_type4->setCurrentText(m_config->ch2_measure_type4);
   ui->checkBox_ch2_measure_hold->setChecked(m_config->ch2_measure_hold);

   ui->comboBox_ch3_measure_type1->setCurrentText(m_config->ch3_measure_type1);
   ui->comboBox_ch3_measure_type2->setCurrentText(m_config->ch3_measure_type2);
   ui->comboBox_ch3_measure_type3->setCurrentText(m_config->ch3_measure_type3);
   ui->comboBox_ch3_measure_type4->setCurrentText(m_config->ch3_measure_type4);
   ui->checkBox_ch3_measure_hold->setChecked(m_config->ch3_measure_hold);

   ui->comboBox_ch4_measure_type1->setCurrentText(m_config->ch4_measure_type1);
   ui->comboBox_ch4_measure_type2->setCurrentText(m_config->ch4_measure_type2);
   ui->comboBox_ch4_measure_type3->setCurrentText(m_config->ch4_measure_type3);
   ui->comboBox_ch4_measure_type4->setCurrentText(m_config->ch4_measure_type4);
   ui->checkBox_ch4_measure_hold->setChecked(m_config->ch4_measure_hold);

   ui->checkBox_channel1->setChecked(m_config->CHANnel1_Status);
   ui->checkBox_channel2->setChecked(m_config->CHANnel2_Status);
   ui->checkBox_channel3->setChecked(m_config->CHANnel3_Status);
   ui->checkBox_channel4->setChecked(m_config->CHANnel4_Status);

   ui->lineEdit_channel1_label->setText(m_config->CHANnel1_Name);
   ui->lineEdit_channel2_label->setText(m_config->CHANnel2_Name);
   ui->lineEdit_channel3_label->setText(m_config->CHANnel3_Name);
   ui->lineEdit_channel4_label->setText(m_config->CHANnel4_Name);

   ui->lineEdit_channel1_offset->setText(m_config->channel1_Offset);
   ui->lineEdit_channel2_offset->setText(m_config->channel2_Offset);
   ui->lineEdit_channel3_offset->setText(m_config->channel3_Offset);
   ui->lineEdit_channel4_offset->setText(m_config->channel4_Offset);

   ui->lineEdit_channel1_scale->setText(m_config->Channel1_Scale);
   ui->lineEdit_channel2_scale->setText(m_config->Channel2_Scale);
   ui->lineEdit_channel3_scale->setText(m_config->Channel3_Scale);
   ui->lineEdit_channel4_scale->setText(m_config->Channel4_Scale);

   ui->comboBox_channel1_coupling->setCurrentText(m_config->CHANnel1_Coupling);
   ui->comboBox_channel2_coupling->setCurrentText(m_config->CHANnel2_Coupling);
   ui->comboBox_channel3_coupling->setCurrentText(m_config->CHANnel3_Coupling);
   ui->comboBox_channel4_coupling->setCurrentText(m_config->CHANnel4_Coupling);

   ui->comboBox_channel1_bandwidth->setCurrentText(m_config->channel1_bandwidth);
   ui->comboBox_channel2_bandwidth->setCurrentText(m_config->channel2_bandwidth);
   ui->comboBox_channel3_bandwidth->setCurrentText(m_config->channel3_bandwidth);
   ui->comboBox_channel4_bandwidth->setCurrentText(m_config->channel4_bandwidth);

   ui->comboBox_Slect_Model->setCurrentText(m_config->Slect_Model);

   ui->textEditProfile->setText(m_config->Profile);
}

void InstrumentWidget::on_lineEdit_channel1_label_textEdited(const QString &val)
{
    if(m_config) m_config->CHANnel1_Name = val;
}

void InstrumentWidget::on_lineEdit_channel2_label_textEdited(const QString &val)
{
    if(m_config) m_config->CHANnel2_Name = val;
}

void InstrumentWidget::on_lineEdit_channel3_label_textEdited(const QString &val)
{
    if(m_config) m_config->CHANnel3_Name = val;
}

void InstrumentWidget::on_lineEdit_channel4_label_textEdited(const QString &val)
{
    if(m_config) m_config->CHANnel4_Name = val;
}

void InstrumentWidget::on_lineEdit_channel1_offset_textEdited(const QString &val)
{
    if(m_config) m_config->channel1_Offset = val;
}

void InstrumentWidget::on_lineEdit_channel2_offset_textEdited(const QString &val)
{
    if(m_config) m_config->channel2_Offset = val;
}

void InstrumentWidget::on_lineEdit_channel3_offset_textEdited(const QString &val)
{
    if(m_config) m_config->channel3_Offset = val;
}

void InstrumentWidget::on_lineEdit_channel4_offset_textEdited(const QString &val)
{
    if(m_config) m_config->channel4_Offset = val;
}

void InstrumentWidget::on_lineEdit_channel1_scale_textEdited(const QString &val)
{
   if(m_config)  m_config->Channel1_Scale = val;
}

void InstrumentWidget::on_lineEdit_channel2_scale_textEdited(const QString &val)
{
    if(m_config) m_config->Channel2_Scale = val;
}

void InstrumentWidget::on_lineEdit_channel3_scale_textEdited(const QString &val)
{
    if(m_config) m_config->Channel3_Scale = val;
}

void InstrumentWidget::on_lineEdit_channel4_scale_textEdited(const QString &val)
{
    if(m_config) m_config->Channel4_Scale = val;
}

void InstrumentWidget::on_comboBox_channel1_coupling_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->CHANnel1_Coupling = val;
}

void InstrumentWidget::on_comboBox_channel2_coupling_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->CHANnel2_Coupling = val;
}

void InstrumentWidget::on_comboBox_channel3_coupling_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->CHANnel3_Coupling = val;
}

void InstrumentWidget::on_comboBox_channel4_coupling_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->CHANnel4_Coupling = val;
}

void InstrumentWidget::on_comboBox_channel1_bandwidth_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->channel1_bandwidth = val;
}

void InstrumentWidget::on_comboBox_channel2_bandwidth_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->channel2_bandwidth = val;
}

void InstrumentWidget::on_comboBox_channel3_bandwidth_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->channel3_bandwidth = val;
}

void InstrumentWidget::on_comboBox_channel4_bandwidth_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->channel4_bandwidth = val;
}

void InstrumentWidget::on_checkBox_channel1_toggled(bool val)
{
    if(m_config) m_config->CHANnel1_Status = val;
}
void InstrumentWidget::on_checkBox_channel2_toggled(bool val)
{
    if(m_config) m_config->CHANnel2_Status = val;
}
void InstrumentWidget::on_checkBox_channel3_toggled(bool val)
{
    if(m_config) m_config->CHANnel3_Status = val;
}
void InstrumentWidget::on_checkBox_channel4_toggled(bool val)
{
    if(m_config) m_config->CHANnel4_Status = val;
}

void InstrumentWidget::on_comboBox_ch1_measure_type1_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch1_measure_type1 = val;
}

void InstrumentWidget::on_comboBox_ch1_measure_type2_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch1_measure_type2 = val;
}

void InstrumentWidget::on_comboBox_ch1_measure_type3_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch1_measure_type3 = val;
}

void InstrumentWidget::on_comboBox_ch1_measure_type4_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch1_measure_type4 = val;
}

void InstrumentWidget::on_comboBox_ch2_measure_type1_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch2_measure_type1 = val;
}

void InstrumentWidget::on_comboBox_ch2_measure_type2_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch2_measure_type2 = val;
}

void InstrumentWidget::on_comboBox_ch2_measure_type3_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch2_measure_type3 = val;
}

void InstrumentWidget::on_comboBox_ch2_measure_type4_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch2_measure_type4 = val;
}

void InstrumentWidget::on_comboBox_ch3_measure_type1_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch3_measure_type1 = val;
}

void InstrumentWidget::on_comboBox_ch3_measure_type2_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch3_measure_type2 = val;
}

void InstrumentWidget::on_comboBox_ch3_measure_type3_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch3_measure_type3 = val;
}

void InstrumentWidget::on_comboBox_ch3_measure_type4_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch3_measure_type4 = val;
}

void InstrumentWidget::on_comboBox_ch4_measure_type1_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch4_measure_type1 = val;
}

void InstrumentWidget::on_comboBox_ch4_measure_type2_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch4_measure_type2 = val;
}

void InstrumentWidget::on_comboBox_ch4_measure_type3_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch4_measure_type3 = val;
}

void InstrumentWidget::on_comboBox_ch4_measure_type4_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->ch4_measure_type4 = val;
}

void InstrumentWidget::on_checkBox_ch1_measure_hold_toggled(bool val)
{
    if(m_config) m_config->ch1_measure_hold = val;
}

void InstrumentWidget::on_checkBox_ch2_measure_hold_toggled(bool val)
{
    if(m_config) m_config->ch2_measure_hold = val;
}

void InstrumentWidget::on_checkBox_ch3_measure_hold_toggled(bool val)
{
    if(m_config) m_config->ch3_measure_hold = val;
}

void InstrumentWidget::on_checkBox_ch4_measure_hold_toggled(bool val)
{
    if(m_config) m_config->ch4_measure_hold = val;
}

void InstrumentWidget::on_comboBox_trigger_channel_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->Trigger_CHANnel = val;
}

void InstrumentWidget::on_doubleSpinBox_trigger_level_valueChanged(double val)
{
    if(m_config) m_config->Trigger_Level = val;
}

void InstrumentWidget::on_comboBox_trigger_edge_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->Trigger_Edge = val;
}

void InstrumentWidget::on_comboBox_trigger_mode_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->Trigger_Model = val;
}

void InstrumentWidget::on_doubleSpinBox_horizontalScale_valueChanged(double val)
{
    if(m_config) m_config->HorizontalScale = val;
}

void InstrumentWidget::on_doubleSpinBox_horizontalPosition_valueChanged(double val)
{
    if(m_config) m_config->HorizontalPosition = val;
}

void InstrumentWidget::on_comboBox_Slect_Model_currentIndexChanged(const QString &val)
{
    if(m_config) m_config->Slect_Model = val;
}

void InstrumentWidget::on_lineEdit_vec_time_textEdited(const QString &val)
{
    if(m_config) m_config->Vec_time = val.toUInt();
}

void InstrumentWidget::on_textEditProfile_textChanged()
{
    if(m_config) m_config->Profile = ui->textEditProfile->toPlainText();
}
