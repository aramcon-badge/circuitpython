from aramcon_badge.badgeio import badge
import displayio
from adafruit_display_text.label import Label
import terminalio
import time

display = badge.display

def run():
    frame = displayio.Group(max_size=16)

    background = displayio.Bitmap(badge.display.width, badge.display.height, 1)
    background_palette = displayio.Palette(1)
    background_palette[0] = 0xffffff
    background_sprite = displayio.TileGrid(background,pixel_shader=background_palette)
    frame.append(background_sprite)

    image = displayio.OnDiskBitmap(open("assets/banner.bmp", "rb"))
    grid = displayio.TileGrid(image, pixel_shader=displayio.ColorConverter())
    frame.append(grid)

    factory_label = Label(terminalio.FONT, text='Factory Test Program')
    factory_label.x = 36
    factory_label.y = 10
    factory_label.color = 0x0
    frame.append(factory_label)
    display.show(frame)

    btns = 0

    (x, y, z) = badge.acceleration

    acceleration_label = Label(terminalio.FONT, text='Acceleration OK: x={:.2f}, y={:.2f}, z={:.2f}'.format(x, y, z))
    acceleration_label.x = 36
    acceleration_label.y = 24
    acceleration_label.color = 0x0
    frame.append(acceleration_label)

    buttons_ok_label = Label(terminalio.FONT, text='Buttons OK')
    buttons_ok_label.x = 36
    buttons_ok_label.y = 38
    frame.append(buttons_ok_label)

    while True:
        pressed = badge.gamepad.get_pressed()
        btns |= pressed
        badge.vibration = pressed & badge.BTN_ACTION
        if pressed:
            badge.pixels.fill((0x10, 0x0, 0x10))
        else:
            if btns == 0x1f:
                # Success : all buttons were pressed
                badge.pixels.fill((0, 0xff, 0))
                buttons_ok_label.color = 0x0
            elif int(time.monotonic() * 2) % 2:
                badge.pixels.fill((0, 0, 0x10))
            else:
                badge.pixels.fill(0)
            if display.time_to_refresh == 0:
                display.refresh()
                time.sleep(0.1)
