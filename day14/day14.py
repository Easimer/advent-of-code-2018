#!/usr/bin/python3

import sys

RECIPE_COUNT = 793061

SRECIPE = [int(c) for c in str(RECIPE_COUNT)]

recipes = [3, 7]

elf0 = 0
elf1 = 1

def apply(elf0, elf1, recipes):
    rsum = recipes[elf0] + recipes[elf1]

    new_recipes = [int(x) for x in str(rsum)]
    recipes += new_recipes


    return((elf0 + 1 + recipes[elf0]) % len(recipes), (elf1 + 1 + recipes[elf1]) % len(recipes))

while len(recipes) < RECIPE_COUNT + 10:
    elf0, elf1 = apply(elf0, elf1, recipes)

print("Part 1", ''.join([str(x) for x in recipes[RECIPE_COUNT:RECIPE_COUNT + 10]]))

match = False
idx = len(recipes) - len(SRECIPE)

while not match:
    elf0, elf1 = apply(elf0, elf1, recipes)
    for i in range(idx, len(recipes) - len(SRECIPE)):
        match = True
        for j in range(len(SRECIPE)):
            if recipes[i + j] != SRECIPE[j]:
                match = False
                break
        if match:
            print("Part 2", i)
            got_p2 = True
    idx = len(recipes) - len(SRECIPE) - 4
