# Python Class 2563
# Lesson 9 Problem 1
# Author: DecagonX (531821)

from tkinter import *
from tkinter import messagebox
import random
import math

#use relief='RAISED' for the unexposed squares and
#relief='SUNKEN'#$ for the exposed squares

class mineSweeperCell(Label):

    def __init__(self, master, coord, mine = False):
        '''mineSweeperCell(master,coord) -> mineSweeperCell
        creates a new blank mineSweeperCell with (row,column) coord
         boolean mine -> if the cell contains a mine'''
        
        Label.__init__(self, master, height = 1, width = 2, text = '', \
                       bg = 'white', font = ('Arial', 24), relief = 'raised')
        self.master = master 
        self.coord = coord #(row, column) coordinate tuple
        self.exposed = False
        self.flagged = False
        self.mine = mine
        self.numOfMinesNear = 0
        #set up listeners 
        self.bind('<Button-1>', self.expose_event)
        self.bind('<Button-2>', self.flag)
        #color for different number
        self.colormap = ['','blue','darkgreen','red','purple','maroon','cyan','black','dim gray']
        self.grid(row = coord[1], column = coord[0])
    def get_coord(self):
        '''return the coordinate of the cell in a tuple'''
        return self.coord
    def get_x_coord(self):
        '''return the x-coordinate of the cell'''
        return self.coord[0]
    def get_y_coord(self):
        '''return the y-coordinate of the cell'''
        return self.coord[1]
    def get_mine(self):
        '''return the boolean if the cell has mine'''
        return self.mine
    def get_num_of_mines(self):
        '''return the num of mines near the cell'''
        return self.numOfMinesNear
    def get_exposed_state(self):
        '''return if the cell is exposed'''
        return self.exposed
    def __str__(self):
        '''Backdoor to know the cells properties'''
        return 'A Mine Sweeping Cell \n' + \
                'Coordinate: ' + str(self.coord) + \
                '\nContains Mine: ' + str(self.mine) + \
                '\nNearby Mine Number: ' + str(self.numOfMinesNear)
    
    def set_mine_num(self, num):
        '''set the number of mines nearby'''
        self.numOfMinesNear = num

    def set_mine(self):
        '''give the cell a mine'''
        self.mine = True
        
    def expose(self):
        '''left click -- expose the cell'''
        #first check flag
        if not self.exposed:
            if not self.flagged: #can only expose not flagged cells
                #then check mine
                if self.mine: #if it is a bomb -> lose
                    print('Oops, A Bomb!')
                    self['bg'] = 'red'
                    self['text'] = 'B'
                    self.master.lose_win('lose')
                    messagebox.showerror('Minesweeper','KABOOM! You lose.', parent=self.master)
                
                else: # if it is not a mine
                    self.exposed = True
                    self['relief'] = 'sunken'
                    #then check if there is mine around
                    if self.numOfMinesNear > 0:
                        colorIndex = self.numOfMinesNear
                        self['text'] = str(colorIndex)
                        self['bg'] = 'grey'
                        self['fg'] = self.colormap[colorIndex]

                    else:
                        self['bg'] = 'grey' #no bomn near, grey background
                    
        
                
    def expose_event(self, event):
        '''accomodate the function for putting onto the button'''
        self.expose()
        #use the blank expose from the grid to auto-expose all blank cell connected to it
        self.master.blank_expose()
        self.master.update_score()

    def flag(self, event):
        '''right click -- flag the cell'''
        if not self.exposed: #if it is not exposed
            self.flagged = not self.flagged #unflag or flag the cell
            if self.flagged: #currently flagged
                self['text'] = '*'
                self.master.flagged += 1
                self.master.update_score()
            elif not self.flagged: #currently not flagged
                self['bg'] = 'white'
                self['text'] = ''
                
                self.master.flagged -= 1
                self.master.update_score()
    def unflag(self):
        '''unflag the cell, not connected to button'''
        self.flagged = False   

