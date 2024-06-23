#pragma once
#include <arm_neon.h>

#include <print>

#define vl_fp32 4

#define vregister float32x4_t
#define vinit(vreg) vreg = vmovq_n_f32(0.0f)
#define vload(vreg, mem) vreg = vld1q_f32(mem)
#define vstore(mem, vreg) vst1q_f32(mem, vreg)
#define vmadd(c, a, b, lane) c = vfmaq_laneq_f32(c, a, b, lane)

void neon_transpose(float *m) {
    vregister row0, row1, row2, row3;
    vload(row0, m);
    vload(row1, m + 4);
    vload(row2, m + 8);
    vload(row3, m + 12);

    float32x4x2_t row01 = vtrnq_f32(row0, row1);
    float32x4x2_t row23 = vtrnq_f32(row2, row3);

    vstore(m, vcombine_f32(vget_low_f32(row01.val[0]), vget_low_f32(row23.val[0])));
    vstore(m + 4, vcombine_f32(vget_low_f32(row01.val[1]), vget_low_f32(row23.val[1])));
    vstore(m + 8, vcombine_f32(vget_high_f32(row01.val[0]), vget_high_f32(row23.val[0])));
    vstore(m + 12, vcombine_f32(vget_high_f32(row01.val[1]), vget_high_f32(row23.val[1])));
}

inline void neon(float *A, float *B, float *C, int M, int N, int K) {
    vregister C0;
    vregister ar, br;

    vinit(C0);

    neon_transpose(B);

    vload(ar, A);
    vload(br, B);

    vmadd(C0, ar, br, 0);
    vmadd(C0, ar, br, 1);
    vmadd(C0, ar, br, 2);
    vmadd(C0, ar, br, 3);

    std::print("\nC Neon\n====\n");
    std::print("{} ", vgetq_lane_f32(C0, 0));

    vst1q_f32(C, C0);
}
