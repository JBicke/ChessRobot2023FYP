from pypose import runRobot


while True:
	fakeUserMove = input("Make a move:")
	runRobot.movePiece(fakeUserMove)
