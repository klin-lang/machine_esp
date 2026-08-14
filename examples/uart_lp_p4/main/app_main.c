/* ESP-IDF entry — calls Klin-exported LP UART loop (MMIO, no IDF uart API). */
void klin_app_main(void);

void app_main(void)
{
    klin_app_main();
}
