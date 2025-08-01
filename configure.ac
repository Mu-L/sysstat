# configure.ac
#
# (C) 2007 - Ivana Varekova <varekova@redhat.com>
# Modified by Sebastien Godard (sysstat <at> orange.fr)

# Initialization of $PACKAGE_VERSION and $PACKAGE_NAME variables
AC_INIT([sysstat],[12.7.9])

# Ensure that a recent enough version of Autoconf is being used
AC_PREREQ([2.71])

# Check whether the configure script is in the right dir
AC_CONFIG_SRCDIR(ioconf.h)

# Check programs
echo .
echo Check programs:
echo .

AC_PROG_CC
AC_USE_SYSTEM_EXTENSIONS
AC_PROG_LN_S

DFLAGS=""

AC_CHECK_PROG(CHMOD, chmod, chmod)
AC_CHECK_PROG(CHOWN, chown, chown)
AC_CHECK_PROG(AR, ar, ar)
AC_CHECK_PROG(INSTALL, install, install)
AC_CHECK_PROG(MSGFMT, msgfmt, msgfmt)
AC_CHECK_PROG(XGETTEXT, xgettext, xgettext)
AC_CHECK_PROG(MSGMERGE, msgmerge, msgmerge)
AC_CHECK_PROG(VER_XML, xmllint, xmllint)
AC_CHECK_PROGS(VER_JSON, json_verify json_pp, true)
AC_CHECK_PROGS(ZIP, xz bzip2 gzip, gzip)
INSTALL_DATA="\${INSTALL} -m 644"
AC_SUBST(INSTALL_DATA)
INSTALL_BIN="\${INSTALL} -m 755"
AC_SUBST(INSTALL_BIN)

AC_SUBST(VER_JSON)
AC_SUBST(VER_XML)

AC_PATH_PROG(PATH_CP, cp)
AC_PATH_PROG(PATH_CHKCONFIG, chkconfig)

# Check for systemd
AC_CHECK_PROG(PKG_CONFIG, pkg-config, pkg-config)
AC_PATH_PROG(SYSTEMCTL, systemctl)
AC_ARG_WITH([systemdsystemunitdir],
    AS_HELP_STRING([--with-systemdsystemunitdir=DIR],[Directory for systemd service files]),
    [], [with_systemdsystemunitdir=$($PKG_CONFIG --variable=systemdsystemunitdir systemd)])
if test -n "$with_systemdsystemunitdir" -a "x$with_systemdsystemunitdir" != xno ; then
    AC_SUBST([SYSTEMD_UNIT_DIR], [$with_systemdsystemunitdir])
fi
AC_ARG_WITH([systemdsleepdir],
    AS_HELP_STRING([--with-systemdsleepdir=DIR],[Directory for systemd suspend/resume scripts]),
    [], [with_systemdsleepdir=$($PKG_CONFIG --variable=systemdsleepdir systemd)])
if test -n "$with_systemdsleepdir" -a "x$with_systemdsleepdir" != xno ; then
    AC_SUBST([SYSTEMD_SLEEP_DIR], [$with_systemdsleepdir])
fi

# Check libraries

# Check header files
echo .
echo Check header files:
echo .
HAVE_LIBINTL_H=
HAVE_LOCALE_H=
HAVE_SYS_SYSMACROS_H=
HAVE_SYS_PARAM_H=
AC_CHECK_HEADERS(ctype.h)
AC_CHECK_HEADERS(errno.h)
AC_CHECK_HEADERS(libintl.h, HAVE_LIBINTL_H=1)
AC_CHECK_HEADERS(locale.h, HAVE_LOCALE_H=1)
AC_CHECK_HEADERS(linux/sched.h, HAVE_LINUX_SCHED_H=1)
AC_CHECK_HEADERS(net/if.h)
AC_CHECK_HEADERS(regex.h)
AC_CHECK_HEADERS(signal.h)
AC_CHECK_HEADERS(stdio.h)
AC_CHECK_HEADERS(stdint.h)
AC_CHECK_HEADERS(fcntl.h)
AC_CHECK_HEADERS(inttypes.h)
AC_CHECK_HEADERS(libgen.h)
AC_CHECK_HEADERS(pwd.h)
AC_CHECK_HEADERS(time.h)
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(pcp/pmapi.h, HAVE_PCP_PMAPI_H=1)
AC_CHECK_HEADERS(pcp/impl.h, HAVE_PCP_IMPL_H=1)
AC_CHECK_HEADERS(sys/time.h)
AC_CHECK_HEADERS(sys/statvfs.h)
AC_CHECK_HEADERS(sys/types.h)
AC_CHECK_HEADERS(sys/file.h)
AC_CHECK_HEADERS(sys/ioctl.h)
AC_CHECK_HEADERS(sys/param.h, HAVE_SYS_PARAM_H=1)
AC_CHECK_HEADERS(sys/stat.h)
AC_CHECK_HEADERS(sys/sysmacros.h, HAVE_SYS_SYSMACROS_H=1)
AC_CHECK_HEADERS(sys/utsname.h)
AC_CHECK_HEADERS(sys/wait.h)

