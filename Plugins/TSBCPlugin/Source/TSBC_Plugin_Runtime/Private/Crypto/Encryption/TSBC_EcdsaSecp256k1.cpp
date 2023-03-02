// Copyright 2022 3S Game Studio OU. All Rights Reserved.

#include "Crypto/Encryption/TSBC_EcdsaSecp256k1.h"

#include "Module/TSBC_RuntimeLogCategories.h"
#include "Crypto/Hash/TSBC_Sha256.h"
#include "Crypto/Hash/TSBC_Sha512.h"
#include "Crypto/Random/TSBC_SecureRandom.h"
#include "Util/TSBC_StringUtils.h"

// @formatter:off
const CTSBC_EcdsaSecp256k1::uECC_Curve CTSBC_EcdsaSecp256k1::curve_secp256k1 = {
    8,
    32,
    256,
    {
        0xFFFFFC2F, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
    },
    {
        0xD0364141, 0xBFD25E8C, 0xAF48A03B, 0xBAAEDCE6,
        0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
    },
    {
        0x7FFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0x5D576E73, 0x57A4501D, 0xDFE92F46, 0x681B20A0
    },
    {
        0x16F81798, 0x59F2815B, 0x2DCE28D9, 0x029BFCDB,
        0xCE870B07, 0x55A06295, 0xF9DCBBAC, 0x79BE667E,
        0xFB10D4B8, 0x9C47D08F, 0xA6855419, 0xFD17B448,
        0x0E1108A8, 0x5DA4FBFC, 0x26A3C465, 0x483ADA77
    },
    {
        0x00000007, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000
    },
    &CTSBC_EcdsaSecp256k1::double_jacobian_secp256k1,
    &CTSBC_EcdsaSecp256k1::mod_sqrt_default,
    &CTSBC_EcdsaSecp256k1::x_side_secp256k1,
    &CTSBC_EcdsaSecp256k1::vli_mmod_fast_secp256k1
};
// @formatter:on


const CTSBC_EcdsaSecp256k1::uECC_Curve* CTSBC_EcdsaSecp256k1::uECC_secp256k1()
{
    return &CTSBC_EcdsaSecp256k1::curve_secp256k1;
}

int32 CTSBC_EcdsaSecp256k1::BITS_TO_WORDS(const int32 num_bits)
{
    return (num_bits + 31) / 32;
}

int32 CTSBC_EcdsaSecp256k1::BITS_TO_BYTES(const int32 num_bits)
{
    return (num_bits + 7) / 8;
}

void CTSBC_EcdsaSecp256k1::mod_sqrt_default(uint32* a, const uECC_Curve* curve)
{
    uint32 p1[8] = {1};
    uint32 l_result[8] = {1};
    const int32 num_words = curve->num_words;

    uECC_vli_add(p1, curve->p, p1, num_words);
    for(int32 i = uECC_vli_numBits(p1, num_words) - 1; i > 1; --i)
    {
        uECC_vli_modSquare_fast(l_result, l_result, curve);
        if(uECC_vli_testBit(p1, i))
        {
            uECC_vli_modMult_fast(l_result, l_result, a, curve);
        }
    }
    uECC_vli_set(a, l_result, num_words);
}

void CTSBC_EcdsaSecp256k1::double_jacobian_secp256k1(
    uint32* X1,
    uint32* Y1,
    uint32* Z1,
    const uECC_Curve* curve)
{
    uint32 t4[8];
    uint32 t5[8];

    if(uECC_vli_isZero(Z1, 8))
    {
        return;
    }

    uECC_vli_modSquare_fast(t5, Y1, curve);
    uECC_vli_modMult_fast(t4, X1, t5, curve);
    uECC_vli_modSquare_fast(X1, X1, curve);
    uECC_vli_modSquare_fast(t5, t5, curve);
    uECC_vli_modMult_fast(Z1, Y1, Z1, curve);

    uECC_vli_modAdd(Y1, X1, X1, curve->p, 8);
    uECC_vli_modAdd(Y1, Y1, X1, curve->p, 8);
    if(uECC_vli_testBit(Y1, 0))
    {
        const bool carry = uECC_vli_add(Y1, Y1, curve->p, 8);
        uECC_vli_rshift1(Y1, 8);
        Y1[8 - 1] |= (carry ? 1 : 0) << (32 - 1);
    }
    else
    {
        uECC_vli_rshift1(Y1, 8);
    }

    uECC_vli_modSquare_fast(X1, Y1, curve);
    uECC_vli_modSub(X1, X1, t4, curve->p, 8);
    uECC_vli_modSub(X1, X1, t4, curve->p, 8);

    uECC_vli_modSub(t4, t4, X1, curve->p, 8);
    uECC_vli_modMult_fast(Y1, Y1, t4, curve);
    uECC_vli_modSub(Y1, Y1, t5, curve->p, 8);
}

void CTSBC_EcdsaSecp256k1::x_side_secp256k1(uint32* result, const uint32* x, const uECC_Curve* curve)
{
    uECC_vli_modSquare_fast(result, x, curve);
    uECC_vli_modMult_fast(result, result, x, curve);
    uECC_vli_modAdd(result, result, curve->b, curve->p, 8);
}

void CTSBC_EcdsaSecp256k1::vli_mmod_fast_secp256k1(uint32* result, uint32* product)
{
    uint32 tmp[2 * 8];

    uECC_vli_clear(tmp, 8);
    uECC_vli_clear(tmp + 8, 8);

    omega_mult_secp256k1(tmp, product + 8);

    uint32 carry = uECC_vli_add(result, product, tmp, 8) ? 1 : 0;
    uECC_vli_clear(product, 8);
    omega_mult_secp256k1(product, tmp + 8);
    carry += uECC_vli_add(result, result, product, 8) ? 1 : 0;

    while(carry > 0)
    {
        --carry;
        uECC_vli_sub(result, result, curve_secp256k1.p, 8);
    }
    if(uECC_vli_cmp_unsafe(result, curve_secp256k1.p, 8) > 0)
    {
        uECC_vli_sub(result, result, curve_secp256k1.p, 8);
    }
}

void CTSBC_EcdsaSecp256k1::omega_mult_secp256k1(uint32* result, const uint32* right)
{
    uint32 carry = 0;

    for(int32 k = 0; k < 8; ++k)
    {
        const uint64 p = static_cast<uint64>(0x3D1) * right[k] + carry;
        result[k] = static_cast<uint32>(p);
        carry = p >> 32;
    }
    result[8] = carry;
    result[1 + 8] = uECC_vli_add(result + 1, result + 1, right, 8) ? 1 : 0;
}

int32 CTSBC_EcdsaSecp256k1::uECC_curve_private_key_size(const uECC_Curve* curve)
{
    return BITS_TO_BYTES(curve->num_n_bits);
}

int32 CTSBC_EcdsaSecp256k1::uECC_curve_public_key_size(const uECC_Curve* curve)
{
    return 2 * curve->num_bytes;
}

