#include <simux/kernel.h>
#include <simux/tty.h>
#include <libc/stdlib.h>
#include <libc/string.h>
#include <stdarg.h>


void khalt(void)
{
    while (true) {
        asm volatile("cli");
        asm volatile("hlt");
    }
}

void kpanic(const char* message, uint32_t data)
{
    asm volatile("cli");

    term_clear();

    printk("Kernel panic!\n");
    printk("Message: %s\n", message);
    printk("Data: 0x%x\n", data);

    khalt();
}

int printk(const char* format, ...)
{
    if (format == NULL) {
        return 0;
    }

    char output[200];
    char temp[20];

    int format_idx = 0;
    int output_idx = 0;

    va_list args;
    va_start(args, format);

    while (true)
    {
        char c = format[format_idx];
        format_idx++;

        if (c == 0)
        {
            break;
        }

        if (c != '%')
        {
            output[output_idx] = c;
            output_idx++;
            continue;
        }
        else
        {
            c = format[format_idx];
            format_idx++;

            switch (c)
            {
                case 'c':
                {
                    char ch = (char)va_arg(args, int);
                    output[output_idx++] = ch;
                    break;
                }
                case 'd':
                {
                    int num = va_arg(args, int);
                    itoa(num, temp, 10);
                    strcpy(output + output_idx, temp);
                    output_idx += strlen(temp);
                    break;
                }
                case 'x':
                {
                    int num = va_arg(args, int);
                    itoa(num, temp, 16);
                    strcpy(output + output_idx, temp);
                    output_idx += strlen(temp);
                    break;
                }
                case 's':
                {
                    char* str = va_arg(args, char*);
                    strcpy(output + output_idx, str);
                    output_idx += strlen(str);
                    break;
                }
                default:
                {
                    output[output_idx++] = c;
                    break;
                }
            }
        }
    }

    va_end(args);

    output[output_idx] = 0;
    term_write_str(output);

    return output_idx;
}
