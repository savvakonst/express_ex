if __name__ == "__main__":
    import numpy as np


    def param(arg):
        return np.cos(np.linspace(0.0, 10.0, 2 ** 16))


def loop_const(ref, a, i):
    return a if (i > 10) else loop_const((ref), a + ref, i + 1)


def recurrence_relation():
    shift = 10

    x = param("#1_A01__1")
    y = np.zeros(len(x) + shift, dtype=x.dtype)

    index = shift
    for i in x:
        y[index] = y[index - 3] + 0.3 * y[index - shift] + i

    return y[shift:]

# print("\n")

# print(loop_const(6, 4, 1), )
