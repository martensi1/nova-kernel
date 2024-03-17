#include <nova/tty.h>
#include <nova/common.h>
#include <nova/console/driver.h>
#include <libc/string.h>


extern struct ConsoleDriver dummyDriver;
extern struct ConsoleDriver vgaDriver;

struct ConsoleDriver* activeDriver = NULL;
static SpinLock lock = SpinLock();


namespace Nova
{
    namespace priv
    {
        ////////////////////////////////////////////////////////////
        static void chooseDriver()
        {
            if (vgaDriver.isAvailable()) {
                activeDriver = &vgaDriver;
            }
            else {
                activeDriver = &dummyDriver;
            }
        }

        static FORCE_INLINE void writeData(const char* data, size_t size)
        {
            for (size_t i = 0; i < size; i++) {
                char c  = data[i];

                if (c == '\n') {
                    activeDriver->writeLineFeed();
                    continue;
                }

                activeDriver->writeChar(c);
            }

            activeDriver->updateCursor();
        }

        static FORCE_INLINE void writeData(const char* data)
        {
            size_t length = strlen(data);
            writeData(data, length);
        }
    }

    ////////////////////////////////////////////////////////////
    void InitializeTerminal()
    {
        SpinGuard guard(lock);
        priv::chooseDriver();

        activeDriver->initialize();
        activeDriver->clear();

        activeDriver->enableCursor();
        activeDriver->updateCursor();

        priv::writeData("\n\n\n\n\n");
        priv::writeData("Terminal initialized (");
        priv::writeData(activeDriver->name);
        priv::writeData(")\n");
    }

    ////////////////////////////////////////////////////////////
    void ClearTerminal()
    {
        SpinGuard guard(lock);
        activeDriver->clear();
    }

    ////////////////////////////////////////////////////////////
    void TerminalWrite(const char* data, size_t size)
    {
        SpinGuard guard(lock);
        priv::writeData(data, size);
    }

    ////////////////////////////////////////////////////////////
    void TerminalWrite(const char* str)
    {
        size_t length = strlen(str);
        priv::writeData(str, length);
    }
}