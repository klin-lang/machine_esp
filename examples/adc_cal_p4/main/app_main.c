/* ESP-IDF entry — calls Klin-exported ADC+regi2c loop (MMIO, no IDF adc API). */
void klin_app_main(void);

void app_main(void)
{
    klin_app_main();
}
