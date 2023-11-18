//
// Created by shaop on 2023/11/18.
//
// HookHelper.cpp
#include "HookHelper.h"

HookHelper::HookHelper() {}

HookHelper::~HookHelper() {}

void HookHelper::addToJavaStack(const std::string& message) {
    // 将消息添加到 java_stack
    java_stack << message << std::endl;
}

std::string HookHelper::getJavaStack() const {
    // 获取 java_stack 的内容
    return java_stack.str();
}
