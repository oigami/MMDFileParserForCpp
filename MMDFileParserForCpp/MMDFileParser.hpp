#pragma once

#include <MMDFileParser/VMDReader.hpp>
#include <MMDFileParser/VMDWriter.hpp>
#include <MMDFileParser/PMDReader.hpp>

#define VS_COMPILER_VER "v141"

#ifdef _DLL

#define RUNTIME_LIBRARY "MD"

#else

#define RUNTIME_LIBRARY "MT"

#endif


#ifdef NDEBUG

#pragma comment(lib,"MMDFileParserForCpp-" VS_COMPILER_VER "-" RUNTIME_LIBRARY ".lib")

#else

#pragma comment(lib,"MMDFileParserForCpp-" VS_COMPILER_VER "-" RUNTIME_LIBRARY "-d.lib")

#endif

#undef RUNTIME_LIBRARY
#undef VS_COMPILER_VER
