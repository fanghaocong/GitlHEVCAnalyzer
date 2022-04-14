#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QDebug>
#include <QMimeData>
#include <QApplication>
#include "model/modellocator.h"
#include "commands/appfrontcontroller.h"
#include "hevcbitstreamversionselector.h"
#include "vvcbitstreamversionselector.h"
#include "model/common/comrom.h"
#include "gitlivkcmdevt.h"
#include "preferencedialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_cPreferenceDialog(this),
    m_cBusyDialog(this),
    m_cThemeGroup(this),
    ui(new Ui::MainWindow)
{

    setModualName("main_window");
    ui->setupUi(this);

    /// set listeners

    listenToParams(QStringList() <<"total_frame_num"<<"current_frame_poc",
                   MAKE_CALLBACK(MainWindow::onPOCInfoChanged));
    listenToParams("theme_stylesheet", MAKE_CALLBACK(MainWindow::onStylesheetChanged));
    listenToParams("snapshot", MAKE_CALLBACK(MainWindow::onSnapshot));
    listenToParams("current_sequence", MAKE_CALLBACK(MainWindow::onSequenceChanged));
    listenToParams("scale", MAKE_CALLBACK(MainWindow::onZooming));

    /// layout hacks
    ui->msgDockWidget->widget()->layout()->setContentsMargins(0,0,0,0);
    ui->pluginFilterListDockWidget->widget()->layout()->setContentsMargins(0,0,0,0);
    ui->sequencesListDockWidget->widget()->layout()->setContentsMargins(0,0,0,0);

    /// exclusive theme action btns
    ui->defaultThemeAction->setActionGroup(&m_cThemeGroup);
    ui->darkThemeAction->setActionGroup(&m_cThemeGroup);

    /// bitstream drop open accepted
    setAcceptDrops(true);

    /// model init, including filter loading, etc..
    ModelLocator::getInstance();

    /// load theme from preferences
    GitlIvkCmdEvt cEvt("switch_theme");
    cEvt.setParameter("load_theme_from_pref", true);
    cEvt.dispatch();
}

MainWindow::~MainWindow()
{
    /// delete cache folder when exit
    GitlIvkCmdEvt cEvt("clean_cache");
    cEvt.dispatch();
    delete ui;
}

void MainWindow::onPOCInfoChanged(GitlUpdateUIEvt& rcEvt)
{
    static int iTotalFrameNum = -1;
    static int iCurrentFrameNum = -1;
    QVariant vValue;
    /// frame number
    if( rcEvt.hasParameter("total_frame_num") )
    {
        vValue = rcEvt.getParameter("total_frame_num");
        iTotalFrameNum = vValue.toInt();
        ui->totalFrameNum->setText(QString("%1").arg(iTotalFrameNum));
    }

    /// total frame number
    if( rcEvt.hasParameter("current_frame_poc") )
    {
        vValue = rcEvt.getParameter("current_frame_poc");
        iCurrentFrameNum = vValue.toInt() + 1;
        ui->currentFrameNum->setText(QString("%1").arg(iCurrentFrameNum));
    }

    /// slide bar position
    int iMin = ui->progressBar->minimum();
    int iMax = ui->progressBar->maximum();
    int iPos = 0;
    if(iTotalFrameNum != 1)
    {
        iPos = (iCurrentFrameNum-1)*(iMax-iMin)/(iTotalFrameNum-1);
    }
    ui->progressBar->setValue(iPos);
}

void MainWindow::onStylesheetChanged(GitlUpdateUIEvt& rcEvt)
{
    qApp->setStyleSheet(rcEvt.getParameter("theme_stylesheet").toString());
    QString strThemeName = rcEvt.getParameter("theme_name").toString();
    if(strThemeName == "default")
        ui->defaultThemeAction->setChecked(true);
    else if(strThemeName == "dark")
        ui->darkThemeAction->setChecked(true);
}

void MainWindow::onSnapshot(GitlUpdateUIEvt& rcEvt)
{
    /// TODO refactor
    QVariant vValue = rcEvt.getParameter("snapshot");
    QPixmap* pcPixMap = (QPixmap*)(vValue.value<void *>());
    xSaveSnapshot(pcPixMap);
}

void MainWindow::onSequenceChanged(GitlUpdateUIEvt &rcEvt)
{

    ComSequence* pcCurSeq = (ComSequence*)(rcEvt.getParameter("current_sequence").value<void*>());
    QString strResInfoText = QString("%1X%2").arg(pcCurSeq->getWidth()).arg(pcCurSeq->getHeight());
    this->ui->resolutionInfoLabel->setText(strResInfoText);
}

void MainWindow::onZooming(GitlUpdateUIEvt &rcEvt)
{
    int iPercent = qRound(rcEvt.getParameter("scale").toDouble()*100);
    this->ui->zoomSpinBox->setValue(iPercent);
}

/// Keyboard Event
void MainWindow::keyPressEvent ( QKeyEvent * event )
{
    if(event->key() == Qt::Key_Comma || event->key() == Qt::LeftArrow )
    {
        /// Next Frame
        on_previousFrame_clicked();
    }
    else if(event->key() == Qt::Key_Period || event->key() == Qt::RightArrow )
    {
        /// Previous Frame
        on_nextFrame_clicked();
    }

}


void MainWindow::on_previousFrame_clicked()
{
    /// invoke command
    GitlIvkCmdEvt cEvt("prev_frame");
    cEvt.dispatch();
}

void MainWindow::on_nextFrame_clicked()
{
    /// invoke command
    GitlIvkCmdEvt cEvt("next_frame");
    cEvt.dispatch();
}

