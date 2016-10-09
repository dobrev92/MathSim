#include <stdarg.h>
#include "debug.h"

#if 1 //__DEBUG__

#include <iostream>
#include <fstream>
#include <string>

int dbg_text(const char* format, ...)
{
	va_list args;
	int done;
	char buffer[150];

	va_start(args, format);
	done = vsprintf(buffer, format, args);
	va_end(args);
	std::string str(buffer);
	std::cout<<str;
	return done;
}
#else

int dbg_info(int n_args, ...)
{
	return 0;
}
#endif
