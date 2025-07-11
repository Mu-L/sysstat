/*
 * sadf_misc.c: Functions used by sadf to display special records
 * (C) 2011-2025 by Sebastien GODARD (sysstat <at> orange.fr)
 *
 ***************************************************************************
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published  by  the *
 * Free Software Foundation; either version 2 of the License, or (at  your *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it  will  be  useful,  but *
 * WITHOUT ANY WARRANTY; without the implied warranty  of  MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License *
 * for more details.                                                       *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335 USA              *
 ***************************************************************************
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "sadf.h"
#include "pcp_def_metrics.h"

#ifdef USE_NLS
#include <locale.h>
#include <libintl.h>
#define _(string) gettext(string)
#else
#define _(string) (string)
#endif

#ifdef HAVE_PCP
#include <pcp/pmapi.h>
#include <pcp/import.h>
#endif

extern uint64_t flags;
extern char *seps[];

extern int palette;
extern unsigned int svg_colors[][SVG_COL_PALETTE_SIZE];

/*
 ***************************************************************************
 * Flush data to PCP archive.
 *
 * IN:
 * @record_hdr	Record header for current sample.
 * @flags	Flags for common options.
 ***************************************************************************
 */
void pcp_write_data(struct record_header *record_hdr, unsigned int flags)
{
#ifdef HAVE_PCP
	int rc;
	unsigned long long utc_sec = record_hdr->ust_time;

	/* Write data to PCP archive */
	if ((rc = pmiWrite(utc_sec, 0)) < 0) {
		fprintf(stderr, "PCP: pmiWrite: %s\n", pmiErrStr(rc));
		exit(4);
	}
#endif
}

/*
 ***************************************************************************
 * Display restart messages (database and ppc formats).
 *
 * IN:
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone.
 * @sep		Character used as separator.
 * @file_hdr	System activity file standard header.
 ***************************************************************************
 */
void print_dbppc_restart(char *cur_date, char *cur_time, char *my_tz, char sep,
			 struct file_header *file_hdr)
{
	printf("%s%c-1%c", file_hdr->sa_nodename, sep, sep);
	if (strlen(cur_date)) {
		printf("%s ", cur_date);
	}
	printf("%s", cur_time);
	if (strlen(cur_date)) {
		printf(" %s", PRINT_LOCAL_TIME(flags) ? my_tz
						      : (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
										: "UTC"));
	}
	printf("%cLINUX-RESTART\t(%u CPU)\n",
	       sep, file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1);
}

/*
 ***************************************************************************
 * Display restart messages (ppc format).
 *
 * IN:
 * @tab		Number of tabulations (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone.
 * @file_hdr	System activity file standard header.
 * @record_hdr	Current record header (unused here).
 ***************************************************************************
 */
__printf_funct_t print_db_restart(int *tab, int action, char *cur_date, char *cur_time,
				  char *my_tz, struct file_header *file_hdr,
				  struct record_header *record_hdr)
{
	/* Actions F_BEGIN and F_END ignored */
	if (action == F_MAIN) {
		print_dbppc_restart(cur_date, cur_time, my_tz, ';', file_hdr);
	}
}

/*
 ***************************************************************************
 * Display restart messages (database format).
 *
 * IN:
 * @tab		Number of tabulations (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone.
 * @file_hdr	System activity file standard header.
 * @record_hdr	Current record header (unused here).
 ***************************************************************************
 */
__printf_funct_t print_ppc_restart(int *tab, int action, char *cur_date, char *cur_time,
				   char *my_tz, struct file_header *file_hdr,
				   struct record_header *record_hdr)
{
	/* Actions F_BEGIN and F_END ignored */
	if (action == F_MAIN) {
		print_dbppc_restart(cur_date, cur_time, my_tz, '\t', file_hdr);
	}
}

/*
 ***************************************************************************
 * Display restart messages (XML format).
 *
 * IN:
 * @tab		Number of tabulations.
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone (unused here).
 * @file_hdr	System activity file standard header.
 * @record_hdr	Current record header (unused here).
 *
 * OUT:
 * @tab		Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_xml_restart(int *tab, int action, char *cur_date, char *cur_time,
				   char *my_tz, struct file_header *file_hdr,
				   struct record_header *record_hdr)
{
	if (action & F_BEGIN) {
		xprintf((*tab)++, "<restarts>");
	}
	if (action & F_MAIN) {
		xprintf(*tab, "<boot date=\"%s\" time=\"%s\" tz=\"%s\" cpu_count=\"%d\"/>",
			cur_date, cur_time,
			PRINT_LOCAL_TIME(flags) ? my_tz
						: (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
									  : "UTC"),
			file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1);
	}
	if (action & F_END) {
		xprintf(--(*tab), "</restarts>");
	}
}

/*
 ***************************************************************************
 * Display restart messages (JSON format).
 *
 * IN:
 * @tab		Number of tabulations.
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone (unused here).
 * @file_hdr	System activity file standard header.
 * @record_hdr	Current record header (unused here).
 *
 * OUT:
 * @tab		Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_json_restart(int *tab, int action, char *cur_date, char *cur_time,
				    char *my_tz, struct file_header *file_hdr,
				    struct record_header *record_hdr)
{
	static int sep = FALSE;

	if (action & F_BEGIN) {
		printf(",\n");
		xprintf((*tab)++, "\"restarts\": [");
	}
	if (action & F_MAIN) {
		if (sep) {
			printf(",\n");
		}
		xprintf((*tab)++, "{");
		xprintf(*tab, "\"boot\": {\"date\": \"%s\", \"time\": \"%s\", \"tz\": \"%s\", \"cpu_count\": %d}",
			cur_date, cur_time,
			PRINT_LOCAL_TIME(flags) ? my_tz
						: (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
									  : "UTC"),
			file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1);
		xprintf0(--(*tab), "}");
		sep = TRUE;
	}
	if (action & F_END) {
		if (sep) {
			printf("\n");
			sep = FALSE;
		}
		xprintf0(--(*tab), "]");
	}
}

/*
 ***************************************************************************
 * Display restart messages (raw format).
 *
 * IN:
 * @tab		Number of tabulations (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone.
 * @file_hdr	System activity file standard header.
 * @record_hdr	Current record header (unused here).
 ***************************************************************************
 */
__printf_funct_t print_raw_restart(int *tab, int action, char *cur_date, char *cur_time,
				   char *my_tz, struct file_header *file_hdr,
				   struct record_header *record_hdr)
{
	/* Actions F_BEGIN and F_END ignored */
	if (action == F_MAIN) {
		printf("%s", cur_time);
		if (strlen(cur_date)) {
			printf(" %s", PRINT_LOCAL_TIME(flags) ? my_tz
							      : (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
											: "UTC"));
		}
		printf("; LINUX-RESTART (%u CPU)\n",
		       file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1);
	}
}

