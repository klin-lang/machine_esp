/* ESP-IDF entry — calls Klin-exported RMT TX (MMIO, no IDF rmt API). */
void klin_app_main(void);

void app_main(void)
{
    klin_app_main();
}
