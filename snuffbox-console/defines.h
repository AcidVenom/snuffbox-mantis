#pragma once

#define wxUSE_MENUS 1
#define wxUSE_STATUSBAR 1

#ifdef SNUFF_WIN32
	#include <wx/setup.h>
	#if wxUSE_WEBVIEW_WEBKIT == 0
		#undef wxUSE_WEBVIEW_WEBKIT
		#define wxUSE_WEBVIEW_WEBKIT 1
	#endif
#endif

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif
