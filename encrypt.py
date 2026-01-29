import sys

def xor_payload(data, key):
    key_len = len(key)
    return bytes([data[i] ^ ord(key[i % key_len]) for i in range(len(data))])

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: encrypt.py <input_file> <key>")
        sys.exit(1)
    
    with open(sys.argv[1], 'rb') as f:
        data = f.read()
    
    key = sys.argv[2]
    encrypted = xor_payload(data, key)
    
    with open(sys.argv[1] + ".enc", 'wb') as f:
        f.write(encrypted)
