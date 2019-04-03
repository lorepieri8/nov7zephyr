/* Bluetooth: Mesh Generic OnOff, Generic Level, Lighting & Vendor Models
 *
 * Copyright (c) 2018 Vikrant More
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ble_mesh.h"
#include "device_composition.h"
#include "storage.h"

static u8_t storage_id;
u8_t reset_counter;

static void save_reset_counter(void)
{
	char buf[5];

	settings_str_from_bytes(&reset_counter, sizeof(reset_counter), buf,
				sizeof(buf));

	settings_save_one("ps/rc", buf);
}

static void save_gen_def_trans_time_state(void)
{
	char buf[5];

	settings_str_from_bytes(&gen_def_trans_time_srv_user_data.tt,
				sizeof(gen_def_trans_time_srv_user_data.tt),
				buf, sizeof(buf));

	settings_save_one("ps/gdtt", buf);
}

static void save_gen_onpowerup_state(void)
{
	char buf[5];

	settings_str_from_bytes(&gen_power_onoff_srv_user_data.onpowerup,
				sizeof(gen_power_onoff_srv_user_data.onpowerup),
				buf, sizeof(buf));

	settings_save_one("ps/gpo", buf);

	if (gen_power_onoff_srv_user_data.onpowerup == 0x02) {
		save_on_flash(LIGHTNESS_TEMP_LAST_STATE);
	}
}

static void save_lightness_temp_def_state(void)
{
	char buf[12];

	light_ctl_srv_user_data.lightness_temp_def =
		(u32_t) ((light_ctl_srv_user_data.lightness_def << 16) |
			 light_ctl_srv_user_data.temp_def);

	settings_str_from_bytes(&light_ctl_srv_user_data.lightness_temp_def,
				sizeof(light_ctl_srv_user_data.lightness_temp_def),
				buf, sizeof(buf));

	settings_save_one("ps/ltd", buf);
}

static void save_lightness_temp_last_state(void)
{
	char buf[12];

	light_ctl_srv_user_data.lightness_temp_last =
		(u32_t) ((light_ctl_srv_user_data.lightness << 16) |
			 light_ctl_srv_user_data.temp);

	settings_str_from_bytes(&light_ctl_srv_user_data.lightness_temp_last,
				sizeof(light_ctl_srv_user_data.lightness_temp_last),
				buf, sizeof(buf));

	settings_save_one("ps/ltl", buf);

	printk("Light CTL Last values have beed saved !!\n");
}

static void storage_work_handler(struct k_work *work)
{
	switch (storage_id) {
	case RESET_COUNTER:
		save_reset_counter();
		break;
	case GEN_DEF_TRANS_TIME_STATE:
		save_gen_def_trans_time_state();
		break;
	case GEN_ONPOWERUP_STATE:
		save_gen_onpowerup_state();
		break;
	case LIGHTNESS_TEMP_DEF_STATE:
		save_lightness_temp_def_state();
		break;
	case LIGHTNESS_TEMP_LAST_STATE:
		save_lightness_temp_last_state();
		break;
	}
}

K_WORK_DEFINE(storage_work, storage_work_handler);

void save_on_flash(u8_t id)
{
	storage_id = id;
	k_work_submit(&storage_work);
}

static int ps_set(int argc, char **argv, char *val)
{
	int len;

	if (argc == 1) {
		if (!strcmp(argv[0], "rc")) {
			len = sizeof(reset_counter);

			return settings_bytes_from_str(val, &reset_counter,
						       &len);
		}

		if (!strcmp(argv[0], "gdtt")) {
			len = sizeof(gen_def_trans_time_srv_user_data.tt);

			return settings_bytes_from_str(val,
				&gen_def_trans_time_srv_user_data.tt, &len);
		}

		if (!strcmp(argv[0], "gpo")) {
			len = sizeof(gen_power_onoff_srv_user_data.onpowerup);

			return settings_bytes_from_str(val,
				&gen_power_onoff_srv_user_data.onpowerup, &len);
		}

		if (!strcmp(argv[0], "ltd")) {
			len = sizeof(light_ctl_srv_user_data.lightness_temp_def);

			return settings_bytes_from_str(val,
				&light_ctl_srv_user_data.lightness_temp_def,
				&len);
		}

		if (!strcmp(argv[0], "ltl")) {
			len = sizeof(light_ctl_srv_user_data.
				     lightness_temp_last);

			return settings_bytes_from_str(val,
				&light_ctl_srv_user_data.lightness_temp_last,
				&len);
		}
	}

	return -ENOENT;
}

static struct settings_handler ps_settings = {
	.name = "ps",
	.h_set = ps_set,
};

int ps_settings_init(void)
{
	int err;

	err = settings_subsys_init();
	if (err) {
		printk("settings_subsys_init failed (err %d)", err);
		return err;
	}

	err = settings_register(&ps_settings);
	if (err) {
		printk("ps_settings_register failed (err %d)", err);
		return err;
	}

	return 0;
}
