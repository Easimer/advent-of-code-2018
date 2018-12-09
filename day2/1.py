import sys
# For every box ID
# Build character map
# Count characters

two_similar = 0
three_similar = 0

for line in sys.stdin:
    cmap = {}
    for ch in line:
        if ch in cmap:
            cmap[ch] += 1
        else:
            cmap[ch] = 1

    c2_incremented = False
    c3_incremented = False
    for k in cmap:
        v = cmap[k]
        if v == 2 and not c2_incremented:
            two_similar += 1
            c2_incremented = True
        if v == 3 and not c3_incremented:
            three_similar += 1
            c3_incremented = True

print(two_similar * three_similar)
