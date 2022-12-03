#ifndef TOOLS_IMGUILOGGER_H_
#define TOOLS_IMGUILOGGER_H_

#include "../logger.h"
#include <functional>

class ImGuiLogger : public Logger {
public:
	ImGuiLogger(std::function<void(const char*)> logFunction)
	: mLogFunc(logFunction)
	{
	}

	void crit(const char* format, ...)  const override;
	void error(const char* format, ...) const override;
	void warn(const char* format, ...)  const override;
	void info(const char* format, ...)  const override;
	void debug(const char* format, ...) const override;

private:
	void log(const char* level, const char* format, va_list args) const noexcept;
	std::function<void(const char*)> mLogFunc;
};

#endif /* TOOLS_IMGUILOGGER_H_ */
