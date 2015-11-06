#include <unistd.h>
#include <QWidget>
#include <QValidator>
#include <QFontMetrics>
#include <QFocusEvent>
#include <QKeyEvent>
#include <gongformatter.h>
#include "gonglineedit.h"
#include "gongguiapplication.h"
#include "gongguimainwindow.h"


namespace gong {

int LineEdit::mGlobalValidating = 0;
int LineEdit::mLastGlobalValidating = 0;
Xtring LineEdit::mSpecialControlKeys = "ABEFM";

class LineEditValidator: public QValidator
{
public:
    LineEditValidator( QObject *parent ) : QValidator( parent ) {}
    QValidator::State validate ( QString & input, int & pos ) const;
};

QValidator::State LineEditValidator::validate( QString & input, int & position ) const
{
    LineEdit *le = static_cast<LineEdit *>(parent());
    if( le->mDoNotValidate )
        return QValidator::Acceptable;
    int startpos = le->mOldCursorPosition;

//     _GONG_DEBUG_PRINT(10, Xtring::printf("Formatting text='%s', pos=%d, oldpos=%d, mask='%s', format='%s'",
//                                          fromGUI(input).c_str(), position, startpos,
//                                          le->getMask().c_str(), le->getFormat().c_str() ) );
    // If the mask is empty, the one from RegConfig is used
    Xtring masked = le->mMaskFormatter->mask_inedit( input.local8Bit(),
                    le->mValueType,
                    le->mMask.c_str(), startpos, &position );
    input = toGUI( masked );
//     _GONG_DEBUG_PRINT(10, Xtring::printf("Result='%s', pos=%d",
//                                          masked.c_str(), position) );
    // If we change input, the validator is called again, so we must keep de position
    le->mOldCursorPosition = position;
    le->setEdited( true );
    le->calcValue();
    le->mValueValid = false;
    le->mFormatted = false;
    return QValidator::Acceptable;
}

LineEdit::LineEdit( QWidget *parent, const Xtring &name,
                    Variant::Type lvaluetype,
                    Xtring lformat, Xtring lmask, char lblankchar )
    : QLineEdit( parent ), mValidating( false ), mGettingFocus( false),
      mValueType( lvaluetype ), mFormat( lformat), mMask( lmask ),
      mValueValid( false ), mFormatted( false ), mJustEdited( false ),
      mWasFocusIn( false ), mCancelling( false ), mDoNotValidate(false),
      mSelectedOnEntry(false), mOldCursorPosition(0)
{
    setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
    setObjectName( toGUI(name) );
    mMaskFormatter = new Formatter( *GongLibraryInstance->getRegConfig(), lblankchar );
    connect( this, SIGNAL( cursorPositionChanged ( int, int ) ),
             this, SLOT( cursorPositionChanged ( int, int ) ) );
    setValidator( new LineEditValidator(this) );
}

LineEdit::~LineEdit()
{
    delete mMaskFormatter;
}


void LineEdit::setBlankChar( char lblankchar )
{
    mMaskFormatter->setBlankChar( lblankchar );
}

char LineEdit::getBlankChar() const
{
    return mMaskFormatter->getBlankChar();
}

void LineEdit::setValueType( Variant::Type lvaluetype )
{
    mValueValid = false;
    mFormatted = false;
    mValueType = lvaluetype;
}

Variant::Type LineEdit::getValueType() const
{
    return mValueType;
}

void LineEdit::setMask( const Xtring &lmask )
{
    mFormatted = false;
    mValueValid = false;
    mMask = lmask;
}

Xtring LineEdit::getMask() const
{
    return mMask;
}

void LineEdit::setFormat( const Xtring &lformat )
{
    mFormatted = false;
    mValueValid = false;
    mFormat = lformat;
}

Xtring LineEdit::getFormat() const
{
    return mFormat;
}

void LineEdit::setWidthInChars(int chars)
{
    setMinimumWidth( chars * fontMetrics().width('M') / 3 * 2 );
    resize( (chars + 1) * fontMetrics().width('M'), size().height() );
    setMaximumWidth( chars * fontMetrics().width('M') / 3 * 4 );
}

void LineEdit::cursorPositionChanged ( int old, int _new )
{
//     _GONG_DEBUG_PRINT(10, Xtring::printf("Position changed from %d to %d", old, _new) );
    if( _new != -1 )
        mOldCursorPosition = _new;
    else
        mOldCursorPosition = 0;
}

int LineEdit::toInt() const
{
    if ( mFormatted )
        return mMaskFormatter->unFormatInt( toString(), getFormat().c_str() );
    else
        return mMaskFormatter->unMaskInt( toString(), getMask().c_str() );
}

bool LineEdit::toBool() const
{
    return toString().toBool();
}


double LineEdit::toDouble() const
{
    // This prevents conversion errors when the decimal or thousand points for money and double differ
    if( mValueType == Variant::tMoney )
        return toMoney().toDouble();
    if ( mFormatted )
        return mMaskFormatter->unFormatDouble( toString(), getFormat().c_str() );
    else
        return mMaskFormatter->unMaskDouble( toString(), getMask().c_str() );
}

Money LineEdit::toMoney() const
{
    // This prevents conversion errors when the decimal or thousand points for money and double differ
    if( mValueType == Variant::tDouble )
        return toDouble();
    if ( mFormatted )
        return mMaskFormatter->unFormatMoney( toString(), getFormat().c_str() );
    else
        return mMaskFormatter->unMaskMoney( toString(), getMask().c_str() );
}

DateTime LineEdit::toDateTime() const
{
    if ( text().isEmpty() )
        return DateTime((const Date &)Date::NullDate, (const Time &)Time::NullTime);
    if ( mFormatted ) {
        return mMaskFormatter->unFormatDateTime( toString(), getFormat().c_str(), getValueType() );
    } else {
        return mMaskFormatter->unMaskDateTime( toString(), getMask().c_str(), getValueType() );
    }
}

Date LineEdit::toDate() const
{
    if ( text().isEmpty() )
        return Date(Date::NullDate);
    if ( mFormatted ) {
        return Date(mMaskFormatter->unFormatDateTime ( toString(), getFormat().c_str(), getValueType() ));
    } else {
        return Date(mMaskFormatter->unMaskDateTime( toString(), getMask().c_str(), getValueType() ) );
    }
}

Time LineEdit::toTime() const
{
    if ( text().isEmpty() )
        return Time(Time::NullTime);
    if ( mFormatted ) {
        return Time(mMaskFormatter->unFormatDateTime ( toString(), getFormat().c_str(), getValueType() ));
    } else {
        return Time(mMaskFormatter->unMaskDateTime( toString(), getMask().c_str(), getValueType() ) );
    }
}

void LineEdit::setText(bool val)
{
    setTextNoValidate( mMaskFormatter->format( val, getFormat().c_str(), getMask().c_str() ) );
    if ( mValueType == Variant::tInt ) {
        mValue = val;
        mValueValid = true;
    } else
        mValueValid = false;
}


void LineEdit::setText( int val )
{
    setTextNoValidate( mMaskFormatter->format( val, getFormat().c_str(), getMask().c_str() ) );
    if ( mValueType == Variant::tInt ) {
        mValue = val;
        mValueValid = true;
    } else
        mValueValid = false;
}

void LineEdit::setText( double val )
{
    mValueValid = false;
    try {
        setTextNoValidate( mMaskFormatter->format( val, getFormat().c_str(), getMask().c_str() ) );
        if ( mValueType == Variant::tDouble ) {
            mValue = val;
            mValueValid = true;
        }
    } catch( std::runtime_error ) {
        mValueValid = false;
        setTextNoValidate( "(Error)" );
    }
}

void LineEdit::setText( const Money &val )
{
    mValueValid = false;
    try {
        setTextNoValidate( mMaskFormatter->format( val, getFormat().c_str(), getMask().c_str() ) );
        if ( mValueType == Variant::tMoney) {
            mValue = val;
            mValueValid = true;
        }
    } catch( std::runtime_error ) {
        mValueValid = false;
        setTextNoValidate( "(Error)" );
    }
}


void LineEdit::setText( const DateTime &val )
{
    mValue = val;
    mValueValid = val.isValid();
    mFormatted = true;
    setTextNoValidate( mMaskFormatter->format( val, getFormat().c_str(), getMask().c_str() ) );
}

void LineEdit::setText( const Date &val )
{
    mValue = val;
    mValueValid = val.isValid();
    mFormatted = true;
    setTextNoValidate( mMaskFormatter->format( val, getFormat().c_str(), getMask().c_str() ) );
}

void LineEdit::setText( const Time &val )
{
    mValue = val;
    mValueValid = val.isValid();
    mFormatted = true;
    setTextNoValidate( mMaskFormatter->format( val, getFormat().c_str(), getMask().c_str() ) );
}


void LineEdit::setText( const Xtring &val )
{
    setTextNoValidate( val );
    if ( mValueType == Variant::tString ) {
        mValue = val;
        mValueValid = true;
    } else
        mValueValid = false;
}

void LineEdit::setText( const char *val)
{
    setTextNoValidate( Xtring(val) );
}

void LineEdit::setText( const Variant &val )
{
    switch ( val.type() ) {
    case Variant::tInt:
        setText( val.toInt() );
        break;
    case Variant::tBool:
        setText( val.toBool() );
        break;
    case Variant::tDouble:
        setText( val.toDouble() );
        break;
    case Variant::tMoney:
        setText( val.toMoney() );
        break;
    case Variant::tDateTime:
    case Variant::tDate:
    case Variant::tTime:
        setText( val.toDateTime() );
        break;
    default:
        setText( val.toString() );
    }
}


// Pseudo-const
Variant LineEdit::toVariant() const
{
    if ( !mValueValid )
        const_cast<LineEdit *>( this )->calcValue();
    return mValue;
}

bool LineEdit::calcValue()
{
    mValueValid = true;
    switch ( mValueType ) {
    case Variant::tInt:
        mValue = toInt();
        break;
    case Variant::tMoney:
        mValue = toMoney();
        break;
    case Variant::tDouble:
        mValue = toDouble();
        break;
    case Variant::tDate:
    {
        Date d = toDate();
        mValue = d;
        return d.isValid();
    }
    case Variant::tTime:
    {
        Time t = toTime();
        mValue = t;
        return t.isValid();
    }
    case Variant::tDateTime:
    {
        DateTime dt = toDateTime();
        mValue = dt;
        return dt.isValid();
    }
    break;
    default:
        mValue = toString();
    }
    return true;
}


/* Redefinida porque no hay ninguna funcion virtual para borrar */
void LineEdit::keyPressEvent( QKeyEvent *e )
{
    mCancelling = false;
    if( !mSpecialControlKeys.isEmpty() && e->modifiers() == Qt::ControlModifier ) {
        _GONG_DEBUG_PRINT(10, Xtring::printf("looking for key %c in %s", e->key(), mSpecialControlKeys.c_str() ) );
        if( mSpecialControlKeys.find( (int)e->key() ) != Xtring::npos ) {
            emit specialControlKeyPressed( this, (char)e->key() );
            return;
        }
    }
    if ( e->state() == 0 || ( (e->state() & Qt::KeypadModifier) && (e->key() == Qt::Key_Enter) ) ) {
        switch ( e->key() ) {
        case Qt::Key_F3:
            emit specialControlKeyPressed( this, 'F' );
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
        case Qt::Key_Down:
            theGuiApp->sendKeyEvent( this, Qt::Key_Tab, '\t', 0 );
            return;
        case Qt::Key_Up:
            theGuiApp->sendKeyEvent( this, Qt::Key_Backtab, '\t', 0 );
            return;
        }
    }
    if( e->key() != Qt::Key_F12 && e->text().length() != 0 )
        mJustEdited = true;
    QLineEdit::keyPressEvent( e );
}


void LineEdit::setReadOnly( bool readonly )
{
    if ( readonly ) {
        QPalette pal = palette();
        QColor inactivecolor = pal.color( QPalette::Disabled, QColorGroup::Window );
        pal.setColor( QPalette::Active, QColorGroup::Base, inactivecolor );
        setPalette( pal );
        setFocusPolicy( Qt::ClickFocus );
    } else {
        unsetPalette();
        setFocusPolicy( Qt::StrongFocus );
    }
    QLineEdit::setReadOnly( readonly );
    update();
}

void LineEdit::focusOutEvent( QFocusEvent *e )
{
//     _GONG_DEBUG_PRINT(10, Xtring::printf("%s lost focus, reason=%d", name(), e->reason() ) );
    if( e->reason() == Qt::ActiveWindowFocusReason && !theGuiApp->focusWidget() )
        return;
//     _GONG_DEBUG_PRINT(10, Xtring::printf("%s::modified=%d, justEdited=%d, cursorpos=%d",
//                                          name(), isModified(), isJustEdited(), cursorPosition() ) );
    mDoNotValidate = true;
    QLineEdit::focusOutEvent( e );
    mDoNotValidate = false;
    if ( mCancelling
            || !theGuiApp->focusWidget()
// 	    || !strlen(theGuiApp->focusWidget()->name())
            || !strcmp(theGuiApp->focusWidget()->name(), "pushCancel")
            || (theGuiApp->getMainWindow() && theGuiApp->getMainWindow()->amIClosing() ) ) {
//         _GONG_DEBUG_PRINT(10, Xtring::printf("%s::Cancelando, se ignora", name() ) );
        mCancelling = false;
        return;
    }
    // Cuando elegimos un menu, es la razon Popup, pero al abrir otra ventana, la razon es Tab
    // por lo tanto eso nos hace validar el editline. Lo ideal seria que cuando se abre otra ventana
    // no se validase
    if ( e->reason() != Qt::MouseFocusReason
            && e->reason() != Qt::TabFocusReason
            && e->reason() != Qt::OtherFocusReason // F12
            && e->reason() != Qt::BacktabFocusReason ) {
//         _GONG_DEBUG_PRINT(10, Xtring::printf("%s::Razón no valida %d. Salgo", name(), e->reason() ) );
        return;
    }
    if ( !mWasFocusIn ) {
        /* There wasn't a clean focusin, so I suppose that this is due to
           some problem validating, for example, a new window pops up from
           within the validation code
        */
//         _GONG_DEBUG_PRINT(10, Xtring::printf( "%s::No he notado el focus in. Salgo", name() ) );
        return;
    } else
        mWasFocusIn = false;
    if ( mGlobalValidating == mLastGlobalValidating && mGlobalValidating ) {
//         _GONG_DEBUG_PRINT(10, Xtring::printf( "%s::Estoy validando. Salgo", name() ) );
        return;
    }
    if ( mGettingFocus ) {
//         _GONG_DEBUG_PRINT(10, Xtring::printf("%s::Estoy retomando el foco. Salgo", name() ) );
        return;
    }
    if ( mValidating ) {
//         _GONG_DEBUG_PRINT(10, Xtring::printf("%s::Estoy validando. Salgo", name() ) );
        return ;
    }
    Xtring former_text = toString();
    bool isvalid = calcValue();
//     _GONG_DEBUG_PRINT(10, Xtring::printf("%s: text=%s, value=%s, type=%s", name(),
//                                          fromGUI(text()).c_str(),
//                                          toVariant().toString().c_str(),
//                                          Variant::typeToName( mValueType ) ) );
    if( isvalid ) {
//         _GONG_DEBUG_PRINT(10, Xtring::printf("%s::mCancelling == false", name() ) );
        mValidating = true;
        mLastGlobalValidating = mGlobalValidating;
        mGlobalValidating++;
//         _GONG_DEBUG_PRINT(10, Xtring::printf("%s::about to emit validate", name() ) );
        try {
            emit validate( this, &isvalid );
        } catch( std::overflow_error &e ) {
            isvalid = false;
        }
//         _GONG_DEBUG_PRINT(10, Xtring::printf("%s::emit validate returned %d", name(), isvalid ) );
    }
    if ( !isvalid ) {
        setTextNoValidate("(Error)");
        for( unsigned int lapso =0; lapso < 2000; lapso ++ ) {
            theGuiApp->processEvents();
            usleep(10);
        }
        mGettingFocus = true;
//         _GONG_DEBUG_PRINT(10, Xtring::printf("%s::como no es valido, retomo el foco", name() ) );
//         _GONG_DEBUG_PRINT(10, Xtring::printf("Setting focus to %s", this->name() ) );
        setFocus();
//         _GONG_DEBUG_PRINT(10, Xtring::printf("%s::ya he retomado el foco", name() ) );
        mGettingFocus = false;
        setTextNoValidate( former_text );
    } else {
        mJustEdited = false;
//         _GONG_DEBUG_PRINT(10, "Setting just edited a FALSE: " + Xtring(name()) );
    }
    mCancelling = false;
    if( isvalid ) {
        Xtring formattedtext = mMaskFormatter->format( toVariant(), getFormat().c_str(), getMask().c_str());
//         _GONG_DEBUG_PRINT(10, Xtring::printf("Formatting %s: value: %s, formatted: %s, masked=%s",
//                                              name(), toVariant().toString().c_str(),
//                                              formattedtext.c_str(), toString().c_str() ) );
        // Cuando formattedtext esta vacio, es nulo, y entonces es diferente de text() vacio
        // Esto se puede solucionar cuando format devuelva y acepte QStrings
        if ( formattedtext != toString() ) {
            setTextNoValidate( formattedtext );
        }
        mFormatted = true;
    }
    mLastGlobalValidating = mGlobalValidating;
    mGlobalValidating--;
    mValidating = false;
}


/* Tries to set the position of the cursor apropiately. But if it is
   of type currency or double and it get focus via a mouse click,
   the setCursorPosition is ignored.
*/
void LineEdit::focusInEvent( QFocusEvent *e )
{
//     _GONG_DEBUG_PRINT(10, Xtring::printf("%s got focus", name() ) );
    if( e->reason() == Qt::ActiveWindowFocusReason && !theGuiApp->focusWidget() )
        return;
    mWasFocusIn = true;
    mOldText = fromGUI( text() );
	int cp = cursorPosition();
//     _GONG_DEBUG_PRINT(0, Xtring::printf("%s::value=%s,modified=%d, justModified=%d, cursorpos=%d",
//                                          name(), mOldText.c_str(), isModified(), isJustEdited(), cp ) );
    QLineEdit::focusInEvent(e);
    if( mCancelling ) {
        mCancelling = false;
        return;
    }
    mCancelling = false;
    if( isReadOnly() ) return;
    if( calcValue() ) {
//         _GONG_DEBUG_PRINT(0, Xtring::printf("%s: text=%s, value=%s, type=%s", name(),
//                                              fromGUI(text()).c_str(),
//                                              toVariant().toString().c_str(),
//                                              Variant::typeToName( mValueType ) ) );
        Xtring maskedtext = mMaskFormatter->mask( toVariant(), getMask().c_str(), getValueType() );
        // Si cambiamos el text en focus in, hay que respetar el estado de la propiedad edited, porque realmente
        // aun no hemos editado nada
        if ( maskedtext != toString() ) {
//             _GONG_DEBUG_PRINT(0, Xtring::printf("%s != %s", maskedtext.c_str(), toString().c_str() ) );
            setTextNoValidate( maskedtext );
        }
        mFormatted = false;
    }
    mPreviousValue = mValue;
    unsigned int pos = 0;
    switch ( getValueType() ) {
    case Variant::tMoney:
    case Variant::tDouble:
    case Variant::tInt:
		if (e->reason() == Qt::MouseFocusReason)
			QTimer::singleShot(0, this, SLOT(selectAll()));
		else
			selectAll();
		break;
    case Variant::tDate:
    case Variant::tTime:
    case Variant::tDateTime:
        if( mSelectedOnEntry ) {
			if (e->reason() == Qt::MouseFocusReason)
				QTimer::singleShot(0, this, SLOT(selectAll()));
			else
				selectAll();
		}
        else {
            pos = Xtring(toString()).find_first_not_of(" ");
            if( pos == Xtring::npos )
                pos = 0;
            setCursorPosition( pos );
        }
        break;
    default:
        if( mSelectedOnEntry ) {
			if (e->reason() == Qt::MouseFocusReason)
				QTimer::singleShot(0, this, SLOT(selectAll()));
			else
				selectAll();
		} else {
            pos = toString().length();
            setCursorPosition( pos );
        }
        break;
    }
}

void LineEdit::setTextNoValidate(const Xtring &text)
{
    int curpos = cursorPosition();
    mValueValid = false;
    mDoNotValidate = true;
    bool edited = isModified();
//     _GONG_DEBUG_PRINT(10, Xtring::printf("%s:setTextNoValidate(%s)", name(), text.c_str() ) );
    QLineEdit::setText( toGUI( text ) );
    setModified( edited );
    setCursorPosition( curpos );
    mDoNotValidate = false;
}

Xtring LineEdit::toString() const
{
    return fromGUI(QLineEdit::text());
}

bool LineEdit::isEdited() const
{
    return isModified();
}

void LineEdit::setEdited( bool edited )
{
    setModified( edited );
}

void LineEdit::setJustEdited(bool justedited)
{
    mJustEdited = justedited;
}

bool LineEdit::isJustEdited() const
{
#ifdef _GONG_DEBUG
    if( !mJustEdited && ( isModified() ) ) {
//         _GONG_DEBUG_PRINT(10, Xtring::printf( "Field %s is not edited but is modified so isJustEdited()==true", name() ) );
    }
    if( !mJustEdited && ( fromGUI( text() ) != mOldText ) ) {
//         _GONG_DEBUG_PRINT(10, Xtring::printf( "Field %s is not edited but text '%s' is different from oldtext '%s' so isJustEdited()==true",
//                                               name(), fromGUI( text() ).c_str(), mOldText.c_str() ) );
    }
//     _GONG_DEBUG_PRINT(10, (mJustEdited ? Xtring("mJustEdited es TRUE") : Xtring("mJustEdited es FALSE")) );
#endif
    return mJustEdited;
}

void LineEdit::emitValidate(bool* isvalid)
{
    emit validate( this, isvalid );
}

void LineEdit::setCursorAtDecPoint()
{
    char decpoint = mMaskFormatter->getRegConfig().getDecimalPoint();
    int pos = toString().find(decpoint);
    setCursorPosition( pos );
//     _GONG_DEBUG_PRINT( 10, Xtring::printf("%s: text=%s, cursorpos=%d",
//                                           name(), toString().c_str(), pos ) );
}

} // namespace
