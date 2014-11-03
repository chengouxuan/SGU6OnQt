#ifndef LOGGER_H
#define LOGGER_H

#include <QTextStream>
#include <QString>
#include <QFile>

class Logger : public QTextStream
{
public:
    explicit Logger(const QString &logFilePath);
    ~Logger();

private:
    QFile file;
};

#endif // LOGGER_H
