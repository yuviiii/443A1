import subprocess

for byte_sizes in [100,2000,30000,80000,160000,900000,1500000,2000000,2500000,3000000]:
	tmp=subprocess.call(["./create_random_file", "mm", "104857600", str(byte_sizes)])

