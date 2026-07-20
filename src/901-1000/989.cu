#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <cstdint>
#include <cuda_runtime.h>

const unsigned long long M = 1000000009ULL;
const unsigned long long N = 1e11; // 10^11

// Cranked to 1 Billion range per batch
const unsigned long long CHUNK_RANGE = 1000000000ULL; 

std::mutex console_mutex;

#define CUDA_CHECK(call) \
    do { \
        cudaError_t err = call; \
        if (err != cudaSuccess) { \
            std::lock_guard<std::mutex> lock(console_mutex); \
            std::cerr << "CUDA Error at " << __FILE__ << ":" << __LINE__ \
                      << " - " << cudaGetErrorString(err) << std::endl; \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

// ---------------------------------------------------------
// DEVICE FUNCTIONS & KERNELS
// (Kernel logic remains exactly the same as the Pure GPU version)
// ---------------------------------------------------------

__device__ void atomicRemoveFactor(unsigned long long* address, unsigned long long p) {
    unsigned long long old = *address;
    unsigned long long assumed;
    do {
        assumed = old;
        if (assumed % p != 0) break; 
        
        unsigned long long next = assumed / p;
        while (next % p == 0) next /= p; 
        
        old = atomicCAS(address, assumed, next);
    } while (assumed != old);
}

__global__ void init_kernel(unsigned long long L, unsigned long long elements, uint8_t* d_valid, uint32_t* d_G_count, unsigned long long* d_residual) {
    unsigned long long i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < elements) {
        d_valid[i] = 1;
        d_G_count[i] = 0;
        d_residual[i] = L + 2 * i; 
    }
}

__global__ void sieve_kernel(unsigned long long L, unsigned long long R, const long long* d_primes, int num_primes, uint8_t* d_valid, uint32_t* d_G_count, unsigned long long* d_residual) {
    int p_idx = blockIdx.x; 
    if (p_idx >= num_primes) return;

    long long p = d_primes[p_idx];
    long long start = ((L + p - 1) / p) * p;
    if (start % 2 == 0) start += p; 

    long long step = 2 * p;

    for (long long j = start + threadIdx.x * step; j <= R; j += blockDim.x * step) {
        unsigned long long idx = (j - L) / 2;

        if (p % 5 == 2 || p % 5 == 3) {
            d_valid[idx] = 0;
        } else if (p == 5) {
            if (j % 25 == 0) {
                d_valid[idx] = 0;
            } else if (d_valid[idx]) {
                atomicRemoveFactor(&d_residual[idx], p);
            }
        } else {
            if (d_valid[idx]) {
                atomicAdd(&d_G_count[idx], 1);
                atomicRemoveFactor(&d_residual[idx], p);
            }
        }
    }
}

__device__ __forceinline__ void fib_fast_doubling(unsigned long long n, unsigned long long& fn, unsigned long long& fn1) {
    if (n == 0) { fn = 0; fn1 = 1; return; }
    
    unsigned long long a = 0, b = 1;
    unsigned long long mask = 1ULL;
    while (mask <= n >> 1) mask <<= 1;
    
    for (; mask > 0; mask >>= 1) {
        unsigned long long t1 = (a * ((2 * b + 1000000009ULL - a) % 1000000009ULL)) % 1000000009ULL;
        unsigned long long t2 = (a * a % 1000000009ULL + b * b % 1000000009ULL) % 1000000009ULL;
        a = t1;
        b = t2;
        
        if (n & mask) {
            unsigned long long c = (a + b) % 1000000009ULL;
            a = b;
            b = c;
        }
    }
    
    fn = a;
    fn1 = b;
}

__global__ void fib_reduce_kernel(unsigned long long L, unsigned long long elements, uint8_t* d_valid, uint32_t* d_G_count, unsigned long long* d_residual, unsigned long long* d_out) {
    extern __shared__ unsigned long long sdata[];
    
    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;
    unsigned long long val = 0;
    
    if (tid == 0) {
        unsigned long long block_start_n = L + 2ULL * ((unsigned long long)blockIdx.x * blockDim.x);
        unsigned long long fn, fn1;
        fib_fast_doubling(block_start_n, fn, fn1);
        
        sdata[0] = fn;
        unsigned long long a = fn, b = fn1;
        for (unsigned int j = 1; j < blockDim.x; j++) {
            unsigned long long elem_idx = (unsigned long long)blockIdx.x * blockDim.x + j;
            if (elem_idx >= elements) break;
            unsigned long long new_a = (a + b) % 1000000009ULL;
            unsigned long long new_b = (new_a + b) % 1000000009ULL;
            sdata[j] = new_a;
            a = new_a;
            b = new_b;
        }
    }
    __syncthreads();
    
    if (i < elements) {
        uint8_t valid = d_valid[i];
        unsigned long long rem = d_residual[i];
        
        if (valid && rem > 1) {
            if (rem % 5 == 2 || rem % 5 == 3) valid = 0;
            else if (rem % 5 == 1 || rem % 5 == 4) d_G_count[i] += 1;
        }

        if (valid) {
            unsigned long long g_val = (1ULL << d_G_count[i]);
            unsigned long long f = sdata[tid];
            val = (f * g_val) % 1000000009ULL;
        }
    }
    
    sdata[tid] = val;
    __syncthreads();
    
    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) sdata[tid] = (sdata[tid] + sdata[tid + s]) % 1000000009ULL;
        __syncthreads();
    }
    
    if (tid == 0) d_out[blockIdx.x] = sdata[0];
}

