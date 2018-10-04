import subprocess

for byte_sizes in [100,520,2018,8888,55555,120000,666666,1000000,2000000,3000000]:
	tmp=subprocess.call(["./create_random_file", "mm", "104857600", str(byte_sizes)])

