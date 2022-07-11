# -*- coding: utf-8 -*-
import numpy as np
import json

db = None
param_list = None
g_is_async = True


def loadDataBase(name):
    db_loc = open(name, "rb")
    db_json = db_loc.read()
    db_loc.close()
    json_map = json.loads(db_json)

    global db, param_list
    db = json_map
    param_list = json_map["Parameters.List"]
    return db


def getNpDType(param_list, param_name):
    ret = None
    for parameter in param_list:
        if parameter[" Name"] == param_name:
            ex_dtype = parameter["Data.Type"]
            dat_field = "f{0:d}" if ex_dtype & 0x0020 else "i{0:d}"
            dat_field = dat_field.format(ex_dtype & 0xf)

            if ex_dtype & 0x1000:  # if async
                return np.dtype([('time', "f4"), ('data', dat_field)])
            else:
                return np.dtype([('data', dat_field)])
    return ret


g_map = {}


def param(param_name, dt=None):
    f_name = param_name + ".dat"
    if dt is None:
        dt = getNpDType(param_list, param_name)
        if dt is None:
            print("there are no parameter with '" + param_name + "' name")
            exit()

    data = np.fromfile(f_name, dtype=dt)
    global g_is_async

    g_is_async = len(dt) == 2

    g_map[param_name] = data
    return data["data"]


def isAsync():
    return g_is_async


def time(param_name, dt=None):
    if not param_name in g_map:
        param(param_name, dt)

    data = g_map[param_name]
    return data["time"]


def double(arr):
    return np.array(arr).astype("f8")


def float(arr):
    return np.array(arr).astype("f4")


def convolve(arr_a, arr_b):
    return np.convolve(arr_a, arr_b, 'same')


import re


def execFile(name):
    f = open(name, "r", encoding='utf-8')
    lines = []
    for i in f.read().split("\n"):
        s = i + "\n"
        m = re.search(r'([^=]*)=([^\?]*)\?([^:]*):([^\n]*)\n', s)
        if m:
            s = m.group(1) + "= np.choose(" + m.group(2) + ", [" + m.group(4) + "," + m.group(3) + "])\n"
        lines.append(s)

    code = "def exFunc():\n\t" + "\t".join(lines)
    print(code)
    f.close()

    exec(code, globals())
    return exFunc()


cos = np.cos
sin = np.sin

# double(
# .astype(
