#pragma once

#include <eosio/asset.hpp>

namespace sapex {

    using eosio::asset;
    using eosio::symbol;
    using eosio::name;

    /**
     * SAPEX markets
     */
    struct [[eosio::table]] markets_row {
        symbol_code            ticker;
        name                   contract;
        uint64_t               reserve_token;
        uint64_t               reserve_sapex;
        uint64_t primary_key() const { return ticker.raw(); }
    };
    typedef eosio::multi_index< "markets"_n, markets_row > markets;

    /**
     * ## STATIC `get_fee`
     *
     * Get Sapex total fee
     *
     * ### returns
     *
     * - `{uint8_t}` - total fee (trade + protocol)
     *
     * ### example
     *
     * ```c++
     * const uint8_t fee = dfs::get_fee();
     * // => 0
     * ```
     */
    static uint8_t get_fee()
    {
        return 0;
    }

    /**
     * ## STATIC `get_reserves`
     *
     * Get reserves for a pair
     *
     * ### params
     *
     * - `{symbol} sym1` - symbol for reserve1
     * - `{symbol} sym2` - symbol for reserve2
     *
     * ### returns
     *
     * - `{pair<asset, asset>}` - pair of reserve assets
     *```
     */
    static std::pair<asset, asset> get_reserves( symbol sym1, symbol sym2)
    {
        // table
        sapex::markets _markets( "sapexamm.eo"_n, "sapexamm.eo"_n.value );
        if(sym1.code().to_string()=="SAPEX"){
            auto row = _markets.get( sym2.code().raw(), "SAPEXLibrary: INVALID_SYMBOL" );
            return { { (int64_t)row.reserve_sapex,sym1 }, { (int64_t)row.reserve_token, sym2 } };
        }
        if(sym2.code().to_string()=="SAPEX"){
            auto row = _markets.get( sym1.code().raw(), "SAPEXLibrary: INVALID_SYMBOL" );
            return { { (int64_t)row.reserve_token,sym1 }, { (int64_t)row.reserve_sapex, sym2 } };
        }
        //neither is SAPEX
        auto row1 = _markets.get( sym1.code().raw(), "SAPEXLibrary: INVALID_SYMBOL" );
        auto row2 = _markets.get( sym2.code().raw(), "SAPEXLibrary: INVALID_SYMBOL" );
        return { { (int64_t)row1.reserve_token, sym1 }, { (int64_t)row2.reserve_token, sym2 } };
    }
}