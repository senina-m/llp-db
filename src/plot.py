import matplotlib.pyplot as plt

f = open("find", "r").read().strip().split("\n")
a, b = [], []

for i in f:
    x, y = i.split(",")
    a.append(int(x)*5)
    b.append(float(y))

plt.plot(a, b)
plt.xlabel("Количество нод в дереве, шт")
plt.ylabel("Время, с")
plt.title("Поиск по условию")
plt.show()