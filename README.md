# encoin

An effort to implement simple yet full-featured cryptocurrency.

## Dependencies

These libraries are required for building:

* [`boost`](https://www.boost.org/users/download/)
* [`openssl`](https://github.com/openssl/openssl#build-and-install)
* [`secp256k1`](https://github.com/bitcoin-core/secp256k1#build-steps)
* [`termcolor`](https://github.com/ikalnytskyi/termcolor#installation)
* [`json`](https://github.com/nlohmann/json#package-managers)

<details><summary><b>Show instructions</b></summary>

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
3. Install header-only json and [`json`](https://github.com/nlohmann/json#package-managers) and [`termcolor`](https://github.com/ikalnytskyi/termcolor#installation) libraries to `/usr/local/include/`
    
</details>
