#!/usr/bin/python3
import sys
import array

def condition(c1, c2):
    if str.upper(c1) != str.upper(c2):
        return False

    if c1 == c2:
        return False

    return True

def round(s):
    found = False

    slices = []

    i = 0
    cs = s
    while i < len(cs) - 1:
        if condition(cs[i], cs[i + 1]):
            slices.append(cs[0:i])
            cs = cs[i + 2:]
            i = 0
            found = True
        else:
            i += 1
    slices.append(cs)

    return (found, ''.join(slices))

def lenP(p):
    sum = 0
    for c in p:
        if c != '*':
            sum += 1
    return sum

def if_removed(p, c):
    arr = array.array('c')

    arr.fromstring(p)

    up = str.upper(c)
    lo = str.lower(c)

    for i in range(len(arr)):
        if arr[i] == up or arr[i] == lo:
            arr[i] = '*'

    s = ''.join([x for x in arr if x != '*'])

    found = True
    while found:
        (found, s) = round(s)
    return len(s)

for line in sys.stdin:
    line = line.strip()
    oline = line
    found = True
    while found:
        (found, line) = round(line)
    print(line)
    print(len(line))
    print(len(oline))

    map2 = {}
    for i in range(ord('a'), ord('z') + 1):
        N = if_removed(oline, chr(i))
        map2[chr(i)] = N

    print(map2)

    C = min(map2, key=map2.get)
    print(C)
