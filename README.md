# XORlock - Simple File Encryption Tool

A simple tool to encrypt and decrypt files using XOR encryption.

## Build
```bash
make
```

## Usage

**Encrypt a file:**
```bash
./xorlock -e filename password
```

**Decrypt a file:**
```bash
./xorlock -d filename.enc password
```

**Show help:**
```bash
./xorlock -h
```

## Example
```bash
./xorlock -e secret.txt mypassword123
./xorlock -d secret.txt.enc mypassword123
```

## Warning
- XOR encryption is not strong
- Wrong passwords will not show errors
- Always check if decrypted files are correct