void CTSBC_EcdsaSecp256k1::uECC_vli_clear(uint32* vli, const int32 num_words)
{
    for(int32 i = 0; i < num_words; ++i)
    {
        vli[i] = 0;
    }
}

bool CTSBC_EcdsaSecp256k1::uECC_vli_isZero(const uint32* vli, const int32 num_words)
{
    uint32 bits = 0;
    for(int32 i = 0; i < num_words; ++i)
    {
        bits |= vli[i];
    }
    return (bits == 0);
}

bool CTSBC_EcdsaSecp256k1::uECC_vli_testBit(const uint32* vli, int32 bit)
{
    return ((vli[bit >> 5] & (static_cast<uint32>(1) << (bit & 0x01F))) != 0);
}

int32 CTSBC_EcdsaSecp256k1::vli_numDigits(const uint32* vli, const int32 max_words)
{
    int32 i;
    for(i = max_words - 1; i >= 0 && vli[i] == 0; --i)
    {
    }

    return (i + 1);
}

int32 CTSBC_EcdsaSecp256k1::uECC_vli_numBits(const uint32* vli, const int32 max_words)
{
    uint32 i;

    const int32 num_digits = vli_numDigits(vli, max_words);
    if(num_digits == 0)
    {
        return 0;
    }

    uint32 digit = vli[num_digits - 1];
    for(i = 0; digit; ++i)
    {
        digit >>= 1;
    }

    return ((num_digits - 1) << 5) + i;
}

void CTSBC_EcdsaSecp256k1::uECC_vli_set(uint32* dest, const uint32* src, const int32 num_words)
{
    for(int32 i = 0; i < num_words; ++i)
    {
        dest[i] = src[i];
    }
}

int32 CTSBC_EcdsaSecp256k1::uECC_vli_cmp_unsafe(
    const uint32* left,
    const uint32* right,
    const int32 num_words)
{
    for(int32 i = num_words - 1; i >= 0; --i)
    {
        if(left[i] > right[i])
        {
            return 1;
        }

        if(left[i] < right[i])
        {
            return -1;
        }
    }

    return 0;
}

bool CTSBC_EcdsaSecp256k1::uECC_vli_equal(
    const uint32* left,
    const uint32* right,
    const int32 num_words)
{
    uint32 diff = 0;
    for(int32 i = num_words - 1; i >= 0; --i)
    {
        diff |= (left[i] ^ right[i]);
    }

    return (diff == 0);
}

int32 CTSBC_EcdsaSecp256k1::uECC_vli_cmp(const uint32* left, const uint32* right, const int32 num_words)
{
    uint32 tmp[8];
    const uint32 neg = uECC_vli_sub(tmp, left, right, num_words) ? 1 : 0;
    const uint32 equal = uECC_vli_isZero(tmp, num_words) ? 1 : 0;

    return (!equal - 2 * neg);
}

void CTSBC_EcdsaSecp256k1::uECC_vli_rshift1(uint32* vli, const int32 num_words)
{
    uint32* end = vli;
    uint32 carry = 0;

    vli += num_words;
    while(vli-- > end)
    {
        const uint32 temp = *vli;
        *vli = (temp >> 1) | carry;
        carry = temp << (32 - 1);
    }
}

bool CTSBC_EcdsaSecp256k1::uECC_vli_add(
    uint32* result,
    const uint32* left,
    const uint32* right,
    const int32 num_words)
{
    uint32 carry = 0;
    for(int32 i = 0; i < num_words; ++i)
    {
        const uint32 sum = left[i] + right[i] + carry;
        if(sum != left[i])
        {
            carry = (sum < left[i]) ? 1 : 0;
        }
        result[i] = sum;
    }

    return (carry != 0);
}

bool CTSBC_EcdsaSecp256k1::uECC_vli_sub(
    uint32* result,
    const uint32* left,
    const uint32* right,
    const int32 num_words)
{
    uint32 borrow = 0;
    for(int32 i = 0; i < num_words; ++i)
    {
        const uint32 diff = left[i] - right[i] - borrow;
        if(diff != left[i])
        {
            borrow = (diff > left[i]) ? 1 : 0;
        }
        result[i] = diff;
    }

    return (borrow != 0);
}

void CTSBC_EcdsaSecp256k1::muladd(const uint32 a, const uint32 b, uint32* r0, uint32* r1, uint32* r2)
{
    const uint64 p = static_cast<uint64>(a) * b;
    uint64 r01 = (static_cast<uint64>(*r1) << 32) | *r0;
    r01 += p;
    *r2 += (r01 < p);
    *r1 = r01 >> 32;
    *r0 = static_cast<uint32>(r01);
}

void CTSBC_EcdsaSecp256k1::uECC_vli_mult(
    uint32* result,
    const uint32* left,
    const uint32* right,
    const int32 num_words)
{
    uint32 r0 = 0;
    uint32 r1 = 0;
    uint32 r2 = 0;
    int32 i, k;

    for(k = 0; k < num_words; ++k)
    {
        for(i = 0; i <= k; ++i)
        {
            muladd(left[i], right[k - i], &r0, &r1, &r2);
        }

        result[k] = r0;
        r0 = r1;
        r1 = r2;
        r2 = 0;
    }

    for(k = num_words; k < num_words * 2 - 1; ++k)
    {
        for(i = (k + 1) - num_words; i < num_words; ++i)
        {
            muladd(left[i], right[k - i], &r0, &r1, &r2);
        }

        result[k] = r0;
        r0 = r1;
        r1 = r2;
        r2 = 0;
    }

    result[num_words * 2 - 1] = r0;
}

void CTSBC_EcdsaSecp256k1::uECC_vli_modAdd(
    uint32* result,
    const uint32* left,
    const uint32* right,
    const uint32* mod,
    const int32 num_words)
{
    const bool carry = uECC_vli_add(result, left, right, num_words);
    if(carry || uECC_vli_cmp_unsafe(mod, result, num_words) != 1)
    {
        uECC_vli_sub(result, result, mod, num_words);
    }
}

void CTSBC_EcdsaSecp256k1::uECC_vli_modSub(
    uint32* result,
    const uint32* left,
    const uint32* right,
    const uint32* mod,
    const int32 num_words)
{
    if(uECC_vli_sub(result, left, right, num_words))
    {
        uECC_vli_add(result, result, mod, num_words);
    }
}

