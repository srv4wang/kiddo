#pragma once
#include <string>
#include <typeinfo>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include <cxxabi.h>

std::string get_msg_id();

int get_cpu_core_num();

template <typename T>
std::string type_name(const T& t) {
    const char* name = typeid(t).name();
    char buf[1024] = "";
    size_t size = sizeof(buf);
    int status;
    char* res = abi::__cxa_demangle(name, buf, &size, &status);
    return res;
}
