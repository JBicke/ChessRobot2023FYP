from pypose import runRobot
import subprocess
import operations


#Run the C++ program as a subprocess
#message_to_cpp = "h2h4 g7g5 h4g5 h7h6 g5h6 h8h7 h1h2 h7g7 h6h7 g8f6 "
#message_to_cpp = "g2g4 h7h5 h2h3 h5g4 h1h2 g4h3 h2g2 h3h2 "
#message_to_cpp = "e2e4 c7c6 e4e5 d7d5 "
#message_to_cpp = ""
#message_to_cpp = "b1c3 e7e5 b2b3 d8f6 c1b2 f8b4 e2e4 d7d6 d1e2 c8e6 "

piece_Matrix =  [[0 for _ in range(8)] for _ in range(8)]
#piece_Matrix[0] = [1]*8 # Row 8
#piece_Matrix[1] = [1]*8 # Row 7
#piece_Matrix[6] = [1]*8 # Row 2
#piece_Matrix[7] = [1]*8 # Row 1 

piece_Matrix = [
    [1, 1, 1, 1, 1, 1, 1, 1],
    [2, 2, 2, 2, 2, 2, 2, 2],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0, 0],
    [2, 2, 2, 2, 2, 2, 2, 2],
    [1, 1, 1, 1, 1, 1, 1, 1]
]

collision = False
pawn_Move = False

W_Castle_K = True
W_Castle_Q = True
B_Castle_K = True
B_Castle_Q = True

W_promotion = False
B_promotion = False
W_promoted_Piece = ''
B_promoted_Piece = ''

en_passant_P = ''
en_passant_R = ''


