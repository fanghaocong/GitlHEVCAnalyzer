#include "vvcbitstreamversionselector.h"
#include "ui_vvcbitstreamversionselector.h"
#include "model/common/comrom.h"
#include <QDebug>

VVCBitstreamVersionSelector::VVCBitstreamVersionSelector(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VVCBitstreamVersionSelector)
{
    ui->setupUi(this);
}

VVCBitstreamVersionSelector::~VVCBitstreamVersionSelector()
{
    delete ui;
}

void VVCBitstreamVersionSelector::on_version160_clicked()
{
    m_iBitstreamVersion = 160;
}

void VVCBitstreamVersionSelector::on_buttonBox_accepted()
{
    xSetDecoderVersion();
}

void VVCBitstreamVersionSelector::showEvent(QShowEvent *event)
{
    /// restore last selection
    int m_iBitstreamVersion = g_cAppSetting.value("last_vvc_bitstream_version", 160).toInt();
    switch (m_iBitstreamVersion)
    {
    case 160:
        ui->version160->setChecked(true);
        break;
    default:
        ui->version160->setChecked(true);
    }
    QDialog::showEvent(event);
}

void VVCBitstreamVersionSelector::hideEvent(QHideEvent *event)
{
    /// save current selection
    xSetDecoderVersion();
    g_cAppSetting.setValue("last_vvc_bitstream_version", m_iBitstreamVersion);
    QDialog::hideEvent(event);
}

void VVCBitstreamVersionSelector::xSetDecoderVersion()
{
    if (ui->version160->isChecked())
    {
        m_iBitstreamVersion = 160;
    }
}
