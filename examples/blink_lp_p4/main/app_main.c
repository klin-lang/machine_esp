/* ESP-IDF entry — calls Klin-exported LP GPIO blink (MMIO, no IDF gpio API). */
void klin_app_main(void);

void app_main(void)
{
    klin_app_main();
}
