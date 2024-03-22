namespace nova
{
    void enter_kernel()
    {
        while (true) {
            asm volatile("hlt");
        }
    }
}