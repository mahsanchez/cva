#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iterator>

#include "mkl_lapacke.h"

// calibration
// load yield curve matrix 25 years
// calculate the difference matrix
// calculate correlation matrix
// calculate eigen values, & eigen vectors
// calculate PCA

const int vols = 3;
const int pathN = 1012;
const int tenors = 51;

using namespace std;


// Eigen Values
std::vector<float> eigen_values = {0.002027, 0.000463, 0.000164 };

std::vector<std::vector<float>> eigen_vectors = {
        { 0.003510,0.056656,0.101143,0.115640,0.121541,0.125682,0.129490,0.133205,0.136820,0.140262,0.143445,0.146283,0.148702,0.150642,0.152070,0.152982,0.153400,0.153377,0.152979,0.152285,0.151381,0.150350,0.149267,0.148192,0.147167,0.146223,0.145379,0.144641,0.144010,0.143486,0.143064,0.142732,0.142474,0.142282,0.142146,0.142059,0.142011,0.141998,0.142015,0.142060,0.142128,0.142214,0.142315,0.142427,0.142550,0.142682,0.142825,0.142978,0.143143,0.143320,0.143510},
        { -0.009726,-0.163267,-0.238915,-0.243456,-0.235099,-0.226564,-0.219032,-0.212070,-0.205164,-0.197917,-0.190011,-0.181180,-0.171207,-0.159904,-0.147125,-0.132784,-0.116868,-0.099436,-0.080630,-0.060677,-0.039880,-0.018610,0.002715,0.023672,0.043847,0.062856,0.080376,0.096160,0.110047,0.121954,0.131846,0.139730,0.145662,0.149737,0.152074,0.152796,0.152033,0.149915,0.146580,0.142164,0.136808,0.130650,0.123817,0.116414,0.108517,0.100186,0.091471,0.082418,0.073073,0.063481,0.053685},
        { 0.001115,-0.273138,-0.402224,-0.355810,-0.274743,-0.195850,-0.125003,-0.062354,-0.007093,0.041354,0.083252,0.118658,0.147546,0.169903,0.185820,0.195522,0.199365,0.197853,0.191599,0.181299,0.167702,0.151557,0.133590,0.114476,0.094812,0.075131,0.055870,0.037362,0.019844,0.003492,-0.011550,-0.025186,-0.037365,-0.048080,-0.057341,-0.065187,-0.071667,-0.076856,-0.080837,-0.083696,-0.085529,-0.086438,-0.086521,-0.085865,-0.084542,-0.082624,-0.080177,-0.077263,-0.073942,-0.070272,-0.066312}
};
// Eigen Vector

