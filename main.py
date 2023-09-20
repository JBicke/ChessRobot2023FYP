from pypose import runRobot
import subprocess

# Run the C++ program as a subprocess
message_to_cpp = "h2h4 g7g5 h4g5 h7h6 g5h6 h8h7 h1h2 h7g7 h6h7 g8f6 "
# message_to_cpp = "g2g4 h7h5 h2h3 h5g4 h1h2 g4h3 h2g2 h3h2 "
# message_to_cpp = ""
piece_Matrix =  [[0 for _ in range(8)] for _ in range(8)]
#piece_Matrix[0] = [1]*8 # Row 8
#piece_Matrix[1] = [1]*8 # Row 7
#piece_Matrix[6] = [1]*8 # Row 2
#piece_Matrix[7] = [1]*8 # Row 1 

piece_Matrix = [
    [1, 1, 1, 1, 1, 1, 0, 0],
    [1, 1, 1, 1, 1, 1, 1, 1],
    [0, 0, 0, 0, 0, 1, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [1, 1, 1, 1, 1, 1, 1, 1],
    [1, 1, 1, 1, 1, 1, 1, 0]
]

collision = False

W_Castle_K = True
W_Castle_Q = True
B_Castle_K = True
B_Castle_Q = True

W_promotion = False
B_promotion = False
W_promoted_Piece = ''
B_promoted_Piece = ''

while True:
	fakeUserMove = input("Make a move:")
	cpp_process = subprocess.Popen(["./chessRobot2"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
	
	# Send a message to the C++ program for player move (for analysing castling & en passent)
	
	cpp_process.stdin.write(message_to_cpp + "\n")
	cpp_process.stdin.flush()
	response_from_cpp = cpp_process.stdout.readline()
	
	#fake user move is length 4 or 5 if promotion
	#playerMove is length 4 always
	
	playerMove = fakeUserMove
	if len(playerMove) == 5:
		W_promoted_Piece = playerMove[4]
		W_promotion = True
		playerMove = playerMove[:4]
	
	FEN_Player = runRobot.extract_Text(response_from_cpp,"Fen: ")
	
	print(FEN_Player)
	
	Line1 = runRobot.extract_Text(FEN_Player," ")
	
	#print(Line1)
	
	Line2 = runRobot.extract_Text(Line1," ")
	
	#print(Line2)

	Castle_Player_Fen = runRobot.del_Text_After(Line2," ")
	
	
	cpp_process.stdin.close()
	cpp_process.stdout.close()
	cpp_process.wait()
	
	cpp_process = subprocess.Popen(["./chessRobot2"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)

	# Send a message to the C++ program for stockfish move
	message_to_cpp = message_to_cpp + fakeUserMove
	
	cpp_process.stdin.write(message_to_cpp + "\n")
	cpp_process.stdin.flush()

	# Read the response from the C++ program
	response_from_cpp = cpp_process.stdout.readline()
	print("Python received:", response_from_cpp)
	
	if response_from_cpp[4] == ' ':
		stockMove = response_from_cpp[:4]
	else:
		stockMove = response_from_cpp[:5]
		print("Promotion Detected")
		B_promoted_Piece = stockMove[4]
		B_promotion = True
		
	message_to_cpp = message_to_cpp + " " + stockMove + " "
	
	
	FEN_Stock = runRobot.extract_Text(response_from_cpp,"Fen: ")
	
	#print(FEN_Stock)
	
	Line1 = runRobot.extract_Text(FEN_Stock," ")
	
	#print(Line1)
	
	Line2 = runRobot.extract_Text(Line1," ")
	
	#print(Line2)

	Castle_Fen = runRobot.del_Text_After(Line2," ")
	
			
	
	
	# Comment for now while testing
	
	moveLocations = runRobot.convertToNumber(playerMove)
	print(playerMove)
	print(moveLocations)
	col1, row1, col2, row2 = moveLocations
	row1 = int(row1)
	col1 = int(col1)	
	row2 = int(row2)
	col2 = int(col2)
	
	row1 = 9-row1
	row2 = 9-row2
	
	piece_Matrix[row1-1][col1-1] = piece_Matrix[row1-1][col1-1] - 1
	
	if piece_Matrix[row2-1][col2-1] == 1: # This is true if a piece is to be taken
		collision = True
		runRobot.takePiece(row2,col2)
		print("Collision Detected")
		collision = False
	else:
		piece_Matrix[row2-1][col2-1] = piece_Matrix[row2-1][col2-1] + 1
	
	#print(piece_Matrix)
	
	final = runRobot.movePiece(playerMove)
	
	if Castle_Player_Fen.find('K') != -1:
		W_Castle_K = True
	else:
		W_Castle_K = False
	
	if Castle_Player_Fen.find('Q') != -1:
		W_Castle_Q = True
	else:
		W_Castle_Q = False
	
	if Castle_Player_Fen.find('k') != -1:
		B_Castle_K = True
	else:
		B_Castle_K = False
	
	if Castle_Player_Fen.find('q') != -1:
		B_Castle_Q = True
	else:
		B_Castle_Q = False
	
	print(playerMove)
	print(W_Castle_K)
	
	if playerMove == "e1g1":
		if W_Castle_K == True:
			runRobot.castle("K")
			piece_Matrix[7][7] = piece_Matrix[7][7] - 1
			piece_Matrix[7][5] = piece_Matrix[7][5] + 1
	if playerMove == "e1c1":
		if W_Castle_Q == True:
			runRobot.castle("Q")
			piece_Matrix[7][0] = piece_Matrix[7][0] - 1
			piece_Matrix[7][3] = piece_Matrix[7][3] + 1
		
	if playerMove == "e8g8":
		if B_Castle_K == True:
			runRobot.castle("k")
			piece_Matrix[0][7] = piece_Matrix[0][7] - 1
			piece_Matrix[0][5] = piece_Matrix[0][5] + 1			
		
	if playerMove == "e8c8":
		if B_Castle_Q == True:
			runRobot.castle("q")
			piece_Matrix[0][0] = piece_Matrix[0][0] - 1
			piece_Matrix[0][3] = piece_Matrix[0][3] + 1
	
	if W_promotion == True:
		runRobot.promotion(playerMove[2:4],'w'+W_promoted_Piece)
	
	moveLocations = runRobot.convertToNumber(stockMove)
	
	print(stockMove)
	print(moveLocations)
	
	col1, row1, col2f, row2f = moveLocations[:4]
	row1 = int(row1)
	col1 = int(col1)	
	row2 = int(row2f)
	col2 = int(col2f)
	
	row1 = 9-row1
	row2 = 9-row2
	
	piece_Matrix[row1-1][col1-1] = piece_Matrix[row1-1][col1-1] - 1

	
	if piece_Matrix[row2-1][col2-1] == 1: # This is true if a piece is to be taken
		collision = True
		runRobot.takePiece(row2,col2)
		collision = False
	else:
		piece_Matrix[row2-1][col2-1] = piece_Matrix[row2-1][col2-1] + 1
	
	#print(piece_Matrix)
	
	runRobot.movePiece(stockMove)
	
	if Castle_Fen.find('K') != -1:
		W_Castle_K = True
	else:
		W_Castle_K = False
	
	if Castle_Fen.find('Q') != -1:
		W_Castle_Q = True
	else:
		W_Castle_Q = False
	
	if Castle_Fen.find('k') != -1:
		B_Castle_K = True
	else:
		B_Castle_K = False
	
	if Castle_Fen.find('q') != -1:
		B_Castle_Q = True
	else:
		B_Castle_Q = False
	
	if stockMove == "e1g1":
		if W_Castle_K == True:
			runRobot.castle("K")
			piece_Matrix[7][7] = piece_Matrix[7][7] - 1
			piece_Matrix[7][5] = piece_Matrix[7][5] + 1
	if stockMove == "e1c1":
		if W_Castle_Q == True:
			runRobot.castle("Q")
			piece_Matrix[7][0] = piece_Matrix[7][0] - 1
			piece_Matrix[7][3] = piece_Matrix[7][3] + 1
		
	if stockMove == "e8g8":
		if B_Castle_K == True:
			runRobot.castle("k")
			piece_Matrix[0][7] = piece_Matrix[0][7] - 1
			piece_Matrix[0][5] = piece_Matrix[0][5] + 1			
		
	if stockMove == "e8c8":
		if B_Castle_Q == True:
			runRobot.castle("q")
			piece_Matrix[0][0] = piece_Matrix[0][0] - 1
			piece_Matrix[0][3] = piece_Matrix[0][3] + 1
	
	if B_promotion == True:
		runRobot.promotion(stockMove[2:4],'b'+B_promoted_Piece)
	
	W_promotion = False
	B_promotion = False
	W_promoted_Piece = ''
	B_promoted_Piece = ''
	
	print(final)

	# Close the subprocess
	cpp_process.stdin.close()
	cpp_process.stdout.close()
	cpp_process.wait()
