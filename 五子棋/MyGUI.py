import tkinter


class MyGUI:
    def __init__(self):
        self.main_window = tkinter.Tk()
        self.canvas = tkinter.Canvas(self.main_window, width=400, height=400)
        self.canvas.create_rectangle(20, 20, 380, 380, fill='#E3CF57')
        self.canvas.create_rectangle(16, 16, 384, 384, width=2)
        i = 40
        while i < 380:
            self.canvas.create_line(20, i, 380, i)
            self.canvas.create_line(i, 20, i, 380)
            i = i + 20

        for i in range(0, 3):
            self.canvas.create_oval(77, 77+i*120, 83, 83+i*120, fill='black')
            self.canvas.create_oval(197, 77+i*120, 203, 83+i*120, fill='black')
            self.canvas.create_oval(317, 77+i*120, 323, 83+i*120, fill='black')
        self.canvas.
    def run(self):
        self.canvas.pack()
        self.main_window.mainloop()
