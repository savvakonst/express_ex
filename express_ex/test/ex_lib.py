# -*- coding: utf-8 -*-
import numpy as np
import json
import os.path

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
            ex_dtype = int(parameter["Data.Type"])
            ex_size = int(parameter["Data.Fragments.List"][0]["Size"])
            dat_field = "f{0:d}" if ex_dtype & 0x0020 else "i{0:d}"
            dat_field = dat_field.format(ex_dtype & 0xf)

            if ex_dtype & 0x1000:  # if async
                dt = np.dtype([('time', "f4"), ('data', dat_field)])
                return dt, ex_size // dt.itemsize
            else:
                dt = np.dtype([('data', dat_field)])
                return dt, ex_size // dt.itemsize
    return ret


g_map = {}


def param(param_name, dt=None, ex_len=0):
    f_name = param_name + ".dat"
    if dt is None:
        dt, ex_len = getNpDType(param_list, param_name)
        if dt is None:
            print("there are no parameter with '" + param_name + "' name")
            exit()

    if not os.path.exists(f_name):
        if len(dt) == 1:
            noise = 0  # np.random.normal(0,5,ex_len)
            gen_data = 4 * (np.cos(np.pi * np.linspace(0.0, 10.0, ex_len)) + noise)  # / 2 ** 14
            np.array(gen_data, dt).tofile(f_name)

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

# double(
# .astype(
