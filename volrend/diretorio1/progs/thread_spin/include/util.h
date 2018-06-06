#pragma once

#include <iostream>

enum version {
    SEQUENTIAL
};

void usage(version v, const char *name);
void fatal(const char *str);