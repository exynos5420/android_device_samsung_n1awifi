$(call inherit-product, device/samsung/lt033g/full_lt033g.mk)

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

PRODUCT_NAME := cm_lt033g
PRODUCT_DEVICE := lt033g

PRODUCT_BUILD_PROP_OVERRIDES += \
    PRODUCT_MODEL=SM-P601 \
    PRODUCT_NAME=lt033g \
    PRODUCT_DEVICE=lt033g \
    TARGET_DEVICE=lt033g
