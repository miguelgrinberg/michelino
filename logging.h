#ifdef LOGGING

// logging is enabled
#include <stdarg.h>

void log(char* format, ...)
{
    char line[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(line, sizeof(line), format, args);
    va_end(args);
    Serial.print(line);
}

#else

// logging is disabled
#define log(...)

#endif
