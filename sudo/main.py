import Sudoku


def main():
    sudoku = Sudoku.Sudoku()
    sudoku.generate()
    sudoku.show_sudoku()
    print("生成谜题中……")
    sudoku.quiz()
    sudoku.solve()


main()
