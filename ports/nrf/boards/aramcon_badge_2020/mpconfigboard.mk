USB_VID = 0x239A
USB_PID = 0x802A
USB_PRODUCT = "AramCon Badge 2020"
USB_MANUFACTURER = "CodeMagic"

MCU_CHIP = nrf52840

QSPI_FLASH_FILESYSTEM = 1
EXTERNAL_FLASH_DEVICE_COUNT = 1
EXTERNAL_FLASH_DEVICES = "W25Q128JV_SQ"

CIRCUITPY_DISPLAYIO = 1
FROZEN_MPY_DIRS += $(TOP)/frozen/Aramcon_Badge
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_BusDevice
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_NeoPixel
FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_Display_Text
#FROZEN_MPY_DIRS += $(TOP)/frozen/Adafruit_CircuitPython_IL0373
