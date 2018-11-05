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

#include "cross_correlation.h"
#include <cstring>
#include <malloc.h>
#include <android/log.h>
#include "../fft/kiss_fft.h"
#include "../fft/qmath.h"
#include "../fft/kiss_fftr.h"

#define FFT_ORDER 64


namespace matrix_hal {
    bool is_inverse_fft = true;

    CrossCorrelation::CrossCorrelation()
            : order_(0),
              in_(NULL),
              A_(NULL),
              B_(NULL),
              C_(NULL){}

    CrossCorrelation::~CrossCorrelation() { Release(); }

    void CrossCorrelation::Release() {
        if (in_) delete in_;
        if (A_) delete A_;
        if (B_) delete B_;
        if (C_) delete C_;
        if (forward_plan_a_) free(forward_plan_a_);

        if (inverse_plan_) free(inverse_plan_);
    }

    bool CrossCorrelation::Init(int N) {
        order_ = N;
        freq_len = order_ / 2 + 2;


//        in_ = new kiss_fft_cpx[order_];
//        forward_plan_a_ =
//                kiss_fft_alloc(freq_len, 0, 0, 0);
//        inverse_plan_ =
//                kiss_fft_alloc(freq_len, is_inverse_fft, 0, 0);
//        if (!forward_plan_a_) return false;

        A_ = new kiss_fft_cpx[freq_len];
        B_ = new kiss_fft_cpx[freq_len];
        C_ = new kiss_fft_cpx[freq_len];

        inf = new kiss_fft_scalar[order_];

        cf = new kiss_fft_scalar[order_];

        rforward_plan =
                kiss_fftr_alloc(freq_len, 0, NULL, NULL);
        rinverse_plan =
                kiss_fftr_alloc(freq_len, is_inverse_fft, NULL, NULL);
        return true;
    }

    kiss_fft_scalar *CrossCorrelation::getResultR() { return cf; }

   // kiss_fft_cpx *CrossCorrelation::getResult() { return c_; }

/*    void CrossCorrelation::Exec(int16_t *a, int16_t *b) {
        for (uint32_t s = 0; s < order_; s++) {
            in_[s].r = float2q(fabs(a[s]) / 64.0);
            in_[s].i = in_[s].r ;//0;
        }
        kiss_fft(forward_plan_a_, in_, A_);
        for (int i = 0; i < order_; i++) {
            in_[i].r = float2q(fabs(b[i]) / 64.0);
            in_[i].i =  in_[i].r ;//0;//
        }
        kiss_fft(forward_plan_a_, in_, B_);
        Corr(C_, A_, B_);
        kiss_fft(inverse_plan_, C_, c_);

        for (int i = 0; i < order_; i++) {
            c_[i].r = c_[i].r / order_;
        }
    }*/

    void CrossCorrelation::ExecR(int16_t *a, int16_t *b) {
//        __android_log_print(ANDROID_LOG_DEBUG, "TODEL ",
//                            "CrossCorrelation::0ExecR :->order| %d|, freq_le =%d|",order_,freq_len);
        for (int i = 0; i < order_; i++) {
            inf[i] =a[i]; //float2q(fabs(a[i]) / 64.0);//a[i]/ 64;//float2q(fabs(a[i]) ); //a[i] / 1024;//
        }
        kiss_fftr(rforward_plan, inf, A_);
//        __android_log_print(ANDROID_LOG_DEBUG, "TODEL ",
//                            "CrossCorrelation::ExecR :->order| %d|, freq_le =%d|",order_,freq_len);
        for (int i = 0; i < order_; i++) {
            inf[i] =b[i];//float2q(fabs(b[i]) / 64.0);// b[i]/ 64;//float2q(fabs(b[i]));//b[i] / 1024;//  /1024.0
        }
        kiss_fftr(rforward_plan, inf, B_);
        Corr(C_, A_, B_);

        kiss_fftri(rinverse_plan, C_, cf);
// in cf  result

    }

    void CrossCorrelation::Corr(kiss_fft_cpx *out, kiss_fft_cpx *x, kiss_fft_cpx *y) {

        for (int j = 0; j < freq_len ; j++) {/// 2

            y[j].i = -y[j].i;
          //  y[order_ - j].i = -y[order_ - j].i;
         //   C_MUL(out[order_ - j], x[order_ - j], y[order_ - j]);
            C_MUL(out[j], x[j], y[j]);

        }
    }



};  // namespace matrix_hal
