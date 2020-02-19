import tkinter as tk
from tkinter import TOP, BOTTOM, LEFT
import serial
import threading

PORT_CHECKING_INTERVAL = 0.1
SERIAL_PORTS_PATH = './SerialPorts'
LED_ICON_SIZE = 40
LED_NUMBER = 8
LED_OFF = 'skyblue'
LED_ON = 'red'
WINDOW_MIN_HEIGHT = 260

app_is_running = True
led_indicators = list
ports = list()
selected_port = ''
serial_port = serial.Serial()


def main():
    global app_is_running, ports
    app_is_running = True

    with open(SERIAL_PORTS_PATH, 'r') as file:
        ports = list(filter(None, file.read().split('\n')))

    serial_communication = threading.Thread(target=read_from_port)
    serial_communication.start()

    window = tk.Tk()
    _ = AppGUI(window)
    window.mainloop()
    app_is_running = False

    serial_communication.join()


class AppGUI:

    def __init__(self, window):
        global led_indicators, ports

        window.title('Led Interface')
        window.minsize(height=WINDOW_MIN_HEIGHT, width=WINDOW_MIN_HEIGHT * 2)

        port_frame = tk.Frame(window)
        button_frame = tk.Frame(window)
        led_frame = tk.Frame(window)

        port_label = tk.Label(
            port_frame,
            text='Choose port',
            font=('Verdana', 12),
        )
        port_name_variable = tk.StringVar(window)
        port_name_variable.set(' ' * 10)

        def select_port(port):
            global selected_port
            selected_port = port

        port_selection = tk.OptionMenu(port_frame, port_name_variable, *ports, command=select_port)
        port_open_button = tk.Button(
            port_frame,
            text='Open',
            font=('Verdana', 10),
            bg='#E2E2E4',
            activebackground='lightgrey',
            command=open_port,
        )

        algorithm1_button = tk.Button(
            button_frame,
            text='Algorithm 1',
            font=('Verdana', 10, 'bold'),
            bg='#0074D6',
            fg='white',
            activebackground='#0074D6',
            activeforeground='white',
            command=start_algorithm1,
        )
        algorithm2_button = tk.Button(
            button_frame,
            text='Algorithm 2',
            font=('Verdana', 10, 'bold'),
            bg='#0074D6',
            fg='white',
            activebackground='#0074D6',
            activeforeground='white',
            command=start_algorithm2,
        )

        led_indicators = [tk.Canvas(led_frame, width=LED_ICON_SIZE, height=LED_ICON_SIZE)
                          for _ in range(LED_NUMBER)]
        led_indicators = list([(canvas, canvas.create_rectangle(0, 0, LED_ICON_SIZE, LED_ICON_SIZE, fill=LED_OFF))
                               for canvas in led_indicators])

        port_frame.pack(side=TOP)
        button_frame.pack()
        led_frame.pack(side=BOTTOM)

        port_frame_padding = {'x': 30, 'y': 5}
        button_frame_padding = {'x': 50, 'y': 30}
        led_frame_padding = {'x': 10, 'y': 30}

        port_label.pack(side=LEFT, fill=tk.Y,
                        padx=port_frame_padding['x'], pady=port_frame_padding['y'])
        port_selection.pack(side=LEFT, fill=tk.Y,
                            padx=port_frame_padding['x'], pady=port_frame_padding['y'])
        port_open_button.pack(side=LEFT, fill=tk.Y,
                              padx=port_frame_padding['x'], pady=port_frame_padding['y'])

        algorithm1_button.pack(side=LEFT,
                               padx=button_frame_padding['x'], pady=button_frame_padding['y'])
        algorithm2_button.pack(side=LEFT,
                               padx=button_frame_padding['x'], pady=button_frame_padding['y'])

        for led_indicator in led_indicators:
            led_indicator[0].pack(side=LEFT, padx=led_frame_padding['x'], pady=led_frame_padding['y'])

        turn_on_led(led_indicators[1])


def turn_on_led(led_indicator):
    global app_is_running

    if app_is_running:
        led_indicator[0].itemconfig(led_indicator[1], fill=LED_ON)


def turn_off_led(led_indicator):
    global app_is_running

    if app_is_running:
        led_indicator[0].itemconfig(led_indicator[1], fill=LED_OFF)


def parse_algorithm_encoding(coded_string):
    return tuple(map(int, coded_string.split(b'\r\n')[0].split(b',')))


def get_led_order_to_indicator_dictionary_from_encoded_data(encoded_data):
    led_order_to_pin = dict()
    for index in range(len(encoded_data)):
        led_order_to_pin[index] = encoded_data[index]
    mapped_pins_to_indicators = sorted(led_order_to_pin, key=led_order_to_pin.get)
    led_order_to_indicator = dict()
    for pin in range(len(mapped_pins_to_indicators)):
        led_order_to_indicator[mapped_pins_to_indicators[pin]] = pin

    return led_order_to_indicator


def get_led_order_to_indicator_dictionary_and_delay_value_from_coded_algorithm(coded_algorithm):
    parsed_encoding = parse_algorithm_encoding(coded_algorithm)
    return get_led_order_to_indicator_dictionary_from_encoded_data(parsed_encoding[:-1]), parsed_encoding[-1]


def open_port():
    global serial_port
    serial_port = serial.Serial(selected_port)
    print(selected_port, 'opened...')


def read_from_port():
    global app_is_running, led_indicators

    event = threading.Event()
    while app_is_running:
        if serial_port.isOpen():
            data = serial_port.readline()
            if data:
                print(data)
                turn_on_led(led_indicators[0])
                event.wait(0.2)
                turn_off_led(led_indicators[0])
        event.wait(PORT_CHECKING_INTERVAL)


def start_algorithm1():
    print('Algorithm 1 start...')


def start_algorithm2():
    print('Algorithm 2 start...')


if __name__ == '__main__':
    main()