void MainWindow::on_progressBar_actionTriggered(int action)
{
    int iBarPercent = int(100*double(ui->progressBar->sliderPosition()-ui->progressBar->minimum()) /
                                       (ui->progressBar->maximum()-ui->progressBar->minimum()));

    /// invoke command
    GitlIvkCmdEvt cEvt("jumpto_percent");
    cEvt.setParameter("percent", iBarPercent);
    cEvt.dispatch();
}


void MainWindow::xSaveSnapshot(QPixmap *pcPixmap)
{
    ///
    QString strFilename;
    QString strLastPath = g_cAppSetting.value("snapshot_saving_path",".").toString();
    strFilename=QFileDialog::getSaveFileName(this,
                                          tr("Save Snapshot"),
                                          strLastPath,
                                          tr("Images (*.png)"));
    if(!strFilename.isEmpty())
    {
        g_cAppSetting.setValue("snapshot_saving_path",strFilename);
        if( pcPixmap->save(strFilename) )
            qDebug() << QString("Snapshot Has Been Saved to %1 !").arg(strFilename);
        else
            qWarning() <<"Snapshot Saving Failed!";
    }


}


void MainWindow::on_actionOpen_HEVC_Bitstream_triggered()
{
    /// select file path
    QString strFilename;
    QString strLastPath = g_cAppSetting.value("open_hevc_bitstream_path",".").toString();
    strFilename=QFileDialog::getOpenFileName(this,
                                          tr("Open HEVC Bitstream File"),
                                          strLastPath,
                                          tr("All Files (*.*)"));

    if(!strFilename.isEmpty())
        g_cAppSetting.setValue("open_hevc_bitstream_path",strFilename);

    if(strFilename.isEmpty() || !QFileInfo(strFilename).exists() )
    {
        qWarning() << "File not found.";
        return;
    }

    /// select HM version
    HEVCBitstreamVersionSelector cBitstreamDig(this);
    if( cBitstreamDig.exec() == QDialog::Rejected )
        return;

    /// prepare & sent event to bus
    GitlIvkCmdEvt cEvt("open_bitstream");
    cEvt.setParameter("filename", strFilename);
    cEvt.setParameter("skip_decode", false);
    cEvt.setParameter("decoder", "HM");
    cEvt.setParameter("version", cBitstreamDig.getBitstreamVersion());
    cEvt.dispatch();
}


void MainWindow::on_actionOpen_VVC_Bitstream_triggered()
{
    /// select file path
    QString strFilename;
    QString strLastPath = g_cAppSetting.value("open_vvc_bitstream_path",".").toString();
    strFilename=QFileDialog::getOpenFileName(this,
                                          tr("Open VVC Bitstream File"),
                                          strLastPath,
                                          tr("All Files (*.*)"));

    if(!strFilename.isEmpty())
        g_cAppSetting.setValue("open_vvc_bitstream_path",strFilename);

    if(strFilename.isEmpty() || !QFileInfo(strFilename).exists() )
    {
        qWarning() << "File not found.";
        return;
    }

    /// select VTM version
    VVCBitstreamVersionSelector cBitstreamDig(this);
    if( cBitstreamDig.exec() == QDialog::Rejected )
        return;

    /// prepare & sent event to bus
    GitlIvkCmdEvt cEvt("open_bitstream");
    cEvt.setParameter("filename", strFilename);
    cEvt.setParameter("skip_decode", false);
    cEvt.setParameter("decoder", "VTM");
    cEvt.setParameter("version", cBitstreamDig.getBitstreamVersion());
    cEvt.dispatch();
}


void MainWindow::on_printScreenBtn_clicked()
{
    /// invoke command
    GitlIvkCmdEvt cEvt("print_screen");
    cEvt.dispatch();
}



void MainWindow::on_actionAbout_triggered()
{
    m_cAboutDialog.show();
}



void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionCheckUpdate_triggered()
{
    /// invoke command
    GitlIvkCmdEvt cEvt("check_update");
    cEvt.dispatch();
}

void MainWindow::on_actionReloadPluginsFilters_triggered()
{
    /// invoke command
    GitlIvkCmdEvt cEvt("reload_filter");
    cEvt.dispatch();
}

void MainWindow::on_actionPreferences_triggered()
{
    m_cPreferenceDialog.show();
}

void MainWindow::on_defaultThemeAction_triggered()
{
    GitlIvkCmdEvt cEvt("switch_theme");
    cEvt.setParameter("theme_name", "default");
    cEvt.dispatch();
}

void MainWindow::on_darkThemeAction_triggered()
{
    GitlIvkCmdEvt cEvt("switch_theme");
    cEvt.setParameter("theme_name", "dark");
    cEvt.dispatch();
}


void MainWindow::on_resetZoomBtn_clicked()
{
    GitlIvkCmdEvt cEvt("zoom_frame");
    cEvt.setParameter("scale", 1.0);
    cEvt.dispatch();
}

void MainWindow::on_zoomSpinBox_valueChanged(int arg1)
{
    /// if changed by user interaction (not programmatic)
    if(ui->zoomSpinBox->hasFocus() == true)
    {
        GitlIvkCmdEvt cEvt("zoom_frame");
        cEvt.setParameter("scale", arg1/100.0);
        cEvt.dispatch();
        ui->zoomSpinBox->clearFocus();  /// prevent infinite loop
    }
}

void MainWindow::on_zoomSpinBox_editingFinished()
{
    GitlIvkCmdEvt cEvt("zoom_frame");
    cEvt.setParameter("scale", ui->zoomSpinBox->value()/100.0);
    cEvt.dispatch();
}
