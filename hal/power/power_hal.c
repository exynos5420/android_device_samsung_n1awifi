/*
 * Copyright (c) 2014 The Android Open Source Project
 * Copyright (c) 2014 Christopher N. Hesse <raymanfx@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define LOG_TAG "Exynos5420PowerHAL"
/* #define LOG_NDEBUG 0 */
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#define BOOSTPULSE_PATH "/sys/devices/system/cpu/cpufreq/interactive/boostpulse"
#define TOUCHSCREEN_PATH "/sys/class/input/input1/enabled"

struct exynos5420_power_module {
    struct power_module base;
    pthread_mutex_t lock;
    int boostpulse_fd;
    int boostpulse_warned;
    const char *touchscreen_power_path;
};

static void sysfs_write(const char *path, char *s)
{
    char errno_str[64];
    int len;
    int fd;

    fd = open(path, O_WRONLY);
    if (fd < 0) {
        strerror_r(errno, errno_str, sizeof(errno_str));
        ALOGE("Error opening %s: %s\n", path, errno_str);
        return;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, errno_str, sizeof(errno_str));
        ALOGE("Error writing to %s: %s\n", path, errno_str);
    }

    close(fd);
}

static void init_touchscreen_power_path(struct exynos5420_power_module *exynos5420_pwr)
{
    exynos5420_pwr->touchscreen_power_path = TOUCHSCREEN_PATH;
}

/*
 * This function performs power management setup actions at runtime startup,
 * such as to set default cpufreq parameters.  This is called only by the Power
 * HAL instance loaded by PowerManagerService.
 */
static void exynos5420_power_init(struct power_module *module)
{
    struct exynos5420_power_module *exynos5420_pwr = (struct exynos5420_power_module *) module;

    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/multi_enter_load", "360");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/multi_enter_time", "80000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/multi_exit_load", "360");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/multi_exit_time", "320000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/single_enter_load", "90");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/single_enter_time", "160000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/single_exit_load", "90");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/single_exit_time", "80000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/param_index", "0");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/timer_rate", "20000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/timer_slack", "20000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/min_sample_time", "99000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/hispeed_freq", "1400000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/go_hispeed_load", "85");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/target_loads", "70 600000:70 800000:75 1500000:80 1700000:90");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/above_hispeed_delay", "20000 1000000:80000 1200000:100000 1700000:20000");
    sysfs_write("/sys/devices/system/cpu/cpufreq/interactive/boostpulse_duration", "40000");

    init_touchscreen_power_path(exynos5420_pwr);
}

/* This function performs power management actions upon the system entering
 * interactive state (that is, the system is awake and ready for interaction,
 * often with UI devices such as display and touchscreen enabled) or
 * non-interactive state (the system appears asleep, display usually turned
 * off).  The non-interactive state is usually entered after a period of
 * inactivity, in order to conserve battery power during such inactive periods.
 */
static void exynos5420_power_set_interactive(struct power_module *module, int on)
{
    struct exynos5420_power_module *exynos5420_pwr = (struct exynos5420_power_module *) module;

    ALOGV("power_set_interactive: %d\n", on);

    sysfs_write(exynos5420_pwr->touchscreen_power_path, on ? "1" : "0");

    ALOGV("power_set_interactive: %d done\n", on);
}

static int boostpulse_open(struct exynos5420_power_module *exynos5420_pwr)
{
    char errno_str[64];

    pthread_mutex_lock(&exynos5420_pwr->lock);

    if (exynos5420_pwr->boostpulse_fd < 0) {
        exynos5420_pwr->boostpulse_fd = open(BOOSTPULSE_PATH, O_WRONLY);
        if (exynos5420_pwr->boostpulse_fd < 0) {
            if (!exynos5420_pwr->boostpulse_warned) {
                strerror_r(errno, errno_str, sizeof(errno_str));
                ALOGE("Error opening %s: %s\n", BOOSTPULSE_PATH, errno_str);
                exynos5420_pwr->boostpulse_warned = 1;
            }
        }
    }

    pthread_mutex_unlock(&exynos5420_pwr->lock);
    return exynos5420_pwr->boostpulse_fd;
}

/*
 * This functions is called to pass hints on power requirements, which may
 * result in adjustment of power/performance parameters of the cpufreq governor
 * and other controls.
 */
static void exynos5420_power_hint(struct power_module *module, power_hint_t hint,
                             void *data)
{
    struct exynos5420_power_module *exynos5420_pwr = (struct exynos5420_power_module *) module;
    char errno_str[64];
    int len;

    switch (hint) {
        case POWER_HINT_INTERACTION:
            if (boostpulse_open(exynos5420_pwr) >= 0) {
                len = write(exynos5420_pwr->boostpulse_fd, "1", 1);

                if (len < 0) {
                    strerror_r(errno, errno_str, sizeof(errno_str));
                    ALOGE("Error writing to %s: %s\n", BOOSTPULSE_PATH, errno_str);
                }
            }

            break;

        case POWER_HINT_VSYNC:
            break;

        default:
            break;
    }
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

struct exynos5420_power_module HAL_MODULE_INFO_SYM = {
    base: {
        common: {
            tag: HARDWARE_MODULE_TAG,
            module_api_version: POWER_MODULE_API_VERSION_0_2,
            hal_api_version: HARDWARE_HAL_API_VERSION,
            id: POWER_HARDWARE_MODULE_ID,
            name: "EXYNOS5420 Power HAL",
            author: "The Android Open Source Project",
            methods: &power_module_methods,
        },

        init: exynos5420_power_init,
        setInteractive: exynos5420_power_set_interactive,
        powerHint: exynos5420_power_hint,
    },

    lock: PTHREAD_MUTEX_INITIALIZER,
    boostpulse_fd: -1,
    boostpulse_warned: 0,
};
