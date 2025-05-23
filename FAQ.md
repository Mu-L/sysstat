## Welcome to the sysstat's Frequently Asked Questions!

### Table of contents

**[1. General questions](#general)**

[1.1.](#1_1) When I compile sysstat, it fails with the following message: `make: msgfmt: Command not found`  
[1.2.](#1_2) When I try to compile sysstat, it fails and says it cannot find some include files.  
[1.3.](#1_3) I don't understand why sysstat displays the time sometimes as `HH:MM:SS` and sometimes as `HH:MM:SS AM/PM`...  
[1.4.](#1_4) What are the units actually used by sysstat commands?  
[1.5.](#1_5) Does sysstat still use odd/even numbers for development/stable versions?

**[2. Questions related to sar, sadc and sadf](#sar)**

[2.1.](#2_1) The sar command complains with the following message: `Invalid system activity file: ...`  
[2.2.](#2_2) The sar command complains with the following message: `Cannot append data to that file (...)`  
[2.3.](#2_3) The sar command complains with the following message: `Inconsistent input data`  
[2.4.](#2_4) I get the following error message when I try to run sar: `Cannot open /var/log/sa/sa30: No such file or directory`  
[2.5.](#2_5) Are sar daily data files fully compatible with Sun Solaris format sar files?  
[2.6.](#2_6) The `Average:` results from the sar command are just rubbish...  
[2.7.](#2_7) My database (e.g. MySQL) doesn't appear to understand the time zone displayed by 'sadf -d'...  
[2.8.](#2_8) I tried to use options -s and -e with sadf. Unfortunately, I have nothing displayed at all / the output doesn't match that of sar.  
[2.9.](#2_9) I cannot see all my disks when I use the sar -d command...  
[2.10.](#2_10) Do you know a tool which can graphically plot the data collected by sar?  
[2.11.](#2_11) When I launch sadc, I get the error message: `flock: Resource temporarily unavailable`  
[2.12.](#2_12) How should I run sysstat / sar so that I get a reading for `00:00:00`?  
[2.13.](#2_13) The sar command complains with the following message: `Requested activities not available in file ...`  
[2.14.](#2_14) Does sar need a lot of resources to run?  
[2.15.](#2_15) Are the measurements gathered by sadc cumulative or instantaneous?  
[2.16.](#2_16) Some fields are always displayed as `0.00` when I use the `sar -d` command.  
[2.17.](#2_17) The sar command complains with the following message: `Requested activities not available`  
[2.18.](#2_18) How can I keep sar data for more than one month?  
[2.19.](#2_19) How can I load sar data into an Oracle database for performance analysis and capacity planning?  
[2.20.](#2_20) The sar command displays some weird output values...  
[2.21.](#2_21) What happened to sar's options -h, -H, -x and -X?  
[2.22.](#2_22) What is the exact meaning of the `count` parameter for sar and sadc?  
[2.23.](#2_23) Why doesn't sar deal with sub-second sampling/monitoring?  
[2.24.](#2_24) Is it possible to make sadc save only some specific activities in my binary daily data files?  
[2.25.](#2_25) The sar and/or sadf command complain(s) with the following message: `End of system activity file unexpected`  

**[3. Questions related to iostat](#iostat)**

[3.1.](#3_1) I can't see all my disks when I use the iostat command...  
[3.2.](#3_2) iostat -x doesn't report disk I/O statistics...  
[3.3.](#3_3) Why can't iostat display extended statistics for partitions with 2.6.x kernels?  
[3.4.](#3_4) I don't understand the output of iostat. It doesn't match what I expect it to be...  
[3.5.](#3_5) Why values displayed by iostat are so different in the first report from those displayed in subsequent ones?  
[3.6.](#3_6) iostat -x displays huge numbers for some fields...  

**[4. Questions related to pidstat](#pidstat)**

[4.1.](#4_1) pidstat -d doesn't report task I/O statistics...  
[4.2.](#4_2) The pidstat command complains with the following message: `Requested activities not available`  
[4.3.](#4_3) pidstat doesn't display statistics for process (task) _xyz_...  
[4.4.](#4_4) I noticed that the total CPU utilization for threads running on an individual CPU can exceed 100%...  

---

### 1. General questions<a name="general"></a>

1.1.<a name="1_1"></a> When I compile sysstat, it fails with the following message:
```
make: msgfmt: Command not found
make: ***[locales] Error 127
```
A: The **msgfmt** command belongs to the GNU gettext package.
If you don't have it on your system, just configure sysstat with NLS disabled like this:
```
$ ./configure --disable-nls
```
or answer 'y' (for "yes") to the question
```
Disable National Language Support (NLS)? (y/n) [--disable-nls]
```
if you use the Interactive Configuration script (_iconfig_),
then compile sysstat as usual (make ; make install).

Please read the _README-nls_ file included in sysstat source package to learn
some more about National Language Support.

---
1.2.<a name="1_2"></a> When I try to compile sysstat, it fails and says it cannot find some include files:
```
In file included from /usr/include/bits/errno.h:25,
                 from /usr/include/errno.h:36,
                 from common.c:26:
/usr/include/linux/errno.h:4: asm/errno.h: No such file or directory
<SNIP>
common.c: In function `get_kb_shift':
common.c:180: `PAGE_SIZE' undeclared (first use in this function)
common.c:178: warning: `size' might be used uninitialized in this function
make: *** [common.o] Error 1
```
A: Make sure that you have the Linux kernel sources installed in
`/usr/src/linux`. Also make sure that the symbolic link exists in the
`/usr/src/linux/include` directory and points to the right architecture, e.g.:
```
# ll /usr/src/linux/include/asm
lrwxrwxrwx   1 root     root            8 May  5 18:31 /usr/src/linux/include/asm -> asm-i386
```
In fact, only the Linux kernel headers should be necessary to be able to compile sysstat.

---
1.3.<a name="1_3"></a> I don't understand why sysstat displays the time sometimes as `HH:MM:SS` and sometimes as `HH:MM:SS AM/PM`...
  
A: The time format used by sysstat tools depends on the locale of your system.
The locale is defined by several environment variables, among which the **LANG**
variable is perhaps the most widely used. See the following example:
```
$ export LANG=en_US
$ sar
Linux 2.4.9 (brooks.seringas.fr)        07/20/04

04:32:11 PM       LINUX RESTART

05:00:00 PM       CPU     %user     %nice   %system   %iowait     %idle
05:10:00 PM       all      0.24      0.00     89.64      0.00     10.12
Average:          all      0.24      0.00     89.64      0.00     10.12

$ export LANG=fr_FR
$ sar
Linux 2.4.9 (brooks.seringas.fr)        20.07.2004

16:32:11          LINUX RESTART

17:00:00          CPU     %user     %nice   %system   %iowait     %idle
17:10:00          all      0,24      0,00     89,64      0,00     10,12
Moyenne:          all      0,24      0,00     89,64      0,00     10,12
```
As you can notice, the time format but also the date, the decimal point, and
even some words (like "Average") have changed according to the specified locale.

---

1.4.<a name="1_4"></a> What are the units actually used by sysstat commands?


A: Although sysstat commands use the following abbreviations: kB, MB, etc.
as part of the metrics names (e.g. kB_read/s, wkB/s, or even sometimes with a
lower 'b': kbmemfree, kbavail...), we always actually refer to kibibytes (kiB),
mebibytes (MiB),...  
A kibibyte is equal to 1024 bytes, and a mebibyte is equal to 1024 kibibytes.  
Metrics names have been defined many years ago. We don't modify them to avoid
breaking third-party programs parsing sysstat commands' output.

---

1.5.<a name="1_5"></a> Does sysstat still use odd/even numbers for development/stable
versions?


No, starting with sysstat versions 12.7.x, sysstat no longer uses odd and even version
numbers to identify development and stable series. The latest sysstat release should
always be considered as a stable version that can be used for distribution packaging.

---
### 2. Questions related to sar, sadc and sadf<a name="sar"></a>

2.1.<a name="2_1"></a> The sar command complains with the following message:
```
Invalid system activity file: ...
```
A: You are trying to use a file which is not a system activity file, or whose
format is no longer compatible with that of files created by current version of sar.
If you were trying to use the standard system activity files located in the
`/var/log/sa` directory then the solution is easy: just log in as root and
remove by hand all the files located in the `/var/log/sa` directory:
```
# rm /var/log/sa/sa??
```
If you are using sysstat 11.1.1 and later, you can also use the sadf command
to convert an old system activity binary datafile (version 9.1.6 and later) to
current up-to-date format. Use the following syntax:
```
$ sadf -c old_datafile > new_datafile
```
Note: Starting with sysstat version 8.1.1 and later, it is possible to
know which version of sar or sadc has been used to create a data file.
Just enter the following command:
```
$ sadf -H /your/datafile | grep sysstat
File created using sar/sadc from sysstat version 8.1.7
```

---
2.2.<a name="2_2"></a> The sar command complains with the following message:
```
Cannot append data to that file (...)
```
A: The internal structure of the data file does not allow sar to append
data to it. The data file may come from another machine, or the components
of the current box, such as the number of processors, may have changed.
Use another data file, or delete the current daily data file, and try again.  
With sysstat version 10.1.3 and later, it is now possible to append data
to a data file even if the number of processors has changed. But first,
you have to make the data file aware of this change by inserting a restart
mark (this is typically done when sadc is called at system restart). You
can then append data to the data file as usual.

---
2.3.<a name="2_3"></a> The sar command complains with the following message:
```
Inconsistent input data
```
A: This error message means that sadc (the system activity data collector that
sar is using) is not consistent with the sar command. In most cases this is
because the sar and sadc commands do not belong to the same release of the
sysstat package. Remember that sar may search for sadc in predefined
directories (`/usr/local/lib/sa`, `/usr/lib/sa`, ...) before looking in the
current directory!  
With sysstat version 11.1.5 and later, enter `sar --sadc`
to determine which data collector is called by sar.

---
2.4.<a name="2_4"></a> I get the following error message when I try to run sar:
```
Cannot open /var/log/sa/sa30: No such file or directory
```
A: Please read the sar(1) manual page! Daily data files are created by default
in the `/var/log/sa` directory using the data collector (sadc) or using
option -o with sar. Once they are created, sar can display statistics
saved in those files.  
But sar can also display statistics collected "on the fly": Just enter
the proper options on the command line to indicate which statistics are
to be displayed, and also specify `interval` and `count` numbers, e.g.:
```
# sar 2 5       --> will report CPU utilization every two seconds, five times.
# sar -n DEV 3  --> will report network device utilization every 3 seconds, in an infinite loop.
```

---
2.5.<a name="2_5"></a> Are sar daily data files fully compatible with Sun Solaris format
sar files?


A: No, the format of the binary data files created by sysstat's sar command
is not compatible with formats from other Unixes, because it contains
data which are closely linked to Linux.  
For the same reason, sysstat cannot work on platforms other than Linux...

---
2.6.<a name="2_6"></a> The `Average:` results from the sar command are just rubbish, e.g.:
```
 11:00:00 AM       CPU     %user     %nice   %system     %idle
 11:10:00 AM       all      0.54      0.00      0.89     98.57
 11:20:01 AM       all      3.02      8.05     22.85     66.08
 11:30:01 AM       all      8.15      0.00      2.31     89.54
 11:40:01 AM       all      8.03      0.00      2.42     89.55
 11:50:01 AM       all     16.04      0.00      2.81     81.16
 12:00:00 PM       all     21.11      0.00      3.23     75.66
 03:40:01 PM       all    100.01    100.01    100.01      0.00
 04:40:00 PM       all    100.00      0.00    100.00      0.00
 04:50:00 PM       all      5.87      0.00      1.26     92.87
 05:00:00 PM       all      4.70      0.00      1.48     93.82
 05:10:00 PM       all      4.93      0.00      1.29     93.78
 Average:          all    100.22    100.20    100.13      0.00
```
A: Your sar command was not installed properly. Whenever your computer
is restarted (as it is the case here between `12:00:00 PM` and `03:40:01 PM`),
the `sysstat` shell script must be called by the system, so that the
LINUX RESTART message can be inserted into the daily data file, indicating
that the relevant kernel counters have been reinitialized...  
You can install the `sysstat` script by hand in the relevant startup
directory, or you can ask sysstat to do it for you during configuration
stage by entering:
```
$ ./configure --enable-install-cron
```
Or you can answer 'y' to the question:
```
Set crontab to start sar automatically? (y/n) [--enable-install-cron]
```
if you use the Interactive Configuration script (iconfig).  
Then compile sysstat as usual and run 'make install' as the last stage.

---
2.7.<a name="2_7"></a> My database (e.g. MySQL) doesn't appear to understand the time zone
displayed by 'sadf -d'...


A: The format includes the timezone detail in the output. This is to make
sure it is communicated clearly that UTC is how the data is always
converted and printed. Moreover we don't depend on the TZ environment
variable and we don't have some data converted to a different timezone
for any reason, known or unknown.  
When you deal with raw accounting data you always want it in UTC.
Of course, you want it to all be the same when loading into a database.
If your database can't deal with timezones, you should write a short script
to strip the "UTC" characters from the data being loaded into the database.

---
2.8.<a name="2_8"></a> I tried to use options -s and -e with sadf. Unfortunately, I have
nothing displayed at all / the output doesn't match that of sar.


A: The way how options -s and -e are interpreted has changed with sysstat's
versions.
First if you don't have any data displayed by sadf, this is because no data
belong to the specified time interval! Up to sysstat version 12.1.4, the
time specified with options -s and -e was always considered as being given
in local time to be consistent with sar's default output. Yet sadf displays
its timestamps in UTC (Coordinated Universal Time) by default (and in local
time with option -T). This could lead to some misunderstandings, as if sadf's
options -s and -e didn't work properly.
So with sysstat version 12.1.5, the time specified with options -s and -e
is now consistent with the timestamps displayed by sadf (either in UTC by
default or in local time with option -T), even if the output doesn't match
that of sar.

---
2.9.<a name="2_9"></a> I cannot see all my disks when I use the sar -d command...


A: See question "I can't see all my disks when I use the iostat command" below.

---
2.10.<a name="2_10"></a> Do you know a tool which can graphically plot the data collected by sar?


A: You can now draw graphs with sysstat's standard tools!
SVG (Scalable Vector Graphics) is a new output format that has been added to
sadf in sysstat version 11.3.1. Read sadf(1) manual page to learn some more
about this new format.  
There are other tools lying around on the internet that you can use to draw
some graphs. I haven't tested all of them and there must still be some
way for improvement...  First, some of them are included in the sysstat
package although they are no longer maintained: isag (a Perl script) or
sargraph (a shell script).
You can also find: kSar (a Java application capable of visualizing a sar file
with static graphs), sarjitsu (a more sophisticated application producing
dynamic visualizations based on Grafana), sarvant, sar2gp, loadgraph,
SysStat Charts, sarplot...
[rrd.cgi](http://haroon.easi.utoronto.ca/rrd/scripts/) is a perl front-end for
rrdtool and can be used to make some graphs (see a demo [here](http://haroon.easi.utoronto.ca/perl/rrd.cgi/sar_stats/)).
[sysstat_mail_report](https://github.com/desbma/sysstat_mail_report) is a script
that automatically generates and sends an email report every day/week/month
with graphs generated from sysstat data.  
I've also heard of commercial tools which use sysstat: PerfMan comes to mind,
among others.

---
2.11.<a name="2_11"></a> When I launch sadc, I get the error message:
```
flock: Resource temporarily unavailable
```
A: You are launching sadc using -L option. With this option, sadc tries to
get an exclusive lock on the output file. The above error message indicates
that another sadc process was running and had already locked the same output
file. Stop all sadc instances and try again.

---
2.12.<a name="2_12"></a> I have sysstat setup to run via cron:
```
0 * * * * /usr/local/lib/sa/sa1 600 6
```
so that I get an activity report every 10 minutes.  
When I use sar to get my output, there is no reading for `00:00:00`. This
means that at midnight every night there is a spike, or dip, in the graphs.
How should I run sysstat / sar so that I get a reading for `00:00:00`?


A: Sysstat does get its data at midnight, but two data samples are needed to
display the values.  
When there is a "file rotation" (beginning of a new day), sadc writes its data
at the end of the previous daily data file (`/var/log/sa/sa<DD>`) **and** at the
beginning of the new one (`/var/log/sa/sa<DD+1>`). Please note that '-' must be
used to specify the output file for sadc to be able to detect such a file
rotation. So a crontab like the following one should enable you to get the
data for midnight at the end of each daily data file:
```
# Activity reports every 10 minutes from 01:00:00 to 22:50:00
0 1-22 * * * /usr/local/lib/sa/sa1 600 6
# Activity reports every 10 minutes from 23:00:00 to 00:00:00
# Reporting until 00:00:00 ensures that a file rotation will be detected
# by sadc
0 23 * * * /usr/local/lib/sa/sa1 600 7
# Activity reports every 10 minutes from 00:10:00 to 00:50:00
10 0 * * * /usr/local/lib/sa/sa1 600 5
```
Another possible crontab would be:
```
*/10 1-22 * * * /usr/lib/sa/sa1 1 1
0,10,20,30,40 23 * * * /usr/lib/sa/sa1 1 1
50 23 * * * /usr/lib/sa/sa1 600 2
10,20,30,40,50 0 * * * /usr/lib/sa/sa1 1 1
```
Things are much easier with recent sysstat versions (12.5.1 and later): You simply have to run
sa1 with its option `--rotate` shortly after midnight to add a statistics record to the system
activity daily data file of the previous day. So your full crontab could be:
```
# Rotate file at midnight
0 0 * * * /usr/lib/sa/sa1 --rotate
# Run system activity accounting tool every 10 minutes
0,10,20,30,40,50 * * * * /usr/lib/sa/sa1 1 1
# Generate a text summary of previous day process accounting at 00:07
7 0 * * * /usr/lib/sa/sa2 -A
```

---
2.13.<a name="2_13"></a> The sar command complains with the following message:
```
Requested activities not available in file ...
```
A: This error message means that you are trying to extract non-existent activities
from the data file. Usually sadc reads all the available activities from the
system and stores them in the data file. However, to prevent data files from
taking too much space on disk, some activities must be explicitly set on the
command line to be read by sadc.  
To tell sadc that an optional activity should be collected, use switch -S
followed by the keyword corresponding to that activity (see sadc(8) manual page).
As of this writing, optional activities are: interrupts, disks, SNMP, IPv6 and
power management statistics.  
IMPORTANT NOTE: The list of activities that are saved in a file can no longer
be modified once the file has been created. So it is important to use the proper
options the first time sadc is called (whether via a crontab, a script like
sa1 or even the script used to insert a RESTART message when the machine is
rebooted).  
NB: If the sar command complains with the error message:
```
Requested activities not available
```
(without mentioning `in file`), then see question 2.17 below.

---
2.14.<a name="2_14"></a> Does sar need a lot of resources to run?


A: No, sar doesn't need a lot of CPU to run, nor does it make your system slow,
contrary to what some people think. In the first place, it only runs every ten
minutes by default. Secondly, when it does run, it is over and done very
quickly. Try:
```
$ time /usr/lib/sa/sa1
```
to verify that for yourself.  
Nor do you have to be concerned about using up all your disk space.
sar will use a few hundred kilobytes for a whole day's worth of data, and it
normally only stores one week worth (this can be configured via the HISTORY
variable in the `/etc/sysconfig/sysstat` file). It is entirely self limiting.
Moreover, you can ask sar to compress its datafiles older than a certain
number of days: see the COMPRESSAFTER parameter in the `/etc/sysconfig/sysstat`
configuration file.

---
2.15.<a name="2_15"></a> Are the measurements gathered by sadc cumulative or instantaneous values?


A: Each counter maintained by the kernel is cumulative since system boot. As a
consequence the measurements gathered by sadc are cumulative values.
Moreover all per-second statistics displayed by sar are average values on the
given time interval. So the value for counter foo at time T is calculated as:
```
foo/s = [foo(T) - foo(T-dt)] / dt
```
where dt is the interval given on the command line.

---
2.16.<a name="2_16"></a> Some fields are always displayed as 0.00 when I use the sar -d
command.


A: See question 3.2 below.

---
2.17.<a name="2_17"></a> The sar command complains with the following message:
```
Requested activities not available
```
A: This error message means that you are trying to display activities that the
kernel itself is unable to provide.  
When this error message is displayed while trying to save the data into an
existing file (`sar -o datafile ...`), this may also be because the target
file cannot accept the requested activities. In this case, just try to use
another file or create a new one. See also question 2.13 above.

---
2.18.<a name="2_18"></a> How can I keep sar data for more than one month?


A: By default sar saves its data in the standard system activity data file,
the `/var/log/sa/saDD` file, where DD is the current day in the month.
To prevent sar from overwriting any existing files, just set the variable
HISTORY in `/etc/sysconfig/sysstat` to the number of days during which data
must be kept. When this variable has a value greater than 28, sa1 script
uses a month-by-month directory structure; datafiles are named `YYYYMM/saDD`
and the script maintains links to these datafiles to mimic the standard
sar datafile structure. However please note that pre-existing datafiles
will be deleted as links will be created and replace them.
Beginning with sysstat version 11.0.0, this tree of directories is no
longer created. When HISTORY has a value greater than 28, sa1 now calls
sadc with option -D set, telling it to use `saYYYYMMDD` instead of `saDD`
as the name for the standard system activity daily data files, where
YYYY stands for the current year, MM for the current month and DD for
the current day. All these files are saved in the same directory
(`/var/log/sa` by default).

---
2.19.<a name="2_19"></a> How can I load sar data into an Oracle database for performance
analysis and capacity planning?


A: The simplest way for that is to use sadf (a command included in sysstat
package) with its option -d. It displays sar data in a format that can
easily be ingested by a relational database system (fields are separated
by a semicolon). It should then be easy for a tool like SQL*Loader to
load the data into the Oracle database.

---
2.20.<a name="2_20"></a> The sar command displays some weird CPU values, e.g.:
```
10:50:01 AM       CPU     %user     %nice   %system   %iowait     %idle
11:00:01 AM       all     90.90      0.00      5.17      3.93      0.00
11:00:01 AM         0     39.40      0.00      2.37      2.07     56.17
11:00:01 AM         1     29.71      0.00      1.73      1.17     67.39
11:00:01 AM         2     42.69      0.00      2.34      1.11     53.85
11:00:01 AM         3     26.24      0.00      1.41      1.61     70.74
...
```
A: Sysstat may have met an overflow condition while reading CPU usage from
your /proc/stat file. This condition is all the more likely to happen as
your machine uptime is high and/or there are many processors.
Sysstat up to version 5.0.6 uses 32-bit integer variables to store CPU usage.
Then, beginning with version 5.1.1, sysstat has shifted to 64-bit variables,
which has fixed the problem. So try to upgrade your version of sysstat to
the latest release and check that the problem has gone.
Also see question 2.6 above.

---
2.21.<a name="2_21"></a> What happened to sar's options -h, -H, -x and -X?


A: These old options have been removed from sar because new commands have been
made available. You should now use the sadf command instead of sar -h or
sar -H, and the pidstat command instead of sar -x or sar -X. Please read
their manual page to learn some more about their respective options.

---
2.22.<a name="2_22"></a> What is the exact meaning of the `count` parameter for sar and sadc?


A: For sadc, `count` is the number of data samples collected.  
For sar, `count` is the number of records to display (a record contains
the average values for counters over the given time interval - See 2.15).


Starting with an empty file `datafile`:
```
sadc datafile 1 6         will write 6 data samples to datafile.
sar -f datafile 1 6       6 is invalid because there are only 5 intervals.
```
Based on the `count` value entered for sadc the "valid" `count` values for
sar are 1 through 5. Any value greater than 5 for sar will give the
same output as 5 in this example. So entering `sar -f datafile 1 2000`
for a file populated with the output of `sadc 1 6 datafile` will give the
same output as `sar -f datafile 1 5`. Note that it all depends on the number
of data samples pre-existing in the data file. If the file is empty
when first running sadc then the above is true.

---
2.23.<a name="2_23"></a> Why doesn't sar deal with sub-second sampling/monitoring?


A: There are two reasons for sar to not handle sub-second intervals:


1) This is not sar's purpose. sar has been created to give the
sys admin a global overview of its machine daily utilization so
that when a problem happens, he has a benchmark and can compare
the statistics gathered by sar with those saved before. For that
reason an interval of 10 minutes (which is the default for sar) is
quite appropriate.


2) Because this is just a dumb idea to try to gather a huge amount
of data on a sub-second interval basis (and sar really collects
a lot of data). This can be resource-consuming and you are all the
more prone to have an influence on the data you are retrieving as
the interval of time is small.

---
2.24.<a name="2_24"></a> Is it possible to make sadc save only some specific activities
in my binary daily data files?


A: sadc's option -S followed by one or more keywords (DISK, SNMP...)
can already be used to specify which optional activities are to be
collected. Without this option, sadc collects a default set of
activities (CPU activity, memory activity, network activity, etc.)  
Yet it is actually possible to specify explicitly which activities
should be collected by sadc! You have to use sadc's option -S
followed by the report name corresponding to the activity you want
to collect (enter "sar --help" to know the formal report names
used by sadc).  
Example: To tell sadc to collect only temperature sensors activity
in addition to the default set of activities, enter:

```
sadc -S A_PWR_TEMP (...)
```

followed by the other classic options (interval, count, filename...)
Now assume you want to collect temperature sensors activity **without**
the other activities collected by default,
add the special report name A_NULL to the list passed to sadc, e.g.:
```
sadc -S A_NULL,A_PWR_TEMP (...)
```
Of course you can enter as many report names as you want to collect
different activities.
Last you can exclude a specific activity from a list by prefixing its
report name with a dash. For example, to collect all possible activities
**except** statistics for interrupts, enter:
```
sadc -S XALL,-A_IRQ (...)
```
This way you can tell sadc to collect only the desired activities.

---
2.25.<a name="2_25"></a> The sar and/or sadf command complain(s) with the following message:
```
End of system activity file unexpected
```
A: sadc, the data collector, was unable to write all its data to the
system activity data file (`/var/log/sa/saDD` by default).
This is probably because there was no space left on the device where
the data file is located.
Make sure there is enough free space on the device. Sometimes it seems
there is enough free space but there may be some jobs run by crontab
(particularly during the night) that can temporarily consume all of
your free space, making sadc fail.

Another reason could be linked to the system activity daily data file
being corrupted. This could possibly happen when several instances of
sadc are trying to update the same data file, especially around midnight
when making a file rotation. See question 2.12 to know how to make such
a file rotation properly.

Last this issue can also be triggered when the system is rebooted forcibly
without data in cache being written to disk. You can use switch -f with sadc
to make sure data are written to disk immediately. Of course sync'ing each
data sample to disk implies a (probably slight) performance penalty.

---
### 3. Questions related to iostat<a name="iostat"></a>

3.1.<a name="3_1"></a> I can't see all my disks when I use the iostat command...


A: Yes. This is a kernel limit. Old kernels (2.2.x for instance) used to
maintain stats for the first four devices.  
The accounting code has changed in 2.4 kernels, and the result may (or
may not) be better for your system. Indeed, Linux 2.4 maintains the stats
in a two dimensional array, with a maximum of 16 devices (DK_MAX_DISK
in the kernel sources). Moreover, if the device major number exceeds
DK_MAX_MAJOR (whose value also defaults to 16 in the kernel sources),
then stats for this device will not be collected.  
So, a solution may be simply to change the values of these limits in
linux/include/linux/kernel_stat.h and recompile your kernel.
You should no longer have any problem with post 2.5 kernels, since
statistics are maintained by the kernel for all the devices.
In the particular case of iostat, also be sure to use the ALL keyword
on the command line to display statistical information relating to
every device, including those that are defined but have never been used
by the system.

---
3.2.<a name="3_2"></a> iostat -x doesn't report disk I/O statistics...


A: For `iostat -x` to be able to report extended disk I/O statistics,
it is better to use a recent version of the Linux kernel (2.6.x).
Indeed, iostat tries to read data from the `/proc/diskstats` file or
from the sysfs filesystem for that.  
But iostat may also be able to display extended statistics with
older kernels (e.g. 2.4.x) providing that all the necessary
statistical information is available in the /proc/partitions file,
which requires that a patch be applied to the kernel (this is
often done on kernels included in various distros). In recent 2.4.x
kernels, the /proc/partitions file has all the necessary data
providing that the kernel has been compiled with CONFIG_BLK_STATS=y.

---
3.3.<a name="3_3"></a> Why can't iostat display extended statistics for partitions with
some 2.6.x kernels?


A: Because the kernel maintains these stats only for devices, and not for
partitions! Here is an excerpt from the document iostats.txt included in
the kernel source documentation:
```
There were significant changes between 2.4 and 2.6 in the I/O subsystem.
As a result, some statistic information disappeared. The translation from
a disk address relative to a partition to the disk address relative to
the host disk happens much earlier.  All merges and timings now happen
at the disk level rather than at both the disk and partition level as
in 2.4.  Consequently, you'll see a different statistics output on 2.6 for
partitions from that for disks.
```
Extended I/O statistics for partitions are available again with kernels
2.6.25 and later.

---
3.4.<a name="3_4"></a> I don't understand the output of iostat. It doesn't match what I expect it
to be...


A: By default iostat displays I/O activity in blocks per second. With old
kernels (i.e. older than 2.4.x) a block is of indeterminate size and therefore
the displayed values are not useful.  
With recent kernels (kernels 2.4 and later), iostat is now able to get disk
activities from the kernel expressed in a number of sectors. If you take a
look at the kernel code, the sector size is actually allowed to vary although
I have never seen anything other than 512 bytes.

---
3.5.<a name="3_5"></a> Why values displayed by iostat are so different in the first report
from those displayed in subsequent ones?


A: Probably because, as written in the manual page, the first report generated
by iostat concerns the time since system startup, whereas subsequent ones
cover only the time since the previous report (that is to say, the interval
of time entered on the command line).

---
3.6.<a name="3_6"></a> iostat -x displays huge numbers for some fields...


A: Because of a Linux kernel bug, iostat -x may display huge I/O response times
(svctm) and a bandwidth utilization (%util) of 100% for some devices. Indeed
these devices have a value for the field #9 (beginning after the device name)
in `/proc/{partitions,diskstats}` which is always different from 0, and even
negative sometimes. Yet this field should go to zero, since it gives the
number of I/Os currently in progress (it is incremented as requests are
submitted, and decremented as they finish).
To (temporarily) fix the problem, you should reboot your system to reset the
counters in `/proc/{partitions,diskstats}`.

---
### 4. Questions related to pidstat<a name="pidstat"></a>

4.1.<a name="4_1"></a> pidstat -d doesn't report task I/O statistics...


A: For pidstat -d to be able to report I/O statistics for tasks, you need
a recent Linux kernel (2.6.20 or later) with the option
CONFIG_TASK_IO_ACCOUNTING compiled in.

---
4.2.<a name="4_2"></a> The pidstat command complains with the following message:
```
Requested activities not available
```
A: This message is displayed when the pidstat command is unable to display
the statistics you have requested. This may happen when you try to display
statistics for child processes (option -T CHILD) because all options of
pidstat don't necessarily work for child processes. Read the manual page
to know which statistics are available for child processes.

---
4.3.<a name="4_3"></a> pidstat doesn't display statistics for process (task) _xyz_...


A: This must be because pidstat only displays statistics for active tasks
by default. If you don't use option -p on the command line, then pidstat
will display only tasks with non-zero statistics. For example, `pidstat -u`
will display only tasks that have actually used some CPU resources since
system startup. You should enter `pidstat -u -p ALL` to make sure that all
the processes are listed in the report.

---
4.4.<a name="4_4"></a> I noticed that the total CPU utilization for threads running on
an individual CPU can exceed 100%...


A: The CPU number displayed by pidstat is the CPU to which the task is attached
when the statistics are actually displayed. This doesn't mean that the task
has spent its whole interval of time attached to it. Hence the CPU ressource
used by a thread on an interval of time as displayed by pidstat may have
concerned several processors.

---
Sebastien Godard (sysstat at orange dot fr) is the author and the current
maintainer of this package.

