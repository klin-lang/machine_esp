/* ESP-IDF entry — calls Klin-exported ADC loop (MMIO LP_ADC, no IDF adc API). */
void klin_app_main(void);

void app_main(void)
{
    klin_app_main();
}
