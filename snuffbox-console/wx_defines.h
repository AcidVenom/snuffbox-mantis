#pragma once

#define wxUSE_MENUS 1
#define wxUSE_STATUSBAR 1

#include <wx/setup.h>
#if wxUSE_WEBVIEW_WEBKIT == 0
	#undef wxUSE_WEBVIEW_WEBKIT
	#define wxUSE_WEBVIEW_WEBKIT 1
#endif

#if wxUSE_STL == 0
	#undef wxUSE_STL
	#define wxUSE_STL 1
	#define wxUSE_STD_STRING_CONV_IN_WXSTRING wxUSE_STL
#endif

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
