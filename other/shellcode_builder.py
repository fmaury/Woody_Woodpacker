import os
import sys

shellcode = ""
hexDmp = os.popen('hexdump -C ' + sys.argv[1]).read()
lines = hexDmp.split('\n')
for line in lines :
    line = line[8:].split('  |')[0]
    shellcode += ' '.join(line.split()).replace(' ','\\x')
print("\\x"+shellcode)