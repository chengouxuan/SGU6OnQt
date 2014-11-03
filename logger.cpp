#include "logger.h"

Logger::Logger(const QString &logFilePath): file(logFilePath)
{
    file.open(QFile::Append);
    setDevice(&file);
}

Logger::~Logger()
{
    if (file.isOpen()) {
        file.close();
    }
}


