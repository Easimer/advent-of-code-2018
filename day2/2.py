import sys

id_list = []

for line in sys.stdin:
    id_list.append(line)

id_list = sorted(id_list)

for i in range(len(id_list)):
    for j in range(len(id_list)):
        if i == j:
            continue
        diff_count = 0
        idx = -1
        for k in range(len(id_list[i])):
            if id_list[i][k] != id_list[j][k]:
                diff_count += 1
                idx = k
            if diff_count > 1:
                break
        if diff_count == 1:
            Id = id_list[i]
            print(Id[0:idx] + Id[idx + 1:])
            sys.exit(0)

