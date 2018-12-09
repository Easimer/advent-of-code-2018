#!/usr/bin/python3

import sys
import collections

def parse_line(line):
    a = line.split()
    return (a[1], a[7])

def find_alphabet_first_without_prereq(graph):
    U = set() # all tasks
    D = set() # tasks that have dependency

    for edge in graph:
        U.add(edge[0])
        U.add(edge[1])
        D.add(edge[1])

    R = U - D

    if len(R) == 0:
        return

    if len(D) == 1 and len(R) == 1:
        yield R.pop()
        yield D.pop()
    else:
        SR = sorted(R)
        yield SR[0]
def without_prereq(graph):
    U = set() # universe
    D = set() # dependants

    for edge in graph:
        U.add(edge[0])
        U.add(edge[1])
        D.add(edge[1])

    R = U - D

    if len(R) == 0:
        return

    if len(D) == 1 and len(R) == 1:
        yield R.pop()
        yield D.pop()
        return

    for x in sorted(R):
        yield x

graph = []
for line in sys.stdin:
    graph.append(parse_line(line))
graph2 = graph.copy()

while len(graph) > 0:
    for a in find_alphabet_first_without_prereq(graph):
        graph = [edge for edge in graph if edge[0] != a]
        print(a, end='')
print()

def assign_task(graph, workers, task):
    for edge in graph:
        if edge[1] == task:
            print(f"Task {task} sanity checked!")
            return False
    t = 60 + ord(task) - ord('A')
    #t = ord(task) - ord('A') + 1

    if not all([worker[0] != task for worker in workers]):
        return True
    worker = [task, t]
    workers.append(worker)
    return True


workers = [[None, 0] for x in range(5)]
T = 0
while len(graph2) > 0 or len(workers) > 0:
    # See if any worker has completed their task: remove every dependency
    # Fetch completeable tasks
    # If there is at least one such: try assigning it to a worker
    # Decrement t of all workers
    # Increment T

    for worker in workers:
        if worker[1] <= 0:
            graph2 = [edge for edge in graph2 if edge[0] != worker[0]]
            print(graph2)
    workers = [worker for worker in workers if worker[1] > 0]

    tasks = [x for x in without_prereq(graph2)]

    for task in tasks:
        if len(workers) == 5:
            break
        if not assign_task(graph2, workers, task):
            pass
    print(T, workers, tasks)

    for worker in workers:
        worker[1] -= 1

    T += 1

print(T- 1)