echo .
echo Check typedefs, structures and compiler characteristics:
echo .
AC_DEFINE_UNQUOTED([RETSIGTYPE],[$ac_cv_type_signal],[Define as the return type of signal handlers
		    (`int' or `void').])

AC_TYPE_SIZE_T
AC_TYPE_OFF_T

echo .
echo Check library functions:
echo .
AC_CHECK_FUNCS(strchr)
AC_CHECK_FUNCS(strcspn)
AC_CHECK_FUNCS(strspn)
AC_CHECK_FUNCS(strstr)

# Should we check for PCP support?
AC_MSG_CHECKING(for PCP support)
AC_ARG_ENABLE(pcp,
	      AS_HELP_STRING([--disable-pcp],[disable PCP support]),
			     PCP=$enableval,PCP=yes)
if test $PCP != "yes"; then
	HAVE_PCP="n"
else
	HAVE_PCP="y"
fi

if test ! $HAVE_PCP_PMAPI_H; then
	HAVE_PCP="n"
	PCP=no
fi
AC_MSG_RESULT($PCP)

# Check for PCP libraries
LFPCP=""
DFPCP=""
if test $HAVE_PCP = "y"; then
	PCP=no
	AC_CHECK_LIB(pcp, pmGetVersion, LFPCP="-lpcp -lpcp_import", HAVE_PCP="n")
	AC_MSG_CHECKING(for PCP libraries)
	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <pcp/pmapi.h>
					     #include <pcp/import.h>]],
					   [[pmiEnd();]])],PCP=yes; DFPCP="-DHAVE_PCP", HAVE_PCP="n"; PCP=no)
	AC_MSG_RESULT($PCP)
fi
AC_SUBST(HAVE_PCP)
AC_SUBST(LFPCP)
AC_SUBST(DFPCP)

if test $HAVE_PCP_IMPL_H; then
   PCP_IMPL="y"
else
   PCP_IMPL="n"
fi
AC_SUBST(PCP_IMPL)

# Should we check for lm_sensors?
AC_MSG_CHECKING(for sensors support)
AC_ARG_ENABLE(sensors,
	      AS_HELP_STRING([--disable-sensors],[disable sensors support]),
			     SENSORS=$enableval,SENSORS=yes)
if test $SENSORS != "yes"; then
	SENSORS_SUPPORT="n"
else
	SENSORS_SUPPORT="y"
fi
AC_MSG_RESULT($SENSORS)

# Check for lm_sensors library
LFSENSORS=""
DFSENSORS=""
HAVE_SENSORS="n"
if test $SENSORS_SUPPORT = "y"; then
	SENSORS=no
	AC_CHECK_LIB(sensors, sensors_get_detected_chips, LFSENSORS="-lsensors", HAVE_SENSORS="n")
	AC_MSG_CHECKING(for sensors library)
	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <sensors/sensors.h>
					     #include <sensors/error.h>]],
					   [[sensors_cleanup();]])], SENSORS=yes; HAVE_SENSORS="y"; DFSENSORS="-DHAVE_SENSORS", HAVE_SENSORS="n"; SENSORS=no)
	AC_MSG_RESULT($SENSORS)
fi
AC_SUBST(HAVE_SENSORS)
AC_SUBST(LFSENSORS)
AC_SUBST(DFSENSORS)