/*
 ***************************************************************************
 * Display restart messages (PCP format).
 *
 * IN:
 * @tab		Number of tabulations (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message (unused here).
 * @cur_time	Time string of current restart message (unused here).
 * @my_tz	Current timezone (unused here).
 * @file_hdr	System activity file standard header.
 * @record_hdr	Current record header.
 ***************************************************************************
 */
__printf_funct_t print_pcp_restart(int *tab, int action, char *cur_date, char *cur_time,
				   char *my_tz, struct file_header *file_hdr,
				   struct record_header *record_hdr)
{
#ifdef HAVE_PCP
	static int def_metrics = FALSE;
	char buf[64];

	if (action & F_BEGIN) {
		if (!def_metrics) {
			pmiAddMetric("system.restart.count",
				     PM_IN_NULL, PM_TYPE_U32, PM_INDOM_NULL, PM_SEM_DISCRETE,
				     pmiUnits(0, 0, 1, 0, 0, PM_COUNT_ONE));

			pmiAddMetric("system.restart.ncpu",
				     PM_IN_NULL, PM_TYPE_U32, PM_INDOM_NULL, PM_SEM_DISCRETE,
				     pmiUnits(0, 0, 1, 0, 0, PM_COUNT_ONE));

			def_metrics = TRUE;
		}
	}
	if (action & F_MAIN) {
		pmiPutValue("system.restart.count", NULL, "1");

		snprintf(buf, sizeof(buf), "%u",
			 file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1);
		pmiPutValue("system.restart.ncpu", NULL, buf);

		/* Write data to PCP archive */
		pcp_write_data(record_hdr, flags);
	}
#endif /* HAVE_PCP */
}

/*
 ***************************************************************************
 * Display comments (database and ppc formats).
 *
 * IN:
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone.
 * @comment	Comment to display.
 * @sep		Character used as separator.
 * @file_hdr	System activity file standard header.
 ***************************************************************************
 */
void print_dbppc_comment(char *cur_date, char *cur_time, char *my_tz, char *comment,
			 char sep, struct file_header *file_hdr)
{
	printf("%s%c-1%c", file_hdr->sa_nodename, sep, sep);
	if (strlen(cur_date)) {
		printf("%s ", cur_date);
	}
	printf("%s", cur_time);
	if (strlen(cur_date)) {
		printf(" %s", PRINT_LOCAL_TIME(flags) ? my_tz
						      : (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
						                                : "UTC"));
	}
	printf("%cCOM %s\n", sep, comment);
}

/*
 ***************************************************************************
 * Display comments (database format).
 *
 * IN:
 * @tab		Number of tabulations (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone.
 * @comment	Comment to display.
 * @file_hdr	System activity file standard header.
 * @record_hdr	Current record header (unused here).
 ***************************************************************************
 */
__printf_funct_t print_db_comment(int *tab, int action, char *cur_date, char *cur_time,
				  char *my_tz, char *comment, struct file_header *file_hdr,
				  struct record_header *record_hdr)
{
	/* Actions F_BEGIN and F_END ignored */
	if (action & F_MAIN) {
		print_dbppc_comment(cur_date, cur_time, my_tz, comment, ';', file_hdr);
	}
}

/*
 ***************************************************************************
 * Display comments (ppc format).
 *
 * IN:
 * @tab		Number of tabulations (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone.
 * @comment	Comment to display.
 * @file_hdr	System activity file standard header.
 * @record_hdr	Current record header (unused here).
 ***************************************************************************
 */
__printf_funct_t print_ppc_comment(int *tab, int action, char *cur_date, char *cur_time,
				   char *my_tz, char *comment, struct file_header *file_hdr,
				   struct record_header *record_hdr)
{
	/* Actions F_BEGIN and F_END ignored */
	if (action & F_MAIN) {
		print_dbppc_comment(cur_date, cur_time, my_tz, comment, '\t', file_hdr);
	}
}

/*
 ***************************************************************************
 * Display comments (XML format).
 *
 * IN:
 * @tab		Number of tabulations.
 * @action	Action expected from current function.
 * @cur_date	Date string of current comment.
 * @cur_time	Time string of current comment.
 * @my_tz	Current timezone.
 * @comment	Comment to display.
 * @file_hdr	System activity file standard header (unused here).
 * @record_hdr	Current record header (unused here).
 *
 * OUT:
 * @tab		Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_xml_comment(int *tab, int action, char *cur_date, char *cur_time,
				   char *my_tz, char *comment, struct file_header *file_hdr,
				   struct record_header *record_hdr)
{
	if (action & F_BEGIN) {
		xprintf((*tab)++, "<comments>");
	}
	if (action & F_MAIN) {
		xprintf(*tab, "<comment date=\"%s\" time=\"%s\" tz=\"%s\" com=\"%s\"/>",
			cur_date, cur_time,
			PRINT_LOCAL_TIME(flags) ? my_tz
						: (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
									  : "UTC"),
			comment);
	}
	if (action & F_END) {
		xprintf(--(*tab), "</comments>");
	}
}

/*
 ***************************************************************************
 * Display comments (JSON format).
 *
 * IN:
 * @tab		Number of tabulations.
 * @action	Action expected from current function.
 * @cur_date	Date string of current comment.
 * @cur_time	Time string of current comment.
 * @my_tz	Current timezone.
 * @comment	Comment to display.
 * @file_hdr	System activity file standard header (unused here).
 * @record_hdr	Current record header (unused here).
 *
 * OUT:
 * @tab		Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_json_comment(int *tab, int action, char *cur_date, char *cur_time,
				    char *my_tz, char *comment, struct file_header *file_hdr,
				    struct record_header *record_hdr)
{
	static int sep = FALSE;

	if (action & F_BEGIN) {
		printf(",\n");
		xprintf((*tab)++, "\"comments\": [");
	}
	if (action & F_MAIN) {
		if (sep) {
			printf(",\n");
		}
		xprintf((*tab)++, "{");
		xprintf(*tab,
			"\"comment\": {\"date\": \"%s\", \"time\": \"%s\", "
			"\"tz\": \"%s\", \"com\": \"%s\"}",
			cur_date, cur_time,
			PRINT_LOCAL_TIME(flags) ? my_tz
						: (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
									  : "UTC"),
			comment);
		xprintf0(--(*tab), "}");
		sep = TRUE;
	}
	if (action & F_END) {
		if (sep) {
			printf("\n");
			sep = FALSE;
		}
		xprintf0(--(*tab), "]");
	}
}

/*
 ***************************************************************************
 * Display comments (raw format).
 *
 * IN:
 * @tab		Number of tabulations (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message.
 * @cur_time	Time string of current restart message.
 * @my_tz	Current timezone.
 * @comment	Comment to display.
 * @file_hdr	System activity file standard header (unused here).
 * @record_hdr	Current record header (unused here).
 ***************************************************************************
 */