void CTSBC_EcdsaSecp256k1::uECC_vli_mmod(
    uint32* result,
    uint32* product,
    const uint32* mod,
    const int32 num_words)
{
    uint32 mod_multiple[2 * 8];
    uint32 tmp[2 * 8];
    uint32* v[2] = {tmp, product};
    uint32 index;

    int32 shift = (num_words * 2 * 32) - uECC_vli_numBits(mod, num_words);
    const int32 word_shift = shift / 32;
    const int32 bit_shift = shift % 32;
    uECC_vli_clear(mod_multiple, word_shift);

    if(bit_shift > 0)
    {
        uint32 carry = 0;
        for(index = 0; index < static_cast<uint32>(num_words); ++index)
        {
            mod_multiple[word_shift + index] = (mod[index] << bit_shift) | carry;
            carry = mod[index] >> (32 - bit_shift);
        }
    }
    else
    {
        uECC_vli_set(mod_multiple + word_shift, mod, num_words);
    }

    for(index = 1; shift >= 0; --shift)
    {
        uint32 borrow = 0;
        for(int32 i = 0; i < num_words * 2; ++i)
        {
            const uint32 diff = v[index][i] - mod_multiple[i] - borrow;
            if(diff != v[index][i])
            {
                borrow = (diff > v[index][i]) ? 1 : 0;
            }

            v[1 - index][i] = diff;
        }

        index = ((index ^ borrow) == 0) ? 1 : 0;
        uECC_vli_rshift1(mod_multiple, num_words);
        mod_multiple[num_words - 1] |= mod_multiple[num_words] << (32 - 1);
        uECC_vli_rshift1(mod_multiple + num_words, num_words);
    }

    uECC_vli_set(result, v[index], num_words);
}

void CTSBC_EcdsaSecp256k1::uECC_vli_modMult(
    uint32* result,
    const uint32* left,
    const uint32* right,
    const uint32* mod,
    const int32 num_words)
{
    uint32 product[2 * 8];
    uECC_vli_mult(product, left, right, num_words);
    uECC_vli_mmod(result, product, mod, num_words);
}

void CTSBC_EcdsaSecp256k1::uECC_vli_modMult_fast(
    uint32* result,
    const uint32* left,
    const uint32* right,
    const uECC_Curve* curve)
{
    uint32 product[2 * 8];
    uECC_vli_mult(product, left, right, curve->num_words);
    curve->mmod_fast(result, product);
}

void CTSBC_EcdsaSecp256k1::uECC_vli_modSquare_fast(
    uint32* result,
    const uint32* left,
    const uECC_Curve* curve)
{
    uECC_vli_modMult_fast(result, left, left, curve);
}

void CTSBC_EcdsaSecp256k1::vli_modInv_update(uint32* uv, const uint32* mod, const int32 num_words)
{
    bool carry = false;
    if(!(!(uv[0] & 1)))
    {
        carry = uECC_vli_add(uv, uv, mod, num_words);
    }

    uECC_vli_rshift1(uv, num_words);
    if(carry)
    {
        uv[num_words - 1] |= 0x80000000;
    }
}

void CTSBC_EcdsaSecp256k1::uECC_vli_modInv(
    uint32* result,
    const uint32* input,
    const uint32* mod,
    const int32 num_words)
{
    uint32 a[8], b[8], u[8], v[8];
    int32 cmpResult;

    if(uECC_vli_isZero(input, num_words))
    {
        uECC_vli_clear(result, num_words);
        return;
    }

    uECC_vli_set(a, input, num_words);
    uECC_vli_set(b, mod, num_words);
    uECC_vli_clear(u, num_words);
    u[0] = 1;
    uECC_vli_clear(v, num_words);

    while((cmpResult = uECC_vli_cmp_unsafe(a, b, num_words)) != 0)
    {
        if((!(a[0] & 1)))
        {
            uECC_vli_rshift1(a, num_words);
            vli_modInv_update(u, mod, num_words);
        }
        else if((!(b[0] & 1)))
        {
            uECC_vli_rshift1(b, num_words);
            vli_modInv_update(v, mod, num_words);
        }
        else if(cmpResult > 0)
        {
            uECC_vli_sub(a, a, b, num_words);
            uECC_vli_rshift1(a, num_words);
            if(uECC_vli_cmp_unsafe(u, v, num_words) < 0)
            {
                uECC_vli_add(u, u, mod, num_words);
            }
            uECC_vli_sub(u, u, v, num_words);
            vli_modInv_update(u, mod, num_words);
        }
        else
        {
            uECC_vli_sub(b, b, a, num_words);
            uECC_vli_rshift1(b, num_words);
            if(uECC_vli_cmp_unsafe(v, u, num_words) < 0)
            {
                uECC_vli_add(v, v, mod, num_words);
            }
            uECC_vli_sub(v, v, u, num_words);
            vli_modInv_update(v, mod, num_words);
        }
    }

    uECC_vli_set(result, u, num_words);
}

bool CTSBC_EcdsaSecp256k1::EccPoint_isZero(const uint32* point, const uECC_Curve* curve)
{
    return uECC_vli_isZero((point), (curve)->num_words * 2);
}

void CTSBC_EcdsaSecp256k1::apply_z(
    uint32* X1,
    uint32* Y1,
    const uint32* const Z,
    const uECC_Curve* curve)
{
    uint32 t1[8];

    uECC_vli_modSquare_fast(t1, Z, curve);
    uECC_vli_modMult_fast(X1, X1, t1, curve);
    uECC_vli_modMult_fast(t1, t1, Z, curve);
    uECC_vli_modMult_fast(Y1, Y1, t1, curve);
}

void CTSBC_EcdsaSecp256k1::XYcZ_initial_double(
    uint32* X1,
    uint32* Y1,
    uint32* X2,
    uint32* Y2,
    const uint32* const initial_Z,
    const uECC_Curve* curve)
{
    uint32 z[8];
    const int32 num_words = curve->num_words;
    if(initial_Z)
    {
        uECC_vli_set(z, initial_Z, num_words);
    }
    else
    {
        uECC_vli_clear(z, num_words);
        z[0] = 1;
    }

    uECC_vli_set(X2, X1, num_words);
    uECC_vli_set(Y2, Y1, num_words);

    apply_z(X1, Y1, z, curve);
    curve->double_jacobian(X1, Y1, z, curve);
    apply_z(X2, Y2, z, curve);
}

void CTSBC_EcdsaSecp256k1::XYcZ_add(
    uint32* X1,
    uint32* Y1,
    uint32* X2,
    uint32* Y2,
    const uECC_Curve* curve)
{
    uint32 t5[8];
    const int32 num_words = curve->num_words;

    uECC_vli_modSub(t5, X2, X1, curve->p, num_words);
    uECC_vli_modSquare_fast(t5, t5, curve);
    uECC_vli_modMult_fast(X1, X1, t5, curve);
    uECC_vli_modMult_fast(X2, X2, t5, curve);
    uECC_vli_modSub(Y2, Y2, Y1, curve->p, num_words);
    uECC_vli_modSquare_fast(t5, Y2, curve);

    uECC_vli_modSub(t5, t5, X1, curve->p, num_words);
    uECC_vli_modSub(t5, t5, X2, curve->p, num_words);
    uECC_vli_modSub(X2, X2, X1, curve->p, num_words);
    uECC_vli_modMult_fast(Y1, Y1, X2, curve);
    uECC_vli_modSub(X2, X1, t5, curve->p, num_words);
    uECC_vli_modMult_fast(Y2, Y2, X2, curve);
    uECC_vli_modSub(Y2, Y2, Y1, curve->p, num_words);

    uECC_vli_set(X2, t5, num_words);
}

