#pragma once
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#ifndef SAFE_DELETE
#define SAFE_DELETE(p){if(p){delete p;p = NULL;}}			
#endif // !SAFE_DELETE
#ifndef SAFE_FREE
#define SAFE_FREE(p)   {if(p){free(p);p = NULL;}}
#endif // !SAFE_FREE(p)

#ifdef _MSC_VER
typedef signed __int64											int64;
typedef signed __int32											int32;
typedef signed __int16											int16;
typedef signed __int8											int8;
typedef unsigned __int64										uint64;
typedef unsigned __int32										uint32;
typedef unsigned __int16										uint16;
typedef unsigned __int8											uint8;
#else
typedef int64_t													int64;
typedef int32_t													int32;
typedef int16_t													int16;
typedef int8_t													int8;
typedef uint64_t												uint64;
typedef uint32_t												uint32;
typedef uint16_t												uint16;
typedef uint8_t													uint8;
#endif

#endif // !_PLATFORM_H_
