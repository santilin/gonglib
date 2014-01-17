#ifndef _GONG_IMAGE_BOX_H_
#define _GONG_IMAGE_BOX_H_

/** @file gongimagebox.h Qt based image viewer
 * Proyecto gestiong. (C) 2003-2013, Francisco Santiago Capel Torres
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * See accompanying file copyright or a copy at <http://www.gnu.org/licenses/>.
 */

#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <gongvariant.h>

namespace gong {

class ImageBox: public QPushButton
{
    Q_OBJECT
public:
    ImageBox( QWidget *parent, const Xtring &name = Xtring::null,
              const Xtring &caption = Xtring::null, bool horizontal = true );
    bool isEdited() const { return mEdited; }
    void setEdited(bool edited) { mEdited = edited; }
    bool isJustEdited() const { return mJustEdited; }
    void setJustEdited(bool justedited) { mJustEdited = justedited; }
    QBoxLayout *getLayout() const { return pLayout; }
    QPushButton *getButton() { return this; }
    const QPixmap *getImage() const { return pImageLabel->pixmap(); }
    bool setImage( QPixmap *image );
    bool mustBeReadOnly() const { return mMustBeReadOnly; }
    void setMustBeReadOnly(bool must) { mMustBeReadOnly = must; }
    bool setImageData( const Variant &data );
    bool loadFromFile( const Xtring &filename );
    Variant toData() const;
    void clear();
    void setProposedFileName( const Xtring &name ) { mProposedFileName = name; }
    const Xtring &getProposedFileName() const { return mProposedFileName; }
    void displayInfo( const Xtring &errmessage = Xtring::null );


protected slots:
    virtual void slot_button_clicked();
private:
    QBoxLayout *pLayout;
    QLabel *pImageLabel, *pInfoLabel;
    QPushButton *pushClear, *pushSaveAs, *pushPaste, *pushCopy, *pushAdvanced;
    Xtring mProposedFileName;
    bool mHorizontal, mMustBeReadOnly, mEdited, mJustEdited;
    size_t mByteCount;
};

} // namespace gong

#endif // _GONG_IMAGE_BOX_H_
