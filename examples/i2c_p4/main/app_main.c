/* ESP-IDF entry — calls Klin-exported blink loop (MMIO Pin, no IDF gpio API). */
void klin_app_main(void);

void app_main(void)
{
    klin_app_main();
}