// ---------------------------------------------------------
// CPU ORCHESTRATOR (Dynamic Multi-GPU)
// ---------------------------------------------------------

std::vector<long long> get_primes(long long limit) {
    std::vector<bool> is_prime(limit + 1, true);
    std::vector<long long> primes;
    for (long long p = 3; p <= limit; p += 2) { 
        if (is_prime[p]) {
            primes.push_back(p);
            for (long long i = p * p; i <= limit; i += 2 * p) is_prime[i] = false;
        }
    }
    return primes;
}

void gpu_worker(int gpu_id, unsigned long long start_range, unsigned long long end_range, const std::vector<long long>& primes, unsigned long long& final_sum) {
    CUDA_CHECK(cudaSetDevice(gpu_id));
    unsigned long long local_sum = 0;
    
    unsigned long long L = start_range;
    if (L % 2 == 0) L++;

    unsigned long long elements_per_chunk = CHUNK_RANGE / 2;
    
    uint8_t* d_valid;
    uint32_t* d_G_count;
    unsigned long long* d_residual;
    long long* d_primes;
    unsigned long long* d_out;

    CUDA_CHECK(cudaMalloc(&d_valid, elements_per_chunk * sizeof(uint8_t)));
    CUDA_CHECK(cudaMalloc(&d_G_count, elements_per_chunk * sizeof(uint32_t)));
    CUDA_CHECK(cudaMalloc(&d_residual, elements_per_chunk * sizeof(unsigned long long)));
    CUDA_CHECK(cudaMalloc(&d_primes, primes.size() * sizeof(long long)));
    CUDA_CHECK(cudaMemcpy(d_primes, primes.data(), primes.size() * sizeof(long long), cudaMemcpyHostToDevice));

    int threadsPerBlock = 256;
    int max_blocks = (elements_per_chunk + threadsPerBlock - 1) / threadsPerBlock;
    CUDA_CHECK(cudaMalloc(&d_out, max_blocks * sizeof(unsigned long long)));
    std::vector<unsigned long long> h_out(max_blocks);

    for (; L <= end_range; L += CHUNK_RANGE) {
        unsigned long long R = std::min(end_range, L + CHUNK_RANGE - 1);
        if (R % 2 == 0) R--; 
        if (L > R) break;

        unsigned long long current_elements = (R - L) / 2 + 1;
        int blocksPerGrid = (current_elements + threadsPerBlock - 1) / threadsPerBlock;

        init_kernel<<<blocksPerGrid, threadsPerBlock>>>(L, current_elements, d_valid, d_G_count, d_residual);
        CUDA_CHECK(cudaDeviceSynchronize());

        sieve_kernel<<<primes.size(), threadsPerBlock>>>(L, R, d_primes, primes.size(), d_valid, d_G_count, d_residual);
        CUDA_CHECK(cudaDeviceSynchronize());

        size_t sharedMemSize = threadsPerBlock * sizeof(unsigned long long);
        fib_reduce_kernel<<<blocksPerGrid, threadsPerBlock, sharedMemSize>>>(L, current_elements, d_valid, d_G_count, d_residual, d_out);
        CUDA_CHECK(cudaDeviceSynchronize());

        CUDA_CHECK(cudaMemcpy(h_out.data(), d_out, blocksPerGrid * sizeof(unsigned long long), cudaMemcpyDeviceToHost));
        
        for (int i = 0; i < blocksPerGrid; ++i) {
            local_sum = (local_sum + h_out[i]) % M;
        }

        std::lock_guard<std::mutex> lock(console_mutex);
        std::cout << "[GPU " << gpu_id << "] Processed up to " << R << ". Subtotal: " << local_sum << std::endl;
    }

    cudaFree(d_valid); cudaFree(d_G_count); cudaFree(d_residual); cudaFree(d_primes); cudaFree(d_out);
    final_sum = local_sum;
}

int main() {
    std::cout << "Target N: " << N << "\nInitializing...\n";

    long long limit = std::sqrt(N) + 1;
    std::vector<long long> primes = get_primes(limit);
    
    int active_gpus[] = {0, 1};
    int num_active = 2;
    
    std::vector<unsigned long long> sums(num_active, 0);
    std::vector<std::thread> workers;

    unsigned long long range_per_gpu = N / num_active;

    for (int i = 0; i < num_active; ++i) {
        unsigned long long start = i * range_per_gpu + 1;
        unsigned long long end = (i == num_active - 1) ? N : (i + 1) * range_per_gpu;
        
        // Pass the actual physical ID (active_gpus[i]) instead of index 0-3
        workers.emplace_back(gpu_worker, active_gpus[i], start, end, std::ref(primes), std::ref(sums[i]));
    }

    for (auto& t : workers) {
        t.join();
    }

    unsigned long long total_sum = 0;
    for (int i = 0; i < num_active; ++i) {
        total_sum = (total_sum + sums[i]) % M;
    }

    std::cout << "\n========================================\n";
    std::cout << "Final Sum modulo " << M << ": " << total_sum << std::endl;
    std::cout << "========================================\n";

    return 0;
}