void CTSBC_EcdsaSecp256k1::XYcZ_addC(
    uint32* X1,
    uint32* Y1,
    uint32* X2,
    uint32* Y2,
    const uECC_Curve* curve)
{
    uint32 t5[8];
    uint32 t6[8];
    uint32 t7[8];
    const int32 num_words = curve->num_words;

    uECC_vli_modSub(t5, X2, X1, curve->p, num_words);
    uECC_vli_modSquare_fast(t5, t5, curve);
    uECC_vli_modMult_fast(X1, X1, t5, curve);
    uECC_vli_modMult_fast(X2, X2, t5, curve);
    uECC_vli_modAdd(t5, Y2, Y1, curve->p, num_words);
    uECC_vli_modSub(Y2, Y2, Y1, curve->p, num_words);

    uECC_vli_modSub(t6, X2, X1, curve->p, num_words);
    uECC_vli_modMult_fast(Y1, Y1, t6, curve);
    uECC_vli_modAdd(t6, X1, X2, curve->p, num_words);
    uECC_vli_modSquare_fast(X2, Y2, curve);
    uECC_vli_modSub(X2, X2, t6, curve->p, num_words);

    uECC_vli_modSub(t7, X1, X2, curve->p, num_words);
    uECC_vli_modMult_fast(Y2, Y2, t7, curve);
    uECC_vli_modSub(Y2, Y2, Y1, curve->p, num_words);

    uECC_vli_modSquare_fast(t7, t5, curve);
    uECC_vli_modSub(t7, t7, t6, curve->p, num_words);
    uECC_vli_modSub(t6, t7, X1, curve->p, num_words);
    uECC_vli_modMult_fast(t6, t6, t5, curve);
    uECC_vli_modSub(Y1, t6, Y1, curve->p, num_words);

    uECC_vli_set(X1, t7, num_words);
}

int32 CTSBC_EcdsaSecp256k1::EccPoint_mult(
    uint32* result,
    const uint32* point,
    const uint32* scalar,
    const uint32* initial_Z,
    const int32 num_bits,
    const uECC_Curve* curve)
{
    uint32 Rx[2][8];
    uint32 Ry[2][8];
    uint32 z[8];
    uint32 nb;
    const int32 num_words = curve->num_words;

    uECC_vli_set(Rx[1], point, num_words);
    uECC_vli_set(Ry[1], point + num_words, num_words);

    XYcZ_initial_double(Rx[1], Ry[1], Rx[0], Ry[0], initial_Z, curve);
    for(int32 i = num_bits - 2; i > 0; --i)
    {
        nb = uECC_vli_testBit(scalar, i) ? 0 : 1;
        XYcZ_addC(Rx[1 - nb], Ry[1 - nb], Rx[nb], Ry[nb], curve);
        XYcZ_add(Rx[nb], Ry[nb], Rx[1 - nb], Ry[1 - nb], curve);
    }
    nb = uECC_vli_testBit(scalar, 0) ? 0 : 1;
    XYcZ_addC(Rx[1 - nb], Ry[1 - nb], Rx[nb], Ry[nb], curve);

    uECC_vli_modSub(z, Rx[1], Rx[0], curve->p, num_words);
    uECC_vli_modMult_fast(z, z, Ry[1 - nb], curve);
    uECC_vli_modMult_fast(z, z, point, curve);
    uECC_vli_modInv(z, z, curve->p, num_words);
    uECC_vli_modMult_fast(z, z, point + num_words, curve);
    uECC_vli_modMult_fast(z, z, Rx[1 - nb], curve);
    XYcZ_add(Rx[nb], Ry[nb], Rx[1 - nb], Ry[1 - nb], curve);
    apply_z(Rx[0], Ry[0], z, curve);

    uECC_vli_set(result, Rx[0], num_words);
    uECC_vli_set(result + num_words, Ry[0], num_words);

    return GetYParity(Ry[0]);
}

bool CTSBC_EcdsaSecp256k1::regularize_k(
    const uint32* const k,
    uint32* k0,
    uint32* k1,
    const uECC_Curve* curve)
{
    const int32 num_n_words = BITS_TO_WORDS(curve->num_n_bits);
    const int32 num_n_bits = curve->num_n_bits;

    const bool carry = uECC_vli_add(k0, k, curve->n, num_n_words)
                       || (num_n_bits < (num_n_words * 4 * 8) && uECC_vli_testBit(k0, num_n_bits));

    uECC_vli_add(k1, k0, curve->n, num_n_words);

    return carry;
}

bool CTSBC_EcdsaSecp256k1::EccPoint_compute_public_key(
    uint32* result,
    const uint32* private_key,
    const uECC_Curve* curve)
{
    uint32 tmp1[8];
    uint32 tmp2[8];
    uint32* p2[2] = {tmp1, tmp2};

    const bool carry = regularize_k(private_key, tmp1, tmp2, curve);
    EccPoint_mult(
        result,
        curve->G,
        p2[carry ? 0 : 1],
        0,
        curve->num_n_bits + 1,
        curve);

    if(EccPoint_isZero(result, curve))
    {
        return false;
    }

    return true;
}

void CTSBC_EcdsaSecp256k1::uECC_vli_nativeToBytes(
    uint8* bytes,
    const int32 num_bytes,
    const uint32* native)
{
    for(int32 i = 0; i < num_bytes; ++i)
    {
        const uint32 b = num_bytes - 1 - i;
        bytes[i] = native[b / 4] >> (8 * (b % 4));
    }
}

void CTSBC_EcdsaSecp256k1::uECC_vli_bytesToNative(
    uint32* native,
    const uint8* bytes,
    const int32 num_bytes)
{
    uECC_vli_clear(native, (num_bytes + (4 - 1)) / 4);
    for(int32 i = 0; i < num_bytes; ++i)
    {
        const uint32 b = num_bytes - 1 - i;
        native[b / 4] |= static_cast<uint32>(bytes[i]) << (8 * (b % 4));
    }
}

bool CTSBC_EcdsaSecp256k1::uECC_generate_random_int(
    uint32* random,
    const uint32* top,
    const int32 num_words)
{
    constexpr uint32 mask = static_cast<uint32>(-1);
    const int32 num_bits = uECC_vli_numBits(top, num_words);

    for(uint32 tries = 0; tries < uECC_RNG_MAX_TRIES; ++tries)
    {
        CTSBC_SecureRandom::randombytes_sysrandom_buf((uint8*)random, num_words * 4);
        random[num_words - 1] &= mask >> (num_words * 4 * 8 - num_bits);
        if(!uECC_vli_isZero(random, num_words) && uECC_vli_cmp(top, random, num_words) == 1)
        {
            return true;
        }
    }

    return false;
}

bool CTSBC_EcdsaSecp256k1::uECC_make_key(uint8* public_key, uint8* private_key, const uECC_Curve* curve)
{
    uint32 _private[8];
    uint32 _public[8 * 2];

    for(uint32 tries = 0; tries < uECC_RNG_MAX_TRIES; ++tries)
    {
        if(!uECC_generate_random_int(_private, curve->n, BITS_TO_WORDS(curve->num_n_bits)))
        {
            return false;
        }

        if(EccPoint_compute_public_key(_public, _private, curve))
        {
            uECC_vli_nativeToBytes(private_key, BITS_TO_BYTES(curve->num_n_bits), _private);
            uECC_vli_nativeToBytes(public_key, curve->num_bytes, _public);
            uECC_vli_nativeToBytes(public_key + curve->num_bytes, curve->num_bytes, _public + curve->num_words);
            return true;
        }
    }

    return false;
}

