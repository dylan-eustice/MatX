////////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2021, NVIDIA Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cuda/std/cmath>
#include <type_traits>

#include "cuda_bf16.h"
#include "cuda_fp16.h"
#include "matx/core/defines.h"

namespace matx {

/**
 * Template class for half precison numbers (__half and __nv_bfloat16). CUDA
 * does not have standardized classes/operators available on both host and
 * device to support these operations, so we define it here.
 *
 */
template <typename T> struct alignas(sizeof(T)) matxHalf {
  using value_type = T; ///< Type of half

  /**
   * @brief Default constructor
   *
   */
  __MATX_INLINE__ matxHalf() = default;

  /**
   * @brief Default copy constructor
   *
   * @param x_ Parameter to copy
   */
  __MATX_INLINE__ matxHalf(const matxHalf<T> &x_) noexcept = default;

  /**
   * @brief Copy constructor from arbitrary type
   *
   * @tparam T2 Type to copy from
   * @param x_ Value to copy
   */
  template <typename T2>
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf(const T2 &x_) noexcept
      : x(static_cast<float>(x_))
  {
  }

  /**
   * @brief Default destructor
   *
   */
  __MATX_INLINE__ ~matxHalf() = default;

  /**
   * @brief Half casting operator
   *
   * @return Underlying half type
   */
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ operator T() const
  {
    return x;
  }

  /**
   * @brief Half reference casting operator
   *
   * @return Underlying reference to half type
   */
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ operator T&()
  {
    return x;
  }

  /**
   * @brief Float casting operator
   *
   * @return float of value
   */
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ operator float() const
  {
    return static_cast<float>(x);
  }

  /**
   * @brief Default copy assignment operator
   *
   * @param rhs Value to copy
   */
  __MATX_INLINE__ matxHalf<T> &operator=(const matxHalf<T> &rhs) = default;

  /**
   * @brief Copy assignment operator
   *
   * @tparam T2 Type to copy from
   * @param rhs Value to copy from
   */
  template <typename T2>
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> &operator=(const T2 &rhs)
  {
    x = static_cast<float>(rhs);
    return *this;
  }

  /**
   * @brief Increment and assign operator
   *
   * @tparam X Type of half
   * @param rhs Value of cource
   * @return Updated object
   */
  template <typename X>
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> &
  operator+=(const matxHalf<X> &rhs)
  {
    *this = *this + rhs;
    return *this;
  }

  /**
   * @brief Decrement and assign operator
   *
   * @tparam X Type of half
   * @param rhs Value of cource
   * @return Updated object
   */
  template <typename X>
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> &
  operator-=(const matxHalf<X> &rhs)
  {
    *this = *this - rhs;
    return *this;
  }

  /**
   * @brief Multiply and assign operator
   *
   * @tparam X Type of half
   * @param rhs Value of cource
   * @return Updated object
   */
  template <typename X>
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> &
  operator*=(const matxHalf<X> &rhs)
  {
    *this = *this * rhs;
    return *this;
  }

  /**
   * @brief Divide and assign operator
   *
   * @tparam X Type of half
   * @param rhs Value of cource
   * @return Updated object
   */
  template <typename X>
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> &
  operator/=(const matxHalf<X> &rhs)
  {
    *this = *this / rhs;
    return *this;
  }

  /**
   * @brief Increment and assign operator from float
   *
   * @param f Value of cource
   * @return Updated object
   */
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> &operator+=(float f) const
  {
#ifdef __CUDA_ARCH__
    return {x + f};
#else
    return {static_cast<T>(static_cast<float>(x) + f)};
#endif
  }

  /**
   * @brief Decrement and assign operator from float
   *
   * @param f Value of cource
   * @return Updated object
   */
  __MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> &operator-=(float f) const
  {
#ifdef __CUDA_ARCH__
    return {x + f};
#else
    return {static_cast<T>(static_cast<float>(x) + f)};
#endif
  }

  T x; ///< Value of half
};

/**
 * @brief Add a half precious wrapper type with native type
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of expression
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator+(const matxHalf<T> &lhs, const T &rhs)
{
  matxHalf<T> tmp{rhs};
  return lhs + rhs;
}

/**
 * @brief Add a half precious wrapper type with native type
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of expression
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator+(const T &lhs, const matxHalf<T> &rhs)
{
  matxHalf<T> tmp{lhs};
  return lhs + rhs;
}

/**
 * @brief Subtract a half precious wrapper type with native type
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of expression
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator-(const matxHalf<T> &lhs, const T &rhs)
{
  matxHalf<T> tmp{rhs};
  return lhs - rhs;
}

/**
 * @brief Subtract a half precious wrapper type with native type
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of expression
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator-(const T &lhs, const matxHalf<T> &rhs)
{
  matxHalf<T> tmp{lhs};
  return lhs - rhs;
}

/**
 * @brief Multiply a half precious wrapper type with native type
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of expression
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator*(const matxHalf<T> &lhs, const T &rhs)
{
  matxHalf<T> tmp{rhs};
  return lhs * rhs;
}

/**
 * @brief Multiply a half precious wrapper type with native type
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of expression
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator*(const T &lhs, const matxHalf<T> &rhs)
{
  matxHalf<T> tmp{lhs};
  return lhs * rhs;
}

/**
 * @brief Divide a half precious wrapper type with native type
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of expression
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator/(const matxHalf<T> &lhs, const T &rhs)
{
  matxHalf<T> tmp{rhs};
  return lhs / rhs;
}

/**
 * @brief Divide a half precious wrapper type with native type
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of expression
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator/(const T &lhs, const matxHalf<T> &rhs)
{
  matxHalf<T> tmp{lhs};
  return lhs / rhs;
}

/**
 * @brief Negate operator
 *
 * @tparam T Half type
 * @param l operator
 * @return Result of negation
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> operator-(const matxHalf<T> &l)
{
#ifdef __CUDA_ARCH__
  return {-l.x};
#else
  return {-static_cast<float>(l.x)};
#endif
}

/**
 * @brief Comparison operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of comparison
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ bool operator==(const matxHalf<T> &lhs,
                                                    const matxHalf<T> &rhs)
{
#ifdef __CUDA_ARCH__
  return lhs.x == rhs.x;
#else
  return static_cast<float>(lhs.x) == static_cast<float>(rhs.x);
#endif
}

/**
 * @brief Comparison operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of comparison
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ bool operator==(const matxHalf<T> &lhs,
                                                    const T &rhs)
{
  matxHalf<T> tmp{rhs};
  return lhs == tmp;
}

/**
 * @brief Comparison operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of comparison
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ bool operator==(const T &lhs,
                                                    const matxHalf<T> &rhs)
{
  matxHalf<T> tmp{lhs};
  return rhs == tmp;
}

/**
 * @brief Not equal operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of comparison
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ bool operator!=(const matxHalf<T> &lhs,
                                                    const matxHalf<T> &rhs)
{
  return !(lhs == rhs);
}

/**
 * @brief Not equal operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of comparison
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ bool operator!=(const matxHalf<T> &lhs,
                                                    const T &rhs)
{
  matxHalf<T> tmp{rhs};
  return !(lhs == tmp);
}

/**
 * @brief Not equal operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of comparison
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ bool operator!=(const T &lhs,
                                                    const matxHalf<T> &rhs)
{
  matxHalf<T> tmp{lhs};
  return !(rhs == tmp);
}

/**
 * @brief Greater than operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of comparison
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ bool operator>(const matxHalf<T> &lhs,
                                                   const matxHalf<T> &rhs)
{
#ifdef __CUDA_ARCH__
  return lhs.x > rhs.x;
#else
  return static_cast<float>(lhs.x) > static_cast<float>(rhs.x);
#endif
}

/**
 * @brief Less than operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of comparison
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ bool operator<(const matxHalf<T> &lhs,
                                                   const matxHalf<T> &rhs)
{
#ifdef __CUDA_ARCH__
  return lhs.x < rhs.x;
#else
  return static_cast<float>(lhs.x) < static_cast<float>(rhs.x);
#endif
}

/**
 * @brief Less than or equal to operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of comparison
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ bool operator<=(const matxHalf<T> &lhs,
                                                    const matxHalf<T> &rhs)
{
  return lhs < rhs || lhs == rhs;
}

/**
 * @brief Greater than or equal to operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of comparison
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ bool operator>=(const matxHalf<T> &lhs,
                                                    const matxHalf<T> &rhs)
{
  return lhs > rhs || lhs == rhs;
}

/**
 * @brief Addition operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of operation
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator+(const matxHalf<T> &lhs, const matxHalf<T> &rhs)
{
#ifdef __CUDA_ARCH__
  return lhs.x + rhs.x;
#else
  return static_cast<T>(static_cast<float>(lhs) + static_cast<float>(rhs));
#endif
}

/**
 * @brief Subtraction operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of operation
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator-(const matxHalf<T> &lhs, const matxHalf<T> &rhs)
{
#ifdef __CUDA_ARCH__
  return lhs.x - rhs.x;
#else
  return static_cast<T>(static_cast<float>(lhs) - static_cast<float>(rhs));
#endif
}

/**
 * @brief Multiplication operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of operation
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator*(const matxHalf<T> &lhs, const matxHalf<T> &rhs)
{
#ifdef __CUDA_ARCH__
  return lhs.x * rhs.x;
#else
  return static_cast<T>(static_cast<float>(lhs.x) * static_cast<float>(rhs.x));
#endif
}

/**
 * @brief Division operator
 *
 * @tparam T Native type
 * @param lhs Left parameter
 * @param rhs Right parameter
 * @return Result of operation
 */
template <typename T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
operator/(const matxHalf<T> &lhs, const matxHalf<T> &rhs)
{
#ifdef __CUDA_ARCH__
  return lhs.x / rhs.x;
#else
  return static_cast<T>(static_cast<float>(lhs.x) / static_cast<float>(rhs.x));
#endif
}

/**
 * @brief Absolute value
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Absolute value of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> abs(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return __habs(x.x);
#else
  return static_cast<T>(cuda::std::abs(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Absolute value
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Absolute value of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
abs(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return __habs(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::abs(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Natural logarithm value
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Natural logarithm of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> log(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hlog(x.x);
#else
  return static_cast<T>(cuda::std::log(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Square root value
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Square root of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> sqrt(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hsqrt(x.x);
#else
  return static_cast<T>(cuda::std::sqrt(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Square root value
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Square root of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
sqrt(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hsqrt(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::sqrt(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Reciprocal Square root value
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Reciprocal square root of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> rsqrt(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hrsqrt(x.x);
#else
  #ifdef __CUDACC__
    return static_cast<__nv_bfloat16>(::rsqrt(static_cast<float>(x.x)));
  #else
    return static_cast<__nv_bfloat16>(1.f / cuda::std::sqrt(static_cast<float>(x.x)));
  #endif
#endif
}

/**
 * @brief Square root value
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Square root of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
rsqrt(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hrsqrt(x.x);
#else
  #ifdef __CUDACC__
    return static_cast<__nv_bfloat16>(::rsqrt(static_cast<float>(x.x)));
  #else
    return static_cast<__nv_bfloat16>(1.f / cuda::std::sqrt(static_cast<float>(x.x)));
  #endif
#endif
}

/**
 * @brief Test for infiity
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return True if input is infinity
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ int isinf(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return __hisinf(x.x);
#else
  return static_cast<int>(cuda::std::isinf(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Test for infiity
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return True if input is infinity
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ int isinf(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return __hisinf(x.x);
#else
  return static_cast<int>(cuda::std::isinf(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Natural logarithm
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Natural log of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
log(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hlog(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::log(static_cast<float>(x.x)));
#endif
}

/**
 * @brief logarithm base 10
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Log base 10 of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> log10(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hlog10(x.x);
#else
  return static_cast<T>(cuda::std::log10(static_cast<float>(x.x)));
#endif
}

/**
 * @brief logarithm base 10
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Log base 10 of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
log10(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hlog10(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::log10(static_cast<float>(x.x)));
#endif
}

/**
 * @brief logarithm base 2
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Log base 2 of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> log2(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hlog2(x.x);
#else
  return static_cast<T>(cuda::std::log2(static_cast<float>(x.x)));
#endif
}

/**
 * @brief logarithm base 2
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Log base 2 of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
log2(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hlog2(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::log2(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Exponential function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Exponential of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> exp(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hexp(x.x);
#else
  return static_cast<T>(cuda::std::exp(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Exponential function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Exponential of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
exp(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hexp(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::exp(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Power function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @param y Value to raise x to
 * @return Power of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> pow(const matxHalf<T> &x,
                                                    const matxHalf<T> &y)
{
  auto tmp = cuda::std::pow(static_cast<float>(x.x), static_cast<float>(y.x));
  return static_cast<T>(tmp);
}

/**
 * @brief Power function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @param y Value to raise x to
 * @return Power of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> pow(const matxHalf<T> &x,
                                                    const T &y)
{
  auto tmp = cuda::std::pow(static_cast<float>(x.x), static_cast<float>(y));
  return static_cast<T>(tmp);
}

/**
 * @brief Power function
 *
 * @tparam T Type of float
 * @param x Value of half
 * @param y Value to raise x to
 * @return Power of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> pow(const T &x,
                                                    const matxHalf<T> &y)
{
  auto tmp = cuda::std::pow(x, static_cast<float>(y.x));
  return static_cast<T>(tmp);
}

/**
 * @brief Floor function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Floor of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> floor(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hfloor(x.x);
#else
  return static_cast<T>(cuda::std::floor(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Floor function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Floor of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
floor(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hfloor(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::floor(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Ceiling function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Ceiling of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> ceil(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hceil(x.x);
#else
  return static_cast<T>(cuda::std::ceil(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Ceiling function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Ceiling of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
ceil(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hceil(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::ceil(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Round function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Rounding of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> round(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hrint(x.x);
#else
  return static_cast<T>(cuda::std::round(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Round function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Rounding of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
round(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hrint(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::round(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Modulo function
 *
 * @tparam T Type of float
 * @param x Value of half
 * @param y Value to raise x to
 * @return Remainder of x / y
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
fmod(const T &x, const matxHalf<T> &y) {
  auto tmp = cuda::std::fmod(x, static_cast<float>(y.x));
  return static_cast<T>(tmp);
}

/**
 * @brief Modulo function
 *
 * @tparam T Type of float
 * @param x Value of half
 * @param y Value to raise x to
 * @return Remainder of x / y
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
fmod(const matxHalf<T> &x, const matxHalf<T> &y) {
  auto tmp = cuda::std::fmod(static_cast<float>(x.x), static_cast<float>(y.x));
  return static_cast<T>(tmp);
}

/**
 * @brief Modulo function
 *
 * @tparam T Type of float
 * @param x Value of half
 * @param y Value to raise x to
 * @return Remainder of x / y
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T>
fmod(const matxHalf<T> &x, const T &y) {
  auto tmp = cuda::std::fmod(static_cast<float>(x.x), y);
  return static_cast<T>(tmp);
}

/**
 * @brief Sine function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Sine of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> sin(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hsin(x.x);
#else
  return static_cast<T>(cuda::std::sin(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Sine function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Sine of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
sin(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hsin(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::sin(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Cosine function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Cosine of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> cos(const matxHalf<T> &x)
{
#ifdef __CUDA_ARCH__
  return hcos(x.x);
#else
  return static_cast<T>(cuda::std::cos(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Cosine function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Cosine of input
 */
template <>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<__nv_bfloat16>
cos(const matxHalf<__nv_bfloat16> &x)
{
#if __CUDA_ARCH__ >= 800
  return hcos(x.x);
#else
  return static_cast<__nv_bfloat16>(cuda::std::cos(static_cast<float>(x.x)));
#endif
}

/**
 * @brief Tangent function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Tangent of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> tan(const matxHalf<T> &x)
{
  return static_cast<T>(cuda::std::tan(static_cast<float>(x.x)));
}

/**
 * @brief Arctangent function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Arctangent of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> asin(const matxHalf<T> &x)
{
  return static_cast<T>(cuda::std::asin(static_cast<float>(x.x)));
}


/**
 * @brief Arccosine function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Arccosine of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> acos(const matxHalf<T> &x)
{
  return static_cast<T>(cuda::std::acos(static_cast<float>(x.x)));
}

/**
 * @brief Arctangent function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Arctangent of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> atan(const matxHalf<T> &x)
{
  return static_cast<T>(cuda::std::atan(static_cast<float>(x.x)));
}

/**
 * @brief Two argument Arctangent function
 *
 * @tparam T Type of half
 * @param x Numerator
 * @param y Denominator
 * @return Arctangent of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> atan2(const matxHalf<T> x, const matxHalf<T> y)
{
  return static_cast<T>(cuda::std::atan2(static_cast<float>(x.x), static_cast<float>(y.x)));
}


/**
 * @brief Hyperbolic arcsine function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Hyperbolic arcsine of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> asinh(const matxHalf<T> &x)
{
  return static_cast<T>(cuda::std::asinh(static_cast<float>(x.x)));
}

/**
 * @brief Hyperbolic arccosine function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Hyperbolic arccosine of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> acosh(const matxHalf<T> &x)
{
  return static_cast<T>(cuda::std::acosh(static_cast<float>(x.x)));
}

/**
 * @brief Hyperbolic arctangent function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Hyperbolic arctangent of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> atanh(const matxHalf<T> &x)
{
  return static_cast<T>(cuda::std::atanh(static_cast<float>(x.x)));
}

/**
 * @brief Hyperbolic sine function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Hyperbolic sine of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> sinh(const matxHalf<T> &x)
{
  return static_cast<T>(cuda::std::sinh(static_cast<float>(x.x)));
}

/**
 * @brief Hyperbolic cosine function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Hyperbolic cosine of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> cosh(const matxHalf<T> &x)
{
  return static_cast<T>(cuda::std::cosh(static_cast<float>(x.x)));
}

/**
 * @brief Hyperbolic tangent function
 *
 * @tparam T Type of half
 * @param x Value of half
 * @return Hyperbolic tangent of input
 */
template <class T>
__MATX_HOST__ __MATX_DEVICE__ __MATX_INLINE__ matxHalf<T> tanh(const matxHalf<T> &x)
{
  return static_cast<T>(cuda::std::tanh(static_cast<float>(x.x)));
}

using matxFp16 = matxHalf<__half>; ///< Alias for fp16
using matxBf16 = matxHalf<__nv_bfloat16>; ///< Alias for bf16

}; // namespace matx
