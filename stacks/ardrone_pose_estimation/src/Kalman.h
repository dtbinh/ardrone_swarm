/*
 *  QUT cyphy linear Kalman Filter implementation for 2D tracking
 *  Copyright (C) 2012, CYPHY lab
 *  Inkyu Sa <i.sa@qut.edu.au>
 *
 *  http://wiki.qut.edu.au/display/cyphy
 *
 *
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef KALMAN_H
#define KALMAN_H

#include <Eigen/Core>
#include <Eigen/StdVector>
#include <vector>
#include <algorithm>

using namespace Eigen;
using namespace std;

/*
#define MEA_VELNOISE 1.0
#define PRO_POSNOISE 0.003;
#define PRO_VELNOISE 0.005;
*/

#define MEA_VELNOISE 0.05
#define MEA_POSNOISE 0.1
#define PRO_POSNOISE 0.1
#define PRO_VELNOISE 0.3



//#define dt 0.005 //sec, 200Hz
#define dt 0.006 //sec, 162Hz

#define alpha 0.2
#define WINDOW_SIZE 10
//#define DEBUG_PRINT

typedef Matrix<float, 2, 4> Matrix2_4f;

class Kalman
{
    public:
        Kalman();
        virtual ~Kalman();
        Matrix<float, 4, 1> process(Matrix<float, 4, 1> measure,double yaw,bool* invertable);
        Matrix<float, 2, 1> LPF(Matrix<float, 2, 1> measure);
        Matrix<float, 2, 1> AVG(Matrix<float, 2, 1> measure);
        Matrix<float, 2, 1> MOVING_AVG(Matrix<float, 2, 1> measure);
        Matrix<float, 2, 1> median(Matrix<float, 2, 1> measure,bool* flag);
        void init();
        Matrix4f m_A,m_Q,m_P;
        Matrix4f m_H;
        Matrix4f m_R;
        //Vector4f m_x;
        Matrix<float, 4, 1> m_x;
        bool m_KM_initial;
        bool m_LPF_initial;
        bool m_AVG_initial;
        bool m_MOVING_AVG_initial;
        int m_total_num_samp;
        int m_moving_num_samp;
        FILE* fp_cov;
        FILE* fp_med_vel;
        vector<float> vmeasure[2];
        vector<float> pmeasure[2];

        Matrix<float, 2, 1> prevX_lpf,lpf_x;
        Matrix<float, 2, 1> prevX_avg,avg_x;
        Matrix<float, 2, 1> prevX_moving,moving_x;
        const double ts_;
    private:
        
};

#endif
