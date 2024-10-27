import numpy as np
import matplotlib
matplotlib.use('TkAgg')
import matplotlib.pyplot as plt
from matplotlib.widgets import Button
import serial, sys, glob
import serial.tools.list_ports as COMs

# Function to find Arduino ports
def port_search():
    if sys.platform.startswith('win'):
        ports = ['COM{0:1.0f}'.format(ii) for ii in range(1, 256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Machine Not pyserial Compatible')

    arduinos = []
    for port in ports:
        if 'Bluetooth' in port:
            continue
        try:
            ser = serial.Serial(port)
            ser.close()
            arduinos.append(port)
        except (OSError, serial.SerialException):
            pass
    return arduinos

# Initialize Serial Port
arduino_ports = port_search()
if not arduino_ports:
    raise Exception("No Arduino found.")
ser = serial.Serial(arduino_ports[0], baudrate=9600, timeout=1)
ser.flush()

# Set up the plot
fig = plt.figure(facecolor='k')
win = fig.canvas.manager.window
dpi = 150.0
fig.set_dpi(dpi)

# Radar Plot
ax = fig.add_subplot(111, polar=True, facecolor='#006d70')
ax.set_position([-0.05, -0.05, 1.1, 1.05])
r_max = 100.0
ax.set_ylim([0.0, r_max])
ax.set_xlim([0.0, np.pi])
ax.grid(color='w', alpha=0.5)
ax.set_rticks(np.linspace(0.0, r_max, 5))
ax.set_thetagrids(np.linspace(0.0, 180.0, 10))
angles = np.arange(0, 181, 1)
theta = angles * (np.pi / 180.0)
dists = np.ones((len(angles),)) * r_max
pols, = ax.plot([], linestyle='', marker='o', markerfacecolor='w',
                markeredgecolor='#EFEFEF', markeredgewidth=1.0,
                markersize=10.0, alpha=0.9)
line1, = ax.plot([], color='w', linewidth=4.0)

fig.canvas.draw()
axbackground = fig.canvas.copy_from_bbox(ax.bbox)

# Button to stop
stop_bool = False
def stop_event(event):
    global stop_bool
    stop_bool = True
prog_stop_ax = fig.add_axes([0.85, 0.025, 0.125, 0.05])
pstop = Button(prog_stop_ax, 'Stop Program', color='#FCFCFC', hovercolor='w')
pstop.on_clicked(stop_event)
fig.show()

# Main loop to update radar
try:
    while not stop_bool:
        try:
            if ser.in_waiting > 0:
                ser_bytes = ser.readline().decode('utf-8').strip()
                if ser_bytes:
                    data = ser_bytes.split(',')
                    if len(data) == 2:
                        angle = int(data[0])
                        dist = float(data[1])
                        dist = min(dist, r_max)
                        dists[angle] = dist

                        if angle % 5 == 0:
                            pols.set_data(theta, dists)
                            fig.canvas.restore_region(axbackground)
                            ax.draw_artist(pols)

                            line1.set_data(np.repeat((angle * (np.pi / 180.0)), 2),
                                           np.linspace(0.0, r_max, 2))
                            ax.draw_artist(line1)

                            fig.canvas.blit(ax.bbox)
                            fig.canvas.flush_events()
        except Exception as e:
            print(f"Error: {e}")
            pass
except KeyboardInterrupt:
    print('Program interrupted.')
finally:
    ser.close()
    plt.close('all')