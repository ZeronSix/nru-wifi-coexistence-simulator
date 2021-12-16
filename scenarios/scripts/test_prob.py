from random import randint

RUNS = 200000

def test():
    d = dict()
    for n in range(16):
        d[n] = 0

    for n in range(20):
        d[randint(0, 15)] += 1

    m = 0
    for n in range(16):
        if d[n] > 0:
            return d[n]

    return m

data = []
for _ in range(RUNS):
    data.append(test())

print(sum(data) / RUNS)