bool CTSBC_EcdsaSecp256k1::uECC_shared_secret(
    const uint8* public_key,
    const uint8* private_key,
    uint8* secret,
    const uECC_Curve* curve)
{
    uint32 _public[8 * 2];
    uint32 _private[8];

    uint32 tmp[8];
    uint32* p2[2] = {_private, tmp};
    const int32 num_words = curve->num_words;
    const int32 num_bytes = curve->num_bytes;

    uECC_vli_bytesToNative(_private, private_key, BITS_TO_BYTES(curve->num_n_bits));
    uECC_vli_bytesToNative(_public, public_key, num_bytes);
    uECC_vli_bytesToNative(_public + num_words, public_key + num_bytes, num_bytes);

    const bool carry = regularize_k(_private, _private, tmp, curve);

    if(!uECC_generate_random_int(p2[carry ? 1 : 0], curve->p, num_words))
    {
        return false;
    }

    const uint32* initial_Z = p2[carry ? 1 : 0];
    EccPoint_mult(
        _public,
        _public,
        p2[carry ? 0 : 1],
        initial_Z,
        curve->num_n_bits + 1,
        curve);
    uECC_vli_nativeToBytes(secret, num_bytes, _public);

    return !EccPoint_isZero(_public, curve);
}

void CTSBC_EcdsaSecp256k1::uECC_compress(
    const uint8* public_key,
    uint8* compressed,
    const uECC_Curve* curve)
{
    for(int32 i = 0; i < curve->num_bytes; ++i)
    {
        compressed[i + 1] = public_key[i];
    }

    compressed[0] = 2 + (public_key[curve->num_bytes * 2 - 1] & 0x01);
}

void CTSBC_EcdsaSecp256k1::uECC_decompress(
    const uint8* compressed,
    uint8* public_key,
    const uECC_Curve* curve)
{
    uint32 point[8 * 2];
    uint32* y = point + curve->num_words;
    uECC_vli_bytesToNative(point, compressed + 1, curve->num_bytes);
    curve->x_side(y, point, curve);
    curve->mod_sqrt(y, curve);

    if((y[0] & 0x01) != (compressed[0] & 0x01))
    {
        uECC_vli_sub(y, curve->p, y, curve->num_words);
    }

    uECC_vli_nativeToBytes(public_key, curve->num_bytes, point);
    uECC_vli_nativeToBytes(public_key + curve->num_bytes, curve->num_bytes, y);
}

bool CTSBC_EcdsaSecp256k1::uECC_valid_point(const uint32* point, const uECC_Curve* curve)
{
    uint32 tmp1[8];
    uint32 tmp2[8];
    const int32 num_words = curve->num_words;

    if(EccPoint_isZero(point, curve))
    {
        return false;
    }

    if(uECC_vli_cmp_unsafe(curve->p, point, num_words) != 1 || uECC_vli_cmp_unsafe(
           curve->p,
           point + num_words,
           num_words) != 1)
    {
        return false;
    }

    uECC_vli_modSquare_fast(tmp1, point + num_words, curve);
    curve->x_side(tmp2, point, curve);

    return uECC_vli_equal(tmp1, tmp2, num_words);
}

bool CTSBC_EcdsaSecp256k1::uECC_valid_public_key(const uint8* public_key, const uECC_Curve* curve)
{
    uint32 _public[8 * 2];

    uECC_vli_bytesToNative(_public, public_key, curve->num_bytes);
    uECC_vli_bytesToNative(_public + curve->num_words, public_key + curve->num_bytes, curve->num_bytes);

    return uECC_valid_point(_public, curve);
}

bool CTSBC_EcdsaSecp256k1::uECC_compute_public_key(
    const uint8* private_key,
    uint8* public_key,
    const uECC_Curve* curve)
{
    uint32 _private[8];
    uint32 _public[8 * 2];

    uECC_vli_bytesToNative(_private, private_key, BITS_TO_BYTES(curve->num_n_bits));

    if(uECC_vli_isZero(_private, BITS_TO_WORDS(curve->num_n_bits)))
    {
        return false;
    }

    if(uECC_vli_cmp(curve->n, _private, BITS_TO_WORDS(curve->num_n_bits)) != 1)
    {
        return false;
    }

    if(!EccPoint_compute_public_key(_public, _private, curve))
    {
        return false;
    }

    uECC_vli_nativeToBytes(public_key, curve->num_bytes, _public);
    uECC_vli_nativeToBytes(public_key + curve->num_bytes, curve->num_bytes, _public + curve->num_words);

    return true;
}

void CTSBC_EcdsaSecp256k1::bits2int(
    uint32* native,
    const uint8* bits,
    uint32 bits_size,
    const uECC_Curve* curve)
{
    const uint32 num_n_bytes = BITS_TO_BYTES(curve->num_n_bits);
    const uint32 num_n_words = BITS_TO_WORDS(curve->num_n_bits);

    if(bits_size > num_n_bytes)
    {
        bits_size = num_n_bytes;
    }

    uECC_vli_clear(native, num_n_words);
    uECC_vli_bytesToNative(native, bits, bits_size);
    if(bits_size * 8 <= static_cast<uint32>(curve->num_n_bits))
    {
        return;
    }

    const int32 shift = bits_size * 8 - curve->num_n_bits;
    uint32 carry = 0;
    uint32* ptr = native + num_n_words;
    while(ptr-- > native)
    {
        const uint32 temp = *ptr;
        *ptr = (temp >> shift) | carry;
        carry = temp << (32 - shift);
    }

    if(uECC_vli_cmp_unsafe(curve->n, native, num_n_words) != 1)
    {
        uECC_vli_sub(native, native, curve->n, num_n_words);
    }
}

