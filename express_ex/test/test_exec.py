from ex_lib import *


def execFile(name):
    if os.path.exists("test_tasks.py"):
        # import sys
        # sys.path.append(os.path.dirname(os.path.abspath(__file__)))
        # print("path: ", os.path.dirname(os.path.abspath(__file__)), sys.path)

        import test_tasks
        def_name = os.path.splitext(name)[0]
        if hasattr(test_tasks, def_name):
            exFuncF = getattr(test_tasks, def_name)
            return exFuncF()

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
