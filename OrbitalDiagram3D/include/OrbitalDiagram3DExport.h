#pragma once


#if defined(ORBITALDIAGRAM3D_BUILD_DLL)
#  define OD_API __declspec(dllexport)
#else
#  define OD_API __declspec(dllimport)
#endif