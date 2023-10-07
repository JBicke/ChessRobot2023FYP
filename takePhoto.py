from pypose import runRobot
import subprocess
import operations

photoName = input("name for photo")
cpp_process = subprocess.Popen(["./take_Img"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
cpp_process.stdin.write(photoName)
cpp_process.stdin.flush()

cpp_process.stdin.close()
cpp_process.stdout.close()
cpp_process.wait()

cpp_process = subprocess.Popen(["./chessRobot7"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
cpp_process.stdin.write(photoName)
cpp_process.stdin.flush()

cpp_process.stdin.close()
cpp_process.stdout.close()
cpp_process.wait()
#response_from_cpp = cpp_process.stdout.readline()
