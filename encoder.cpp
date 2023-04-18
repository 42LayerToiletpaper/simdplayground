//
// Created by mo on 4/11/23.
//

#include "encoder.h"

//#include <string>



void Encoder::encodeString(const std::string &in, std::string &out) {
    out.clear();
    out.resize((in.size() / 3) * 4 + 16) ;
    const char* inPtr = &in[0];
    char* outPtr = &out[0];

    const size_t loop = in.size() / 12;

    for(int i = 0; i < loop; i++){
        //everytime the last 4 out of 16 byte string data get lost through processing, therefore 12 byte jumps
        simde__m128i data = simde_mm_lddqu_si128((simde__m128i*) (inPtr + i*12) );

        registerDecoder(data);

        simde_mm_storeu_si128((simde__m128*) (outPtr + i * 16), data);

    }
    size_t read = loop *12;
    size_t written = loop * 16;
    std::string rest = in.substr(read);
    std::string appendix(16 - rest.size(), 0);
    rest = rest + appendix;

    simde__m128i data = simde_mm_lddqu_si128((simde__m128i*) & rest[0]);
    registerDecoder(data);
    simde_mm_storeu_si128((simde__m128*) (outPtr + written), data);

    uint8_t padding = (3 - in.size() % 3 ) %3;

    if(padding) {
        out.resize((in.size() / 3) * 4 + 4);
        for(auto it = out.rbegin(); it < out.crbegin() + padding; it++){
            *it = '=';
        }

    }else{
        out.resize((in.size() / 3) * 4);
    }







}

void Encoder::registerDecoder(simde__m128i &data) {
    data = simde_mm_shuffle_epi8(data, shuffleMask);
    //by shuffling every 3 8bit ASCII Letters now take up 4 bytes, "ABC" gets shuffled to "CCBA" to match the 4 bytes of the Base64 Encoding, and deal with little Endianness.

    data =   simde_mm_srli_epi32(data, 2 ) & mask1 |
             simde_mm_srli_epi32(data, 4 ) & mask2 |
             simde_mm_srli_epi32(data, 6 ) & mask3 |
             data & mask4;

    data = simde_mm_shuffle_epi8(data, shuffleMask2);


    simde__m128i bigLetterMask= simde_mm_cmplt_epi8(data, _mm_set1_epi8(26)); // (a < b) ? 0xFF : 0x00
    simde__m128i allMask= bigLetterMask;
    simde__m128i smallLetterMask = simde_mm_andnot_si128(allMask, simde_mm_cmplt_epi8(data,simde_mm_set1_epi8(52))); //not allMask and  b where b is 0xFF if binaries are smaller than 52
    allMask |= smallLetterMask;
    simde__m128i numberMask = simde_mm_andnot_si128(allMask, simde_mm_cmplt_epi8(data, simde_mm_set1_epi8(62)));
    allMask |= numberMask;
    simde__m128i plusMask = simde_mm_andnot_si128(allMask, simde_mm_cmplt_epi8(data, simde_mm_set1_epi8(63)));
    allMask |= plusMask;
    simde__m128i & slashAntiMask = allMask;

    data =  bigLetterMask   & simde_mm_add_epi8(data, allA)|
            smallLetterMask & simde_mm_add_epi8(data, alla)|
            numberMask      & simde_mm_add_epi8(data, all0)|
            plusMask        & allPlus                      |
            simde_mm_andnot_si128(slashAntiMask, allSlash);
}
