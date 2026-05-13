# C++ Password Manager (Learning Project)

A CLI password manager written in C++ using [Crypto++](https://www.cryptopp.com/). 

My first C++ project, written to learn the language and cryptographic primitives.

> **DO NOT ACTUALLY USE THIS!** Use a production tool like [Bitwarden](https://bitwarden.com/), [1Password](https://1password.com/), or [KeePassXC](https://keepassxc.org/). This was a learning project and is for documenting what I learned to myself.

## Features

- Master password setup with strength checking (length, dictionary, sequential/repetitive patterns, case mix)
- PBKDF2-HMAC-SHA256 password hashing
- AES-256-EAX authenticated file encryption with pseudorandom IV
- Credential storage in XML (via [tinyxml2](https://github.com/leethomason/tinyxml2))
- Password generation across alphabet, digits, and symbols

## Cryptographic design

Choices I made and why:

- PBKDF2-HMAC-SHA256, 500,000 iterations for master password key derivation because it's slow and makes offline brute-force costly
- AES-256-EAX for file encryption since authenticated encryption provides confidentiality and integrity, so tampered ciphertext is detected on decryption
- `AutoSeededRandomPool` for Cryptographically secure PRNG for salt and IV generation
- Uses a fresh random IV for each operation! Don't reuse the same IV. It's bad.
- `SecByteBlock` is Crypto++'s type, and inherently limits how long key material lingers in memory

## What I'd do differently now

Things I now recognise as imperfect:

- Remove debug output of secrets — keys, IVs, and ciphertext are currently printed to stdout, leftover from development.
- Mask password input — `std::cin >>` echoes the master password to the terminal; should use `termios` (Unix) or `SetConsoleMode` (Windows) to suppress echo
- Replace recursive retry calls with loops to avoid stack-growth concerns on persistent bad input
- Replace `std::string` with a type that zeroes on destruction for storing secrets in memory
- Use constant-time comparison for hash verification rather than `==`
- Replace `exit(1)` calls inside library functions with proper error returns
- Add a cross-platform CMake build instead of Visual Studio-only `.sln`/`.vcxproj`

This section exists because the value of keeping the project public is showing growth; code is a snapshot of where you were when you wrote it.

## Build

Open `PasswordManager.sln` in Visual Studio. Requires [Crypto++](https://www.cryptopp.com/) installed and linked

## Dependencies

- [Crypto++](https://www.cryptopp.com/): cryptographic primitives
- [tinyxml2](https://github.com/leethomason/tinyxml2): XML parsing (vendored)
- [`words_alpha`](https://github.com/dwyl/english-words): dictionary for strength checking

## License

MIT — see [LICENSE](LICENSE).

## Author

Max Ferstenberg, written July 2025