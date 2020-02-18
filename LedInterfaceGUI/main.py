import tkinter as tk
from tkinter import TOP, BOTTOM, LEFT

LED_ICON_SIZE = 40
LED_NUMBER = 8
LED_OFF = 'skyblue'
LED_ON = 'red'
WINDOW_MIN_HEIGHT = 260

selected_port = None


def main():
    window = tk.Tk(baseName='GUI')
    window.title('Led Interface')
    window.minsize(height=WINDOW_MIN_HEIGHT, width=WINDOW_MIN_HEIGHT * 2)

    port_frame = tk.Frame(window)
    button_frame = tk.Frame(window)
    led_frame = tk.Frame(window)

    port_label = tk.Label(
        port_frame,
        text='COM port',
        font=('Verdana', 12),
    )
    ports = {'COM1': 1, 'COM2': 2, 'COM3': 3}
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
    led_indicators = [(canvas, canvas.create_rectangle(0, 0, LED_ICON_SIZE, LED_ICON_SIZE, fill=LED_OFF))
                      for canvas in led_indicators]

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

    window.mainloop()


def turn_on_led(led_indicator):
    led_indicator[0].itemconfig(led_indicator[1], fill=LED_ON)


def turn_off_led(led_indicator):
    led_indicator[0].itemconfig(led_indicator[1], fill=LED_OFF)


def open_port():
    print('Opened', selected_port)


def start_algorithm1():
    print('Algorithm 1 start...')


def start_algorithm2():
    print('Algorithm 2 start...')


if __name__ == '__main__':
    main()
