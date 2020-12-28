#include <stdio.h>

#include <stm32h7xx_hal.h>

#include "bq24072.h"

#include "utils.h"

typedef enum {
    BQ24072_PIN_CHG,
    BQ24072_PIN_PGOOD,
    BQ24072_PIN_COUNT       // Keep this last
} bq24072_pin_t;

static const struct {
    uint32_t        pin;
    GPIO_TypeDef*   bank;
} bq_pins[BQ24072_PIN_COUNT] = {
    [BQ24072_PIN_CHG]   = { .pin = GPIO_PIN_7, .bank = GPIOE},
    [BQ24072_PIN_PGOOD] = { .pin = GPIO_PIN_2, .bank = GPIOA},
};

int32_t bq24072_init(void)
{
    // PE7 - CHG
    // PE8 - CE
    // PA2 - PGOOD

    int     i;

    GPIO_InitTypeDef    gpio = {0};

    // Setup pins
    for (i = 0; i < ARRAY_SIZE(bq_pins); i++)
    {
        gpio.Pin = bq_pins[i].pin;
        gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
        gpio.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(bq_pins[i].bank, &gpio);
    }

    // Enable interrupts
    HAL_NVIC_SetPriority(EXTI2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    printf("bq24072 init\n");

    if (HAL_GPIO_ReadPin(bq_pins[BQ24072_PIN_PGOOD].bank, bq_pins[BQ24072_PIN_PGOOD].pin) == GPIO_PIN_SET)
    {
        printf("PGOOD 0: 1\n");
    }
    else
    {
        printf("PGOOD 0: 0\n");
    }

    return 0;
}

void bq24072_handle_power_good(void)
{
    if (HAL_GPIO_ReadPin(bq_pins[BQ24072_PIN_PGOOD].bank, bq_pins[BQ24072_PIN_PGOOD].pin) == GPIO_PIN_SET)
    {
        printf("PGOOD: 1\n");
    }
    else
    {
        printf("PGOOD: 0\n");
    }
}

void bq24072_handle_charging(void)
{
    if (HAL_GPIO_ReadPin(bq_pins[BQ24072_PIN_CHG].bank, bq_pins[BQ24072_PIN_CHG].pin) == GPIO_PIN_SET)
    {
        printf("CHG: 1\n");
    }
    else
    {
        printf("CHG: 0\n");
    }
}

int bq24072_get_power_good(void)
{
    if (HAL_GPIO_ReadPin(bq_pins[BQ24072_PIN_PGOOD].bank, bq_pins[BQ24072_PIN_PGOOD].pin) != GPIO_PIN_SET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int bq24072_get_charging(void)
{
    if (HAL_GPIO_ReadPin(bq_pins[BQ24072_PIN_CHG].bank, bq_pins[BQ24072_PIN_CHG].pin) != GPIO_PIN_SET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

