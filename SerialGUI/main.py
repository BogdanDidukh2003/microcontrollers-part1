import threading
import tkinter as tk
from tkinter import TOP, LEFT, RIGHT, messagebox
import serial
import serial.tools.list_ports as lp

BAUD_RATE = 9600
PORT_CHECKING_INTERVAL = 0.1
SERIAL_PORTS_PATH = './DefaultSerialPorts'
WINDOW_MIN_HEIGHT = 400

app_is_running = False
ports = list()
selected_port = ''
serial_port = serial.Serial()

SLAVE1 = 'Slave 1'
SLAVE2 = 'Slave 2'
data_sections = dict.fromkeys((SLAVE1, SLAVE2))


def main():
    global app_is_running, ports
    app_is_running = True

    for port in sorted(lp.comports()):
        ports.append(port.device)
    if not ports:
        with open(SERIAL_PORTS_PATH, 'r') as file:
            ports = list(filter(None, file.read().split('\n')))

    serial_communication = threading.Thread(target=read_from_port)
    serial_communication.daemon = True
    serial_communication.start()

    window = tk.Tk()
    _ = AppGUI(window)
    window.mainloop()
    serial_port.close()
    app_is_running = False
    serial_communication.join()


class AppGUI:

    def __init__(self, window):
        global ports, data_sections

        window.title('Serial Interface')
        window.minsize(height=WINDOW_MIN_HEIGHT, width=round(WINDOW_MIN_HEIGHT * 1.5))

        port_frame = tk.Frame(window)
        data_labels_frame = tk.Frame(window)
        data_frame = tk.Frame(window)
        button_frame = tk.Frame(window)

        port_label = tk.Label(
            port_frame,
            text='Select port',
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

        slave1_label = tk.Label(data_labels_frame, text='Slave 1', font=('Verdana', 14))
        slave2_label = tk.Label(data_labels_frame, text='Slave 2', font=('Verdana', 14))

        data_section_slave1 = tk.Text(data_frame, state='disabled', height=10, width=25, bg='black', fg='white')
        data_section_slave2 = tk.Text(data_frame, state='disabled', height=10, width=25, bg='black', fg='white')
        data_sections[SLAVE1] = data_section_slave1
        data_sections[SLAVE2] = data_section_slave2

        from_slave1_button = tk.Button(
            button_frame,
            text='From Slave 1',
            font=('Verdana', 10, 'bold'),
            bg='#E2E2E4',
            fg='black',
            command=request_data_from_slave1,
        )
        from_slave2_button = tk.Button(
            button_frame,
            text='From Slave 2',
            font=('Verdana', 10, 'bold'),
            bg='#E2E2E4',
            fg='black',
            command=request_data_from_slave2,
        )

        port_frame.pack(side=TOP)
        data_labels_frame.pack()
        data_frame.pack()
        button_frame.pack()

        port_frame_padding = {'x': 30, 'y': 5}
        data_labels_frame_padding = {'x': 100, 'y': 5}
        data_frame_padding = {'x': 15, 'y': 15}
        button_frame_padding = {'x': 60, 'y': 15}

        port_label.pack(side=LEFT, fill=tk.Y,
                        padx=port_frame_padding['x'], pady=port_frame_padding['y'])
        port_selection.pack(side=LEFT, fill=tk.Y,
                            padx=port_frame_padding['x'], pady=port_frame_padding['y'])
        port_open_button.pack(side=LEFT, fill=tk.Y,
                              padx=port_frame_padding['x'], pady=port_frame_padding['y'])

        slave1_label.pack(side=LEFT, fill=tk.Y,
                          padx=data_labels_frame_padding['x'], pady=data_labels_frame_padding['y'])
        slave2_label.pack(side=RIGHT, fill=tk.Y,
                          padx=data_labels_frame_padding['x'], pady=data_labels_frame_padding['y'])

        data_section_slave1.pack(side=LEFT,
                                 padx=data_frame_padding['x'], pady=data_frame_padding['y'])
        data_section_slave2.pack(side=RIGHT,
                                 padx=data_frame_padding['x'], pady=data_frame_padding['y'])

        from_slave1_button.pack(side=LEFT,
                               padx=button_frame_padding['x'], pady=button_frame_padding['y'])
        from_slave2_button.pack(side=RIGHT,
                               padx=button_frame_padding['x'], pady=button_frame_padding['y'])

        show_data(SLAVE1, '> Data Channel 1\n...')
        show_data(SLAVE2, '> Data Channel 2\n...')


def request_data_from_slave1():
    global serial_port
    if serial_port.isOpen():
        serial_port.write(b'1')


def request_data_from_slave2():
    global serial_port
    if serial_port.isOpen():
        serial_port.write(b'2')


def show_data(slave, data):
    data_sections[slave].configure(state='normal')
    data_sections[slave].insert('end', data)
    data_sections[slave].configure(state='disabled')
    data_sections[slave].see('end')


def open_port():
    global app_is_running, serial_port

    try:
        serial_port = serial.Serial(selected_port, BAUD_RATE)
    except serial.serialutil.SerialException as e:
        serial_port.close()
        if app_is_running:
            messagebox.showerror('Error', e.strerror)
    else:
        messagebox.showinfo(selected_port, 'Port is opened successfully')


def read_from_port():
    global app_is_running, serial_port

    event = threading.Event()
    while app_is_running:
        if serial_port.isOpen():
            data = serial_port.readline()
            print(data)
        event.wait(PORT_CHECKING_INTERVAL)


if __name__ == '__main__':
    main()