bool CTSBC_EcdsaSecp256k1::uECC_sign_with_k(
    const uint8* private_key,
    const uint8* message_hash,
    const uint32 hash_size,
    uint32* k,
    uint8* signature,
    const uECC_Curve* curve)
{
    uint32 tmp[8];
    uint32 s[8];
    uint32* k2[2] = {tmp, s};
    uint32 p[8 * 2];
    const int32 num_words = curve->num_words;
    const int32 num_n_words = BITS_TO_WORDS(curve->num_n_bits);
    const int32 num_n_bits = curve->num_n_bits;

    if(uECC_vli_isZero(k, num_words) || uECC_vli_cmp(curve->n, k, num_n_words) != 1)
    {
        return false;
    }

    const bool carry = regularize_k(k, tmp, s, curve);
    const int32 YParity = EccPoint_mult(
        p,
        curve->G,
        k2[carry ? 0 : 1],
        0,
        num_n_bits + 1,
        curve);
    if(uECC_vli_isZero(p, num_words))
    {
        return false;
    }

    if(!uECC_generate_random_int(tmp, curve->n, num_n_words))
    {
        return false;
    }

    uECC_vli_modMult(k, k, tmp, curve->n, num_n_words);
    uECC_vli_modInv(k, k, curve->n, num_n_words);
    uECC_vli_modMult(k, k, tmp, curve->n, num_n_words);
    uECC_vli_nativeToBytes(signature, curve->num_bytes, p);

    uECC_vli_bytesToNative(tmp, private_key, BITS_TO_BYTES(curve->num_n_bits));
    s[num_n_words - 1] = 0;
    uECC_vli_set(s, p, num_words);
    uECC_vli_modMult(s, tmp, s, curve->n, num_n_words);
    bits2int(tmp, message_hash, hash_size, curve);
    uECC_vli_modAdd(s, tmp, s, curve->n, num_n_words);
    uECC_vli_modMult(s, s, k, curve->n, num_n_words);
    if(uECC_vli_numBits(s, num_n_words) > curve->num_bytes * 8)
    {
        return false;
    }

    if(uECC_vli_cmp(s, curve->nhalf, num_n_words) > 0)
    {
        // EIP-2
        return false;
    }

    uECC_vli_nativeToBytes(signature + curve->num_bytes, curve->num_bytes, s);
    signature[curve->num_bytes * 2] = YParity;

    return true;
}

bool CTSBC_EcdsaSecp256k1::uECC_sign(
    const uint8* private_key,
    const uint8* message_hash,
    const uint32 hash_size,
    uint8* signature,
    const uECC_Curve* curve)
{
    uint32 k[8];

    for(uint32 tries = 0; tries < uECC_RNG_MAX_TRIES; ++tries)
    {
        if(!uECC_generate_random_int(k, curve->n, BITS_TO_WORDS(curve->num_n_bits)))
        {
            return false;
        }

        if(uECC_sign_with_k(private_key, message_hash, hash_size, k, signature, curve))
        {
            return true;
        }
    }

    return false;
}

void CTSBC_EcdsaSecp256k1::HMAC_init(const uECC_HashContext* hash_context, const uint8* K)
{
    uint8* pad = hash_context->tmp + 2 * hash_context->result_size;
    uint32 i;
    for(i = 0; i < hash_context->result_size; ++i)
    {
        pad[i] = K[i] ^ 0x36;
    }

    for(; i < hash_context->block_size; ++i)
    {
        pad[i] = 0x36;
    }

    hash_context->init_hash(hash_context);
    hash_context->update_hash(hash_context, pad, hash_context->block_size);
}

void CTSBC_EcdsaSecp256k1::HMAC_update(
    const uECC_HashContext* hash_context,
    const uint8* message,
    const uint32 message_size)
{
    hash_context->update_hash(hash_context, message, message_size);
}

void CTSBC_EcdsaSecp256k1::HMAC_finish(
    const uECC_HashContext* hash_context,
    const uint8* K,
    uint8* result)
{
    uint8* pad = hash_context->tmp + 2 * hash_context->result_size;
    uint32 i;
    for(i = 0; i < hash_context->result_size; ++i)
    {
        pad[i] = K[i] ^ 0x5c;
    }

    for(; i < hash_context->block_size; ++i)
    {
        pad[i] = 0x5c;
    }

    hash_context->finish_hash(hash_context, result);

    hash_context->init_hash(hash_context);
    hash_context->update_hash(hash_context, pad, hash_context->block_size);
    hash_context->update_hash(hash_context, result, hash_context->result_size);
    hash_context->finish_hash(hash_context, result);
}

void CTSBC_EcdsaSecp256k1::update_V(const uECC_HashContext* hash_context, uint8* K, uint8* V)
{
    HMAC_init(hash_context, K);
    HMAC_update(hash_context, V, hash_context->result_size);
    HMAC_finish(hash_context, K, V);
}

bool CTSBC_EcdsaSecp256k1::uECC_sign_deterministic(
    const uint8* private_key,
    const uint8* message_hash,
    const uint32 hash_size,
    const uECC_HashContext* hash_context,
    uint8* signature,
    const uECC_Curve* curve)
{
    uint8* K = hash_context->tmp;
    uint8* V = K + hash_context->result_size;
    const int32 num_bytes = curve->num_bytes;
    const int32 num_n_words = BITS_TO_WORDS(curve->num_n_bits);
    const int32 num_n_bits = curve->num_n_bits;
    uint32 i;
    for(i = 0; i < hash_context->result_size; ++i)
    {
        V[i] = 0x01;
        K[i] = 0;
    }

    HMAC_init(hash_context, K);
    V[hash_context->result_size] = 0x00;
    HMAC_update(hash_context, V, hash_context->result_size + 1);
    HMAC_update(hash_context, private_key, num_bytes);
    HMAC_update(hash_context, message_hash, hash_size);
    HMAC_finish(hash_context, K, K);

    update_V(hash_context, K, V);

    HMAC_init(hash_context, K);
    V[hash_context->result_size] = 0x01;
    HMAC_update(hash_context, V, hash_context->result_size + 1);
    HMAC_update(hash_context, private_key, num_bytes);
    HMAC_update(hash_context, message_hash, hash_size);
    HMAC_finish(hash_context, K, K);

    update_V(hash_context, K, V);

    for(uint32 tries = 0; tries < uECC_RNG_MAX_TRIES; ++tries)
    {
        uint32 T[8];
        uint8* T_ptr = reinterpret_cast<uint8*>(T);
        int32 T_bytes = 0;

        bool loop = true;
        while(loop)
        {
            update_V(hash_context, K, V);
            for(i = 0; i < hash_context->result_size; ++i)
            {
                T_ptr[T_bytes++] = V[i];
                if(T_bytes >= num_n_words * 4)
                {
                    loop = false;
                    break;
                }
            }
        }

        if(num_n_words * 4 * 8 > num_n_bits)
        {
            constexpr uint32 mask = static_cast<uint32>(-1);
            T[num_n_words - 1] &= mask >> (num_n_words * 4 * 8 - num_n_bits);
        }

        if(uECC_sign_with_k(private_key, message_hash, hash_size, T, signature, curve))
        {
            return true;
        }

        HMAC_init(hash_context, K);
        V[hash_context->result_size] = 0x00;
        HMAC_update(hash_context, V, hash_context->result_size + 1);
        HMAC_finish(hash_context, K, K);

        update_V(hash_context, K, V);
    }

    return false;
}

