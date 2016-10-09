#ifndef __DEBUG_H__
#define __DEBIG_H__

int dbg_text(const char* format, ...);

#define dbg_info(format) dbg_text("%s: ", __PRETTY_FUNCTION__); \
			 dbg_text(format);

#endif