# Check for 32-bit version of lm_sensors library
LFSENSORS32=""
DFSENSORS32=""
HAVE_SENSORS32="n"
if test $SENSORS_SUPPORT = "y"; then
	CFLAGS_SAVE=$CFLAGS
        # Was: CFLAGS+=" -m32" which is specific to bash and not sh
	CFLAGS="$CFLAGS -m32"
	LDFLAGS_SAVE=$LDFLAGS
	LDFLAGS="$LDFLAGS -lsensors"
	SENSORS=no
#	Check for another function to avoid using cached result
	AC_CHECK_LIB(sensors, sensors_cleanup, LFSENSORS32="-lsensors", HAVE_SENSORS32="n")
	AC_MSG_CHECKING(for sensors 32-bit library)
	AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <sensors/sensors.h>
					  #include <sensors/error.h>]],
					[[sensors_cleanup();]])], SENSORS=yes; HAVE_SENSORS32="y"; DFSENSORS32="-DHAVE_SENSORS32", HAVE_SENSORS32="n"; SENSORS=no)
	AC_MSG_RESULT($SENSORS)
	CFLAGS=$CFLAGS_SAVE
	LDFLAGS=$LDFLAGS_SAVE
fi
AC_SUBST(HAVE_SENSORS32)
AC_SUBST(LFSENSORS32)
AC_SUBST(DFSENSORS32)

# Check for 32-bit system libraries
TGLIB32=no
CFLAGS_SAVE=$CFLAGS
CFLAGS="$CFLAGS -m32"
AC_MSG_CHECKING(for 32-bit system libraries)
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <stdio.h>]],
				   [[printf("%d\n", sizeof(long));]])],TGLIB32=yes, TGLIB32=no)
AC_MSG_RESULT($TGLIB32)
CFLAGS=$CFLAGS_SAVE
AC_SUBST(TGLIB32)

echo .
echo Check system services:
echo .
AC_SYS_LARGEFILE

echo .
echo Check configuration:
echo .
# Check arguments used
#
# Optional Features:
#  --disable-FEATURE        do not include FEATURE (same as --enable-FEATURE=no)
#  --enable-FEATURE[=ARG]   include FEATURE [ARG=yes]
#  --disable-largefile      omit support for large files
#  --disable-nls            disable National Language Support
#  --disable-file-attr      don't set attributes on files being installed
#  --enable-install-cron    tell sysstat to install cron scripts
#  --collect-all            tell sadc to collect all possible data
#  --enable-clean-sa-dir    clean system activity directory
#  --disable-compress-manpg do not compress manual pages when installed
#  --enable-debuginfo       enable debug output (--debuginfo option)
#  --disable-documentation  do not install documentation (man pages...)
#  --disable-sensors        do not link against libsensors even if available
#  --disable-pcp            do not link against PCP libraries even if available
#  --disable-stripping      do not strip object files
#  --enable-copy-only       only copy files when installing sysstat
#  --enable-use-crond       use standard cron daemon
#  --enable-lto	            compile with Link Time Optimizations
#
# Some influential environment variables:
#  rcdir         directory where startup scripts are installed
#  sa_lib_dir    sadc, sa1 and sa2 directory
#  sa_dir        system activity daily datafiles directory
#  sar_dir       sar binary location. Used only in sa2 shell script
#  conf_dir      sysstat configuration directory (default is /etc/sysconfig)
#  conf_file     sysstat configuration file (default is sysstat)
#  history       number of daily datafiles to keep (default value is 7)
#  delay_range   maximum delay (in seconds) to wait before sa2 script generates its reports
#  compressafter number of days after which datafiles are compressed
#  man_group     group for man pages
#  cron_owner    crontab owner
#  cron_interval crontab sampling interval
#  sadc_options  options to be passed to sadc
#
# Fine tuning the installation directories:
#  --mandir=DIR           man documentation directory [PREFIX/man]
#  --docdir=DIR           other documentation directory [PREFIX/share/doc]
#
# Installation directories:
#  --prefix=PREFIX         install architecture-independent files in PREFIX
#                            [/usr/local]
#  --exec-prefix=EPREFIX   install architecture-dependent files in EPREFIX
#                            [PREFIX]

