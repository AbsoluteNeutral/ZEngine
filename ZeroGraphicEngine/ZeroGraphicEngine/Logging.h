
#ifndef _LOGGING_H
#define _LOGGING_H

#include <stdio.h>

#define _GENERATE_ERROR_LOG 1
#define _USE__MESSAGE_BOX 1
//#define _USE__ASSERT 1

//Different kind of breaks
#ifdef _MSC_VER
	#define G_DEBUG_BREAK __debugbreak()
#else
	#define G_DEBUG_BREAK do { __asm int 3 } while(0)
#endif

//Enable Warning/Error message
#if !defined(ZERO_ENABLE_ERROR) 
	#if defined(_DEBUG)
		#define ZERO_ENABLE_ERROR 1
	#else
		//#define ZERO_ENABLE_ERROR 1
		#define ZERO_ENABLE_ERROR 0
	#endif
#endif

//Signal Box Pop-up
ZG_API void SignalBox(const char* msg, const char* functionName_, bool iserror = true);
ZG_API void PrintColor(bool Expression_, const char* functionname, const char* format, ...);
ZG_API void JustPrint(const char* format, ...);
ZG_API void PrintError(const char* format, ...);
#if defined(_DEBUG)

#else

#endif

//Printf wrap
static int gConditionalFalseConstant = 0;

//Pop-up message box
#define PopUp_WarnIf(exp, m) \
		if (exp) { SignalBox(m, __FUNCTION__, false); }

#define PopUp_Warn(m) \
		{ SignalBox(m, __FUNCTION__, false); }

#define PopUp_ErrorIf(exp, m) \
		if (exp) { SignalBox(m, __FUNCTION__);}

#define PopUp_Error(m) \
		SignalBox(m, __FUNCTION__);

#define Logg(...)\
	{ JustPrint(__VA_ARGS__); fprintf(stderr, "\n"); }

#define TestLog(Expression, ...)\
	{ PrintColor(Expression, __FUNCTION__,  __VA_ARGS__); fprintf(stderr, "\n"); }

#if ZERO_ENABLE_ERROR

#define UnusedParameter(param) param

//Console Message


#define Warn(...)\
do { PrintError(__VA_ARGS__); fprintf(stderr, "\n"); } while(gConditionalFalseConstant)

#define WarnIf(Expression, ...) \
do { if((Expression)) { PrintError(__VA_ARGS__); fprintf(stderr, "\n"); } } while(gConditionalFalseConstant)

#define ErrorMsg(...)\
do { G_DEBUG_BREAK; PrintError(__VA_ARGS__); fprintf(stderr, "\n"); } while(gConditionalFalseConstant)

#define ErrorIf(Expression, ...) \
do { if((Expression)) { \
  G_DEBUG_BREAK; PrintError(__VA_ARGS__); fprintf(stderr, "\n"); } } while(gConditionalFalseConstant)

#define Assert(Expression, ...) \
do { if(!(Expression)) { \
  G_DEBUG_BREAK; PrintError(__VA_ARGS__); fprintf(stderr, "\n"); } } while(gConditionalFalseConstant)


#define ErrorWithCondititionAssert(Expression, ...) \
do { if((Expression)) G_DEBUG_BREAK; PrintError(__VA_ARGS__); fprintf(stderr, "\n"); } while(gConditionalFalseConstant)


#define FileErrorIf(Expression, file, Line, ...) \
do { G_DEBUG_BREAK; PrintError(__VA_ARGS__); fprintf(stderr, "\n"); } while(gConditionalFalseConstant)

#define Verify(funccall) ErrorIf(funcall != 0);

#else
#define UnusedParameter(param)



//Console Message
#define WarnIf(...) ((void)0)
#define ErrorIf(...) ((void)0)
#define Assert(...) ((void)0)
#define ErrorMsg(...) ((void)0)
#define ErrorWithCondititionAssert( ...) ((void)0)
#define Warn(...) ((void)0)
#define FileErrorIf(...) ((void)0)
#define Verify(funccall) funccall

#endif

#define ReturnIf(Expression , whatToReturn, ...) \
  do { if(Expression) {                          \
    WarnIf(Expression, __VA_ARGS__);             \
    return whatToReturn;                         \
  } } while(gConditionalFalseConstant)			 

#endif //_LOGGING_H
