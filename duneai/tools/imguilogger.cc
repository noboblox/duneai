/*
 * imguilogger.cc
 *
 *  Created on: 03.12.2022
 *      Author: Rene
 */

#include "imguilogger.h"
#include <cstdio>

#include "../imgui/imgui.h"

void ImGuiLogger::crit(const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	log("CRIT", format, args);
	va_end(args);
}

void ImGuiLogger::error(const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	log("ERROR", format, args);
	va_end(args);
}

void ImGuiLogger::warn(const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	log("WARN", format, args);
	va_end(args);
}

void ImGuiLogger::info(const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	log("INFO", format, args);
	va_end(args);
}

void ImGuiLogger::debug(const char* format, ...) const
{
	va_list args;
	va_start(args, format);
	log("DEBUG", format, args);
	va_end(args);
}

void ImGuiLogger::log(const char* level, const char* format, va_list args) const noexcept
{
	static char out[256];
	out[0] = '\0';

	std::vsnprintf(out, sizeof(out), format, args);
	mLogFunc(out);
}
