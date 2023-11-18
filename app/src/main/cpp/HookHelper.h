//
// Created by shaop on 2023/11/18.
//

// HookHelper.h
#ifndef HOOK_HELPER_H
#define HOOK_HELPER_H

#include <sstream>
#include <string>

class HookHelper {
public:
    HookHelper();
    ~HookHelper();

    void addToJavaStack(const std::string& message);
    std::string getJavaStack() const;

private:
    std::ostringstream java_stack;
};

#endif  // HOOK_HELPER_H
