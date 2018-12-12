import sys
import array

def initial_state():
    line = sys.stdin.readline()
    state = line.split(':')[1].strip()
    ret = array.array('B')
    for c in state:
        ret.append(1 if c == '#' else 0)
    return ret

def get_rules():
    sys.stdin.readline()
    ret = []
    for line in sys.stdin:
        a = [x.strip() for x in line.split('=>')]
        pattern = [1 if x == '#' else 0 for x in a[0]]
        action = 1 if a[1] == '#' else 0
        ret.append([pattern, action])

    return ret

# Returns bool if the rule matches
def match_one(state, i, rule):
    if i >= 0 and i < len(state) and rule[2] != state[i]:
        return False
    for j in range(5):
        P = 0
        ri = i - 2 + j
        if ri < 0: P = 0
        elif ri >= len(state): P = 0
        else: P = state[i - 2 + j]

        if rule[j] != P:
            return False
    return True

# Returns bool if there will be a plant in the pot
def match(state, zero, rules):
    ret = array.array('B')
    for i in range(-2, len(state) + 2):
        matched = False

        for rule in rules:
            if match_one(state, i, rule[0]):
                #print(f"Rule matches on pot {i}: {rule}")
                ret.append(1 if rule[1] == 1 else 0)
                matched = True
                break
        if not matched:
            ret.append(0)
    return ret, zero + 2

def dump(state, zero, gen):
    mini = None
    maxi = None
    for i in range(len(state)):
        if state[i]:
            if not mini or i < mini:
                mini = i
            if not maxi or i > maxi:
                maxi = i

    print(gen, mini - zero, maxi - zero)
    for i in range(mini, maxi + 1):
        print('#' if state[i] == 1 else '.', end='')
    print()

def state_mini_maxi(state, zero):
    mini = None
    maxi = None
    for i in range(len(state)):
        if state[i]:
            if not mini or i < mini:
                mini = i
            if not maxi or i > maxi:
                maxi = i
    return mini, maxi


orig_state = initial_state()
state = orig_state
zero = 0
rules = get_rules()

if "-g" in sys.argv:
    import time
    i = 0
    while True:
        for x in state:
            print('#' if x == 1 else '.', end='')
        print()
        state, zero = match(state, zero, rules)
        time.sleep(0.4)

for i in range(20):
    state, zero = match(state, zero, rules)

print("Part 1")
print(sum([idx - zero for idx,x in enumerate(state) if x == 1]))

# After large enough number of iterations, the state won't change
# anymore, only shift one to the right every generations.
# The index of the first and last plant is proportional to the
# generation ID.
state = orig_state
zero = 0
for i in range(250):
    state, zero = match(state, zero, rules)

# calc idx of first plant
mini, maxi = state_mini_maxi(state, zero)
miniv = mini - zero
maxiv = maxi - zero

Fmini = (lambda G: G - miniv)
Fmaxi = (lambda G: G + miniv)

mini5 = Fmini(50000000000) - Fmini(250)
maxi5 = Fmaxi(50000000000) - Fmaxi(250)

sum = 0
for i in range(mini, maxi + 1):
    if state[i] == 1:
        sum += i - zero + mini5

print("Part 2")
print(sum)

