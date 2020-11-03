class Game:
    def __init__(self) :
        ranks = ["A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"]
        suits = ["s", "h", "c", "d"]
        self.__cards__ = []
        self.__AI__ = []
        self.__player__ = []
        for r in ranks:
            for s in suits:
                self.__cards__.append(r+s)

    def show(self):
        print("电脑拥有的牌为：")
        for a in self.__AI__:
            print(a,end="")
        print("电脑的目前点数为："+self.count("AI"))
        print("玩家拥有的牌为：")
        for p in self.__player__:
            print(p, end="")
        print("玩家的目前点数为：" + self.count("player"))

    def count(self,target):
        points = 0
        if target.equals("AI"):
            for a in self.__AI__:
                points = points + a
        elif target.equals("player"):
            for p in self.__player__:
                points = points + p
        else:
            print("wrong code")
        return  points