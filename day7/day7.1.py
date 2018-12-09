#!/usr/bin/python3

import sys

def parse_line(line):
    a = line.split()
    return (a[1], a[7])

def tasks_without_dependencies(graph):
    U = set() # all tasks
    D = set() # tasks that depend on another 

    for edge in graph:
        U.add(edge[0])
        U.add(edge[1])

        D.add(edge[1])

    R = U - D # Tasks without dependency

    # No more tasks
    if len(R) == 0:
        return

    if len(D) == 1 and len(R) == 1:
        yield R.pop()
        yield D.pop()
    else:
        SR = sorted(R)
        yield SR[0]

deps = []
for line in sys.stdin:
    deps.append(parse_line(line))

while len(deps) > 0:
    for task in tasks_without_dependencies(deps):
        deps = [dep for dep in deps if dep[0] != task]
        print(task, end='')
print()