class mineSweeperGrid(Frame):
    '''object for a mineSweeper grid
        take integer arguments, width, height and number of mines'''
    def __init__(self, master, width, height, numOfMine):
        #initialize a neww Frame
        #, width, height, numOfMine
        Frame.__init__(self, master, bg = 'black')
        self.grid()
        self.win = False #won boolean variable 
            #this attribute of the grid will determine the function of update_score()
            #update_socre is used in all expose functions
            #expose functions is used in blank_expose
            #if not modified after winning, update_score will keep going
            #lose_win will keep going, and keep popping up message box
        self.width = width
        self.height = height
        self.numOfMine = numOfMine #number of mines on the grid
        self.flagged = 0 #number of cells flagged
        self.numOfMineLeft = self.numOfMine
        #store the cells in a 1D list to set the mines later
        self.booleanList = []
        for i in range(self.width*self.height):
            self.booleanList.append(False)
        num = self.numOfMine
        while num != 0: #if the random choice met a false, re-choose the index
            index = random.randint(0, len(self.booleanList)-1)
            if self.booleanList[index] == False:
                self.booleanList[index] = True
                num -= 1 
            else:
                continue
            
        self.cells = [] #store the cells in a 2D list
        #create a nested arrays of the individual cells
        #access by the self.cells[x-cor][y-cor]
        #column index will be x-cor, row index will be y_cor
        i = 0
        for column in range(self.width):
            self.cells.append([])
            for row in range(self.height):
                self.cells[column].append(mineSweeperCell(self, (column, row), mine = self.booleanList[i]))
                i+=1
        #calculate the number of mines in nearby cells for all cells initilization
        #Also return the number into the individual cells
        self.calc_mines_in_nearby_cells()

        #create the socre label
        self.scoreLabel = Label(self, height = 1, width = self.width*2, \
                                text = "Mines Left: " + str(self.numOfMine), \
                       bg = 'white', font = ('Arial', 20))
        self.scoreLabel.grid(row = self.height, column = 0, \
                        sticky = '', columnspan = self.width) #stay in the middle
        #back door to expose all cells for debugging (and cheating (: )
        #self.expose_all_cells()
        
    def update_score(self):
        '''update the score label down the cell board'''
        self.scoreLabel['text'] = "Mines Left: " + str(self.numOfMine - self.flagged)
        if not self.win: #hasn't won yet
            #if already won, stop functioning bascially
            #or after the player wins, the functiono will keep going
            #popping numerous meesage boxes
            #since update_score is used in numerous places: expose function
            #expose function is used in blank_expose ......   
            deminedCells = []
            exposedCells = []
            for column in range(self.width):
                for row in range(self.height):
                    theCell = self.cells[column][row]
                    if theCell.flagged and theCell.get_mine():#flag the correct cells 
                       deminedCells.append(theCell)
                    elif theCell.get_exposed_state(): #all the exposed cells (not flagged)
                        exposedCells.append(theCell)

            self.numOfMineLeft = self.numOfMine - len(deminedCells)
            exposedcorrect = (self.width*self.height) - self.numOfMine
            #print(self.numOfMineLeft) #for debugging
            

            #flag all the mine cells and expose all the remaining cells to win
            if self.numOfMineLeft == 0 and exposedcorrect == len(exposedCells):
                print("YOU WON!!!!")
                self.lose_win('win')
                     
    def return_nearby_cells(self, Cell):
        '''return all 8 cells near Cell in a 1D list'''
        nearbyCells = []
        xcenter = Cell.get_x_coord()
        ycenter = Cell.get_y_coord()

        for x in range(-1, 2):
            for y in range(-1, 2):
                #excluding all the conditions where the cell is outside of the grid
                if not (xcenter+x < 0 or xcenter+x >= self.width \
               or ycenter+y < 0 or ycenter+y >= self.height):
                    nearbyCells.append(self.cells[xcenter+x][ycenter+y])

        #take off the x = 0, y = 0 condition from the list
        #(take off the center cell itself)
        nearbyCells.remove(self.cells[xcenter][ycenter]) 
        return nearbyCells
    
    def expose_all_cells(self):
        '''expose all the cells on the grid'''
        for column in range(self.width):
            for row in range(self.height):
                self.cells[column][row].expose()

    def calc_mines_in_nearby_cells(self):
        '''For ALL cells on the grid
        calculate the number of mines surrounding the cells 
        return this number to the individual cells'''
        for x in range(self.width):
            for y in range(self.height):
                numOfmines = 0
                Centercell = self.cells[x][y]
                if not Centercell.mine: #if the identified cell does not contain mine, proceed 
                    nearbyCells = self.return_nearby_cells(Centercell)
                    for cell in nearbyCells:
                        if cell.get_mine():
                            numOfmines += 1
                            
                    #return this number into the individual cell
                    Centercell.set_mine_num(numOfmines)
        
    def blank_expose(self):
        '''A function that will expose all cells nearby if the cell is blank'''
        #This is messy but it works, and is quite safe
        #repeat checking from four directions
        #-> ensure all blank cells connected are auto-exposed
        #left to right, top to bottom
        for column in range(self.width): #x cor
            for row in range(self.height): #y cor
                centerCell = self.cells[column][row]
                if centerCell.get_exposed_state() and (not centerCell.get_mine()) \
                   and centerCell.get_num_of_mines() == 0:
                    nearbyCell = self.return_nearby_cells(centerCell)
                    for theCell in nearbyCell:
                        theCell.expose()
        #right to left, bottom to top
        for column in range(self.width-1, 0, -1):
            for row in range(self.height-1, 0, -1):
                centerCell = self.cells[column][row]
                if centerCell.get_exposed_state() and (not centerCell.get_mine()) \
                   and centerCell.get_num_of_mines() == 0:
                    nearbyCell = self.return_nearby_cells(centerCell)
                    for theCell in nearbyCell:
                        theCell.expose()
        #right to left, top to bottom
        for column in range(self.width-1, 0, -1):
            for row in range(self.height):
                centerCell = self.cells[column][row]
                if centerCell.get_exposed_state() and (not centerCell.get_mine()) \
                   and centerCell.get_num_of_mines() == 0:
                    nearbyCell = self.return_nearby_cells(centerCell)
                    for theCell in nearbyCell:
                        theCell.expose()
        #left to right, bottom to top
        for column in range(self.width):
            for row in range(self.height-1, 0, -1):
                centerCell = self.cells[column][row]
                if centerCell.get_exposed_state() and (not centerCell.get_mine()) \
                   and centerCell.get_num_of_mines() == 0:
                    nearbyCell = self.return_nearby_cells(centerCell)
                    for theCell in nearbyCell:
                        theCell.expose()
                        
    def lose_win(self, state):
        '''what happens after winning or losing'''
        if state == 'win':
            self.win = True #already won
            #stop most of the function of update_score()
            
        for column in range(self.width): #x cor
            for row in range(self.height): #y cor
                centerCell = self.cells[column][row]
                #centerCell.unflag()
                #centerCell.expose()
                if state == 'lose': #lose condition
                    if centerCell.get_mine():
                        centerCell['text'] = 'B'
                        centerCell['bg'] = 'red'
                    
                elif state == 'win':#win condition
                    messagebox.showinfo('Minesweeper','Congratulations -- you won!',parent=self)
                    state = ''
                    if centerCell.get_mine():
                        centerCell['text'] = 'B'
                        centerCell['fg'] = 'green'
                        centerCell['bg'] = 'white'
        
def playMineSweeper(width = 12, height = 10, numOfMines = 10):
    '''play the Mine Sweeper Game
        Set with integer -> width, height, Number of mines
        Default: Width = 12, Height = 10, Number of mines = 15'''
    root = Tk()
    root.title('Mine Sweeper')
    game = mineSweeperGrid(root, width, height, numOfMines)
    
    root.mainloop()

playMineSweeper(10, 10, 10)
    
        
        
