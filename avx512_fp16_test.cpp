#include <xbyak/xbyak.h>
#include <xbyak/xbyak_util.h>
#include <iostream>
#include <cstdint>
using namespace Xbyak::util;

constexpr std::uint64_t pow2(unsigned n) { return 1ULL << n; }

int main() {
    Cpu cpu_;
    if (cpu_.has(Cpu::tAVX512_FP16)) {
        printf("@yc : cpu support tAVX512_FP16\n");
    } else {
        printf("@yc : cpu not support tAVX512_FP16 !\n");
    }

    if (cpu_.has(Cpu::tAVX_VNNI_INT8)) {
        printf("@yc : cpu support tAVX_VNNI_INT8\n");
    } else {
        printf("@yc : cpu not support tAVX_VNNI_INT8 !\n");
    }

    if (cpu_.has(Cpu::tAVX2)) { // 这里你代码里重复写了两次 INT8，确认是不是另一个？
        printf("@yc : cpu support tAVX2\n");
    } else {
        printf("@yc : cpu not support tAVX2!\n");
    }

    if (cpu_.has(Cpu::tAVX_NE_CONVERT)) {
        printf("@yc : cpu support tAVX_NE_CONVERT\n");
    } else {
        printf("@yc : cpu not support tAVX_NE_CONVERT !\n");
    }

    return 0;
}
