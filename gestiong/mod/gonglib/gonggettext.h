#ifndef _GONG_GETTEXT_H
#define _GONG_GETTEXT_H

#ifdef ENABLE_NLS
#include <libintl.h>
#define _(String) gettext( String )
#define gettext_noop(String) String
#else
#define _(String) String
#define gettext_noop(String) String
#endif

#define QT_NO_TRANSLATION_UTF8

#endif // _GONG_GETTEXT_H