while True:
	# print(piece_Matrix)
    photoName = input("Name for photo:")
	
    cpp_process = subprocess.Popen(["./chessRobot7"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
    cpp_process.stdin.write(photoName)
    cpp_process.stdin.flush()
    detectedMove = cpp_process.stdout.readline()	
    cpp_process.stdin.close()
    cpp_process.stdout.close()
    cpp_process.wait()
    
	
    cpp_process = subprocess.Popen(["./chessRobot2"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
	# Send a message to the C++ program for player move (for analysing castling & en passent)
    cpp_process.stdin.write(message_to_cpp + "\n")
	
    cpp_process.stdin.flush()
	
    response_from_cpp = cpp_process.stdout.readline()
	
    playerMove = detectedMove   
    if len(playerMove) == 5:
        W_promoted_Piece = playerMove[4]
        W_promotion = True
        playerMove = playerMove[:4]
		
    
    FEN_Player = runRobot.extract_Text(response_from_cpp,"Fen: ")
	
    print(FEN_Player)
	
    Line1 = runRobot.extract_Text(FEN_Player," ")
	
	#print(Line1)
	
    Line2 = runRobot.extract_Text(Line1," ")
	
	#Store player en passant possibility from Fen_Player
    en_passant_P = runRobot.extract_eP(Line2)
	
	#print(Line2)

    Castle_Player_Fen = runRobot.del_Text_After(Line2," ")
	
	
    cpp_process.stdin.close()
    cpp_process.stdout.close()
    cpp_process.wait()
	
    cpp_process = subprocess.Popen(["./chessRobot2"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)

	# Send a message to the C++ program for stockfish move
    message_to_cpp = message_to_cpp + detectedMove
	
    cpp_process.stdin.write(message_to_cpp + "\n")
    cpp_process.stdin.flush()

	# Read the response from the C++ program
    response_from_cpp = cpp_process.stdout.readline()
	# print("Python received:", response_from_cpp)
	
    if response_from_cpp[4] == ' ':
        stockMove = response_from_cpp[:4]
    else:
        stockMove = response_from_cpp[:5]
		# print("Promotion Detected")
        B_promoted_Piece = stockMove[4]
        B_promotion = True
		
    message_to_cpp = message_to_cpp + " " + stockMove + " "
	
	
    FEN_Stock = runRobot.extract_Text(response_from_cpp,"Fen: ")
	
    print(FEN_Stock)
	
    Line1 = runRobot.extract_Text(FEN_Stock," ")
	
    Line2 = runRobot.extract_Text(Line1," ")
	
	#Store robot en passant possibility from Fen_Stock

    en_passant_R = runRobot.extract_eP(Line2)


    Castle_Fen = runRobot.del_Text_After(Line2," ")
	
	# Close the subprocess
    cpp_process.stdin.close()
    cpp_process.stdout.close()
    cpp_process.wait()
		
	
	
	# turns the player move into the correct matrix references
    row1, row2, col1, col2 = operations.orientSquares(playerMove)
	
    print(piece_Matrix)

	# Required to store information about the pawn move for en passant later
    if piece_Matrix[row1-1][col1-1] == 2:
        pawn_Move = True

	# Remove piece if in the way
    if piece_Matrix[row2-1][col2-1] == 1:
        runRobot.takePiece(row2,col2)
    elif piece_Matrix[row2-1][col2-1] == 2:
        runRobot.takePiece(row2,col2)
	
	# Adjust the matrix for the player move 
    piece_Matrix = operations.adjust_Piece_Matrix(piece_Matrix, row1, col1, row2, col2, W_promotion)
		
    print(piece_Matrix)

	# Make the player move
    final = runRobot.movePiece(playerMove)
	
	#Check if special case for en passant is required
    if(en_passant_P == playerMove[-2:]):
        if pawn_Move == True: # en passant happened			
            runRobot.takePiece(row2+1,col2)
            piece_Matrix[row2][col2-1] = piece_Matrix[row2][col2-1] - 2

	#Reset pawn move check			
    pawn_Move = False
	
    W_Castle_K, W_Castle_Q = operations.CastleCheckWhite(Castle_Player_Fen)
	
	# If it is a castle move and they can castle, then castle and update piece matrix
    if playerMove == "e1g1":
        if W_Castle_K == True:
            runRobot.castle("K")
            operations.adjust_Piece_Matrix_Castle(piece_Matrix,"K")
    if playerMove == "e1c1":
        if W_Castle_Q == True:
            runRobot.castle("Q")
            operations.adjust_Piece_Matrix_Castle(piece_Matrix,"Q")
	
	# Special case for if a promotion occurs
    if W_promotion == True:
        runRobot.promotion(playerMove[2:4],'w'+W_promoted_Piece)
	

    row1, row2, col1, col2 = operations.orientSquares(stockMove)
	
    print(piece_Matrix)

    if piece_Matrix[row1-1][col1-1] == 2:
        pawn_Move = True

	# Remove piece if in the way
    if piece_Matrix[row2-1][col2-1] == 1:
        runRobot.takePiece(row2,col2)
    elif piece_Matrix[row2-1][col2-1] == 2:
        runRobot.takePiece(row2,col2)
	
    piece_Matrix = operations.adjust_Piece_Matrix(piece_Matrix, row1, col1, row2, col2, B_promotion)
	
    print(piece_Matrix)
	
	#Do Stockfishes move	
    runRobot.movePiece(stockMove)
	
	#Special case for en passant
    if(en_passant_R == stockMove[-2:]):
        if pawn_Move == True:
			# print('en passant happened')
            runRobot.takePiece(row2-1,col2)
            piece_Matrix[row2-2][col2-1] = piece_Matrix[row2-2][col2-1] - 2
	
    pawn_Move = False
	
    B_Castle_K, B_Castle_Q = operations.CastleCheckBlack(Castle_Fen)

	# If a castling move is made and castling has not happened yet	
    if stockMove == "e8g8":
        if B_Castle_K == True:
            runRobot.castle("k")
            operations.adjust_Piece_Matrix_Castle(piece_Matrix,"k")		
		
    if stockMove == "e8c8":
        if B_Castle_Q == True:
            runRobot.castle("q")
            operations.adjust_Piece_Matrix_Castle(piece_Matrix,"q")

	#Special case for promotion
    if B_promotion == True:
        runRobot.promotion(stockMove[2:4],'b'+B_promoted_Piece)
	
	#Reset variables
    W_promotion = False
    B_promotion = False
    W_promoted_Piece = ''
    B_promoted_Piece = ''
