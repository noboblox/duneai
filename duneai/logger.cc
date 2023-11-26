/*
 * Logger.cpp
 *
 *  Created on: 25.11.2022
 *      Author: Rene
 */

#include "logger.h"
#include <cstdarg>
#include <cstdio>

static const StdoutLogger sDefaultLogger;
const Logger* gLog = &sDefaultLogger;

void StdoutLogger::crit(const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	printConsole("CRIT", format, args);
	va_end(args);
}

void StdoutLogger::error(const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	printConsole("ERROR", format, args);
	va_end(args);
}

void StdoutLogger::warn(const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	printConsole("WARN", format, args);
	va_end(args);
}

void StdoutLogger::info(const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	printConsole("INFO", format, args);
	va_end(args);
}

void StdoutLogger::debug(const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	printConsole("DEBUG", format, args);
	va_end(args);
}

void StdoutLogger::printConsole(const char* level, const char* format, va_list args) const noexcept
{
	printf("[%5s] ", level);
	vprintf(format, args);
	putchar('\n');
}
