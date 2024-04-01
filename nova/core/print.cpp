////////////////////////////////////////////////////////////
//
// Nova OS
// Copyright (C) 2024 Simon Alm Mårtensson
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
#include <nova/print.h>

#include <nova/tty.h>
#include <libc/stdlib.h>
#include <libc/string.h>
#include <libc/ctype.h>
#include <stdarg.h>



static int _print_terminal(const char* fmt, va_list args)
{
    if (fmt == NULL) {
        return 0;
    }

    char buffer[250];
    char temp[30];

    char* out = buffer;
    char c = 0;

    while (true)
    {
        c = *fmt++;

        if (c == '\0')
        {
            break;
        }

        if (c != '%')
        {
            *out++ = c;
            continue;
        }

        c = *fmt++;
        char padchar = ' ';
        int padlen = 0;

        if (c == '0')
        {
            padchar = '0';
            static_cast<void>(padchar);
            c = *fmt++;
        }

        while (isdigit(c))
        {
            padlen = padlen * 10 + c - '0';
            c = *fmt++;
        }

        switch (c)
        {
            case 'c':
            {
                char ch = (char)va_arg(args, int);
                *out++ = ch;
                break;
            }
            case 'd':
            case 'x':
            {
                int num = va_arg(args, int);
                int base = c == 'd' ? 10 : 16;

                itoa(num, temp, base);

                int temp_len = strlen(temp);

                if (padlen > temp_len)
                {
                    padlen = padlen - temp_len;

                    for (u8 i = 0; i < padlen; i++)
                    {
                        *out++ = padchar;
                    }
                }

                strcpy(out, temp);
                out += strlen(temp);

                break;
            }
            case 's':
            {
                char* str = va_arg(args, char*);
                strcpy(out, str);
                out += strlen(str);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    *out = '\0';
    terminal_write(buffer);
    terminal_flush();

    return out - buffer;
}

void log(const char* fmt, ...)
{
    if (fmt == NULL) {
        return;
    }

    char buffer[300];
    
    strcpy(buffer, "[Kernel]: ");
    strcat(buffer, fmt);
    strcat(buffer, "\n");

    va_list args;
    va_start(args, fmt);

    _print_terminal(buffer, args);
    va_end(args);
}

void print(const char* fmt, ...)
{
    if (fmt == NULL) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    _print_terminal(fmt, args);
    va_end(args);
}