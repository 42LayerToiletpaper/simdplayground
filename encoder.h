//
// Created by mo on 4/11/23.
//
#include <string>
#include </home/mo/IT/openMS/simde-master/simde/x86/sse.h>
#include </home/mo/IT/openMS/simde-master/simde/x86/sse2.h>
#include </home/mo/IT/openMS/simde-master/simde/x86/ssse3.h>

#ifndef SSEPLAYGROUND_ENCODER_H
#define SSEPLAYGROUND_ENCODER_H


class Encoder {

public:

    void encodeString(const std::string &in, std::string &out);

private:

    void registerDecoder(simde__m128i & data);
    const simde__m128i mask1 = simde_mm_set1_epi32(0x3F000000);//00111111 00000000 00000000 00000000
    const simde__m128i mask2 = simde_mm_set1_epi32(0x003F0000);//00000000 00111111 00000000 00000000
    const simde__m128i mask3 = simde_mm_set1_epi32(0x00003F00);//00000000 00000000 00111111 00000000
    const simde__m128i mask4 = simde_mm_set1_epi32(0x0000003F);//00000000 00000000 00000000 00111111



    const simde__m128i allA = simde_mm_set1_epi8('A');
    const simde__m128i alla = simde_mm_set1_epi8('a' - 26);
    const simde__m128i all0 = simde_mm_set1_epi8('0' - 52);
    const simde__m128i allPlus = simde_mm_set1_epi8('+');
    const simde__m128i allSlash = simde_mm_set1_epi8('/');

    const simde__m128i shuffleMask = simde_mm_setr_epi8(2, 2, 1, 0, 5, 5, 4, 3, 8, 8, 7, 6, 11, 11, 10, 9);
    const simde__m128i shuffleMask2 = simde_mm_setr_epi8(3,2,1,0,7,6,5,4,11,10,9,8,15,14,13,12);
    //simde__m512i bigLetterMask, smallLetterMask, numberMask, plusMask, allMask;
};

#endif //SSEPLAYGROUND_ENCODER_H