bool CTSBC_EcdsaSecp256k1::uECC_verify(
    const uint8* public_key,
    const uint8* message_hash,
    const uint32 hash_size,
    const uint8* signature,
    const uECC_Curve* curve)
{
    uint32 u1[8], u2[8];
    uint32 z[8];
    uint32 sum[8 * 2];
    uint32 rx[8];
    uint32 ry[8];

    uint32 tx[8];
    uint32 ty[8];
    uint32 tz[8];

    const uint32* points[4];
    uint32 _public[8 * 2];
    uint32 r[8], s[8];
    const int32 num_words = curve->num_words;
    const int32 num_n_words = BITS_TO_WORDS(curve->num_n_bits);

    rx[num_n_words - 1] = 0;
    r[num_n_words - 1] = 0;
    s[num_n_words - 1] = 0;

    uECC_vli_bytesToNative(_public, public_key, curve->num_bytes);
    uECC_vli_bytesToNative(_public + num_words, public_key + curve->num_bytes, curve->num_bytes);
    uECC_vli_bytesToNative(r, signature, curve->num_bytes);
    uECC_vli_bytesToNative(s, signature + curve->num_bytes, curve->num_bytes);

    if(uECC_vli_isZero(r, num_words) || uECC_vli_isZero(s, num_words))
    {
        return false;
    }

    if(uECC_vli_cmp_unsafe(curve->n, r, num_n_words) != 1 || uECC_vli_cmp_unsafe(curve->n, s, num_n_words) != 1)
    {
        return false;
    }

    uECC_vli_modInv(z, s, curve->n, num_n_words);
    u1[num_n_words - 1] = 0;
    bits2int(u1, message_hash, hash_size, curve);
    uECC_vli_modMult(u1, u1, z, curve->n, num_n_words);
    uECC_vli_modMult(u2, r, z, curve->n, num_n_words);

    uECC_vli_set(sum, _public, num_words);
    uECC_vli_set(sum + num_words, _public + num_words, num_words);
    uECC_vli_set(tx, curve->G, num_words);
    uECC_vli_set(ty, curve->G + num_words, num_words);
    uECC_vli_modSub(z, sum, tx, curve->p, num_words);
    XYcZ_add(tx, ty, sum, sum + num_words, curve);
    uECC_vli_modInv(z, z, curve->p, num_words);
    apply_z(sum, sum + num_words, z, curve);

    points[0] = 0;
    points[1] = curve->G;
    points[2] = _public;
    points[3] = sum;
    const int32 num_bits_a = uECC_vli_numBits(u1, num_n_words);
    const int32 num_bits_b = uECC_vli_numBits(u2, num_n_words);
    const int32 num_bits = (num_bits_a > num_bits_b) ? num_bits_a : num_bits_b;

    const uint32* point = points[(uECC_vli_testBit(u1, num_bits - 1) ? 1 : 0) | ((uECC_vli_testBit(u2, num_bits - 1)
                                     ? 1
                                     : 0) << 1)];
    uECC_vli_set(rx, point, num_words);
    uECC_vli_set(ry, point + num_words, num_words);
    uECC_vli_clear(z, num_words);
    z[0] = 1;

    for(int32 i = num_bits - 2; i >= 0; --i)
    {
        curve->double_jacobian(rx, ry, z, curve);

        const uint32 index = (uECC_vli_testBit(u1, i) ? 1 : 0) | ((uECC_vli_testBit(u2, i) ? 1 : 0) << 1);
        point = points[index];
        if(point)
        {
            uECC_vli_set(tx, point, num_words);
            uECC_vli_set(ty, point + num_words, num_words);
            apply_z(tx, ty, z, curve);
            uECC_vli_modSub(tz, rx, tx, curve->p, num_words);
            XYcZ_add(tx, ty, rx, ry, curve);
            uECC_vli_modMult_fast(z, z, tz, curve);
        }
    }

    uECC_vli_modInv(z, z, curve->p, num_words);
    apply_z(rx, ry, z, curve);

    if(uECC_vli_cmp_unsafe(curve->n, rx, num_n_words) != 1)
    {
        uECC_vli_sub(rx, rx, curve->n, num_n_words);
    }

    return uECC_vli_equal(rx, r, num_words);
}

CTSBC_EcdsaSecp256k1::uECC_HashContext CTSBC_EcdsaSecp256k1::GetHashContextSha256(
    TArray<uint8>& Buffer)
{
    const uint32 BlockSize = CTSBC_Sha256::BLOCK_SIZE;
    constexpr uint32 ResultSize = 32;
    Buffer.Init(0, 2 * ResultSize + BlockSize);
    const uECC_HashContext HashContext = {
        [](const uECC_HashContext* context)
        {
        },
        [](const uECC_HashContext* context, const uint8* message, unsigned message_size)
        {
            CTSBC_Sha256 instance;
            TArray<uint8> BufIn;
            BufIn.Init(0, message_size);
            FMemory::Memcpy(BufIn.GetData(), message, message_size);
            TArray<uint8> BufOut = instance.Hash(BufIn);
            FMemory::Memcpy(context->tmp, BufOut.GetData(), BufOut.Num());
        },
        [](const uECC_HashContext* context, uint8* hash_result)
        {
        },
        BlockSize,
        ResultSize,
        Buffer.GetData(),
    };
    return HashContext;
}

CTSBC_EcdsaSecp256k1::uECC_HashContext CTSBC_EcdsaSecp256k1::GetHashContextSha512(
    TArray<uint8>& Buffer)
{
    const uint32 BlockSize = CTSBC_Sha512::BLOCK_SIZE;
    constexpr uint32 ResultSize = 64;
    Buffer.Init(0, 2 * ResultSize + BlockSize);
    const uECC_HashContext HashContext = {
        [](const uECC_HashContext* context)
        {
        },
        [](const uECC_HashContext* context, const uint8* message, unsigned message_size)
        {
            CTSBC_Sha512 instance;
            TArray<uint8> BufIn;
            BufIn.Init(0, message_size);
            FMemory::Memcpy(BufIn.GetData(), message, message_size);
            TArray<uint8> BufOut = instance.Hash(BufIn);
            FMemory::Memcpy(context->tmp, BufOut.GetData(), BufOut.Num());
        },
        [](const uECC_HashContext* context, uint8* hash_result)
        {
        },
        BlockSize,
        ResultSize,
        Buffer.GetData(),
    };
    return HashContext;
}

bool CTSBC_EcdsaSecp256k1::Secp256k1_GeneratePrivateKey(TArray<uint8>& PrivateKey)
{
    const uECC_Curve* curve = uECC_secp256k1();
    TArray<uint8> PublicKey;
    PublicKey.SetNum(uECC_curve_public_key_size(curve));
    PrivateKey.SetNum(uECC_curve_private_key_size(curve));
    if(!uECC_make_key(PublicKey.GetData(), PrivateKey.GetData(), curve))
    {
        PublicKey.Empty();
        PrivateKey.Empty();
        return false;
    }

    if(!IsPublicKeyValid(PublicKey, curve) || !IsPrivateKeyValid(PrivateKey, curve))
    {
        PublicKey.Empty();
        PrivateKey.Empty();
        return false;
    }

    return true;
}

bool CTSBC_EcdsaSecp256k1::Secp256k1_GeneratePublicKey(
    const TArray<uint8>& PrivateKey,
    TArray<uint8>& PublicKey)
{
    const uECC_Curve* curve = uECC_secp256k1();
    PublicKey.SetNum(uECC_curve_public_key_size(curve));
    if(!IsPrivateKeyValid(PrivateKey, curve))
    {
        PublicKey.Empty();
        return false;
    }

    if(!uECC_compute_public_key(PrivateKey.GetData(), PublicKey.GetData(), curve))
    {
        PublicKey.Empty();
        return false;
    }

    if(!IsPublicKeyValid(PublicKey, curve))
    {
        PublicKey.Empty();
        return false;
    }

    return true;
}

