#!/usr/bin/python3

import sys
import matplotlib.pyplot as plt

def manhattan(p1, p2):
    return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])

points = []

for line in sys.stdin:
    points.append([int(x) for x in line.split(',')])

# Find bounding box
BB = [[0,0], [128, 128]]

for point in points:
    if BB[1][0] < point[0]:
        BB[1][0] = point[0]
    if BB[1][1] < point[1]:
        BB[1][1] = point[1]

# Add margin

BB = [[BB[0][0], BB[0][1]], [BB[1][0], BB[1][1]]]

#print(f"Bounding box: {BB}")

def fill_field(BB, points):
    field_w = BB[1][0] - BB[0][0]
    field_h = BB[1][1] - BB[0][1]

    field = [[-1 for x in range(BB[1][0])] for x in range(BB[1][1])]

    for y in range(field_h):
        for x in range(field_w):
            min_dist = 0
            min_idx = -1 
            for i in range(len(points)):
                point = points[i]
                dist = manhattan(point, (x, y))

                if dist < min_dist or min_idx == -1:
                    min_idx = i
                    min_dist = dist

            field[y][x] = min_idx

    for y in range(field_h):
        for x in range(field_w):
            idx = field[y][x]
            dist = manhattan(points[idx], (x, y))
            for i in range(len(points)):
                if i == idx:
                    continue
                point = points[i]
                if manhattan(point, (x, y)) == dist:
                    field[y][x] = -1
                    break


    plt.imsave("plot_inf.png", field)
    

    pset = set()

    for x in range(field_w):
        pset.add(field[0][x])
        pset.add(field[field_h - 1][x])
    for y in range(field_h):
        pset.add(field[y][0])
        pset.add(field[y][field_w - 1])

    pamap = {}

    for y in range(BB[0][1], BB[1][1]):
        for x in range(BB[0][0], BB[1][0]):
            idx = field[y - field_h][x - field_w]
            if idx in pset:
                continue
            if not idx in pamap:
                pamap[idx] = 0
            pamap[idx] += 1

    for y in range(field_h):
        for x in range(field_w):
            if not field[y][x] in pamap:
                field[y][x] = -1

    plt.imsave("plot_fin.png", field)

    return (field, pamap)

def p2(field, points):
    dist_from_points = [0] * len(points)

    for i in range(len(points)):
        dist_from_points[i] = points[i][0] + points[i][1]

    x = 0
    y = 0
    ret = 0

    while y < len(field):
        x = 0
        while x < len(field[0]):
            dist_from_points = [manhattan((x, y), point) for point in points]
            if sum(dist_from_points) < 10000:
                ret += 1
            x += 1
        y += 1

    return ret

(field, min_field) = fill_field(BB, points)

# Find finite areas

area = 0

for k in min_field:
    v = min_field[k]
    print(k, v)
    if v > area:
        area = v

print(area)

# Shrink bounding box
BB[0] = BB[1].copy()
for point in points:
    if point[0] < BB[0][0]:
        BB[0][0] = point[0]
    if point[1] < BB[0][1]:
        BB[0][1] = point[1]

print(p2(field, points))
