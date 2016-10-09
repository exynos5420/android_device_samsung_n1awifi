#Inherit device configuration
$(call inherit-product, device/samsung/n1awifi/full_n1awifi.mk)

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/aosp_base.mk)

# Inherit from our custom product configuration
$(call inherit-product, vendor/du/config/common_full_tablet_wifionly.mk)

PRODUCT_NAME := du_n1awifi
PRODUCT_DEVICE := n1awifi
PRODUCT_BRAND := samsung
PRODUCT_MODEL := SM-P600
PRODUCT_MANUFACTURER := samsung

$(call inherit-product, device/samsung/n1awifi/device.mk)
$(call inherit-product-if-exists, vendor/samsung/n1awifi/device-vendor.mk)
