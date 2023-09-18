from pypose import runRobot
import subprocess

# Run the C++ program as a subprocess
message_to_cpp = ""
piece_Matrix =  [[0 for _ in range(8)] for _ in range(8)]
piece_Matrix[0] = [1]*8 # Row 8
piece_Matrix[1] = [1]*8 # Row 7
piece_Matrix[6] = [1]*8 # Row 2
piece_Matrix[7] = [1]*8 # Row 1 
collision = False

W_Castle_K = True
W_Castle_Q = True
B_Castle_K = True
B_Castle_Q = True


while True:
	fakeUserMove = input("Make a move:")
	cpp_process = subprocess.Popen(["./chessRobot2"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)

	# Send a message to the C++ program
	message_to_cpp = message_to_cpp + fakeUserMove
	
	cpp_process.stdin.write(message_to_cpp + "\n")
	cpp_process.stdin.flush()

	# Read the response from the C++ program
	response_from_cpp = cpp_process.stdout.readline()
	print("Python received:", response_from_cpp)
	
	stockMove = response_from_cpp[:4]
	
	message_to_cpp = message_to_cpp + " " + stockMove + " "
	
	
	FEN = runRobot.extract_Text(response_from_cpp,"Fen: ")
	
	print(FEN)
	
	Line1 = runRobot.extract_Text(FEN," ")
	
	print(Line1)
	
	Line2 = runRobot.extract_Text(Line1," ")
	
	print(Line2)

	Castle_Fen = runRobot.del_Text_After(Line2," ")
	
	print(Castle_Fen)
	
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
	
	if stockMove = "e1g1":
		if W_Castle_K == True:
			#Call Castle Function	
	if stockMove = "e1c1":
		if W_Castle_Q == True:
			#Call Castle Function
		
	if stockMove = "e8g8":
		if B_Castle_K == True:
			#Call Castle Function			
		
	if stockMove = "e8c8":
		if B_Castle_Q == True:
			#Call Castle Function			
	
	
	# Comment for now while testing
	
	moveLocations = runRobot.convertToNumber(fakeUserMove)
	
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
	
	print(piece_Matrix)
	
	final = runRobot.movePiece(fakeUserMove)
	
	moveLocations = runRobot.convertToNumber(stockMove)
	
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
		collision = False
	else:
		piece_Matrix[row2-1][col2-1] = piece_Matrix[row2-1][col2-1] + 1
	
	print(piece_Matrix)
	
	runRobot.movePiece(stockMove)
	

	print(final)

	# Close the subprocess
	cpp_process.stdin.close()
	cpp_process.stdout.close()
	cpp_process.wait()
