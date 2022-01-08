# Encoin

An effort to implement simple yet full-featured cryptocurrency.

- [Encoin](#encoin)
- [Usage](#usage)
- [Dependencies](#dependencies)
  - [How to install them](#dependencies-installation)


# Usage

```bash
./encoin <command> [options]
```



## Settings

> Read and write settings

```bash
encoin get [OPTION]
encoin set [OPTION]=[VALUE]
```

Available options:

Option| Description| Type| Default
:-----:|:-----:|:-----:|:-----:
`port`|A port that other nodes would use to access your instance|unsigned short|5001
`reward_address`|The address reward fee is sent to|string|null


## Creating test data

> Create test random blocks and transactions locally

```bash
encoin create-random
```


## Run node

> Run node to be accessible from other peers

```bash
encoin run-node
```


## Print blockchain

> Print local copy of blockchain as json

```bash
encoin print-blockchain
```


## Print wallet

> Print all of your addresses and their balances on the net

```bash
encoin print-wallet
```


## Mining

> Begin mining process

```bash
encoin mine
```

## Sending

> Send certain amount of encoins to somebody else's address

```bash
encoin wallet-send --to [TARGET] --amount [AMOUNT] 
```

Available flags:

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
    $ brew install cmake autoconf automake libtool boost libssl-dev
    ```
    On Ubuntu:
    ```sh
    $ sudo apt install cmake autoconf automake libtool libboost-all-dev
    ```

2. Compile and install [`secp256k1`](https://github.com/bitcoin-core/secp256k1#build-steps) library
3. Install header-only [`json`](https://github.com/nlohmann/json#package-managers) and [`termcolor`](https://github.com/ikalnytskyi/termcolor#installation) libraries to `/usr/local/include/`
