#pragma once

#include <eosio/asset.hpp>

namespace sapex {

    using eosio::asset;
    using eosio::symbol;
    using eosio::name;
    using eosio::symbol_code;
    using eosio::check;

    const name id = "sapex"_n;
    const name code = "sapexamm.eo"_n;
    const std::string description = "Sapex Swap Converter";
    const eosio::extended_symbol SAPEX { symbol{"SAPEX",4}, "sapexcoin.eo"_n };

    /**
     * SAPEX markets
     */
    struct [[eosio::table]] sapextbl_row {
        symbol_code            ticker;
        name                   contract;
        uint64_t               token;
        uint64_t               sapex;
        uint64_t primary_key() const { return ticker.raw(); }
    };
    typedef eosio::multi_index< "sapextbl"_n, sapextbl_row > sapextbl;

    /**
     * ## STATIC `get_fee`
     *
     * Get Sapex fee. 0.10%
     *
     * ### params
     *
     *
     * ### returns
     *
     * - `{uint8_t}` - total fee (trade + protocol)
     *
     * ### example
     *
     * ```c++
     * const uint8_t fee = sapex::get_fee();
     * // => 0
     * ```
     */
    static uint8_t get_fee()
    {
        return 10;
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
    static std::pair<uint64_t, uint64_t> get_reserves( symbol sym1, symbol sym2)
    {
        check(sym1!=sym2, "SAPEXLibrary: INVALID_PAIR");
        // table
        sapex::sapextbl _markets( sapex::code, sapex::code.value );
        if(sym1 == SAPEX.get_symbol()){   //SAPEX->XXX
            auto row = _markets.get( sym2.code().raw(), "SAPEXLibrary: INVALID_SYMBOL" );
            return { row.sapex, row.token};
        }
        if(sym2 == SAPEX.get_symbol()){   //XXX->SAPEX
            auto row = _markets.get( sym1.code().raw(), "SAPEXLibrary: INVALID_SYMBOL" );
            return { row.token, row.sapex };
        }
        check(false, "SAPEXLibrary: Getting reserves for non-Sapex pair");
        return {0, 0}; //unreachable
    }

    /**
     * ## STATIC `get_amount_out`
     *
     * Get expected conversion return
     *
     * ### params
     *
     * - `{asset} in` - amount in
     * - `{symbol} out_sym` - return symbol
     *
     * ### returns
     *
     * - `{asset}` - expected return
     *```
     */
    static asset get_amount_out(asset in, symbol out_sym) {

        check(in.symbol != out_sym, "SAPEXLibrary: INVALID_PAIR");
        double fee = get_fee();
        if(in.symbol!=SAPEX.get_symbol() && out_sym!=SAPEX.get_symbol()) {      //if XXX->YYY - convert XXX to SAPEX first
            auto [res_in, res_out] = get_reserves(in.symbol, SAPEX.get_symbol());
            in.amount = res_out * (static_cast<double>(in.amount) / (res_in + in.amount));
            in.amount *= (10000-fee)/10000;
            in.symbol = SAPEX.get_symbol();
        }

        //now guaranteed to be XXX->SAPEX or SAPEX->YYY
        auto [res_in, res_out] = get_reserves(in.symbol, out_sym);
        auto amount_out = res_out * (static_cast<double>(in.amount) / (res_in + in.amount));
        amount_out *= (10000-fee)/10000;
        return { static_cast<int64_t>(amount_out), out_sym };
    }
}