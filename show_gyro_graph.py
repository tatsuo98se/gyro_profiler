# -*- coding: utf-8 -*-
from __future__ import unicode_literals, print_function

import numpy as np
import matplotlib.pyplot as plt
import zmq
import json
import time
from collections import deque
import threading
from enum import IntEnum, auto

class Gyro(IntEnum):
    GX=0
    GY=auto()
    GZ=auto()
    AX=auto()
    AY=auto()
    AZ=auto()

g_is_stop = False
g_lock = threading.Lock()
g_data_label = [
    'gx',
    'gy',
    'gz',
    'ax',
    'ay',
    'az',
]


def gyro_receive_loop(gyro):
    context = zmq.Context()
    socket = context.socket(zmq.SUB)

    socket.connect("tcp://raspberrypi.local:5801")
    socket.setsockopt(zmq.SUBSCRIBE, b'')

    while not g_is_stop:
        rd = socket.recv_string()
        data = json.loads(rd)

        g_lock.acquire()
        append_and_pop(gyro[Gyro.GX], data[g_data_label[Gyro.GX]])
        append_and_pop(gyro[Gyro.GY], data[g_data_label[Gyro.GY]])
        append_and_pop(gyro[Gyro.GZ], data[g_data_label[Gyro.GZ]])
        append_and_pop(gyro[Gyro.AX], data[g_data_label[Gyro.AX]])
        append_and_pop(gyro[Gyro.AY], data[g_data_label[Gyro.AY]])
        append_and_pop(gyro[Gyro.AZ], data[g_data_label[Gyro.AZ]])
        g_lock.release()


def plots(ax, x, graphs):
    ret = []
    for i, graph in enumerate(graphs):
        ret.extend(ax.plot(x,graph, label=g_data_label[i]))
    return ret

def append_and_pop(list, newvalue):
    list.append(newvalue)
    list.popleft()

def pause_plot():
    xsize = 10000
    fig, ax = plt.subplots(1, 1)
    ax.set_ylim((-0x9000, 0x9000))

    x = np.arange(0, xsize, 1)
    gyro = [deque([0 for _ in range(xsize)]) for _ in range(6)]

    lines = plots(ax, x, gyro)
    plt.legend(loc=2)

    th = threading.Thread(name="gyro_receive_loop", target=gyro_receive_loop, args=(gyro,))
    th.setDaemon(True)
    th.start()

    try:

        # ここから無限にplotする
        while True:
            for i in range(len(lines)):
                g_lock.acquire()
                lines[i].set_data(x, gyro[i])
                g_lock.release()

            ax.set_xlim((x.min(), x.max()))
            plt.pause(.01)
    except:
        g_is_stop = True
        th.join()

if __name__ == "__main__":
    pause_plot()