# Set directories
if test -d /etc/init.d && test ! -L /etc/init.d; then
   if test -d /etc/init.d/rc2.d; then
      RC_DIR=/etc/init.d
      INITD_DIR=.
   else
      RC_DIR=/etc
      INITD_DIR=init.d
   fi
   INIT_DIR=/etc/init.d
elif test -d /sbin/init.d; then
   RC_DIR=/sbin/init.d
   INIT_DIR=/sbin/init.d
   INITD_DIR=.
else
   RC_DIR=/etc/rc.d
   INIT_DIR=/etc/rc.d/init.d
   INITD_DIR=init.d
fi

AC_MSG_CHECKING(run-commands directory)
AC_ARG_VAR([rcdir],[run-commands directory])
if test x$rcdir != x""; then
	# Override previous rc directories values
	RC_DIR=$rcdir
	INIT_DIR=$rcdir/init.d
	INITD_DIR=init.d
fi
AC_MSG_RESULT($RC_DIR)
if test ! -d $RC_DIR; then
	echo "INFO: Directory ${RC_DIR} doesn't exist."
	echo "INFO: Startup scripts won't be installed."
fi

AC_SUBST(RC_DIR)
AC_SUBST(INIT_DIR)
AC_SUBST(INITD_DIR)

if test $HAVE_SYS_SYSMACROS_H; then
   SYSMACROS="y"
else
   SYSMACROS="n"
fi
AC_SUBST(SYSMACROS)

if test $HAVE_LINUX_SCHED_H; then
   LINUX_SCHED="y"
else
   LINUX_SCHED="n"
fi
AC_SUBST(LINUX_SCHED)

if test $HAVE_SYS_PARAM_H; then
   SYSPARAM="y"
else
   SYSPARAM="n"
fi
AC_SUBST(SYSPARAM)

# Set sadc directory
if test $prefix != "NONE"; then
   AuxPrefix=$prefix
else
   AuxPrefix=/usr/local
fi

SADC_DIR=$AuxPrefix/lib
if test -d $AuxPrefix/lib64; then
    # Verify that this OS is really 64 bit
    BITS=$(getconf LONG_BIT 2>/dev/null)
    if test $? = 0; then
        if test $BITS = 64; then
            SADC_DIR=$AuxPrefix/lib64
        fi
    else
        # Fallback: look for lm (long mode) flag to know if CPU is 64 bit
        grep " lm " /proc/cpuinfo >/dev/null 2>&1
        if test $? = 0; then
            SADC_DIR=$AuxPrefix/lib64
        fi
    fi
fi

AC_MSG_CHECKING(sadc directory)
AC_ARG_VAR([sa_lib_dir],[sadc directory])
if test x$sa_lib_dir != x""; then
   SA_LIB_DIR=$sa_lib_dir
else
   SA_LIB_DIR=$SADC_DIR/sa
fi
AC_MSG_RESULT($SA_LIB_DIR)
if test ! -d $SA_LIB_DIR; then
   echo "INFO: Directory ${SA_LIB_DIR} will be created during installation stage."
fi
AC_SUBST(SA_LIB_DIR)

# Set system activity directory
AC_MSG_CHECKING(system activity directory)
AC_ARG_VAR([sa_dir],[system activity directory])
if test x$sa_dir != x""; then
   SA_DIR=$sa_dir
else
   SA_DIR=/var/log/sa
fi
AC_MSG_RESULT($SA_DIR)
if test ! -d $SA_DIR; then
   echo "INFO: Directory ${SA_DIR} will be created during installation stage."
fi
AC_SUBST(SA_DIR)

# sar binary location. Used only in sa2 shell script
AC_MSG_CHECKING(sar directory)
AC_ARG_VAR([sar_dir],[sar directory])
if test x$sar_dir != x""; then
   SAR_DIR=$sar_dir
else
   SAR_DIR=
fi
AC_MSG_RESULT($SAR_DIR)
AC_SUBST(SAR_DIR)

# Set configuration directory
AC_MSG_CHECKING(sysstat configuration directory)
AC_ARG_VAR([conf_dir],[sysstat configuration directory])
if test x$conf_dir != x""; then
   SYSCONFIG_DIR=$conf_dir
