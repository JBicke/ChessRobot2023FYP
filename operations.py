from pypose import runRobot

def orientSquares(Move):
	moveLocations = runRobot.convertToNumber(Move)
	col1, row1, col2, row2 = moveLocations
	row1 = int(row1)
	col1 = int(col1)	
	row2 = int(row2)
	col2 = int(col2)
	
	row1 = 9-row1
	row2 = 9-row2
	
	return row1, row2, col1, col2  

def adjust_Piece_Matrix(piece_Matrix, row1, col1, row2, col2, promotion):
	pawn_Move = False
	if piece_Matrix[row1-1][col1-1] == 2:
		piece_Matrix[row1-1][col1-1] = piece_Matrix[row1-1][col1-1] - 2
		pawn_Move = True
	else:
		piece_Matrix[row1-1][col1-1] = piece_Matrix[row1-1][col1-1] - 1	
	if piece_Matrix[row2-1][col2-1] == 1: # This is true if a piece is to be taken
		if pawn_Move == True:
			piece_Matrix[row2-1][col2-1] = piece_Matrix[row2-1][col2-1] + 1
	elif piece_Matrix[row2-1][col2-1] == 2:
		if pawn_Move == False:
			piece_Matrix[row2-1][col2-1] = piece_Matrix[row2-1][col2-1] - 1
	else:
		if pawn_Move == False:
			piece_Matrix[row2-1][col2-1] = piece_Matrix[row2-1][col2-1] + 1
		else:
			piece_Matrix[row2-1][col2-1] = piece_Matrix[row2-1][col2-1] + 2
	
	if promotion == True:
		piece_Matrix[row2-1][col2-1] = piece_Matrix[row2-1][col2-1] - 1
	
	return piece_Matrix

def adjust_Piece_Matrix_Castle(piece_Matrix,castleType):
	if castleType == "K":
		piece_Matrix[7][7] = piece_Matrix[7][7] - 1
		piece_Matrix[7][5] = piece_Matrix[7][5] + 1
	if castleType == "Q":
		piece_Matrix[7][0] = piece_Matrix[7][0] - 1
		piece_Matrix[7][3] = piece_Matrix[7][3] + 1
	if castleType == "k":
		piece_Matrix[0][7] = piece_Matrix[0][7] - 1
		piece_Matrix[0][5] = piece_Matrix[0][5] + 1	
	if castleType == "q":
		piece_Matrix[0][0] = piece_Matrix[0][0] - 1
		piece_Matrix[0][3] = piece_Matrix[0][3] + 1
	return piece_Matrix

def CastleCheckWhite(Castle_Fen):
	if Castle_Fen.find('K') != -1:
		Castle_K = True
	else:
		Castle_K = False
	if Castle_Fen.find('Q') != -1:
		Castle_Q = True
	else:
		Castle_Q = False
	return Castle_K, Castle_Q

def CastleCheckBlack(Castle_Fen):
	if Castle_Fen.find('k') != -1:
		Castle_K = True
	else:
		Castle_K = False
	if Castle_Fen.find('q') != -1:
		Castle_Q = True
	else:
		Castle_Q = False
	return Castle_K, Castle_Q

	
