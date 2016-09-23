#Inherit device configuration
$(call inherit-product, device/samsung/n1awifi/full_n1awifi.mk)

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/aosp_base.mk)

PRODUCT_NAME := aosp_n1awifi
PRODUCT_DEVICE := n1awifi

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRODUCT_MODEL=SM-P600 \
    PRODUCT_NAME=n1awifi \
    PRODUCT_DEVICE=n1awifi \
	TARGET_DEVICE=n1awifi
