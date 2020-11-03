import random
import copy


class Sudoku:
    def __init__(self):
        self.__board = [["0" for col in range(9)] for row in range(9)]

    # 展示数独盘面
    def show_sudoku(self):
        for row in self.__board:
            for grid in row:
                if grid == 0:
                    print("?\t",end="")
                else:
                    print(grid + "\t", end="")
            print("")

    # 清理数独盘面为空
    def clear(self):
        self.__board = [["0" for col in range(9)] for row in range(9)]

    # 清理数独盘面某一行为空
    def clear_row(self, row):
        self.__board[row] = ["0"] * 9

    # 检测对应坐标的方格中是否可以存放提供的数字
    def if_available(self, rno, cno, value):
        for row in self.__board:
            if value == row[cno]:
                return False
        for column in self.__board[rno]:
            if value == column:
                return False
        block_headr = rno // 3 * 3
        block_hedac = cno // 3 * 3
        for row in range(3):
            for column in range(3):
                if value == self.__board[block_headr + row][block_hedac + column]:
                    return False
        return True

    # 生成完整的数独盘面
    def generate(self):
        numbers = []
        for i in range(1, 10):
            # 可选的九个数字
            numbers.append(str(i))
        row = 0
        column = 0
        impasse = False
        while row < 9:
            # 本行中还可选的数字
            row_selectable = copy.deepcopy(numbers)
            while column < 9:
                # 如果现有数独的生成还有解
                if not impasse:
                    # 本格中还可选的数字
                    grid_selectable = copy.deepcopy(row_selectable)
                    temp = random.choice(grid_selectable)
                    while not self.if_available(row, column, temp):
                        grid_selectable.remove(temp)
                        if len(grid_selectable) == 0:
                            impasse = True
                            break
                        temp = random.choice(grid_selectable)
                    if not impasse:
                        self.__board[row][column] = temp
                        row_selectable.remove(temp)
                        column = column + 1
                else:
                    self.clear_row(row)
                    column = 0
                    impasse = False
                    row_selectable = copy.deepcopy(numbers)
            row = row + 1
            column = 0

    # 从现有的数独完盘中，挖掉数字，形成数独问题
    def quiz(self):
        row = random.randint(0, 8)
        column = random.randint(0, 8)
        # 难度系数，即尝试挖洞次数
        hard = 20
        go_deep = True
        while go_deep:
            solution = 0
            temp = self.__board[row][column]
            self.__board[row][column] = 0
            for i in range(1, 10):
                if self.if_available(row, column, str(i)):
                    solution = solution + 1
            if solution > 1:
                self.__board[row][column] = temp
                if hard < 1:
                    go_deep = False
                else:
                    hard = hard - 1
                    row = random.randint(0, 8)
                    column = random.randint(0, 8)
            else:
                hard = hard - 1
                row = random.randint(0, 8)
                column = random.randint(0, 8)
        self.show_sudoku()

    # 解决数独问题
    def solve(self):
        buffer = []
        row = 0
        column = 0
        if_rollback = False
        start_point = 1
        while row < 9:
            while column < 9:
                if self.__board[row][column] == 0:
                    i = start_point
                    while i < 10:
                        if self.if_available(row, column, str(i)):
                            self.__board[row][column] = str(i)
                            buffer.append((row, column, i))
                            start_point = 1
                            break
                        else:
                            i = i + 1
                    if_rollback = (i == 10)
                column = column + 1
            row = row + 1
            column = 0
            if if_rollback:
                last = buffer.pop()
                row = last[0]
                column = last[1]
                start_point = last[2] + 1
                if_rollback = False
        print("谜题解析中……")
        self.show_sudoku()






