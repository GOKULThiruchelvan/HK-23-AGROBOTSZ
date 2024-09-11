#!/system/bin/sh

# (c) 2018-2021 changes for blu_spark by eng.stk

# Wait to set proper init values
sleep 30

# Disable zram
swapoff /dev/block/zram0

# Set TCP congestion algorithm
echo "westwood" > /proc/sys/net/ipv4/tcp_congestion_control

# Tweak IO performance after boot complete
echo "zen" > /sys/block/sda/queue/scheduler
echo 512 > /sys/block/sda/queue/read_ahead_kb
echo 128 > /sys/block/sda/queue/nr_requests

# Input boost and stune configuration
echo "0:1056000 1:0 2:0 3:0 4:0 5:0 6:0 7:0" > /sys/module/cpu_boost/parameters/input_boost_freq
echo 500 > /sys/module/cpu_boost/parameters/input_boost_ms
echo 5 > /dev/stune/top-app/schedtune.boost

# Disable scheduler core_ctl
echo 0 > /sys/devices/system/cpu/cpu0/core_ctl/enable
echo 0 > /sys/devices/system/cpu/cpu4/core_ctl/enable

# Set min cpu freq
echo 576000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq
echo 825600 > /sys/devices/system/cpu/cpu4/cpufreq/scaling_min_freq

# Configure cpu governor settings
echo "schedutil" > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo 0 > /sys/devices/system/cpu/cpu0/cpufreq/schedutil/up_rate_limit_us
echo 20000 > /sys/devices/system/cpu/cpu0/cpufreq/schedutil/down_rate_limit_us
echo 0 > /sys/devices/system/cpu/cpu0/cpufreq/schedutil/hispeed_freq
echo 90 > /sys/devices/system/cpu/cpu0/cpufreq/schedutil/hispeed_load
echo 0 > /sys/devices/system/cpu/cpu0/cpufreq/schedutil/pl
echo 1 > /sys/devices/system/cpu/cpu0/cpufreq/schedutil/iowait_boost_enable
echo "schedutil" > /sys/devices/system/cpu/cpu4/cpufreq/scaling_governor
echo 0 > /sys/devices/system/cpu/cpu4/cpufreq/schedutil/up_rate_limit_us
echo 20000 > /sys/devices/system/cpu/cpu4/cpufreq/schedutil/down_rate_limit_us
echo 0 > /sys/devices/system/cpu/cpu4/cpufreq/schedutil/hispeed_freq
echo 90 > /sys/devices/system/cpu/cpu4/cpufreq/schedutil/hispeed_load
echo 0 > /sys/devices/system/cpu/cpu4/cpufreq/schedutil/pl
echo 0 > /sys/devices/system/cpu/cpu4/cpufreq/schedutil/iowait_boost_enable

# Disable swap file
swapoff /data/vendor/swap/swapfile

# Set zram config
echo 1 > /sys/block/zram0/reset
echo "lz4" > /sys/block/zram0/comp_algorithm
echo 2202009600 > /sys/block/zram0/disksize
mkswap /dev/block/zram0
swapon /dev/block/zram0 -p 32758

echo "Boot blu_spark completed " >> /dev/kmsg
