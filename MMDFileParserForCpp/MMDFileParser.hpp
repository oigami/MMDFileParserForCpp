#pragma once

#include <MMDFileParser/VMDReader.hpp>
#include <MMDFileParser/VMDWriter.hpp>
#include <MMDFileParser/PMDReader.hpp>

#ifdef NDEBUG

# ifdef _DLL
#pragma comment(lib,"MMDFileParserForCpp-MD.lib")
# else
#pragma comment(lib,"MMDFileParserForCpp-MT.lib")
# endif

#else

# ifdef _DLL
#pragma comment(lib,"MMDFileParserForCpp-MD-d.lib")
# else
#pragma comment(lib,"MMDFileParserForCpp-MT-d.lib")
# endif

#endif


