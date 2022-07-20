def loop_const(ref, a, i):
    return a if (i > 10) else loop_const((ref), a + ref, i + 1)


print("\n")

print(loop_const(6, 4, 1), )
