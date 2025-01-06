import random

days_of_week = ["mon", "tue", "wed", "thu", "fri", "sat", "sun"]
zones = ["z1", "z2", "z3"]
data = []

for _ in range(211):
    day = random.choice(days_of_week)
    zone = random.choice(zones)
    st_HH = random.randint(0, 23)
    st_MM = random.randint(0, 59)
    et_HH = random.randint(st_HH, 23)
    et_MM = random.randint(st_MM, 59)
    workedFlag = random.randint(0, 1)
    enabledFlag = random.randint(0, 1)
    actuatorDetail = hex(random.randint(0, 0xFFFFFFFF))
    
    data.append(f"{day}, {zone}, {st_HH:02d}:{st_MM:02d}, {et_HH:02d}:{et_MM:02d}, {workedFlag}, {enabledFlag}, {actuatorDetail}")

for line in data:
    print(line)