__printf_funct_t print_raw_comment(int *tab, int action, char *cur_date, char *cur_time,
				   char *my_tz, char *comment, struct file_header *file_hdr,
				   struct record_header *record_hdr)
{
	/* Actions F_BEGIN and F_END ignored */
	if (action & F_MAIN) {
		printf("%s", cur_time);
		if (strlen(cur_date)) {
			printf(" %s",
			       PRINT_LOCAL_TIME(flags) ? my_tz
						       : (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
										 : "UTC"));
		}
		printf("; COM %s\n", comment);
	}
}

/*
 ***************************************************************************
 * Display comments (PCP format).
 *
 * IN:
 * @tab		Number of tabulations (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current restart message (unused here).
 * @cur_time	Time string of current restart message (unused here).
 * @my_tz	Current timezone (unused here).
 * @comment	Comment to display.
 * @file_hdr	System activity file standard header (unused here).
 * @record_hdr	Current record header.
 ***************************************************************************
 */
__printf_funct_t print_pcp_comment(int *tab, int action, char *cur_date, char *cur_time,
				   char *my_tz, char *comment, struct file_header *file_hdr,
				   struct record_header *record_hdr)
{
#ifdef HAVE_PCP
	static int def_metrics = FALSE;

	if (action & F_BEGIN) {
		if (!def_metrics) {
			pmiAddMetric("system.comment.value",
				     PM_IN_NULL, PM_TYPE_STRING, PM_INDOM_NULL, PM_SEM_DISCRETE,
				     pmiUnits(0, 0, 0, 0, 0, 0));

			def_metrics = TRUE;
		}
	}
	if (action & F_MAIN) {
		pmiPutValue("system.comment.value", NULL, comment);

		/* Write data to PCP archive */
		pcp_write_data(record_hdr, flags);
	}
#endif /* HAVE_PCP */
}

/*
 ***************************************************************************
 * Display the "statistics" part of the report (XML format).
 *
 * IN:
 * @tab		Number of tabulations.
 * @action	Action expected from current function.
 * @act		Array of activities (unused here).
 * @id_seq	Activity sequence (unused here).
 *
 * OUT:
 * @tab		Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_xml_statistics(int *tab, int action, struct activity *act[],
				      unsigned int id_seq[])
{
	if (action & F_BEGIN) {
		xprintf((*tab)++, "<statistics>");
	}
	if (action & F_END) {
		xprintf(--(*tab), "</statistics>");
	}
}

/*
 ***************************************************************************
 * Display the "statistics" part of the report (JSON format).
 *
 * IN:
 * @tab		Number of tabulations.
 * @action	Action expected from current function.
 * @act		Array of activities (unused here).
 * @id_seq	Activity sequence (unused here).
 *
 * OUT:
 * @tab		Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_json_statistics(int *tab, int action, struct activity *act[],
				       unsigned int id_seq[])
{
	static int sep = FALSE;

	if (action & F_BEGIN) {
		printf(",\n");
		xprintf((*tab)++, "\"statistics\": [");
	}
	if (action & F_MAIN) {
		if (sep) {
			xprintf(--(*tab), "},");
		}
		xprintf((*tab)++, "{");
		sep = TRUE;
	}
	if (action & F_END) {
		if (sep) {
			xprintf(--(*tab), "}");
			sep = FALSE;
		}
		xprintf0(--(*tab), "]");
	}
}

/*
 ***************************************************************************
 * Display the "statistics" part of the report (PCP format).
 *
 * IN:
 * @tab		Number of tabulations (unused here).
 * @action	Action expected from current function.
 * @act		Array of activities.
 * @id_seq	Activity sequence.
 ***************************************************************************
 */
__printf_funct_t print_pcp_statistics(int *tab, int action, struct activity *act[],
				      unsigned int id_seq[])
{
#ifdef HAVE_PCP
	if (action & F_BEGIN) {
		int i, p;

		for (i = 0; i < NR_ACT; i++) {
			if (!id_seq[i])
				continue;	/* Activity not in file */

			p = get_activity_position(act, id_seq[i], EXIT_IF_NOT_FOUND);
			if (!IS_SELECTED(act[p]->options))
				continue;	/* Activity not selected */

			switch (act[p]->id) {

				case A_CPU:
				case A_PWR_CPU:
				case A_NET_SOFT:
					pcp_def_cpu_metrics(act[p]);
					break;

				case A_PCSW:
					pcp_def_pcsw_metrics();
					break;

				case A_IRQ:
					pcp_def_irq_metrics(act[p]);
					pcp_def_cpu_metrics(act[p]);	/* For per_CPU int metrics */
					break;

				case A_SWAP:
					pcp_def_swap_metrics();
					break;

				case A_PAGE:
					pcp_def_paging_metrics();
					break;

				case A_IO:
					pcp_def_io_metrics();
					break;

				case A_MEMORY:
					pcp_def_memory_metrics(act[p]);
					break;

				case A_KTABLES:
					pcp_def_ktables_metrics();
					break;

				case A_QUEUE:
					pcp_def_queue_metrics();
					break;

				case A_SERIAL:
					pcp_def_serial_metrics(act[p]);
					break;

				case A_DISK:
					pcp_def_disk_metrics(act[p]);
					break;

				case A_NET_DEV:
				case A_NET_EDEV:
					pcp_def_net_dev_metrics(act[p]);
					break;

				case A_NET_NFS:
					pcp_def_net_nfs_metrics();
					break;

				case A_NET_NFSD:
					pcp_def_net_nfsd_metrics();
					break;

				case A_NET_SOCK:
					pcp_def_net_sock_metrics();
					break;

				case A_NET_IP:
					pcp_def_net_ip_metrics();
					break;

				case A_NET_EIP:
					pcp_def_net_eip_metrics();
					break;

				case A_NET_ICMP:
					pcp_def_net_icmp_metrics();
					break;

				case A_NET_EICMP:
					pcp_def_net_eicmp_metrics();
					break;

				case A_NET_TCP:
					pcp_def_net_tcp_metrics();
					break;

				case A_NET_ETCP:
					pcp_def_net_etcp_metrics();
					break;

				case A_NET_UDP:
					pcp_def_net_udp_metrics();
					break;

				case A_NET_SOCK6:
					pcp_def_net_sock6_metrics();
					break;

				case A_NET_IP6:
					pcp_def_net_ip6_metrics();
					break;

				case A_NET_EIP6:
					pcp_def_net_eip6_metrics();
					break;

				case A_NET_ICMP6:
					pcp_def_net_icmp6_metrics();
					break;

				case A_NET_EICMP6:
					pcp_def_net_eicmp6_metrics();
					break;

				case A_NET_UDP6:
					pcp_def_net_udp6_metrics();
					break;

				case A_HUGE:
					pcp_def_huge_metrics();
					break;

				case A_PWR_FAN:
					pcp_def_pwr_fan_metrics(act[p]);
					break;

				case A_PWR_TEMP:
					pcp_def_pwr_temp_metrics(act[p]);
					break;

				case A_PWR_IN:
					pcp_def_pwr_in_metrics(act[p]);
					break;

				case A_PWR_BAT:
					pcp_def_pwr_bat_metrics(act[p]);
					break;

				case A_PWR_USB:
					pcp_def_pwr_usb_metrics(act[p]);
					break;

				case A_FS:
					pcp_def_filesystem_metrics(act[p]);
					break;

				case A_NET_FC:
					pcp_def_fchost_metrics(act[p]);
					break;

				case A_PSI_CPU:
				case A_PSI_IO:
				case A_PSI_MEM:
					pcp_def_psi_metrics(act[p]);
					break;
			}
		}
	}
#endif /* HAVE_PCP */
}

