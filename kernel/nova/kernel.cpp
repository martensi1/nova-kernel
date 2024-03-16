#include <nova/kernel.h>
#include <nova/tty.h>
#include <libc/stdlib.h>
#include <libc/string.h>
#include <libc/ctype.h>
#include <stdarg.h>


static int _printk(const char* fmt, va_list args)
{
     if (fmt == NULL) {
        return 0;
    }

    char buffer[200];
    char temp[20];

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
    term_write_str(buffer);

    return out - buffer;
}



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

void logk(const char* fmt, ...)
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
    _printk(buffer, args);
    va_end(args);
}

int printk(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int ret = _printk(fmt, args);
    va_end(args);

    return ret;
}
