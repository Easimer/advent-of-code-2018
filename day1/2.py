import sys

dm = {}

sum = 0
dm[0] = 1

seq = []

for line in sys.stdin:
    seq.append(int(line))

i = 0
while True:
    sum += seq[i]

    if sum in dm:
        print(sum)
        break
    else:
        dm[sum] = 1

    i += 1
    if i == len(seq):
        i = 0