else
   SYSCONFIG_DIR=/etc/sysconfig
fi
AC_MSG_RESULT($SYSCONFIG_DIR)
if test ! -d $SYSCONFIG_DIR; then
   echo "INFO: Directory ${SYSCONFIG_DIR} will be created during installation stage."
fi
AC_SUBST(SYSCONFIG_DIR)

# Set configuration directory
AC_MSG_CHECKING(sysstat configuration file)
AC_ARG_VAR([conf_file],[sysstat configuration file])
if test x$conf_file != x""; then
   SYSCONFIG_FILE=$conf_file
else
   SYSCONFIG_FILE=sysstat
fi
AC_MSG_RESULT($SYSCONFIG_FILE)
AC_SUBST(SYSCONFIG_FILE)

# National Language Support
AC_MSG_CHECKING(National Language Support)
AC_ARG_ENABLE(nls,
	      AS_HELP_STRING([--disable-nls],[disable National Language Support]),
			     AUX_NLS=$enableval,AUX_NLS=yes)
if test $AUX_NLS != "no" && test $HAVE_LIBINTL_H && test $HAVE_LOCALE_H; then
   AUX_NLS="yes"
   NLS="y"
else
   AUX_NLS="no"
   NLS="n"
fi
AC_MSG_RESULT($AUX_NLS)
AC_CHECK_LIB(intl, gettext, LFINTL="-lintl")
AC_SUBST(LFINTL)
LACKING_GETTEXT="n"
if test $AUX_NLS = "yes" &&  test x$MSGFMT != x"msgfmt"; then
   echo "WARNING: msgfmt command not found!"
   LACKING_GETTEXT="y"
fi
if test $AUX_NLS = "yes" &&  test x$XGETTEXT != x"xgettext"; then
   echo "WARNING: xgettext command not found!"
   LACKING_GETTEXT="y"
fi
if test $AUX_NLS = "yes" &&  test x$MSGMERGE != x"msgmerge"; then
   echo "WARNING: msgmerge command not found!"
   LACKING_GETTEXT="y"
fi
if test $NLS = "y" && test $LACKING_GETTEXT = "y"; then
   echo "WARNING: Disabling NLS"
   NLS="n"
fi
AC_SUBST(NLS)

# Link Time Optimization
AC_MSG_CHECKING(Link Time Optimization support)
AC_ARG_ENABLE(lto,
	      AS_HELP_STRING([--enable-lto],[enable Link Time Optimization]),
			     AUX_LTO=$enableval,AUX_LTO=no)
AC_MSG_RESULT($AUX_LTO)
LTO="n"
if test $AUX_LTO = "yes"; then
	AC_CHECK_PROG(GCC_AR, gcc-ar, gcc-ar)
	if test x$GCC_AR != x"gcc-ar"; then
		echo "WARNING: gcc-ar command not found! Disabling LTO support"
		AUX_LTO="no"
	else
		LTO="y"
	fi
fi
AC_SUBST(LTO)

# Data history to keep by sa2
AC_MSG_CHECKING(number of daily data files to keep)
AC_ARG_VAR([history],[number of daily data files to keep (default value is 7)])
if test x$history = x""; then
   HISTORY=7
else
   HISTORY=$history
fi
AC_MSG_RESULT($HISTORY)
AC_SUBST(HISTORY)

# Number of days after which datafiles are compressed
AC_MSG_CHECKING(number of days after which data files are compressed)
AC_ARG_VAR([compressafter],[number of days after which data files are compressed (default value is 10)])
if test x$compressafter = x""; then
   COMPRESSAFTER=10
else
   COMPRESSAFTER=$compressafter
fi
AC_MSG_RESULT($COMPRESSAFTER)
AC_SUBST(COMPRESSAFTER)

# Random delay to wait before sa2 script runs
AC_MSG_CHECKING(random delay to wait before sa2 script runs)
AC_ARG_VAR([delay_range],[maximum delay (in seconds) to wait before sa2 script generates its reports])
if test x$delay_range = x""; then
   DELAY_RANGE=0
else
   DELAY_RANGE=$delay_range
fi
AC_MSG_RESULT($DELAY_RANGE)
AC_SUBST(DELAY_RANGE)

