from sys import argv
from struct import pack
from os import SEEK_END

def write64le(f, n):
    f.write(pack('<q', n))

base_path = argv[2]

with open(argv[1], 'wb') as dst:
    # Count of all files
    write64le(dst, len(argv) - 3)

    for i in range(3, len(argv)):
        # File name
        dst.write(bytes(argv[i] + '\0', encoding='utf-8'))

        with open(base_path + argv[i], 'rb') as src:
            start = src.tell()
            size = src.seek(0, SEEK_END)
            src.seek(start)

            # File size
            write64le(dst, size)

            # File data
            while True:
                data = src.read(0x8000)
                dst.write(data)
                if len(data) < 0x8000:
                    break
