import sys

EVENT_ERROR = 0
EVENT_SHIFT = 1
EVENT_SLEEP = 2
EVENT_AWAKE = 3

def read_input():
    events = []
    for line in sys.stdin:
        tokens = line.split()
        date = [int(x) for x in tokens[0][1:].split('-')]
        time = [int(x) for x in tokens[1][0:-1].split(':')]
        event = ' '.join(tokens[2:])

        event_type = EVENT_ERROR
        guard = -1

        if tokens[2] == "Guard":
            event_type = EVENT_SHIFT
            guard = int(tokens[3][1:])
        elif tokens[2] == "falls":
            event_type = EVENT_SLEEP
        elif tokens[2] == "wakes":
            event_type = EVENT_AWAKE

        entry = (tuple(date[0:3]), tuple(time[0:2]), event_type, guard)
        events.append(entry)
    return events

def render(timeline, interval):
    for i in range(interval[0][1], interval[1][1]):
        timeline[i] = 1
    return timeline

def simulate(events):
    timetable = []
    current_guard = -1
    # ((month, day), guard, row)
    entry = [(0, 0), -1, [0] * 60]
    # until not empty:
    # fetch SHIFT entry
    # fetch SLEEP-AWAKE entry pairs until it's not a SLEEP entry

    i = 0
    while i < len(events):
        event1 = events[i]
        if event1[2] == EVENT_SLEEP:
            event2 = events[i + 1]
            if event2[2] != EVENT_AWAKE:
                print("FUCK")
            interval = (event1[1], event2[1])
            entry[2] = render(entry[2], interval)
            i += 2
            print(f"Guard {entry[1]} sleeping {interval}")
        elif event1[2] == EVENT_SHIFT:
            if entry[1] != -1:
                timetable.append(entry)
                entry = [(0, 0), -1, [0] * 60]
            entry[1] = event1[3]
            entry[0] = event1[0][1:3]
            i += 1
            continue
    if entry[1] != -1:
        timetable.append(entry)
    return timetable

def visualize_timetable(timetable):
    for timeline in timetable:
        print(timeline[0], end = '')
        print(str(timeline[1]) + '\t', end = '')
        for m in timeline[2]:
            if m == 0:
                print('.', end='')
            else:
                print('#', end='')
        print()

def find_sleepiest_guard(timetable):
    gmap = {}
    for timeline in timetable:
        if timeline[1] in gmap:
            gmap[timeline[1]] += sum(timeline[2])
        else:
            gmap[timeline[1]] = sum(timeline[2])
    guard = max(gmap, key = gmap.get)
    return guard

def guard_sleepiest_minute(timetable, guard):
    mmap = {}

    for timeline in timetable:
        if timeline[1] != guard:
            continue
        i = 0
        while i < 60:
            if not i in mmap:
                mmap[i] = 0
            mmap[i] += timeline[2][i]
            i += 1
            
    minute = max(mmap, key = mmap.get)
    return minute

def part2(timetable):
    # (guard, minute) -> N
    # max(N, (guard, minute))

    gmmap = {}

    for timeline in timetable:
        for i in range(60):
            if timeline[2][i]:
                key = (timeline[1], i)
                if not key in gmmap:
                    gmmap[key] = 0
                gmmap[key] += 1
    
    mkey = None
    mkeym = 0
    for k in gmmap:
        if gmmap[k] > mkeym:
            mkeym = gmmap[k]
            mkey = k

    for k in gmmap:
        if gmmap[k] == mkeym:
            print(k)

    #print(gmmap)
    return mkey

def main():
    events = read_input()
    events.sort()
    timetable = simulate(events)
    visualize_timetable(timetable)
    guard = find_sleepiest_guard(timetable)
    minute = guard_sleepiest_minute(timetable, guard)
    print(f"Part 1: {guard * minute}")
    mkey = part2(timetable)
    print(f"Part 2: {mkey[0]}x{mkey[1]} = {mkey[0] * (mkey[1])}")

main()