# Manual page group
grep ^man: /etc/group >/dev/null 2>&1
if test $? = 0; then
   GRP=man
else
   GRP=root
fi

AC_MSG_CHECKING(group for manual pages)
AC_ARG_VAR([man_group],[group for manual pages])
if test x$man_group = x""; then
   MAN_GROUP=$GRP
   AC_MSG_RESULT($MAN_GROUP)
else
   grep ^$man_group: /etc/group >/dev/null 2>&1
   if test $? = 1; then
      MAN_GROUP=$GRP
      AC_MSG_RESULT($MAN_GROUP)
      echo "WARNING: Group ${man_group} not found: Using ${GRP} instead"
   else
      MAN_GROUP=$man_group
      AC_MSG_RESULT($MAN_GROUP)
   fi
fi
AC_SUBST(MAN_GROUP)

# Don't set attributes on files being installed?
AC_MSG_CHECKING(whether attributes should not be set on files being installed)
AC_ARG_ENABLE(file-attr,
	      AS_HELP_STRING([--disable-file-attr],[do not set attributes on files being installed]),
			     AUX_IMG=$enableval,AUX_IMG=yes)
if test $AUX_IMG != "no"; then
   IGNORE_FILE_ATTRIBUTES=n
   AUX_IMG=no
else
   IGNORE_FILE_ATTRIBUTES=y
   AUX_IMG=yes
fi
AC_MSG_RESULT($AUX_IMG)
AC_SUBST(IGNORE_FILE_ATTRIBUTES)

# Compress manual pages?
AC_MSG_CHECKING(whether manual pages should be compressed)
AC_ARG_ENABLE(compress-manpg,
	      AS_HELP_STRING([--disable-compress-manpg],[do not compress sysstat manual pages]),
			     AUX_MPG=$enableval,AUX_MPG=yes)
if test $AUX_MPG != "yes"; then
   COMPRESS_MANPG=n
else
   COMPRESS_MANPG=y
fi
AC_MSG_RESULT($AUX_MPG)
AC_SUBST(COMPRESS_MANPG)

# Check whether sa directory should be cleaned
AC_MSG_CHECKING(whether system activity directory should be cleaned)
AC_ARG_ENABLE(clean-sa-dir,
	      AS_HELP_STRING([--enable-clean-sa-dir],[clean system activity directory]),
			    AUX_CSD=$enableval,AUX_CSD=no)
if test $AUX_CSD != "yes"; then
   CLEAN_SA_DIR=n
   AUX_CSD=no
else
   CLEAN_SA_DIR=y
fi
AC_MSG_RESULT($AUX_CSD)
AC_SUBST(CLEAN_SA_DIR)

# Start crontab
AC_MSG_CHECKING(whether cron should start sar automatically)
AC_ARG_ENABLE(install-cron,
	      AS_HELP_STRING([--enable-install-cron],[install a crontab to start sar]),
			     INSTALL_CRON=$enableval,INSTALL_CRON=n)
if test $INSTALL_CRON != "yes"; then
   INSTALL_CRON=n
   AUX_CRON=no
else
   INSTALL_CRON=y
   AUX_CRON=yes
fi
AC_MSG_RESULT($AUX_CRON)
AC_SUBST(INSTALL_CRON)

# Crontab owner
CUSR="root"
if test $INSTALL_CRON = "y"; then
   AC_MSG_CHECKING(crontab owner)
   AC_ARG_VAR([cron_owner],[crontab owner])
   if test x$cron_owner = x""; then
      CRON_OWNER=$CUSR
      AC_MSG_RESULT($CRON_OWNER)
   else
      grep ^$cron_owner: /etc/passwd >/dev/null 2>&1
      if test $? = 1; then
         CRON_OWNER=$CUSR;
         AC_MSG_RESULT($CRON_OWNER)
         echo "WARNING: User ${cron_owner} not found: Using ${CUSR} instead."
      else
         CRON_OWNER=$cron_owner
         AC_MSG_RESULT($CRON_OWNER)
      fi
   fi
   echo "INFO: Crontab for ${CRON_OWNER} will be saved in current directory if necessary"
   if test $CRON_OWNER = "root"; then
      SU_C_OWNER=""
      QUOTE=""
      REM_CHOWN="# REM_CHOWN"
   else
      SU_C_OWNER="su $CRON_OWNER -c "
      QUOTE=\"
      # " (ignore this line)
      REM_CHOWN=$CHOWN
   fi

