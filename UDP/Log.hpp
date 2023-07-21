#include <cstdint>
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include <cassert>
#include <stdlib.h>
#include <ctime>

#define DEBUG    1
#define NOTICE   2
#define WARNING  3
#define FATAL    4

const char* log_level[] = {"", "DEBUG", "NOTICE", "WARNING", "FALSE"};

// 格式化控制日志
void logMessage(int level, const char *format, ...)
{
	assert(level >= DEBUG);
	assert(level <= FATAL);

	char *name = getenv("USER");
	char logInfo[1024];
	va_list ap;

	// 特定用法
	va_start(ap, format);
	vsnprintf(logInfo, sizeof(logInfo) - 1, format, ap);
	va_end(ap);

	FILE *out = (level == FATAL)?stderr:stdout;
	fprintf(out, "%s | %u | %s | %s\n", log_level[level], (unsigned int)time(nullptr), name==nullptr?"unknow":name, logInfo);
}