std::vector<std::vector<float>> pca/* (vols, std::vector<float>(tenors, 0.0))*/ = {
        {0.000158052,  0.00255092,   0.004553935,  0.005206658,  0.005472358,  0.005658831,  0.005830249,  0.005997511,  0.006160278,  0.006315277,  0.006458599,  0.006586383,  0.006695282,  0.006782641,  0.006846943,  0.006887967,  0.006906816,  0.006905764,  0.006887838,  0.006856589,  0.00681588,   0.006769484,  0.006720733, 0.006672296, 0.006626153, 0.006583674, 0.00654567,  0.006512419, 0.006484015, 0.006460448, 0.006441442,  0.006426462,  0.006414877,  0.006406218,  0.006400114,  0.006396192,  0.006394035,  0.006393418,  0.006394203,  0.006396235,  0.006399289,  0.006403156,  0.006407686,  0.006412758,  0.006418275,  0.006424234,  0.006430655,  0.006437563,  0.006444989, 0.006452958,  0.006461498},
        {-0.000209176, -0.00351128,  -0.005138186, -0.00523585,  -0.005056114, -0.004872556, -0.004710585, -0.004560842, -0.004412323, -0.004256474, -0.004086436, -0.003896528, -0.003682047, -0.003438956, -0.003164122, -0.002855704, -0.002513401, -0.002138505, -0.001734055, -0.001304938, -0.000857673, -0.000400225, 5.83957E-05, 0.000509103, 0.000942984, 0.001351808, 0.001728602, 0.002068045, 0.002366709, 0.00262279,  0.002835516,  0.00300508,   0.003132648,  0.003220292,  0.00327055,   0.003286087,  0.003269671,  0.003224134,  0.003152399,  0.003057425,  0.002942248,  0.00280981,   0.002662849,  0.002503631,  0.002333814,  0.002154633,  0.0019672,    0.001772514,  0.001571541, 0.001365245,  0.001154578},
        {1.42604E-05,  -0.003493048, -0.005143881, -0.004550311, -0.00351357,  -0.00250465,  -0.001598615, -0.000797418, -9.0714E-05,  0.000528853,  0.001064675,  0.001517464,  0.001886902,  0.002172825,  0.002376375,  0.002500449,  0.002549591,  0.00253026,   0.002450278,  0.002318565,  0.002144675,  0.0019382,    0.001708426, 0.00146399,  0.001212517, 0.000960819, 0.000714499, 0.000477804, 0.000253771, 4.46544E-05, -0.000147713, -0.000322088, -0.000477846, -0.000614876, -0.000733315, -0.000833644, -0.000916521, -0.000982878, -0.001033789, -0.001070356, -0.001093791, -0.001105416, -0.001106481, -0.001098088, -0.001081175, -0.001056643, -0.001025353, -0.000988089, -0.00094561, -0.000898676, -0.000848036}
};

// Volatility Calibration
//std::vector<std::vector<float>> volatilities(vols, std::vector<float>(tenors, 0.0));
/*=
{
{0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655,0.006430655 },
{-0.003556543,-0.003811644,-0.004010345,-0.004155341,-0.004249328,-0.004295001,-0.004295055,-0.004252185,-0.004169088,-0.004048459,-0.003892993,-0.003705385,-0.003488331,-0.003244526,-0.002976667,-0.002687447,-0.002379563,-0.00205571,-0.001718584,-0.001370879,-0.001015292,-0.000654518,-0.000291251,7.18113E-05,0.000431975,0.000786544,0.001132823,0.001468117,0.001789731,0.002094968,0.002381133,0.002645532,0.002885468,0.003098246,0.003281171,0.003431548,0.00354668,0.003623873,0.00366043,0.003653657,0.003600859,0.003499339,0.003346403,0.003139354,0.002875498,0.002552139,0.002166581,0.00171613,0.00119809,0.000609765,-5.15406E-05 },
{-0.004750672,-0.003908573,-0.003134891,-0.00242728,-0.001783395,-0.001200891,-0.000677421,-0.00021064,0.000201797,0.000562236,0.000873023,0.001136502,0.00135502,0.001530923,0.001666555,0.001764262,0.00182639,0.001855285,0.001853291,0.001822755,0.001766022,0.001685437,0.001583346,0.001462095,0.00132403,0.001171495,0.001006836,0.000832399,0.00065053,0.000463573,0.000273875,8.37816E-05,-0.000104363,-0.000288212,-0.00046542,-0.000633643,-0.000790533,-0.000933746,-0.001060936,-0.001169758,-0.001257866,-0.001322914,-0.001362557,-0.001374449,-0.001356245,-0.0013056,-0.001220167,-0.001097601,-0.000935557,-0.000731689,-0.000483652}
};*/


// Drift Callibration

//std::vector<float> drifts(tenors);
/*
 *  {0.000000,0.000036,0.000069,0.000099,0.000128,0.000155,0.000182,0.000208,0.000233,0.000257,0.000280,0.000303,0.000324,0.000345,0.000364,0.000381,0.000397,0.000412,0.000426,0.000438,0.000449,0.000459,0.000469,0.000478,0.000487,0.000496,0.000505,0.000515,0.000526,0.000538,0.000551,0.000566,0.000583,0.000601,0.000621,0.000643,0.000667,0.000692,0.000718,0.000746,0.000774,0.000803,0.000832,0.000861,0.000889,0.000916,0.000943,0.000967};
*/

