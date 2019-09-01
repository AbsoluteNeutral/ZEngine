#ifndef _ZG_CONFIG_H
#define _ZG_CONFIG_H

#if _WIN32 || _WIN64
	#if _WIN64
		#define _64BITMACHINE
	#else
		#define _32BITMACHINE
	#endif
#endif

// Check GCC
#if __GNUC__
	#if __x86_64__ || __ppc64__
		#define _64BITMACHINE
	#else
		#define _32BITMACHINE
	#endif
#endif

#endif
