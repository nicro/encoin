# Encoin

An effort to implement simple yet full-featured cryptocurrency.

- [Encoin](#encoin)
- [Usage](#usage)
- [Dependencies](#dependencies)
  - [How to install them](#dependencies-installation)
- [Compiling](#compiling)


# Usage

```bash
encoin <command> [options]

encoin set [option]
encoin set [option=value]
encoin create-random
encoin run-node
encoin print-blockchain
encoin print-wallet
encoin mine
encoin wallet-send --to [target] --amount [amount]
```


Available setting options:

Option| Description| Type| Default
:-----:|:-----:|:-----:|:-----:
`port`|A port that other nodes would use to access your instance|unsigned short|5001
`reward_address`|The address reward fee is sent to|string|null


Send parameters:

Option| Description| Type| Required
:-----:|:-----:|:-----:|:-----:
`--to`|target address|string|true
`--amount`|amount of encoins|uint64|true




# Dependencies

These libraries are required for building:

* [`boost`](https://www.boost.org/users/download/)
* [`openssl`](https://github.com/openssl/openssl#build-and-install)
* [`secp256k1`](https://github.com/bitcoin-core/secp256k1#build-steps)
* [`termcolor`](https://github.com/ikalnytskyi/termcolor#installation)
* [`json`](https://github.com/nlohmann/json#package-managers)

# Dependencies installation

1. Install boost, openssl and other dependencies:

    On macOS:
    ```sh
    $ brew install cmake autoconf automake libtool boost openssl termcolor nlohmann-json
    ```
    On Ubuntu:
    ```sh
    $ sudo apt install cmake autoconf automake libtool libboost-all-dev
    ```
    Install header-only [`json`](https://github.com/nlohmann/json#package-managers) and [`termcolor`](https://github.com/ikalnytskyi/termcolor#installation) libraries to `/usr/local/include/`
<br />
2. Compile and install `secp256k1` library using these [instructions](https://github.com/bitcoin-core/secp256k1#build-steps)

# Compiling

> Please ensure you have installed all of the libraries from above before compiling the sources

```
$ git clone https://github.com/nicro/encoin.git
$ cd encoin
$ mkdir build && cd build
$ cmake ..
$ make
$ ./encoin
```