/*
 ***************************************************************************
 * Display the "timestamp" part of the report (db and ppc format).
 *
 * IN:
 * @fmt		Output format (F_DB_OUTPUT or F_PPC_OUTPUT).
 * @file_hdr	System activity file standard header.
 * @cur_date	Date string of current record.
 * @cur_time	Time string of current record.
 * @my_tz	Current timezone.
 * @itv		Interval of time with preceding record.
 *
 * RETURNS:
 * Pointer on the "timestamp" string.
 ***************************************************************************
 */
char *print_dbppc_timestamp(int fmt, struct file_header *file_hdr, char *cur_date,
			    char *cur_time, char *my_tz, unsigned long long itv)
{
	int isdb = (fmt == F_DB_OUTPUT);
	static char pre[512];
	char temp1[128], temp2[256];

	/* This substring appears on every output line, preformat it here */
	snprintf(temp1, sizeof(temp1), "%s%s%llu%s",
		 file_hdr->sa_nodename, seps[isdb], itv, seps[isdb]);
	if (strlen(cur_date)) {
		snprintf(temp2, sizeof(temp2), "%s%s ", temp1, cur_date);
	}
	else {
		strcpy(temp2, temp1);
	}

	if (strlen(cur_date) && (!PRINT_TRUE_TIME(flags) ||
				 (PRINT_TRUE_TIME(flags) && strlen(file_hdr->sa_tzname)))) {
		snprintf(pre, sizeof(pre), "%s%s %s", temp2, cur_time,
			 PRINT_LOCAL_TIME(flags) ? my_tz
						 : (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
									   : "UTC"));
	}
	else {
		snprintf(pre, sizeof(pre), "%s%s", temp2, cur_time);
	}

	pre[sizeof(pre) - 1] = '\0';

	if (DISPLAY_HORIZONTALLY(flags)) {
		printf("%s", pre);
	}

	return pre;
}

/*
 ***************************************************************************
 * Display the "timestamp" part of the report (ppc format).
 *
 * IN:
 * @parm	Pointer on specific parameters (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current record.
 * @cur_time	Time string of current record.
 * @my_tz	Current timezone.
 * @itv		Interval of time with preceding record.
 * @record_hdr	Record header for current sample (unused here).
 * @file_hdr	System activity file standard header.
 * @flags	Flags for common options (unused here).
 *
 * RETURNS:
 * Pointer on the "timestamp" string.
 ***************************************************************************
 */
__tm_funct_t print_ppc_timestamp(void *parm, int action, char *cur_date,
				 char *cur_time, char *my_tz, unsigned long long itv,
				 struct record_header *record_hdr,
				 struct file_header *file_hdr, unsigned int flags)
{
	if (action & F_BEGIN) {
		return print_dbppc_timestamp(F_PPC_OUTPUT, file_hdr, cur_date, cur_time,
					     my_tz, itv);
	}

	return NULL;
}

/*
 ***************************************************************************
 * Display the "timestamp" part of the report (db format).
 *
 * IN:
 * @parm	Pointer on specific parameters (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current record.
 * @cur_time	Time string of current record.
 * @my_tz	Current timezone.
 * @itv		Interval of time with preceding record.
 * @record_hdr	Record header for current sample (unused here).
 * @file_hdr	System activity file standard header.
 * @flags	Flags for common options.
 *
 * RETURNS:
 * Pointer on the "timestamp" string.
 ***************************************************************************
 */
__tm_funct_t print_db_timestamp(void *parm, int action, char *cur_date,
				char *cur_time, char *my_tz, unsigned long long itv,
				struct record_header *record_hdr,
				struct file_header *file_hdr, unsigned int flags)
{
	if (action & F_BEGIN) {
		return print_dbppc_timestamp(F_DB_OUTPUT, file_hdr, cur_date, cur_time,
					     my_tz, itv);
	}
	if (action & F_END) {
		if (DISPLAY_HORIZONTALLY(flags)) {
			printf("\n");
		}
	}

	return NULL;
}

/*
 ***************************************************************************
 * Display the "timestamp" part of the report (XML format).
 *
 * IN:
 * @parm	Specific parameter. Here: number of tabulations.
 * @action	Action expected from current function.
 * @cur_date	Date string of current comment.
 * @cur_time	Time string of current comment.
 * @my_tz	Current timezone.
 * @itv		Interval of time with preceding record.
 * @record_hdr	Record header for current sample (unused here).
 * @file_hdr	System activity file standard header (unused here).
 * @flags	Flags for common options.
 ***************************************************************************
 */
__tm_funct_t print_xml_timestamp(void *parm, int action, char *cur_date,
				 char *cur_time, char *my_tz, unsigned long long itv,
				 struct record_header *record_hdr,
				 struct file_header *file_hdr, unsigned int flags)
{
	int *tab = (int *) parm;

	if (action & F_BEGIN) {
		xprintf((*tab)++, "<timestamp date=\"%s\" time=\"%s\" tz=\"%s\" interval=\"%llu\">",
			cur_date, cur_time,
			PRINT_LOCAL_TIME(flags) ? my_tz
						: (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
									  : "UTC"),
			itv);
	}
	if (action & F_END) {
		xprintf(--(*tab), "</timestamp>");
	}

	return NULL;
}

/*
 ***************************************************************************
 * Display the "timestamp" part of the report (JSON format).
 *
 * IN:
 * @parm	Specific parameter. Here: number of tabulations.
 * @action	Action expected from current function.
 * @cur_date	Date string of current comment.
 * @cur_time	Time string of current comment.
 * @my_tz	Current timezone.
 * @itv		Interval of time with preceding record.
 * @record_hdr	Record header for current sample (unused here).
 * @file_hdr	System activity file standard header (unused here).
 * @flags	Flags for common options.
 ***************************************************************************
 */
