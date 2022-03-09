#include "hevcbitstreamversionselector.h"
#include "ui_hevcbitstreamversionselector.h"
#include "model/common/comrom.h"
#include <QDebug>
HEVCBitstreamVersionSelector::HEVCBitstreamVersionSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HEVCBitstreamVersionSelector)
{
    ui->setupUi(this);

}

HEVCBitstreamVersionSelector::~HEVCBitstreamVersionSelector()
{
    delete ui;
}

void HEVCBitstreamVersionSelector::on_version40_clicked()
{
    m_iBitstreamVersion = 40;
}



void HEVCBitstreamVersionSelector::on_version52_clicked()
{
    m_iBitstreamVersion = 52;
}



void HEVCBitstreamVersionSelector::on_version100_clicked()
{
    m_iBitstreamVersion = 100;
}


void HEVCBitstreamVersionSelector::on_version120_clicked()
{
    m_iBitstreamVersion = 120;
}

void HEVCBitstreamVersionSelector::showEvent(QShowEvent * event)
{
    /// restore last selection
    int m_iBitstreamVersion = g_cAppSetting.value("last_hevc_bitstream_version", 100).toInt();
    switch(m_iBitstreamVersion)
    {
    case 40:
        ui->version40->setChecked(true);
        break;
    case 52:
        ui->version52->setChecked(true);
        break;
    case 100:
        ui->version100->setChecked(true);
        break;
    case 120:
        ui->version120->setChecked(true);
        break;
    default:
        ui->version100->setChecked(true);
        break;

    }
    QDialog::showEvent(event);
}

void HEVCBitstreamVersionSelector::hideEvent(QHideEvent * event)
{
    /// save current selection
    xSetDecoderVersion();
    g_cAppSetting.setValue("last_hevc_bitstream_version", m_iBitstreamVersion);
    QDialog::hideEvent(event);
}

void HEVCBitstreamVersionSelector::on_buttonBox_accepted()
{
    xSetDecoderVersion();
}

void HEVCBitstreamVersionSelector::xSetDecoderVersion()
{
    if(ui->version40->isChecked())
    {
        m_iBitstreamVersion = 40;
    }
    else if(ui->version52->isChecked() )
    {
        m_iBitstreamVersion = 52;
    }
    else if(ui->version100->isChecked() )
    {
        m_iBitstreamVersion = 100;
    }
    else if(ui->version120->isChecked() )
    {
        m_iBitstreamVersion = 120;
    }
}
