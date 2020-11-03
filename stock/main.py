import matplotlib.pyplot as plt
from pylab import mpl
mpl.rcParams['font.sans-serif'] = ['SimHei']


def main():
    price = []
    days = []
    avg = []
    total = 0
    infile = open("SH#999999.txt", "r")
    line = infile.readline()
    while line != '':
        amount = float(line[11:18])
        price.append(amount)
        days.append(len(days))
        if len(days) < 60:
            total = total + amount
            avg.append(total/len(days))
        else:
            total = total + amount - price[len(price)-61]
            avg.append(total/60.0)
        line = infile.readline()
    infile.close()
    plt.plot(days, price)
    plt.plot(days, avg)
    plt.title("Stock Index & 60days Move Average line")
    plt.show()


main()
