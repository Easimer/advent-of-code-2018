import sys

def paint(field, candidates, claim):
    x = claim[1]
    y = claim[2]
    w = claim[3]
    h = claim[4]
    while h > 0:
        cw = w
        while cw > 0:
            e = field[y + h][x + cw]
            if e[1] == 0:
                field[y + h][x + cw] = (claim[0], 1)
            else:
                if e[0] in candidates:
                    candidates.remove(e[0])
                if claim[0] in candidates:
                    candidates.remove(claim[0])
                field[y + h][x + cw] = (-1, e[1] + 1)
            cw -= 1
        h -= 1

def create_field():
    FIELD_SIZ = 1000
    field = []
    for i in range(FIELD_SIZ):
        row = []
        for j in range(FIELD_SIZ):
            row.append((0, 0))
        field.append(row)
    return field

def read_input():
    claims = []

    for line in sys.stdin:
        cols = line.split()
        Id = int(cols[0][1:])
        Coord = [int(x) for x in cols[2][0:-1].split(',')[0:2]]
        Size = [int(x) for x in cols[3].split('x')]
        claims.append((Id, Coord[0], Coord[1], Size[0], Size[1]))

    return claims

def generate_candidate_set(claims):
    return set([claim[0] for claim in claims])

def main():
    field = create_field()
    claims = read_input()

    candidates = generate_candidate_set(claims)

    for claim in claims:
        paint(field, candidates, claim)

    sum = 0
    for row in field:
        for col in row:
            if col[1] > 1:
                sum += 1
    print(sum)
    print(candidates.pop())

main()
