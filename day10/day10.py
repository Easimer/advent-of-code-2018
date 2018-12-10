#!/usr/bin/python3

import multiprocessing
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, RadioButtons
import math
import time

def points():
    import sys
    for line in open("input.txt"):
        line = line.replace("position=", "").replace("velocity=", "").replace(' ', '').replace('<', '').strip()
        a = line.split('>')
        yield [tuple([int(y) for y in x.split(',')]) for x in a[0:2]]

def produce_state(PP, PV, T):
    ret = [] 
    for p in zip(PP, PV):
        v = p[1]
        p = p[0]
        ret.append([p[0] + v[0] * T, p[1] + v[1] * T])

    return ret

def render(PP, T):
    plt.scatter([p[0] for p in PP], [p[1] for p in PP])
    plt.show(block=True)

def BB(PP):
    sx = 0
    sy = 0
    ex = 0
    ey = 0

    for p in PP:
        if p[0] < sx: sx = p[0]
        if p[0] > ex: ex = p[0]
        if p[1] < sy: sy = p[1]
        if p[1] > ey: ey = p[1]

    return sx, sy, ex, ey

P = [point for point in points()]

# calc BB
PP = [point[0] for point in P]
PV = [point[1] for point in P]

T = 0

Emin = 0
Tmin = -1

step = 1
def update(T):
    S = produce_state(PP, PV, T)
    sx, sy, ex, ey = BB(S)
    #render(S, T)
    return (math.sqrt((sx - ex) ** 2 + (sy - ey) ** 2))

fuck = 256000
scale = 100
start = int(10518.9 * scale)
end = int((1 + 10519) * scale)
#for T in range(start, end, 1):
for i in range(1):
    T = 10519
    #T = T / scale
    S = produce_state(PP, PV, T)
    sx, sy, ex, ey = BB(S)
    cunt = (math.sqrt((sx - ex) ** 2 + (sy - ey) ** 2))
    print(T)
    #if fuck< cunt:
    #    print(T)
    #    render(S, T)
    #    break
    render(S, T)
    fuck = cunt
