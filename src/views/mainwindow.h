#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QActionGroup>
#include "gitlmodual.h"
#include "busydialog.h"
#include "aboutdialog.h"
#include "sequencelist.h"
#include "gitlview.h"
#include "preferencedialog.h"
namespace Ui {
    class MainWindow;
}

class MainWindow :  public QMainWindow, public GitlView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void onUIUpdate(GitlUpdateUIEvt& rcEvt);
    void onPOCInfoChanged(GitlUpdateUIEvt& rcEvt);
    void onStylesheetChanged(GitlUpdateUIEvt& rcEvt);
    void onSnapshot(GitlUpdateUIEvt& rcEvt);
    void onSequenceChanged(GitlUpdateUIEvt& rcEvt);
    void onZooming(GitlUpdateUIEvt& rcEvt);

protected:
    virtual void keyPressEvent ( QKeyEvent * event );

protected:

    void xSaveSnapshot(QPixmap* pcPixmap);

private slots:

    void on_previousFrame_clicked();

    void on_nextFrame_clicked();

    void on_progressBar_actionTriggered(int action);

    void on_actionOpen_HEVC_Bitstream_triggered();

    void on_actionOpen_VVC_Bitstream_triggered();

    //void on_actionOpen_bitstream_info_folder_triggered();



    void on_printScreenBtn_clicked();

    void on_actionAbout_triggered();

    void on_actionExit_triggered();

    void on_actionCheckUpdate_triggered();

    void on_actionReloadPluginsFilters_triggered();

    void on_actionPreferences_triggered();

    void on_defaultThemeAction_triggered();

    void on_darkThemeAction_triggered();


    void on_resetZoomBtn_clicked();

    void on_zoomSpinBox_valueChanged(int arg1);

    void on_zoomSpinBox_editingFinished();

private:
    Ui::MainWindow *ui;


private:
    ADD_CLASS_FIELD_PRIVATE(BusyDialog, cBusyDialog)
    ADD_CLASS_FIELD_PRIVATE(AboutDialog, cAboutDialog)
    ADD_CLASS_FIELD_PRIVATE(PreferenceDialog, cPreferenceDialog)
    ADD_CLASS_FIELD_PRIVATE(QActionGroup, cThemeGroup)
};

#endif // MAINWINDOW_H