#  Check whether we should use the standard cron daemon
   AC_MSG_CHECKING(whether we should use the standard cron daemon)
   AC_ARG_ENABLE(use-crond,
	         AS_HELP_STRING([--enable-use-crond],[use standard cron daemon]),
			        UCROND=$enableval,UCROND=no)
   if test $UCROND != "yes"; then
      USE_CROND=n
      UCROND=no
   else
      USE_CROND=y
   fi
   AC_MSG_RESULT($UCROND)
   AC_SUBST(USE_CROND)

#  Crontab interval
   AC_MSG_CHECKING(crontab interval)
   AC_ARG_VAR([cron_interval],[crontab interval])
   if test x$cron_interval = x""; then
      CRON_INTERVAL=10
   else
      CRON_INTERVAL=$cron_interval
   fi
   AC_MSG_RESULT($CRON_INTERVAL)
   CRON_INTERVAL_SEC=`expr ${CRON_INTERVAL} \* 60`
   CRON_COUNT=`expr 60 / ${CRON_INTERVAL}`

#  Check whether sadc should collect all possible activities
   AC_MSG_CHECKING(whether sadc should collect all possible activities)
   AC_ARG_ENABLE(collect-all,
	         AS_HELP_STRING([--enable-collect-all],[collect all possible activities]),
			        COLLECT_ALL=$enableval,COLLECT_ALL=n)
   if test $COLLECT_ALL != "yes"; then
      COLLECT_ALL=""
      AUX_COLL=no
   else
      COLLECT_ALL="-S XALL"
      AUX_COLL=yes
   fi
   AC_MSG_RESULT($AUX_COLL)

   AC_MSG_CHECKING(options to be passed to sadc)
   AC_ARG_VAR([sadc_options],[options to be passed to sadc])
   if test x"$sadc_options" != x""; then
	SADC_OPT="$sadc_options"
   else
	SADC_OPT=
   fi
   AC_MSG_RESULT($SADC_OPT)
   AC_SUBST(SADC_OPT)

#  Check whether files should only be copied
   AC_MSG_CHECKING(whether files should only be copied)
   AC_ARG_ENABLE(copy-only,
	         AS_HELP_STRING([--enable-copy-only],[only copy files when installing]),
			        OCOPY=$enableval,OCOPY=no)
   if test $OCOPY != "yes"; then
      COPY_ONLY=n
      OCOPY=no
   else
      COPY_ONLY=y
   fi
   AC_MSG_RESULT($OCOPY)
   AC_SUBST(COPY_ONLY)

else
   CRON_OWNER="root"
   SU_C_OWNER=""
   QUOTE=""
   REM_CHOWN="# REM_CHOWN"
   CRON_INTERVAL=10
   CRON_INTERVAL_SEC=600
   CRON_COUNT=6
   COLLECT_ALL=""
fi
AC_SUBST(CRON_OWNER)
AC_SUBST(SU_C_OWNER)
AC_SUBST(CRON_INTERVAL)
AC_SUBST(CRON_INTERVAL_SEC)
AC_SUBST(CRON_COUNT)
AC_SUBST(QUOTE)
AC_SUBST(REM_CHOWN)
AC_SUBST(COLLECT_ALL)

# Check whether documentation should be installed
AC_MSG_CHECKING(whether documentation should be installed)
AC_ARG_ENABLE(documentation,
	      AS_HELP_STRING([--disable-documentation],[do not install documentation]),
			     AUX_DOC=$enableval,AUX_DOC=yes)
if test $AUX_DOC != "no"; then
   AUX_DOC="yes"
   INSTALL_DOC="y"
else
   AUX_DOC="no"
   INSTALL_DOC="n"
fi
AC_MSG_RESULT($AUX_DOC)
AC_SUBST(INSTALL_DOC)

# Set directory for installing manual pages (see comment in Makefile)
AC_SUBST(mandir)

