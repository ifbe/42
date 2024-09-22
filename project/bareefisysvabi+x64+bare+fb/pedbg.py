#pip install pefile
#python3.11 pedbg.py

import sys
import pefile

def main():
	pe=pefile.PE(sys.argv[1])
	for section in pe.sections:
		print(section)

if __name__ == "__main__":
	if len(sys.argv) < 2:
		printf("python3.11 pedbg.py xxxx.efi")
		sys.exit(1)
	main()