__tm_funct_t print_json_timestamp(void *parm, int action, char *cur_date,
				  char *cur_time, char *my_tz, unsigned long long itv,
				  struct record_header *record_hdr,
				  struct file_header *file_hdr, unsigned int flags)
{
	int *tab = (int *) parm;

	if (action & F_BEGIN) {
		xprintf0(*tab,
			 "\"timestamp\": {\"date\": \"%s\", \"time\": \"%s\", "
			 "\"tz\": \"%s\", \"interval\": %llu}",
			 cur_date, cur_time,
			 PRINT_LOCAL_TIME(flags) ? my_tz
						 : (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
									   : "UTC"),
			itv);
	}
	if (action & F_MAIN) {
		printf(",\n");
	}
	if (action & F_END) {
		printf("\n");
	}

	return NULL;
}

/*
 ***************************************************************************
 * Display the "timestamp" part of the report (raw format).
 *
 * IN:
 * @parm	Pointer on specific parameters (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current record.
 * @cur_time	Time string of current record.
 * @my_tz	Current timezone.
 * @itv		Interval of time with preceding record (unused here).
 * @record_hdr	Record header for current sample (unused here).
 * @file_hdr	System activity file standard header (unused here).
 * @flags	Flags for common options.
 *
 * RETURNS:
 * Pointer on the "timestamp" string.
 ***************************************************************************
 */
__tm_funct_t print_raw_timestamp(void *parm, int action, char *cur_date,
				 char *cur_time, char *my_tz, unsigned long long itv,
				 struct record_header *record_hdr,
				 struct file_header *file_hdr, unsigned int flags)
{
	static char pre[80];

	if (action & F_BEGIN) {
		if (strlen(cur_date) && (!PRINT_TRUE_TIME(flags) ||
					 (PRINT_TRUE_TIME(flags) && strlen(file_hdr->sa_tzname)))) {
			snprintf(pre, sizeof(pre), "%s %s", cur_time,
				 PRINT_LOCAL_TIME(flags) ? my_tz
							 : (PRINT_TRUE_TIME(flags) ? file_hdr->sa_tzname
										   : "UTC"));
		}
		else {
			snprintf(pre, sizeof(pre), "%s", cur_time);
		}
		pre[sizeof(pre) - 1] = '\0';

		return pre;
	}

	return NULL;
}

/*
 ***************************************************************************
 * Display the "timestamp" part of the report (PCP format).
 *
 * IN:
 * @parm	Pointer on specific parameters (unused here).
 * @action	Action expected from current function.
 * @cur_date	Date string of current record (unused here).
 * @cur_time	Time string of current record (unused here).
 * @my_tz	Current timezone (unused here).
 * @itv		Interval of time with preceding record (unused here).
 * @record_hdr	Record header for current sample.
 * @file_hdr	System activity file standard header (unused here).
 * @flags	Flags for common options.
 *
 * RETURNS:
 * Pointer on the "timestamp" string.
 ***************************************************************************
 */
__tm_funct_t print_pcp_timestamp(void *parm, int action, char *cur_date,
				 char *cur_time, char *my_tz, unsigned long long itv,
				 struct record_header *record_hdr,
				 struct file_header *file_hdr, unsigned int flags)
{
	if (action & F_END) {
		pcp_write_data(record_hdr, flags);
	}

	return NULL;
}

/*
 ***************************************************************************
 * Display the header of the report (XML format).
 *
 * IN:
 * @parm	Specific parameter. Here: number of tabulations.
 * @action	Action expected from current function.
 * @dfile	Unused here (PCP archive file).
 * @my_tz	Current timezone (unused here).
 * @file_magic	System activity file magic header.
 * @file_hdr	System activity file standard header.
 * @act		Array of activities (unused here).
 * @id_seq	Activity sequence (unused here).
 * @file_actlst	List of (known or unknown) activities in file (unused here).
 *
 * OUT:
 * @parm	Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_xml_header(void *parm, int action, char *dfile, char *my_tz,
				  struct file_magic *file_magic,
				  struct file_header *file_hdr,
				  struct activity *act[], unsigned int id_seq[],
				  struct file_activity *file_actlst)
{
	struct tm rectime, loc_t;
	time_t t = file_hdr->sa_ust_time;
	int *tab = (int *) parm;

	if (action & F_BEGIN) {
		char cur_time[TIMESTAMP_LEN];

		printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		xprintf(*tab, "<sysstat\n"
			      "xmlns=\"https://sysstat.github.io\"\n"
			      "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
			      "xsi:schemaLocation=\"https://sysstat.github.io https://sysstat.github.io/sysstat.xsd\">");

		xprintf(++(*tab), "<sysdata-version>%s</sysdata-version>",
			XML_DTD_VERSION);

		xprintf(*tab, "<host nodename=\"%s\">", file_hdr->sa_nodename);
		xprintf(++(*tab), "<sysname>%s</sysname>", file_hdr->sa_sysname);
		xprintf(*tab, "<release>%s</release>", file_hdr->sa_release);

		xprintf(*tab, "<machine>%s</machine>", file_hdr->sa_machine);
		xprintf(*tab, "<number-of-cpus>%d</number-of-cpus>",
			file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1);

		/* Fill file timestmap structure (rectime) */
		get_file_timestamp_struct(flags, &rectime, file_hdr);
		strftime(cur_time, sizeof(cur_time), "%Y-%m-%d", &rectime);
		xprintf(*tab, "<file-date>%s</file-date>", cur_time);

		if (gmtime_r(&t, &loc_t) != NULL) {
			strftime(cur_time, sizeof(cur_time), "%T", &loc_t);
			xprintf(*tab, "<file-utc-time>%s</file-utc-time>", cur_time);
		}

		xprintf(*tab, "<timezone>%s</timezone>", file_hdr->sa_tzname);
	}
	if (action & F_END) {
		xprintf(--(*tab), "</host>");
		xprintf(--(*tab), "</sysstat>");
	}
}

/*
 ***************************************************************************
 * Display the header of the report (JSON format).
 *
 * IN:
 * @parm	Specific parameter. Here: number of tabulations.
 * @action	Action expected from current function.
 * @dfile	Unused here (PCP archive file).
 * @my_tz	Current timezone (unused here).
 * @file_magic	System activity file magic header.
 * @file_hdr	System activity file standard header.
 * @act		Array of activities (unused here).
 * @id_seq	Activity sequence (unused here).
 * @file_actlst	List of (known or unknown) activities in file (unused here).
 *
 * OUT:
 * @parm	Number of tabulations.
 ***************************************************************************
 */
__printf_funct_t print_json_header(void *parm, int action, char *dfile, char *my_tz,
				   struct file_magic *file_magic,
				   struct file_header *file_hdr,
				   struct activity *act[], unsigned int id_seq[],
				   struct file_activity *file_actlst)
{
	struct tm rectime, loc_t;
	time_t t = file_hdr->sa_ust_time;
	int *tab = (int *) parm;

