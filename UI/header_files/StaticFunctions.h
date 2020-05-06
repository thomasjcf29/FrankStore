#ifndef FRANKSTOREUI_STATICFUNCTIONS_H
#define FRANKSTOREUI_STATICFUNCTIONS_H

#include <string.h>

class StaticFunctions{
private:
    StaticFunctions();
public:
    static std::string commandExec(const char* cmd);
};

#endif //FRANKSTOREUI_STATICFUNCTIONS_H
