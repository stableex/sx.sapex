# **`Sapex`**

> Peripheral EOSIO smart contracts for interacting with Sapex exchange

## Dependencies

- [`sx.uniswap`](https://github.com/stableex/sx.uniswap)

## Quickstart

```c++
#include <sx.sapex/sapex.hpp>
#include <sx.uniswap/uniswap.hpp>

// user input
const asset quantity = asset{10000, symbol{"EOS", 4}};
const symbol_out = symbol{"USDT", 4}};

// get sapex info
const auto [ reserve_in, reserve_out ] = sapex::get_reserves( quantity.symbol, symbol_out );
const uint8_t fee = sapex::get_fee();

// calculate out price
const asset out = uniswap::get_amount_out( quantity, reserves_in, reserves_out, fee );
// => "2.6500 USDT"
```

## Table of Content

- [STATIC `get_reserves`](#static-get_reserves)
- [STATIC `get_fee`](#static-get_fee)

## STATIC `get_reserves`

Get reserves for a pair

### params

- `{symbol} symbol_in` - symbol to convert from
- `{symbol} symbol_out` - symbol to convert to

### returns

- `{pair<asset, asset>}` - pair of reserve assets

### example

```c++
const symbol from = symbol{"EOS", 4};
const symbol to = symbol{"USDT", 4};

const auto [reserve0, reserve1] = defibox::get_reserves( from, to );
// reserve0 => "4585193.1234 EOS"
// reserve1 => "12568203.3533 USDT"
```

## STATIC `get_fee`

Get Sapex total fee

### returns

- `{uint8_t}` - total fee (trade + protocol)

### example

```c++
const uint8_t fee = sapex::get_fee();
// => 0
```
