

servo_angles_matrix = []
x=1
y=40

for _ in range(8):
	row = []
	for _ in range(8):
		square_angles = [x, y]
		x=x+2
		y=y+1
		row.append(square_angles)
	servo_angles_matrix.append(row)

print(servo_angles_matrix)

rank = 3
file = 4
servo1_angle, servo2_angle = servo_angles_matrix[rank-1][file-1]

print(servo1_angle)
print(servo2_angle)
