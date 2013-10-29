#ifndef _GONG_FRMIMGADVANCED_H_
#define _GONG_FRMIMGADVANCED_H_

/** @file gongfrmimgadvanced.h Form for advanced editing of images
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QLabel>
#include "gongcombobox.h"
#include "gongfrmbase.h"

class QRubberBand;

namespace gong {
class ImageBox;

class ResizableImageLabel: public QLabel
{
public:
    ResizableImageLabel( QWidget *parent, const char *name )
        : QLabel( parent, name), pRubberBand(0) {}
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QRubberBand *getRubberBand() const {
        return pRubberBand;
    }
private:
    QPoint mOrigin;
    QRubberBand *pRubberBand;
};

class FrmResizeImage: public FrmBase
{
    Q_OBJECT
public:
    FrmResizeImage(int sizex, int sizey, QWidget *parent );
    int getSizeX() const {
        return pEditSizeX->toInt();
    }
    int getSizeY() const {
        return pEditSizeY->toInt();
    }
    int getByteCount() const {
        return pEditByteCount->toInt();
    }
    bool getAspect() const {
        return pCheckAspect->isOn();
    }
protected slots:
    void validateFields(QWidget *, bool *);
private:
    LineEdit *pEditSizeX, *pEditSizeY, *pEditByteCount;
    QCheckBox *pCheckAspect;
    double mAspectRatio;
};


class FrmImgAdvanced: public FrmBase
{
    Q_OBJECT
public:
    FrmImgAdvanced( ImageBox *imgbox, const Xtring &convert_path,
                    QWidget *parent, const char *name );
    void setImageData( const Variant &data );
    Variant getImageData() const;
    void accept();
    void displayInfo();
protected:
    int execCommandAndShow( const Xtring &command );
protected slots:
    virtual void slot_button_clicked();
    virtual void slot_combo_activated(int);
private:
    uint mByteCount;
    QLabel *pInfoLabel;
    ResizableImageLabel *pImageLabel;
    QPushButton *pushResize, *pushCrop, *pushReset;
    ComboBox<int> *comboEffects;
    ImageBox *pImageBox;
    Xtring mConvertPath;
};

} // namespace gong

#endif // _GONG_FRMIMGADVANCED_H_
