/*
 * Copyright (c) 2017 Oticon A/S
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "bs_tracing.h"
#include "bs_oswrap.h"
#include "bs_dump_files.h"
#include "argparse.h"
#include "bstests.h"
#include "NRF_hw_args.h"
#include "bs_cmd_line_typical.h"
#include "NRF_HWLowL.h"

char executable_name[] = "bs_nrf52_bsim_..";

void component_print_post_help(void)
{
	fprintf(stdout, "\nZephyr and a given app compiled with models of the "
			"NRF52 HW\n\n");
}

static struct NRF_bsim_args_t *args_g;
static char *testid;
const char *bogus_sim_id = "bogus";

static void cmd_trace_lvl_found(char *argv, int offset)
{
	bs_trace_set_level(args_g->verb);
}

static void cmd_gdev_nbr_found(char *argv, int offset)
{
	bs_trace_set_prefix_dev(args_g->global_device_nbr);
}

static void cmd_testid_found(char *argv, int offset)
{
	bst_set_testapp_mode(testid);
}

static void cmd_testlist_found(char *argv, int offset)
{
	bst_print_testslist();
	exit(0);
}

static void cmd_nosim_found(char *argv, int offset)
{
	hwll_set_nosim(true);
}

static void save_test_arg(struct NRF_bsim_args_t *args, char *argv)
{
	if (args->test_case_argc >= MAXPARAMS_TESTCASES) {
		bs_trace_error_line("Too many testcase arguments (at '%s'), "
				    "maximum is %i\n", argv,
				    MAXPARAMS_TESTCASES);
	} else {
		bs_trace_raw(9, "cmdarg: adding '%s' to testcase args[%i]\n",
			     argv, args->test_case_argc);
		args->test_case_argv[args->test_case_argc++] = argv;
	}
}

static void print_no_sim_warning(void)
{
	bs_trace_warning("Neither simulation id or the device number "
			"have been set. I assume you want to run "
			"without a BabbleSim phy (-nosim)\n");
	bs_trace_warning("If this is not what you wanted, check with "
			"--help how to set them\n");
	bs_trace_raw(3, "setting sim_id to 'bogus', device number to 0 "
			"and nosim\n");
}

/**
 * Check the arguments provided in the command line: set args based on it or
 * defaults, and check they are correct
 */
void nrfbsim_argsparse(int argc, char *argv[], struct NRF_bsim_args_t *args)
{
	bool nosim;

	args_g = args;
	bs_args_struct_t args_struct[] = {
		ARG_TABLE_S_ID,
		ARG_TABLE_P_ID_2G4,
		ARG_TABLE_DEV_NBR,
		ARG_TABLE_GDEV_NBR,
		ARG_TABLE_VERB,
		ARG_TABLE_SEED,
		ARG_TABLE_COLOR,
		ARG_TABLE_NOCOLOR,
		ARG_TABLE_FORCECOLOR,
		_NRF_HW_SUB_CMD_ARG_STRUCT,
		/*
		 * Fields:
		 * manual, mandatory, switch,
		 * option_name, var_name, type,
		 * destination, callback,
		 * description
		 */
		{false, false, true,
		"nosim", "", 'b',
		(void *)&nosim, cmd_nosim_found,
		"(debug feature) Do not connect to the phy"},
		BS_DUMP_FILES_ARGS,
		{false, false, false,
		"testid", "testid", 's',
		(void *)&testid, cmd_testid_found,
		"Which of the tests shall be run. Run -testslist for more info"
		},
		{false, false, true,
		"testslist", "", 'b',
		NULL, cmd_testlist_found,
		"Print information about the available FW application tests"},
		{true, false, false,
		"argstest", "arg", 'l',
		NULL, NULL,
		"The arguments that follow will be passed straight to the "
		"testcase init function"},
		{true, false, false,
		"argsmain", "arg", 'l',
		NULL, NULL,
		"The arguments that follow will be passed to main (default)"},
		ARG_TABLE_ENDMARKER
	};

	bs_args_set_defaults(args_struct);
	args->verb   = 2;
	bs_trace_set_level(args->verb);
	args->test_case_argv[0] = 0;
	args->test_case_argc = 0;
	nrf_hw_sub_cmline_set_defaults(&args->nrf_hw);
	static const char default_phy[] = "2G4";

	enum {Main = 0, Test = 1} parsing = Main;

	for (int i = 1; i < argc; i++) {
		if (bs_is_option(argv[i], "argstest", 0)) {
			parsing = Test;
			continue;
		} else if (bs_is_option(argv[i], "argsmain", 0)) {
			parsing = Main;
			continue;
		}

		if (parsing == Main) {
			if (!bs_args_parse_one_arg(argv[i], args_struct)) {
				bs_args_print_switches_help(args_struct);
				bs_trace_error_line("Incorrect option %s\n",
						    argv[i]);
			}
		} else if (parsing == Test) {
			save_test_arg(args, argv[i]);
		} else {
			bs_trace_error_line("Bad error\n");
		}
	}

	if ((args->s_id == NULL) && (args->device_nbr == UINT_MAX)) {
		if (!nosim) {
			print_no_sim_warning();
		}

		args->s_id = (char *)bogus_sim_id;
		args->device_nbr = 0;
		hwll_set_nosim(true);
	}

	bs_args_typical_dev_post_check((bs_basic_dev_args_t *)args, args_struct,
					(char *)default_phy);

	if (args->rseed == UINT_MAX) {
		args->rseed = 0x1000 + args->device_nbr;
	}
}
