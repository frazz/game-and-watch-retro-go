#pragma once
#ifndef BQ24072_H
#define BQ24072_H

#include <stdbool.h>

int32_t bq24072_init(void);

void bq24072_handle_power_good(void);
void bq24072_handle_charging(void);

int bq24072_get_power_good(void);
int bq24072_get_charging(void);

#endif // BQ24072_H
