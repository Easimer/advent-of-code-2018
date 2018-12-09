#!/usr/bin/python3

import sys

def get_line():
    for line in sys.stdin:
        a = line.split()
        yield (a[1], a[7])
    return

def all_tasks(graph):
    U = set()

    for edge in graph:
        U.add(edge[0])
        U.add(edge[1])

    return U

def tasks_nodeps(graph):
    U = set() # all tasks
    D = set() # tasks with dependency

    for edge in graph:
        U.add(edge[0])
        U.add(edge[1])
        D.add(edge[1])

    return U - D
    
def working(workers):
    return any([worker[0] != None for worker in workers])

graph = [x for x in get_line()]
tasks = all_tasks(graph)
workers = [[None, 0] for i in range(5)]

T = 0

def print_state(T, workers, tasks, avail):
    print(f"[{T}]\t", end='')
    for worker in workers:
        if worker[0] == None:
            print('. ', end='')
        else:
            print(f'{worker[0]} ', end='')

    print(tasks, avail)

state_changed = False
while len(tasks) > 0 or working(workers):
    avail = tasks_nodeps(graph)
    if len(avail) == 0 and len(graph) == 0:
        avail = tasks
        #state_changed = True
    avail2 = avail.copy()
    for worker in workers:
        if len(avail) == 0: break
        if worker[0] == None:
            while len(avail) > 0:
                task = sorted(avail)[0]
                avail.remove(task)
                if any([worker[0] == task for worker in workers]):
                    #print(f"Ignoring task {task}: already processing")
                    pass
                else:
                    worker[0] = task
                    worker[1] = 60 + (ord(worker[0]) - ord('A') + 1)
                    #worker[1] = (ord(worker[0]) - ord('A') + 1)
                    state_changed = True
                    break

    if state_changed:
        print_state(T, workers, tasks, avail2)
        state_changed = False
    for worker in workers:
        worker[1] = worker[1] - 1
        if worker[1] == 0:
            task = worker[0]
            worker[0] = None
            tasks.discard(task)
            print(task)
            graph = [edge for edge in graph if edge[0] != task]
            state_changed = True

    T += 1

print(T)

