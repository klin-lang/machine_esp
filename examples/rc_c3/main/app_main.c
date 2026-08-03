/* ESP-IDF entry — calls Klin-exported PWM fade (MMIO LEDC, no IDF ledc API). */
void klin_app_main(void);

void app_main(void)
{
    klin_app_main();
}
