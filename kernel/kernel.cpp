#include <simux/kernel.h>
#include <simux/tty.h>
#include <simux/types.h>
#include <libc/stdlib.h>
#include <libc/string.h>
#include <libc/ctype.h>
#include <stdarg.h>


void khalt(void)
{
    asm volatile("cli");

    while (true) {
        asm volatile("hlt");
    }
}

void kpanic(const char* message, const u32 data)
{
    asm volatile("cli");
    term_clear();

    printk("\n\nKernel panic!\n");
    printk("Message: %s\n", message);
    printk("Data: 0x%x\n", data);

    khalt();
}

int printk(const char* fmt, ...)
{
    if (fmt == NULL) {
        return 0;
    }

    char buffer[200];
    char temp[20];

    char* out = buffer;
    char c = 0;

    va_list args;
    va_start(args, fmt);

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
            {
                int num = va_arg(args, int);
                itoa(num, temp, 10);
                break;
            }
            case 'x':
            {
                int num = va_arg(args, int);
                itoa(num, temp, 16);
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
                *out++ = c;
                break;;
            }
        }

        int temp_len = strlen(temp);
        if (temp_len > 0)
        {
            padlen = padlen - temp_len;

            for (u8 i = 0; i < padlen; i++)
            {
                *out++ = padchar;
            }

            strcpy(out, temp);
            out += temp_len;
        }

        temp[0] = '\0';
    }

    va_end(args);

    *out = 0;
    term_write_str(buffer);

    return out - buffer;
}
