from pypose import runRobot
import subprocess
import operations

cpp_process = subprocess.Popen(["./take_Img"], stdin=subprocess.PIPE, stdout=subprocess.PIPE, text=True)
cpp_process.stdin.write("140")
cpp_process.stdin.flush()

cpp_process.stdin.close()
cpp_process.stdout.close()
cpp_process.wait()
#response_from_cpp = cpp_process.stdout.readline()
