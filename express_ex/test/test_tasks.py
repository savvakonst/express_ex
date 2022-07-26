from ex_lib import *


def loop_const(ref, a, i):
    return a if (i > 10) else loop_const((ref), a + ref, i + 1)


def recurrence_relation():
    shift = 1

    x = param("#1_A01__2")
    y = np.zeros(len(x) + shift, dtype=x.dtype)

    index = shift
    for i in x:
        y[index] = y[index - shift] + i
        index += 1

    return y[shift:]

# print("\n")

# print(loop_const(6, 4, 1), )