	if (action & F_BEGIN) {
		char cur_time[TIMESTAMP_LEN];

		xprintf(*tab, "{\"sysstat\": {");

		xprintf(++(*tab), "\"hosts\": [");
		xprintf(++(*tab), "{");
		xprintf(++(*tab), "\"nodename\": \"%s\",", file_hdr->sa_nodename);
		xprintf(*tab, "\"sysname\": \"%s\",", file_hdr->sa_sysname);
		xprintf(*tab, "\"release\": \"%s\",", file_hdr->sa_release);

		xprintf(*tab, "\"machine\": \"%s\",", file_hdr->sa_machine);
		xprintf(*tab, "\"number-of-cpus\": %d,",
			file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1);

		/* Fill file timestmap structure (rectime) */
		get_file_timestamp_struct(flags, &rectime, file_hdr);
		strftime(cur_time, sizeof(cur_time), "%Y-%m-%d", &rectime);
		xprintf(*tab, "\"file-date\": \"%s\",", cur_time);

		if (gmtime_r(&t, &loc_t) != NULL) {
			strftime(cur_time, sizeof(cur_time), "%T", &loc_t);
			xprintf(*tab, "\"file-utc-time\": \"%s\",", cur_time);
		}

		xprintf0(*tab, "\"timezone\": \"%s\"", file_hdr->sa_tzname);
	}
	if (action & F_END) {
		printf("\n");
		xprintf(--(*tab), "}");
		xprintf(--(*tab), "]");
		xprintf(--(*tab), "}}");
	}
}

/*
 ***************************************************************************
 * Display data file header.
 *
 * IN:
 * @parm	Specific parameter (unused here).
 * @action	Action expected from current function.
 * @dfile	Name of system activity data file (unused here).
 * @my_tz	Current timezone (unused here).
 * @file_magic	System activity file magic header.
 * @file_hdr	System activity file standard header.
 * @act		Array of activities.
 * @id_seq	Activity sequence.
 * @file_actlst	List of (known or unknown) activities in file.
 ***************************************************************************
 */
__printf_funct_t print_hdr_header(void *parm, int action, char *dfile, char *my_tz,
				  struct file_magic *file_magic,
				  struct file_header *file_hdr,
				  struct activity *act[], unsigned int id_seq[],
				  struct file_activity *file_actlst)
{
	/* Actions F_MAIN and F_END ignored */
	if (action & F_BEGIN) {
		struct tm rectime, loc_t;
		time_t t = file_hdr->sa_ust_time;
		int i, p;
		char cur_time[TIMESTAMP_LEN];
		struct file_activity *fal;

		printf(_("System activity data file: %s (%#x)\n"),
		       dfile, file_magic->format_magic);

		display_sa_file_version(stdout, file_magic);

		if (file_magic->format_magic != FORMAT_MAGIC) {
			return;
		}

		printf(_("Genuine sa datafile: %s (%x)\n"),
		       file_magic->upgraded ? _("no") : _("yes"),
		       file_magic->upgraded);

		printf(_("Host: "));
		print_gal_header(localtime_r(&t, &rectime),
				 file_hdr->sa_sysname, file_hdr->sa_release,
				 file_hdr->sa_nodename, file_hdr->sa_machine,
				 file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1,
				 PLAIN_OUTPUT);

		/* Fill file timestamp structure (rectime) */
		get_file_timestamp_struct(flags, &rectime, file_hdr);
		strftime(cur_time, sizeof(cur_time), "%Y-%m-%d", &rectime);
		printf(_("File date: %s\n"), cur_time);

		if (gmtime_r(&t, &loc_t) != NULL) {
			printf(_("File time: "));
			strftime(cur_time, sizeof(cur_time), "%T", &loc_t);
			printf("%s UTC (%llu)\n", cur_time, file_hdr->sa_ust_time);
		}

		printf(_("Timezone: %s\n"), file_hdr->sa_tzname);

		/* File composition: file_header, file_activity, record_header */
		printf(_("File composition: (%u,%u,%u),(%u,%u,%u),(%u,%u,%u)\n"),
		       file_magic->hdr_types_nr[0], file_magic->hdr_types_nr[1], file_magic->hdr_types_nr[2],
		       file_hdr->act_types_nr[0], file_hdr->act_types_nr[1], file_hdr->act_types_nr[2],
		       file_hdr->rec_types_nr[0], file_hdr->rec_types_nr[1], file_hdr->rec_types_nr[2]);

		printf(_("Size of a long int: %d\n"), file_hdr->sa_sizeof_long);
		printf("HZ = %lu\n", file_hdr->sa_hz);
		printf(_("Number of activities in file: %u\n"),
		       file_hdr->sa_act_nr);
		printf(_("Extra structures available: %c\n"),
		       file_hdr->extra_next ? 'Y' : 'N');

		printf(_("List of activities:\n"));
		fal = file_actlst;
		for (i = 0; i < file_hdr->sa_act_nr; i++, fal++) {

			p = get_activity_position(act, fal->id, RESUME_IF_NOT_FOUND);

			printf("%02u: [%02x] ", fal->id, fal->magic);
			if (p >= 0) {
				printf("%-20s", act[p]->name);
			}
			else {
				printf("%-20s", _("Unknown activity"));
			}
			printf(" %c:%4d", fal->has_nr ? 'Y' : 'N', fal->nr);
			if (fal->nr2 > 1) {
				printf("x%d", fal->nr2);
			}
			printf("\t(%u,%u,%u)", fal->types_nr[0], fal->types_nr[1], fal->types_nr[2]);
			if ((p >= 0) && (act[p]->magic != fal->magic)) {
				printf(_(" \t[Unknown format]"));
			}
			printf("\n");
		}
	}
}

/*
 ***************************************************************************
 * Display the header of the report (SVG format).
 *
 * IN:
 * @parm	Specific parameters. Here: number of rows of views to display
 *		or canvas height entered on the command line (@graph_nr), and
 *		max number of views on a single row (@views_per_row).
 * @action	Action expected from current function.
 * @dfile	Name of system activity data file (unused here).
 * @my_tz	Current timezone (unused here).
 * @file_magic	System activity file magic header (unused here).
 * @file_hdr	System activity file standard header.
 * @act		Array of activities (unused here).
 * @id_seq	Activity sequence (unused here).
 * @file_actlst	List of (known or unknown) activities in file (unused here).
 ***************************************************************************
 */
__printf_funct_t print_svg_header(void *parm, int action, char *dfile, char *my_tz,
				  struct file_magic *file_magic,
				  struct file_header *file_hdr,
				  struct activity *act[], unsigned int id_seq[],
				  struct file_activity *file_actlst)
{
	struct svg_hdr_parm *hdr_parm = (struct svg_hdr_parm *) parm;
	struct tm rectime;
	time_t t = file_hdr->sa_ust_time;

