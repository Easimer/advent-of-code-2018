#!/usr/bin/python3

import sys
import collections

class Tree:
    children = None
    metadata = None

    def __init__(self):
        self.children = collections.deque()
        self.metadata = collections.deque()

def parse_input():
    line = next(sys.stdin).strip()

    i = 0
    while i < len(line):
        num = 0
        while line[i] != ' ' and i < len(line):
            num *= 10
            num += int(line[i])
            i += 1
        i += 1
        yield num
    return


def build_tree(line = None, i = None):
    if not line:
        line = [int(x) for x in next(sys.stdin).split()]
        i = 0

    t = Tree()
    nc = line[i + 0]
    nm = line[i + 1]

    if nc == 0:
        for j in range(nm):
            t.metadata.append(line[i + 2 + j])
        return (t, i + 2 + nm)

    ni = i + 2
    for j in range(nc):
        (st, ni) = build_tree(line, ni)
        t.children.append(st)

    for j in range(nm):
        t.metadata.append(line[ni])
        ni += 1

    return (t, ni)

def checksum(tree):
    ret = sum(tree.metadata)
    for subtree in tree.children:
        ret += checksum(subtree)

    return ret

def value(tree):
    if len(tree.children) == 0:
        return sum(tree.metadata)

    ret = 0
    for entry in tree.metadata:
        if entry == 0:
            continue
        idx = entry - 1
        if idx < len(tree.children):
            ret += value(tree.children[idx])
    return ret

def main():
    (t, _) = build_tree()
    print(checksum(t))
    print(value(t))

if __name__ == "__main__":
    main()
