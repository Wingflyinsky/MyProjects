import matplotlib.pyplot as plt
from pylab import mpl
mpl.rcParams['font.sans-serif'] = ['SimHei']


def main():
    price = []
    week = []
    infile = open("1994_Weekly_Gas_Averages.txt", "r")
    line = infile.readline()
    while line != '':
        amount = float(line)
        price.append(amount)
        week.append(len(week) + 1)
        line = infile.readline()
    infile.close()
    plt.plot(week, price)
    plt.title("1994年间每周天然气的平均价格")
    plt.xlabel("周数")
    plt.ylabel("平均价格")
    plt.show()


main()
