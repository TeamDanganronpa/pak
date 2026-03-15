#ifndef PAK_DLL_API_H
#define PAK_DLL_API_H

#ifdef _WIN32
	#ifdef PAK_DLL
		#ifdef PAK_COMPILATION
			#define PAK_API __declspec(dllexport)
		#else
			#define PAK_API __declspec(dllimport)
		#endif
	#else
		#define PAK_API
	#endif
#else
	#define PAK_API
#endif

#endif //PAK_DLL_API_H