/*
 * Copyright (C) 2017 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */
#include <linux/types.h>
#include <mt-plat/mtk_battery.h>
#include <mt-plat/mtk_boot.h>
#include <mtk_gauge_class.h>
#include <mtk_battery_internal.h>
#include <mt-plat/upmu_common.h>
#include <mach/upmu_sw.h>

/*prize-lifenfen-20181207, add fuel gauge cw2015 start*/
#if defined(CONFIG_MTK_CW2015_SUPPORT)
extern int g_cw2015_capacity;
extern int g_cw2015_vol;
extern int cw2015_exit_flag;
#endif
/*prize-lifenfen-20181207, add fuel gauge cw2015 end*/

#if (CONFIG_MTK_GAUGE_VERSION != 30)
signed int battery_get_bat_voltage(void)
{
	return 4000;
}

signed int battery_get_bat_current(void)
{
	return 0;
}

signed int battery_get_bat_current_mA(void)
{
	return 0;
}

signed int battery_get_soc(void)
{
	return 50;
}

signed int battery_get_uisoc(void)
{
	int boot_mode = get_boot_mode();

	if ((boot_mode == META_BOOT) ||
		(boot_mode == ADVMETA_BOOT) ||
		(boot_mode == FACTORY_BOOT) ||
		(boot_mode == ATE_FACTORY_BOOT))
		return 75;

	return 50;
}

signed int battery_get_bat_temperature(void)
{
	return 25;
}

signed int battery_get_ibus(void)
{
	return 0;
}

signed int battery_get_vbus(void)
{
	return 0;
}

signed int battery_get_bat_avg_current(void)
{
	return 0;
}
#else
signed int battery_get_bat_voltage(void)
{
/*prize-lifenfen-20181207, add fuel gauge cw2015 start*/
#if defined(CONFIG_MTK_CW2015_SUPPORT)
	if(cw2015_exit_flag==1)
		return g_cw2015_vol;
	else
		return pmic_get_battery_voltage();
#else
/*prize-lifenfen-20181207, add fuel gauge cw2015 end*/
	return pmic_get_battery_voltage();
/*prize-lifenfen-20181207, add fuel gauge cw2015 start*/
#endif
/*prize-lifenfen-20181207, add fuel gauge cw2015 end*/
}

signed int battery_get_bat_current(void)
{
	int curr_val;
	bool is_charging;

	is_charging = gauge_get_current(&curr_val);
	if (is_charging == false)
		curr_val = 0 - curr_val;
	return curr_val;
}

signed int battery_get_bat_current_mA(void)
{
	return battery_get_bat_current() / 10;
}

signed int battery_get_soc(void)
{
/* prize-lifenfen-20181207, add fuel gauge cw2015 start */
#if defined(CONFIG_MTK_CW2015_SUPPORT)
	if(cw2015_exit_flag==1)
		return g_cw2015_capacity;
	else
		return get_mtk_battery()->soc;
#else
/* prize-lifenfen-20181207, add fuel gauge cw2015 end */
	return get_mtk_battery()->soc;
/* prize-lifenfen-20181207, add fuel gauge cw2015 start */
#endif
/* prize-lifenfen-20181207, add fuel gauge cw2015 end */
}

signed int battery_get_uisoc(void)
{
//prize wyq 20181108 return real uisoc in meta and factory mode for battery test before delivery start    
#if 0
	int boot_mode = get_boot_mode();

	if ((boot_mode == META_BOOT) ||
		(boot_mode == ADVMETA_BOOT) ||
		(boot_mode == FACTORY_BOOT) ||
		(boot_mode == ATE_FACTORY_BOOT))
		return 75;
#endif
//prize wyq 20181108 return real uisoc in meta and factory mode for battery test before delivery end

/*prize-lifenfen-20181207, add fuel gauge cw2015 start*/
#if defined(CONFIG_MTK_CW2015_SUPPORT)
	if(cw2015_exit_flag==1)
		return g_cw2015_capacity;
	else
		return get_mtk_battery()->ui_soc;
#else
/*prize-lifenfen-20181207, add fuel gauge cw2015 end*/
	return get_mtk_battery()->ui_soc;
/*prize-lifenfen-20181207, add fuel gauge cw2015 start*/
#endif
/*prize-lifenfen-20181207, add fuel gauge cw2015 end*/
}

signed int battery_get_bat_temperature(void)
{
	/* TODO */
	if (is_battery_init_done())
		return force_get_tbat(true);
	else
		return -127;
}

signed int battery_get_ibus(void)
{
	return pmic_get_ibus();
}

signed int battery_get_vbus(void)
{
	return pmic_get_vbus();
}

signed int battery_get_bat_avg_current(void)
{
	bool valid;

	return gauge_get_average_current(&valid);
}

/* GM25 only */
int en_intr_VBATON_UNDET(int en)
{
	pmic_enable_interrupt(INT_VBATON_UNDET, en, "VBATON_UNDET");
	return 0;
}

int reg_VBATON_UNDET(void (*callback)(void))
{
	pmic_register_interrupt_callback(INT_VBATON_UNDET, callback);
	return 0;
}

#endif
