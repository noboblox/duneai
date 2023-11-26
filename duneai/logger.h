#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdarg.h>

class Logger;
extern const Logger* gLog;

class Logger
{
public:
	Logger() {}
	virtual ~Logger() {}

	virtual void crit(const char* format, ...)  const = 0;
	virtual void error(const char* format, ...) const = 0;
	virtual void warn(const char* format, ...)  const = 0;
	virtual void info(const char* format, ...)  const = 0;
	virtual void debug(const char* format, ...) const = 0;
};


class StdoutLogger : public Logger
{
public:
	StdoutLogger() {}

	void crit(const char* format, ...)  const override;
	void error(const char* format, ...) const override;
	void warn(const char* format, ...)  const override;
	void info(const char* format, ...)  const override;
	void debug(const char* format, ...) const override;

private:
	void printConsole(const char* level, const char* format, va_list args) const noexcept;
};

#endif /* LOGGER_H_ */