bool CTSBC_EcdsaSecp256k1::Secp256k1_IsPublicKeyValid(const TArray<uint8>& PublicKey)
{
    const uECC_Curve* curve = uECC_secp256k1();

    return IsPublicKeyValid(PublicKey, curve);
}

bool CTSBC_EcdsaSecp256k1::Secp256k1_IsPrivateKeyValid(const TArray<uint8>& PrivateKey)
{
    const uECC_Curve* curve = uECC_secp256k1();

    return IsPrivateKeyValid(PrivateKey, curve);
}

bool CTSBC_EcdsaSecp256k1::Secp256k1_GenerateSharedSecret(
    const TArray<uint8>& PublicKey,
    const TArray<uint8>& PrivateKey,
    TArray<uint8>& Secret)
{
    const uECC_Curve* curve = uECC_secp256k1();
    if(!IsPublicKeyValid(PublicKey, curve) || !IsPrivateKeyValid(PrivateKey, curve))
    {
        Secret.Empty();
        return false;
    }

    Secret.SetNum(curve->num_bytes);
    if(!uECC_shared_secret(PublicKey.GetData(), PrivateKey.GetData(), Secret.GetData(), curve))
    {
        Secret.Empty();
        return false;
    }

    return true;
}

bool CTSBC_EcdsaSecp256k1::Secp256k1_CreateSignature(
    const TArray<uint8>& PrivateKey,
    const TArray<uint8>& Hash,
    TArray<uint8>& Signature)
{
    const uECC_Curve* curve = uECC_secp256k1();
    if(!IsPrivateKeyValid(PrivateKey, curve))
    {
        Signature.Empty();
        return false;
    }

    Signature.SetNum(curve->num_bytes * 2 + 1);
    if(!uECC_sign(PrivateKey.GetData(), Hash.GetData(), Hash.Num(), Signature.GetData(), curve))
    {
        Signature.Empty();
        return false;
    }

    return true;
}

bool CTSBC_EcdsaSecp256k1::Secp256k1_VerifySignature(
    const TArray<uint8>& PublicKey,
    const TArray<uint8>& Hash,
    const TArray<uint8>& Signature)
{
    const uECC_Curve* curve = uECC_secp256k1();
    if(!IsPublicKeyValid(PublicKey, curve))
    {
        return false;
    }

    if(!uECC_verify(PublicKey.GetData(), Hash.GetData(), Hash.Num(), Signature.GetData(), curve))
    {
        return false;
    }

    return true;
}

bool CTSBC_EcdsaSecp256k1::Secp256k1_CompressPublicKey(
    const TArray<uint8>& PublicKey,
    TArray<uint8>& CompressedPublicKey)
{
    const uECC_Curve* curve = uECC_secp256k1();
    if(!IsPublicKeyValid(PublicKey, curve))
    {
        CompressedPublicKey.Empty();
        return false;
    }

    CompressedPublicKey.SetNum(curve->num_bytes + 1);
    uECC_compress(PublicKey.GetData(), CompressedPublicKey.GetData(), curve);

    return true;
}

bool CTSBC_EcdsaSecp256k1::Secp256k1_DecompressPublicKey(
    const TArray<uint8>& CompressedPublicKey,
    TArray<uint8>& PublicKey)
{
    const uECC_Curve* curve = uECC_secp256k1();
    if(CompressedPublicKey.Num() != curve->num_bytes + 1)
    {
        PublicKey.Empty();
        return false;
    }

    PublicKey.SetNum(uECC_curve_public_key_size(curve));
    uECC_decompress(CompressedPublicKey.GetData(), PublicKey.GetData(), curve);
    if(!IsPublicKeyValid(PublicKey, curve))
    {
        PublicKey.Empty();
        return false;
    }

    return true;
}

FString CTSBC_EcdsaSecp256k1::GetFromSignatureValueV(
    const TArray<uint8>& Signature,
    const uint32 ChainId)
{
    constexpr int32 Offset = 64;
    constexpr int32 Length = 1;

    if(Offset + Length > Signature.Num())
    {
        TSBC_LOG(Error, TEXT("Could not get V-value from signature"));
        return "";
    }

    TArray<uint8> Buffer;
    Buffer.Init(0, Length);
    FMemory::Memcpy(Buffer.GetData(), Signature.GetData() + Offset, Buffer.Num());
    const FString Value = TSBC_StringUtils::BytesToHex(Buffer, false);
    const uint32 YParity = Value == "00" ? 0 : 1;
    const int32 ValueVDec = YParity + ChainId * 2 + 35;
    const FString ValueV = FString::Printf(TEXT("0x%02x"), ValueVDec);

    return ValueV;
}

FString CTSBC_EcdsaSecp256k1::GetFromSignatureValueR(const TArray<uint8>& Signature)
{
    constexpr int32 Offset = 0;
    constexpr int32 Length = 32;

    if(Offset + Length > Signature.Num())
    {
        TSBC_LOG(Error, TEXT("Could not get R-value from signature"));
        return "";
    }

    TArray<uint8> Buffer;
    Buffer.Init(0, Length);
    FMemory::Memcpy(Buffer.GetData(), Signature.GetData() + Offset, Buffer.Num());
    const FString ValueR = TSBC_StringUtils::BytesToHex(Buffer);

    return ValueR;
}

FString CTSBC_EcdsaSecp256k1::GetFromSignatureValueS(const TArray<uint8>& Signature)
{
    constexpr int32 Offset = 32;
    constexpr int32 Length = 32;

    if(Offset + Length > Signature.Num())
    {
        TSBC_LOG(Error, TEXT("Could not get V-value from signature"));
        return "";
    }

    TArray<uint8> Buffer;
    Buffer.Init(0, Length);
    FMemory::Memcpy(Buffer.GetData(), Signature.GetData() + Offset, Buffer.Num());
    const FString ValueS = TSBC_StringUtils::BytesToHex(Buffer);

    return ValueS;
}

uint32 CTSBC_EcdsaSecp256k1::GetYParity(const uint32* Point)
{
    return IsOddParity(Point) ? 1 : 0;
}

bool CTSBC_EcdsaSecp256k1::IsPublicKeyValid(const TArray<uint8>& PublicKey, const uECC_Curve* curve)
{
    if(PublicKey.Num() != uECC_curve_public_key_size(curve))
    {
        return false;
    }

    if(!uECC_valid_public_key(PublicKey.GetData(), curve))
    {
        return false;
    }

    return true;
}

bool CTSBC_EcdsaSecp256k1::IsPrivateKeyValid(const TArray<uint8>& PrivateKey, const uECC_Curve* curve)
{
    if(PrivateKey.Num() != uECC_curve_private_key_size(curve))
    {
        return false;
    }

    return true;
}

bool CTSBC_EcdsaSecp256k1::IsOddParity(const uint32* Bytes)
{
    return Bytes[0] & 1;
}