# Check whether debug mode should be activated
AC_MSG_CHECKING(whether debug mode should be activated)
AC_ARG_ENABLE(debuginfo,
	      AS_HELP_STRING([--enable-debuginfo],[enable debug output (--debuginfo option)]),
			     WITH_DEBUG=yes ; DFLAGS="$DFLAGS -DDEBUG" , WITH_DEBUG=no)
AC_MSG_RESULT($WITH_DEBUG)
AC_SUBST(DFLAGS)
AC_SUBST(WITH_DEBUG)

# Check whether object files should be stripped
AC_MSG_CHECKING(whether object files should be stripped)
AC_ARG_ENABLE(stripping,
	      AS_HELP_STRING([--disable-stripping],[do not strip object files]),
			     AUX_STRIP=$enableval,AUX_STRIP=yes)
if test $AUX_STRIP != "no"; then
   AUX_STRIP="yes"
   STRIP="-s"
else
   AUX_STRIP="no"
   STRIP=
fi
AC_MSG_RESULT($AUX_STRIP)
AC_SUBST(STRIP)

# Create files
echo .
echo Now create files:
echo .
AC_CONFIG_FILES([sa1], [chmod +x sa1])  	# Permissions must be changed
AC_CONFIG_FILES([sa2], [chmod +x sa2])		# Permissions must be changed
AC_CONFIG_FILES([cron/crontab:cron/crontab.sample])	# File must be renamed
AC_CONFIG_FILES([sysstat.sysconfig])
AC_CONFIG_FILES([version.h:version.in])		# File must be renamed
AC_CONFIG_FILES([sysconfig.h:sysconfig.in])	# File must be renamed
AC_CONFIG_FILES([cron/sysstat.cron.daily])
AC_CONFIG_FILES([cron/sysstat.cron.hourly])
AC_CONFIG_FILES([cron/sysstat.crond])
AC_CONFIG_FILES([cron/sysstat.crond.sample.in:cron/sysstat.crond.in], [sed s/^/#/ cron/sysstat.crond.sample.in > cron/sysstat.crond.sample])
AC_CONFIG_FILES([sysstat], [chmod +x sysstat])	# Permissions must be changed
AC_CONFIG_FILES([sysstat.service])
AC_CONFIG_FILES([cron/sysstat-collect.service])
AC_CONFIG_FILES([cron/sysstat-collect.timer])
AC_CONFIG_FILES([cron/sysstat-summary.service])
AC_CONFIG_FILES([cron/sysstat-summary.timer])
AC_CONFIG_FILES([cron/sysstat-rotate.service])
AC_CONFIG_FILES([cron/sysstat-rotate.timer])
AC_CONFIG_FILES([cron/sysstat.sleep], [chmod +x cron/sysstat.sleep])	# Permissions must be changed
AC_CONFIG_FILES([man/sa1.8:man/sa1.in])		# File must be renamed
AC_CONFIG_FILES([man/sa2.8:man/sa2.in])		# File must be renamed
AC_CONFIG_FILES([man/sadc.8:man/sadc.in])	# File must be renamed
AC_CONFIG_FILES([man/sadf.1:man/sadf.in])	# File must be renamed
AC_CONFIG_FILES([man/sar.1:man/sar.in])		# File must be renamed
AC_CONFIG_FILES([man/sysstat.5:man/sysstat.in])	# File must be renamed
AC_CONFIG_FILES([man/iostat.1:man/iostat.in])	# File must be renamed
AC_CONFIG_FILES([man/cifsiostat.1:man/cifsiostat.in])	# File must be renamed
AC_CONFIG_FILES([tests/variables])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo "
   Sysstat version:		$PACKAGE_VERSION
   Installation prefix:		$prefix
   rc directory:		${RC_DIR}
   Init directory:		${INIT_DIR}"
if test "$UCROND" != "yes"; then
   echo "   Systemd unit dir:		${with_systemdsystemunitdir}"
   echo "   Systemd sleep dir:		${with_systemdsleepdir}"
else
   echo "   Use standard cron daemon"
fi
echo "   Configuration file:		${SYSCONFIG_DIR}/${SYSCONFIG_FILE}
   Man pages directory:		$mandir
   Compiler:			$CC
   Compiler flags:		$CFLAGS
   Linker flags:		$LDFLAGS
"