void callibrate_volatilities(/*std::vector<float> &eigen_values, std::vector<std::vector<float>> &eigen_vectors, */ std::vector<std::vector<float>> &volatilities, std::vector<float> &drifts) {
    //obtain eigen values and eigen vectors using Intel MKL LAPACKE_dgeev

    //pca - eigen_values std deviation
    /*
    for (int i = 0; i < vols; i++) {
        const float std_dev = std::sqrt(eigen_values[i]);
        std::transform(eigen_vectors[i].begin(), eigen_vectors[i].end(), pca[i].begin(), [std_dev](float value) {
            return std_dev * value;
        });
    };
     */

    // Estimate b0, b1, b2, b3 by linear least square fitting for Vol1, Vol2 using intel MKL/BLAS LAPACKE_sgelsd

    float b3 = -3.59394E-06, b2 = 0.000118192, b1 = -0.0005684, b0 = -0.003556543;
    auto vol_2 = [b0, b1, b2, b3](float tau) {
        float result = b0;
        result += tau * b1;
        result += std::pow(tau, 2) * b2;
        result += std::pow(tau, 3) * b3;
        return result;
    };

    b3 = 3.127E-6; b2 = -0.000141525; b1 = 0.001754178; b0 = -0.004750672;
    auto vol_3 = [b0, b1, b2, b3](float tau) {
        float result = b0;
        result += tau*b1;
        result += std::pow(tau, 2) * b2;
        result += std::pow(tau, 3) *b3;
        return result;
    };

    // Vol_1
    b0 = std::accumulate(pca[0].begin(), pca[0].end(), 0.0);
    b0 = b0/tenors;
    std::fill(volatilities[0].begin(), volatilities[0].end(), b0);

    // Vol_2
    int idx = 0;
    std::transform(volatilities[1].begin(), volatilities[1].end(), volatilities[1].begin(), [&idx](float tau) {
        float result = 0.5 * idx;
        idx++;
        return result;
    });
    std::transform(volatilities[1].begin(), volatilities[1].end(), volatilities[1].begin(), vol_2);

    // Vol_3
    idx = 0;
    std::transform(volatilities[2].begin(), volatilities[2].end(), volatilities[2].begin(), [&idx](float tau) {
        float result = 0.5 * idx;
        idx++;
        return result;
    });
    std::transform(volatilities[2].begin(), volatilities[2].end(), volatilities[2].begin(), vol_3);

    // drift : This funciton carries out integration for all principal factors. It uses the fact that volatility is function of time in HJM model
    idx = 0;
    std::transform(drifts.begin(), drifts.end(), drifts.begin(), [&idx](float value){
        float result = 0.5 * idx;
        idx++;
        return result;
    });

    std::transform(drifts.begin(), drifts.end(), drifts.begin(), [&volatilities, &vol_2, &vol_3](float tau) {
        float m = 0.0;
        if (tau != 0) {
            float dtau = 0.01;
            int N = tau / dtau;
            dtau = tau / N;
            const float vol0 = volatilities[0][0];

            // using trapezium rule to compute M1
            float m1 = 0.5 * vol0;
            m1 += N * vol0;
            m1 += 0.5 * vol0;
            m1 *= dtau;
            m1 *= vol0;
            m += m1;

            // using trapezium rule to compute M2
            float m2 = 0.5 * vol_2(0.0);
            for (int i = 1; i < N; i++) {
                m2 += vol_2(i * dtau);
            }
            m2 += 0.5 * vol_2(tau);
            m2 *= dtau;
            m2 *= vol_2(tau);
            m += m2;

            // using trapezium rule to compute M3
            float m3 = 0.5 * vol_3(0.0);
            for (int i = 1; i < N; i++) {
                m3 += vol_3(i * dtau);
            }
            m3 += 0.5 * vol_3(tau);
            m3 *= dtau;
            m3 *= vol_3(tau);
            m += m3;
        }
        return m;
    });
}
