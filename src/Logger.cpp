//author:Patryk Karbownik

#include "Logger.h"

#include <utility>

Logger::Logger(string loggerName) {
    this->loggerName = std::move(loggerName);
    createLoggerFile();
}

void Logger::createLoggerFile()
{
    mkdir(loggerDir, 0777);

    string fileName = loggerDir;
    fileName += "/";
    fileName += loggerName;
    streamToLoggerFile.open(fileName);

    if( !streamToLoggerFile)
        throw runtime_error("Not able to create logger file");
}

void Logger::writeDate(char *date)
{
    string theDate(date);
    streamToLoggerFile << theDate.substr(0, theDate.size() - 1);
}

void Logger::write(string s)
{
    time_t now = time(0);
    char* cNow = ctime(&now);

    write(s, cNow);
}

void Logger::write(string s, char *date)
{
    writeDate(date);
    streamToLoggerFile << ": " + s << endl;
}

void Logger::close()
{
    streamToLoggerFile.close();
}

Logger::~Logger()
{
    close();
}
