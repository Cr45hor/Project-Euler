import numpy as np
from scipy.optimize import root_scalar

# Define the constants
A = np.array([-50, 0])
B = np.array([50, 0])
speeds = [10, 9, 8, 7, 6, 5, 10]

# y-intercepts C_i for the boundaries: y = x + C_i
C = [(50 / np.sqrt(2)) - (i * 20 / np.sqrt(2)) for i in range(6)]

def trace_ray(theta0, return_path=False):
    """
    Fires a ray from point A with an initial angle theta0 (relative to the normal).
    Returns the final Y coordinate when X hits 50, the total time, and the path.
    """
    # 1. Calculate the Snell Constant (K) for this specific ray
    K = np.sin(theta0) / speeds[0]
    
    X, Y = A[0], A[1]
    time = 0.0
    path = [(X, Y)]
    
    # 2. Trace the ray through the 6 boundaries
    for i in range(6):
        # Snell's Law determines the exact angle in the current medium
        theta_i = np.arcsin(K * speeds[i])
        
        # Convert angle relative to the normal (-45 degrees) back to absolute grid angle
        phi_i = theta_i - np.pi/4 
        
        # Calculate distance 's' along the ray until it intersects the next boundary (y = x + C_i)
        s = (X - Y + C[i]) / (np.sin(phi_i) - np.cos(phi_i))
        
        # Move forward
        X += s * np.cos(phi_i)
        Y += s * np.sin(phi_i)
        time += s / speeds[i]
        path.append((X, Y))
        
    # 3. Final leg through the 7th medium to the finish line at X = 50
    theta_6 = np.arcsin(K * speeds[6])
    phi_6 = theta_6 - np.pi/4
    
    # Calculate distance 's' to hit exactly X = 50
    s_final = (50 - X) / np.cos(phi_6)
    X = 50.0 
    Y += s_final * np.sin(phi_6)
    time += s_final / speeds[6]
    path.append((X, Y))
    
    if return_path:
        return Y, time, path
    return Y, time

def objective_function(theta0):
    """ We want the final Y coordinate to be exactly 0 (Point B). """
    y_final, _ = trace_ray(theta0)
    return y_final

# 4. Root Finding: Find the exact initial angle that makes y_final = 0
# A straight line East is 45 degrees (pi/4) relative to the normal. 
# We search between 0 and 90 degrees (pi/2).
result = root_scalar(
    objective_function, 
    bracket=[0, np.pi/2], 
    method='brentq', 
    xtol=1e-12
)

if result.converged:
    best_theta0 = result.root
    final_y, optimal_time, best_path = trace_ray(best_theta0, return_path=True)
    
    print("Snell's Law Optimization Successful!")
    print(f"Optimal starting angle (relative to normal): {np.degrees(best_theta0):.4f}°")
    print(f"Shortest possible time: {optimal_time:.10f} days")
    print(f"Final destination reached: ({best_path[-1][0]:.1f}, {best_path[-1][1]:.10f})")
else:
    print("Optimization failed to converge.")