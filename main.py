from pypose import runRobot
import subprocess

# Run the C++ program as a subprocess
message_to_cpp = ""

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
	
	message_to_cpp = message_to_cpp + " " + response_from_cpp[:4] + " "
	
	# Comment for now while testing
	final = runRobot.movePiece(fakeUserMove)
	runRobot.movePiece(response_from_cpp[:4])

	print(final)

	# Close the subprocess
	cpp_process.stdin.close()
	cpp_process.stdout.close()
	cpp_process.wait()
