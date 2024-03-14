////////////////////////////////////////////////////////////
//
// Nova OS
// Copyright (C) 2024 Simon Mårtensson
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////
#define NOVA_DO_STRINGIZE(x) #x
#define NOVA_STRINGIZE(X) NOVA_DO_STRINGIZE(x)


#define NOVA_BUILD_DATE __DATE__
#define NOVA_BUILD_TIME __TIME__
#define NOVA_BUILD_DATETIME NOVA_BUILD_DATE ", " NOVA_BUILD_TIME


#ifdef NOVA_GIT_TAG
    #define NOVA_VERSION NOVA_STRINGIZE(NOVA_GIT_TAG)
#else
    #define NOVA_VERSION "-"
#endif


#ifdef NOVA_ARCH_I386
    #if !defined(__i386__) || !defined(__GNUC__)
        #error Must be compiled with an GCC x86 compiler!
    #endif

    #if !defined(__GXX_EXPERIMENTAL_CXX0X__) && !(__cplusplus >= 201103L)
        #error Non supported C++ version (C++11 expected)
    #endif

    #if __BYTE_ORDER__ != __ORDER_LITTLE_ENDIAN__
        #error Must be compiled with a little endian compiler!
    #endif

    #define NOVA_ARCHITECTURE "i386"
    #define NOVA_COMPILER "GCC v" __VERSION__
#else
    #error "Unknown target arhitecture"
#endif
