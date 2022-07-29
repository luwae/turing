import sys

hexdigits = "0123456789abcdef"
filename = sys.argv[1]
nb = 0


with open(filename, "r") as fi, open(f"{filename}.ctm", "wb") as fo:
    for line in fi:
        splits = line.split()
        if not splits:
            print("skip line:", line)
            continue
        raw = splits[0]
        if len(raw) != 2 or raw[0] not in hexdigits or raw[1] not in hexdigits:
            print("skip raw:", raw)
            continue
        value = (hexdigits.index(raw[0]) << 4) | hexdigits.index(raw[1])
        fo.write(bytes([value]))
        nb += 1
print(f"wrote {nb} bytes")
