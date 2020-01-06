from aramcon_badge.badgeio import badge
import displayio
from adafruit_display_text.label import Label
import terminalio

display = badge.display

def show_frame(group, refresh = True):
    display.show(group)
    while refresh:
        if display.time_to_refresh == 0:
            display.refresh()
            time.sleep(0.1)
            break

def run():
    frame = displayio.Group()
    connected_label = Label(terminalio.FONT, text='test')
    connected_label.x = 80
    connected_label.y = 60
    frame.append(connected_label)
    show_frame(frame)