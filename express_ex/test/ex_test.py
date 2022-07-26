# -*- coding: utf-8 -*-
import argparse
from ex_lib import *
from test_exec import *
import matplotlib.pyplot as plt
from matplotlib.widgets import Button

parser = argparse.ArgumentParser()

# parser.add_argument('--executable', help='path to express_ex_app', required=True, type=str)

parser.add_argument('--generate', help='if this option is used it will generate dataset', type=bool)
parser.add_argument('--db', help='database path', required=True, type=str)
parser.add_argument('-i', help='input *.kex file',
                    required=True, type=str)
parser.add_argument(
    '--otype', help='output type file.\npossible values: f8,f4,..,f1,i8,..,u8,.. ', default="f8", type=str)
args = parser.parse_args()

loadDataBase(args.db)
py_data = execFile(args.i)
print("py_data", py_data)
print("py_data len:", len(py_data))

dt = np.dtype([('time', "f4"), ('data', args.otype)]) if isAsync() else np.dtype([('data', args.otype)])
express_data = param("out", dt)

print("express_data len:", len(express_data))

try:
    diff_data = py_data - express_data
    # plt.plot(diff)
    # plt.show()

    fig, ax = plt.subplots()
    plt.subplots_adjust(bottom=0.2)
    l, = plt.plot(diff_data, lw=2)

    ax = plt.gca()


    class Index:

        def pyData(self, event):
            l.set_ydata(py_data)
            ax.relim()
            ax.autoscale_view()
            plt.draw()

        def expressData(self, event):
            l.set_ydata(express_data)
            ax.relim()
            ax.autoscale_view()
            plt.draw()

        def diffData(self, event):
            l.set_ydata(diff_data)
            ax.relim()
            ax.autoscale_view()
            plt.draw()


    callback = Index()

    plt.suptitle(args.i)
    diff_data_button_ax = plt.axes([0.18, 0.05, 0.2, 0.075])
    py_data_button_ax = plt.axes([0.4, 0.05, 0.2, 0.075])
    express_data_button_ax = plt.axes([0.62, 0.05, 0.2, 0.075])

    diff_data_button = Button(diff_data_button_ax, 'diff data')
    diff_data_button.on_clicked(callback.diffData)
    py_data_button = Button(py_data_button_ax, 'py data')
    py_data_button.on_clicked(callback.pyData)
    express_data_button = Button(express_data_button_ax, 'express data')
    express_data_button.on_clicked(callback.expressData)

    plt.show()

except Exception as e:
    print(str(e))
    plt.suptitle(str(e) + " express_data in " + args.i)
    # plt.plot(py_data)
    plt.plot(express_data)
    plt.show()
    plt.suptitle(str(e) + " py_data in" + args.i)
    plt.plot(py_data)
    plt.show()