	if (action & F_BEGIN) {
		printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
		printf("<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" ");
		printf("\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");
		printf("<svg xmlns=\"http://www.w3.org/2000/svg\"");
		if (DISPLAY_TOC(flags)) {
			printf(" xmlns:xlink=\"http://www.w3.org/1999/xlink\"");
		}
		if (action & F_END) {
			printf(">\n");
		}
	}

	if (action & F_MAIN) {
		unsigned int height;

		if (SET_CANVAS_HEIGHT(flags)) {
			/*
			 * Option "-O height=..." used: @graph_nr is
			 * the SVG canvas height set on the command line.
			 */
			height = hdr_parm->graph_nr;
		}
		else {
			height = SVG_H_YSIZE +
				 SVG_C_YSIZE * (DISPLAY_TOC(flags) ? hdr_parm->nr_act_dispd : 0) +
				 SVG_T_YSIZE * hdr_parm->graph_nr;
		}
		if (height < MIN_CANVAS_HEIGHT) {
			/* There is a min canvas height (at least to display "No data") */
			height = MIN_CANVAS_HEIGHT;
		}
		printf(" width=\"%d\" height=\"%u\""
		       " fill=\"black\" stroke=\"#%06x\" stroke-width=\"1\">\n",
		       SVG_T_XSIZE * (hdr_parm->views_per_row), height,
		       svg_colors[palette][SVG_COL_DEFAULT_IDX]);
		printf("<text x=\"0\" y=\"30\" text-anchor=\"start\" stroke=\"#%06x\">",
		       svg_colors[palette][SVG_COL_HEADER_IDX]);
		print_gal_header(localtime_r(&t, &rectime),
				 file_hdr->sa_sysname, file_hdr->sa_release,
				 file_hdr->sa_nodename, file_hdr->sa_machine,
				 file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1,
				 PLAIN_OUTPUT);
		printf("</text>\n");
		if (DISPLAY_TOC(flags)) {
			unsigned int ht = 0;
			int i, p;

			for (i = 0; i < NR_ACT; i++) {
				if (!id_seq[i])
					continue;	/* Activity not in file */

				p = get_activity_position(act, id_seq[i], EXIT_IF_NOT_FOUND);
				if (!IS_SELECTED(act[p]->options) || !act[p]->g_nr)
					continue;	/* Activity not selected or no graph available */

				printf("<a xlink:href=\"#g%u-0\" xlink:title=\"%s\">\n",
				       act[p]->id, act[p]->name);
				printf("<text x=\"10\" y=\"%u\">%s</text></a>\n",
				       SVG_H_YSIZE + ht, act[p]->desc);
				ht += SVG_C_YSIZE;
			}
		}
	}

	if (action & F_END) {
		if (!(action & F_BEGIN)) {
			if (!hdr_parm->graph_nr) {
				/* No views displayed */
				printf("<text x= \"0\" y=\"%d\" text-anchor=\"start\" stroke=\"#%06x\">",
				       SVG_H_YSIZE +
				       SVG_C_YSIZE * (DISPLAY_TOC(flags) ? hdr_parm->nr_act_dispd : 0),
				       svg_colors[palette][SVG_COL_ERROR_IDX]);
				printf("No data!</text>\n");
			}
			/* Give actual SVG height */
			printf("<!-- Actual canvas height: %d -->\n",
			       SVG_H_YSIZE +
			       SVG_C_YSIZE * (DISPLAY_TOC(flags) ? hdr_parm->nr_act_dispd : 0) +
			       SVG_T_YSIZE * hdr_parm->graph_nr);
		}
		printf("</svg>\n");
	}
}

/*
 ***************************************************************************
 * PCP header function.
 *
 * IN:
 * @parm	Specific parameter (unused here).
 * @action	Action expected from current function.
 * @dfile	Name of PCP archive file.
 * @my_tz	Current timezone (unused here).
 * @file_magic	System activity file magic header (unused here).
 * @file_hdr	System activity file standard header (unused here).
 * @act		Array of activities (unused here).
 * @id_seq	Activity sequence (unused here).
 * @file_actlst	List of (known or unknown) activities in file (unused here).
 ***************************************************************************
 */
__printf_funct_t print_pcp_header(void *parm, int action, char *dfile, char *my_tz,
				  struct file_magic *file_magic,
				  struct file_header *file_hdr,
				  struct activity *act[], unsigned int id_seq[],
				  struct file_activity *file_actlst)
{
#ifdef HAVE_PCP
	char buf[64];
	unsigned long long utc_sec = file_hdr->sa_ust_time;

	if (action & F_BEGIN) {
		/* Create new PCP context */
		pmiStart(dfile, FALSE);

		/* Set timezone */
		pmiSetTimezone(file_hdr->sa_tzname);

		/* Save hostname */
		pmiSetHostname(file_hdr->sa_nodename);

		/* Save number of CPU in PCP archive */
		pmiAddMetric("hinv.ncpu",
			     pmiID(60, 0, 32), PM_TYPE_U32, PM_INDOM_NULL,
			     PM_SEM_DISCRETE, pmiUnits(0, 0, 0, 0, 0, 0));
		snprintf(buf, sizeof(buf), "%u",
			 file_hdr->sa_cpu_nr > 1 ? file_hdr->sa_cpu_nr - 1 : 1);
		pmiPutValue("hinv.ncpu", NULL, buf);

		/* Save uname(2) information */
		pmiAddMetric("kernel.uname.release",
			     pmiID(60, 12, 0), PM_TYPE_STRING, PM_INDOM_NULL,
			     PM_SEM_DISCRETE, pmiUnits(0, 0, 0, 0, 0, 0));
		pmiPutValue("kernel.uname.release", NULL, file_hdr->sa_release);
		pmiAddMetric("kernel.uname.sysname",
			     pmiID(60, 12, 2), PM_TYPE_STRING, PM_INDOM_NULL,
			     PM_SEM_DISCRETE, pmiUnits(0, 0, 0, 0, 0, 0));
		pmiPutValue("kernel.uname.sysname", NULL, file_hdr->sa_sysname);
		pmiAddMetric("kernel.uname.machine",
			     pmiID(60, 12, 3), PM_TYPE_STRING, PM_INDOM_NULL,
			     PM_SEM_DISCRETE, pmiUnits(0, 0, 0, 0, 0, 0));
		pmiPutValue("kernel.uname.machine", NULL, file_hdr->sa_machine);
		pmiAddMetric("kernel.uname.nodename",
			     pmiID(60, 12, 4), PM_TYPE_STRING, PM_INDOM_NULL,
			     PM_SEM_DISCRETE, pmiUnits(0, 0, 0, 0, 0, 0));
		pmiPutValue("kernel.uname.nodename", NULL, file_hdr->sa_nodename);
	}

