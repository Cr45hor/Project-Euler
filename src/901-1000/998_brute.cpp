#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <algorithm>

// Define a bulletproof PI
const double PI = std::acos(-1.0);

const int NUM_ANGLES = 360000;
std::vector<double> cos_t(NUM_ANGLES);
std::vector<double> sin_t(NUM_ANGLES);

void precompute_angles() {
    for (int i = 0; i < NUM_ANGLES; ++i) {
        // Swap M_PI for our defined PI
        double theta = (PI / 2.0) * i / (NUM_ANGLES - 1.0);
        cos_t[i] = std::cos(theta);
        sin_t[i] = std::sin(theta);
    }
}

double get_min_bounding_square(double a, double b, double c) {
    double x_A = (a*a + c*c - b*b) / (2.0 * a);
    double y_A_sq = c*c - x_A*x_A;
    
    if (y_A_sq <= 0.0) return 0.0;
    
    double y_A = std::sqrt(y_A_sq);
    
    double vx[3] = {0.0, a, x_A};
    double vy[3] = {0.0, 0.0, y_A};
    
    double min_sq = 1e18; // Start with a massive float
    
    // Dense angle sweep
    for (int i = 0; i < NUM_ANGLES; ++i) {
        double ct = cos_t[i];
        double st = sin_t[i];
        
        double min_x = 1e18, max_x = -1e18;
        double min_y = 1e18, max_y = -1e18;
        
        for (int j = 0; j < 3; ++j) {
            double px = vx[j] * ct - vy[j] * st;
            double py = vx[j] * st + vy[j] * ct;
            
            if (px < min_x) min_x = px;
            if (px > max_x) max_x = px;
            if (py < min_y) min_y = py;
            if (py > max_y) max_y = py;
        }
        
        double w = max_x - min_x;
        double h = max_y - min_y;
        double sq_size = std::max(w, h);
        
        if (sq_size < min_sq) {
            min_sq = sq_size;
        }
    }
    return min_sq;
}

void compute_T(int n) {
    std::cout << "--- Computing T(" << n << ") [C++ Multithreaded Sweep] ---\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    
    long long total_perimeter = 0;
    int max_side = static_cast<int>(n * std::sqrt(2.0)) + 1;
    double tol = 1e-5;

    // Utilize all available cores
    #pragma omp parallel for reduction(+:total_perimeter) schedule(dynamic)
    for (int a = 1; a <= max_side; ++a) {
        for (int b = 1; b <= a; ++b) {
            for (int c = a - b + 1; c <= b; ++c) {
                
                double min_sq = get_min_bounding_square(a, b, c);
                if (min_sq == 0.0) continue;
                
                double nearest_int = std::round(min_sq);
                
                if (std::abs(min_sq - nearest_int) < tol && nearest_int <= n) {
                    long long p = a + b + c;
                    total_perimeter += p;
                    
                    // Critical section for clean I/O printing
                    #pragma omp critical
                    {
                        std::cout << "(" << std::setw(2) << a << ", " 
                                  << std::setw(2) << b << ", " 
                                  << std::setw(2) << c << ") | "
                                  << std::fixed << std::setprecision(6) << min_sq 
                                  << " -> " << static_cast<int>(nearest_int) 
                                  << " | P = " << p << "\n";
                    }
                }
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << "-------------------------------------------------------\n";
    std::cout << "T(" << n << ") = " << total_perimeter << "\n";
    std::cout << "Time taken: " << std::fixed << std::setprecision(3) << elapsed.count() << " seconds\n";
}

int main() {
    // Setup precomputed tables
    precompute_angles();
    
    // Explicit format setup to prevent e-notation
    std::cout << std::fixed << std::setprecision(6);
    
    // Test the specific edge case
    double test_val = get_min_bounding_square(19, 19, 17);
    std::cout << "Absolute pure minimum for (19, 19, 17) is: " << test_val << "\n\n";
    
    compute_T(40);
    return 0;
}