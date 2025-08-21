#pragma once
#include <xbyak/xbyak.h>
#include <immintrin.h>
#include <cstdint>
#include <iostream>

/// @brief JIT 内核类 (基于 AVX2 + F16C)
/// 功能：将输入的 fp16 数据做 3x3 kernel, stride=2 的 avgpool (简单演示版)
class JitAvgPool3x3s2 : public Xbyak::CodeGenerator {
public:
    typedef void (*kernel_fn)(const uint16_t* even, const uint16_t* odd, float* dst, int OW);

    JitAvgPool3x3s2(bool odd_pad) {
        using namespace Xbyak;

        // 函数签名: void kernel(const uint16_t* even, const uint16_t* odd, float* dst, int OW)
        Xbyak::Reg64 reg_even = rdi;  // 输入行 even
        Xbyak::Reg64 reg_odd  = rsi;  // 输入行 odd
        Xbyak::Reg64 reg_dst  = rdx;  // 输出
        Xbyak::Reg64 reg_ow   = rcx;  // 输出宽度

        Xbyak::Reg64 reg_loop = r8;

        // SIMD 宽度 = 8 float (256 bit)
        const int simd_w = 8;
        Xbyak::Ymm ymm0 = ymm0, ymm1 = ymm1, ymm2 = ymm2, ymm_sum = ymm3;

        // 循环初始化
        xor_(reg_loop, reg_loop);

        L("loop");
        {
            if (odd_pad) {
                // odd pad 分支
                vmovdqu(xmm0, ptr[reg_odd + reg_loop*2 - 2]);  // s0
                vmovdqu(xmm1, ptr[reg_even + reg_loop*2]);     // s1
                vmovdqu(xmm2, ptr[reg_odd + reg_loop*2]);      // s2
            } else {
                // even pad 分支
                vmovdqu(xmm0, ptr[reg_even + reg_loop*2]);     // s0
                vmovdqu(xmm1, ptr[reg_odd  + reg_loop*2]);     // s1
                vmovdqu(xmm2, ptr[reg_even + reg_loop*2 + 2]); // s2
            }

            // 半精度转 float32
            vcvtph2ps(ymm0, xmm0);
            vcvtph2ps(ymm1, xmm1);
            vcvtph2ps(ymm2, xmm2);

            vaddps(ymm_sum, ymm0, ymm1);
            vaddps(ymm_sum, ymm_sum, ymm2);

            // 存结果
            vmovups(ptr[reg_dst + reg_loop*4], ymm_sum);

            add(reg_loop, simd_w);
            cmp(reg_loop, reg_ow);
            jl("loop");
        }

        ret();
    }

    kernel_fn get() {
        return (kernel_fn)getCode<void*>();
    }
};