	if (action & F_END) {
		if (action & F_BEGIN) {
			int rc;

			if ((rc = pmiWrite(utc_sec, 0)) < 0) {
				fprintf(stderr, "PCP: pmiWrite: %s\n", pmiErrStr(rc));
				exit(4);
			}
		}
		pmiEnd();
	}
#endif
}

/*
 ***************************************************************************
 * Count the number of new network interfaces in current sample. If a new
 * interface is found then add it to the linked list starting at
 * @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new interfaces identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_net_dev(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_net_dev *sndc;

	for (i = 0; i < a->nr[curr]; i++) {
		sndc = (struct stats_net_dev *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list), sndc->interface, MAX_IFACE_LEN, NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Count the number of new network interfaces in current sample. If a new
 * interface is found then add it to the linked list starting at
 * @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new interfaces identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_net_edev(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_net_edev *snedc;

	for (i = 0; i < a->nr[curr]; i++) {
		snedc = (struct stats_net_edev *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list), snedc->interface, MAX_IFACE_LEN, NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Count the number of new filesystems in current sample. If a new
 * filesystem is found then add it to the linked list starting at
 * @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new filesystems identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_filesystem(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_filesystem *sfc;

	for (i = 0; i < a->nr[curr]; i++) {
		sfc = (struct stats_filesystem *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list),
				    get_fs_name_to_display(a, flags, sfc),
				    MAX_FS_LEN, NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Count the number of new fchosts in current sample. If a new
 * fchost is found then add it to the linked list starting at
 * @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new fchosts identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_fchost(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_fchost *sfcc;

	for (i = 0; i < a->nr[curr]; i++) {
		sfcc = (struct stats_fchost *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list), sfcc->fchost_name, MAX_FCH_LEN, NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Count the number of new block devices in current sample. If a new
 * block device is found then add it to the linked list starting at
 * @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new block devices identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_disk(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_disk *sdc;

	for (i = 0; i < a->nr[curr]; i++) {
		sdc = (struct stats_disk *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list),
				    get_device_name(sdc->major, sdc->minor, sdc->wwn, sdc->part_nr,
						    DISPLAY_PRETTY(flags), DISPLAY_PERSIST_NAME_S(flags),
						    USE_STABLE_ID(flags), NULL),
				    MAX_DEV_LEN, NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Count the number of interrupts in current sample. Add each interrupt name
 * to the linked list starting at @a->item_list.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of interrupts added to the list.
 ***************************************************************************
 */
__nr_t count_new_int(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_irq *stc_cpuall_irq;

	if (a->item_list)
		/*
		 * If a list already exists, do nothing. This means that a list has been
		 * explicitly entered on the command line using option "--int=", or that
		 * the list has already been created here (remember that the number of
		 * interrupts cannot change in file: @nr2, the second matrix dimension,
		 * is a constant).
		 */
		return 0;

	for (i = 0; i < a->nr2; i++) {
		stc_cpuall_irq = (struct stats_irq *) ((char *) a->buf[curr] + i * a->msize);

		nr += add_list_item(&(a->item_list), stc_cpuall_irq->irq_name,
				    MAX_SA_IRQ_LEN, NULL);
	}

	return nr;
}

/*
 * **************************************************************************
 * Count the number of new batteries in current sample. If a new
 * battery is found then add it to the linked list starting at
 * @a->item_list.
 * Mainly useful to create a list of battery names (BATx) that will be used
 * as instance names for sadf PCP output format.
 *
 * IN:
 * @a		Activity structure with statistics.
 * @curr	Index in array for current sample statistics.
 *
 * RETURNS:
 * Number of new batteries identified in current sample that were not
 * previously in the list.
 ***************************************************************************
 */
__nr_t count_new_bat(struct activity *a, int curr)
{
	int i, nr = 0;
	struct stats_pwr_bat *spbc;
	char bat_name[16];

	for (i = 0; i < a->nr[curr]; i++) {
		spbc = (struct stats_pwr_bat *) ((char *) a->buf[curr] + i * a->msize);

		snprintf(bat_name, sizeof(bat_name), "BAT%d", (int) spbc->bat_id);
		nr += add_list_item(&(a->item_list), bat_name, sizeof(bat_name), NULL);
	}

	return nr;
}

/*
 ***************************************************************************
 * Init custom color palette used to draw graphs (sadf -g).
 ***************************************************************************
 */
void init_custom_color_palette(void)
{
	char *e, *p;
	int len;
	unsigned int val;

	/* Read S_COLORS_PALETTE environment variable */
	if ((e = __getenv(ENV_COLORS_PALETTE)) == NULL)
		/* Environment variable not set */
		return;

	for (p = strtok(e, ":"); p; p =strtok(NULL, ":")) {

		len = strlen(p);
		if ((len > 8) || (len < 3) || (*(p + 1) != '=') ||
		    (strspn(p + 2, "0123456789ABCDEFabcdef") != (len - 2)))
			/* Ignore malformed codes */
			continue;

		sscanf(p + 2, "%x", &val);

		if ((*p >= '0') && (*p <= '9')) {
			svg_colors[SVG_CUSTOM_COL_PALETTE][*p & 0xf] = val;
			continue;
		}
		else if (((*p >= 'A') && (*p <= 'F')) ||
			 ((*p >= 'a') && (*p <= 'f'))) {
			svg_colors[SVG_CUSTOM_COL_PALETTE][9 + (*p & 0xf)] = val;
			continue;
		}

		switch (*p) {
			case 'G':
				svg_colors[SVG_CUSTOM_COL_PALETTE][SVG_COL_GRID_IDX] = val;
				break;
			case 'H':
				svg_colors[SVG_CUSTOM_COL_PALETTE][SVG_COL_HEADER_IDX] = val;
				break;
			case 'I':
				svg_colors[SVG_CUSTOM_COL_PALETTE][SVG_COL_INFO_IDX] = val;
				break;
			case 'K':
				svg_colors[SVG_CUSTOM_COL_PALETTE][SVG_COL_BCKGRD_IDX] = val;
				break;
			case 'L':
				svg_colors[SVG_CUSTOM_COL_PALETTE][SVG_COL_DEFAULT_IDX] = val;
				break;
			case 'T':
				svg_colors[SVG_CUSTOM_COL_PALETTE][SVG_COL_TITLE_IDX] = val;
				break;
			case 'W':
				svg_colors[SVG_CUSTOM_COL_PALETTE][SVG_COL_ERROR_IDX] = val;
				break;
			case 'X':
				svg_colors[SVG_CUSTOM_COL_PALETTE][SVG_COL_AXIS_IDX] = val;
				break;
		}
	}
}
