#include "uclothdebug.h"

FuncCallBack Debug::callbackInstance = nullptr;

void Debug::Log(const char* message) {
    if (callbackInstance != nullptr)
        callbackInstance(message, (int)strlen(message));
}

void  Debug::Log(const std::string message) {
    const char* tmsg = message.c_str();
    if (callbackInstance != nullptr)
        callbackInstance(tmsg, (int)strlen(tmsg));
}

void  Debug::Log(const int message) {
    std::stringstream ss;
    ss << message;
    SendLog(ss);
}

void  Debug::Log(const char message) {
    std::stringstream ss;
    ss << message;
    SendLog(ss);
}

void  Debug::Log(const float message) {
    std::stringstream ss;
    ss << message;
    SendLog(ss);
}

void  Debug::Log(const double message) {
    std::stringstream ss;
    ss << message;
    SendLog(ss);
}

void Debug::Log(const bool message) {
    std::stringstream ss;
    if (message)
        ss << "true";
    else
        ss << "false";

    SendLog(ss);
}

void Debug::SendLog(const std::stringstream &ss) {
    const std::string tmp = ss.str();
    const char* tmsg = tmp.c_str();
    if (callbackInstance != nullptr)
        callbackInstance(tmsg, (int)strlen(tmsg));
}
