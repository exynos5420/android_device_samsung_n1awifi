# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base.mk)

# Inherit from n1awifi device
$(call inherit-product, device/samsung/n1awifi/device.mk)

PRODUCT_NAME := full_n1awifi
PRODUCT_DEVICE := n1awifi
PRODUCT_BRAND := samsung
PRODUCT_MANUFACTURER := samsung
PRODUCT_MODEL := SM-P600

# Inherit some common ld stuff.
$(call inherit-product, vendor/ld/config/common_full_tablet_wifionly.mk)

PRODUCT_NAME := ld_n1awifi
PRODUCT_DEVICE := n1awifi
PRODUCT_MANUFACTURER := samsung
PRODUCT_GMS_CLIENTID_BASE := android-samsung

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRODUCT_MODEL=SM-P600 \
    PRODUCT_NAME=n1awifi \
    PRODUCT_DEVICE=n1awifi \
    TARGET_DEVICE=n1awifi \
    BUILD_FINGERPRINT="samsung/lt03wifiue/lt03wifiue:5.1.1/LMY47X/P600UEUDOK1:user/release-keys" \
    PRIVATE_BUILD_DESC="lt03wifiue-user 5.1.1 LMY47X P600UEUDOK1 release-keys"
