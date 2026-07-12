#pragma once

#ifdef _WIN32
#include <BaseTsd.h>

#ifndef ssize_t
using ssize_t = SSIZE_T;
#endif // ssize_t

#endif // _WIN32
