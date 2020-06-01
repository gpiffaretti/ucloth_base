#ifndef UCLOTH_DEBUG_H
#define UCLOTH_DEBUG_H

#include <stdio.h>
#include <string>
#include <stdio.h>
#include <sstream>

typedef void(*FuncCallBack)(const char* message, int size);

class Debug
{
public:
    static void Log(const char* message);
    static void Log(const std::string message);
    static void Log(const int message);
    static void Log(const char message);
    static void Log(const float message);
    static void Log(const double message);
    static void Log(const bool message);

    static FuncCallBack callbackInstance;

private:
    static void SendLog(const std::stringstream &ss);

    
    
};

#endif