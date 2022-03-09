#ifndef VVCBITSTREAMVERSIONSELECTOR_H
#define VVCBITSTREAMVERSIONSELECTOR_H

#include <QDialog>
#include "gitldef.h"

namespace Ui {
class VVCBitstreamVersionSelector;
}

class VVCBitstreamVersionSelector : public QDialog
{
    Q_OBJECT

public:
    explicit VVCBitstreamVersionSelector(QWidget *parent = nullptr);
    ~VVCBitstreamVersionSelector();

    ADD_CLASS_FIELD(int, iBitstreamVersion, getBitstreamVersion, setBitstreamVersion)

    private slots:
    void on_version160_clicked();

    void on_buttonBox_accepted();


protected:
    virtual void showEvent(QShowEvent * event);
    virtual void hideEvent(QHideEvent * event);
    void xSetDecoderVersion();

private:
    Ui::VVCBitstreamVersionSelector *ui;
};

#endif // VVCBITSTREAMVERSIONSELECTOR_H
