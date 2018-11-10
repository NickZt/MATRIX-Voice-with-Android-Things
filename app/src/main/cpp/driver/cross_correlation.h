/*
 * Copyright 2016 <Admobilize>
 * MATRIX Labs  [http://creator.matrix.one]
 * This file is part of MATRIX Creator HAL
 *
 * MATRIX Creator HAL is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPP_CROSS_CORRELATION_H_
#define CPP_CROSS_CORRELATION_H_

#include <stdint.h>
#include "../fft/kiss_fft.h"
#include "../fft/_kiss_fft_guts.h"
#include "../fft/kiss_fftr.h"

namespace matrix_hal {

/*
Cross-correlation between signals implemented in frequency domain.
*/
    class CrossCorrelation {
    public:
        CrossCorrelation();

        ~CrossCorrelation();

        bool Init(int N);

        void Release();

        //   void Exec(int16_t *a, int16_t *b);

        //   kiss_fft_cpx * getResult();

        void ExecR(int16_t *a, int16_t *b);

        int32_t *getResultR();

    private:
        void Corr(kiss_fft_cpx *out, kiss_fft_cpx *x, kiss_fft_cpx *y);

        int freq_len;
        int freq_ini_len;
        int order_;
        kiss_fft_cpx *in_;
        kiss_fft_cpx *A_;
        kiss_fft_cpx *B_;
        kiss_fft_cpx *C_;


        kiss_fft_cfg forward_plan_a_;//kiss_fft_state

        kiss_fft_cfg inverse_plan_;//fftwf_plan
        //    void CorrR(int32_t *out, kiss_fft_cpx *x, kiss_fft_cpx *y);

        kiss_fft_scalar *inf;

        kiss_fft_scalar *cf;
        kiss_fftr_cfg rforward_plan;
        kiss_fftr_cfg rinverse_plan;


    };

};      // namespace matrix_hal
#endif  // CPP_CROSS_CORRELATION_H_
