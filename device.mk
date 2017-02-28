#
# Copyright (C) 2013 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := device/samsung/n1awifi

PRODUCT_CHARACTERISTICS := tablet
DEVICE_PACKAGE_OVERLAYS += device/samsung/n1awifi/overlay

# Device uses high-density artwork where available
PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := xhdpi

# Boot animation
TARGET_SCREEN_HEIGHT := 1600
TARGET_SCREEN_WIDTH := 2560

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/audio/mixer_paths_0.xml:system/etc/mixer_paths_0.xml

# GPS
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/gps/gps.conf:system/etc/gps.conf \
    $(LOCAL_PATH)/configs/gps/gps.xml:system/etc/gps.xml

# GPS Shim
PRODUCT_PACKAGES += \
    libdmitry

# Touchscreen
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/idc/Synaptics_HID_TouchPad.idc:system/usr/idc/Synaptics_HID_TouchPad.idc \
    $(LOCAL_PATH)/configs/idc/Synaptics_RMI4_TouchPad_Sensor.idc:system/usr/idc/Synaptics_RMI4_TouchPad_Sensor.idc \
    $(LOCAL_PATH)/configs/idc/sec_e-pen.idc:system/usr/idc/sec_e-pen.idc \
    $(LOCAL_PATH)/configs/keylayout/sec_touchscreen.kl:system/usr/keylayout/sec_touchscreen.kl

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/tablet_core_hardware.xml:system/etc/permissions/tablet_core_hardware.xml

# Gello (CM browser)
PRODUCT_PACKAGES += \
    Gello

# Ramdisk
PRODUCT_PACKAGES += \
    fstab.universal5420 \
    init.target.rc \
    ueventd.universal5420.rc

# call dalvik heap and hwui config
$(call inherit-product, frameworks/native/build/phone-xxhdpi-3072-dalvik-heap.mk)

$(call inherit-product-if-exists, frameworks/native/build/phone-xxhdpi-3072-hwui-memory.mk)

# Call the proprietary setup
$(call inherit-product, vendor/samsung/n1awifi/n1awifi-vendor.mk)

# Import the common tree changes
include device/samsung/exynos5420-common/exynos5420.mk
