import numpy as np
from scipy.optimize import minimize
import matplotlib.pyplot as plt


# Define the constants
A = np.array([-50, 0])
B = np.array([50, 0])
speeds = [10, 9, 8, 7, 6, 5, 10]

# Define the y-intercepts C_i for the 6 lines: y - x = C_i
# C_i = 50/sqrt(2) - i * 20/sqrt(2)
C = [(50 / np.sqrt(2)) - (i * 20 / np.sqrt(2)) for i in range(6)]

def get_point(t, i):
    """Returns the coordinates of point P_i given parameter t on line L_i."""
    return np.array([t, t + C[i]])

def objective_function(T):
    """
    T is an array of 6 parameters: [t_0, t_1, t_2, t_3, t_4, t_5].
    Calculates the total travel time.
    """
    # Generate the points P_0 to P_5 based on the parameters
    P = [get_point(T[i], i) for i in range(6)]
    
    # Calculate the distances for each segment
    time = 0.0
    
    # 1. From A to P_0
    time += np.linalg.norm(A - P[0]) / speeds[0]
    
    # 2. Through the 5 marsh regions (P_0->P_1, P_1->P_2, P_2->P_3, P_3->P_4, P_4->P_5)
    for i in range(5):
        time += np.linalg.norm(P[i] - P[i+1]) / speeds[i+1]
        
    # 3. From P_5 to B
    time += np.linalg.norm(P[5] - B) / speeds[6]
    
    return time

# Initial guess: the direct path from A to B (the x-axis, where y = 0).
# Since line L_i is defined as y - x = C_i, setting y = 0 gives x = -C_i.
# Because x = t, our initial t_i is simply -C_i.
initial_guess = [-c for c in C]

# Check the time of the direct path (matches the problem description)
direct_time = objective_function(initial_guess)
print(f"Time for direct straight-line path: {direct_time:.4f} days")

# Run the optimization
# BFGS or Nelder-Mead are good choices here; BFGS utilizes gradients for faster convergence.
result = minimize(
    objective_function, 
    initial_guess, 
    method='BFGS', 
    tol=1e-12, # High tolerance for 10 decimal places of accuracy
    options={'disp': True, 'maxiter': 10000}
)

if result.success:
    optimal_time = result.fun
    print("\nOptimization Successful!")
    print(f"Shortest possible time: {optimal_time:.10f} days")
    print("Optimal parameters (t_0 to t_5):")
    for i, t in enumerate(result.x):
        print(f"t_{i} = {t:.6f}")
else:
    print("Optimization failed to converge.")
    print(result.message)

# Extract the optimal points from the minimize result
P_opt = [A] + [get_point(result.x[i], i) for i in range(6)] + [B]
P_opt = np.array(P_opt)

# Extract the straight-line points for comparison
P_straight = [A] + [get_point(initial_guess[i], i) for i in range(6)] + [B]
P_straight = np.array(P_straight)

# Set up the plot
plt.figure(figsize=(12, 6))
plt.title("Frodo and Sam's Optimal Path vs. Straight Line")

# Plot the marsh boundaries
x_vals = np.linspace(-60, 60, 400)
for i in range(6):
    plt.plot(x_vals, x_vals + C[i], 'k--', alpha=0.3)
    
# Fill the marsh regions to make them visible
for i in range(5):
    plt.fill_between(x_vals, x_vals + C[i], x_vals + C[i+1], color='orange', alpha=0.1 + (i*0.05))

# Plot the paths
plt.plot(P_straight[:, 0], P_straight[:, 1], 'r--', label='Straight Path (13.4738 days)')
plt.plot(P_opt[:, 0], P_opt[:, 1], 'b-', marker='.', linewidth=2, label=f'Optimal Path ({optimal_time:.4f} days)')

# Plot start and end points
plt.scatter([-50, 50], [0, 0], c='black', zorder=5)
plt.text(-50, 2, ' A (-50, 0)', fontsize=10, fontweight='bold')
plt.text(50, 2, ' B (50, 0)', fontsize=10, fontweight='bold')

# Configure axes
plt.axis('equal') # Crucial to see the true geometric angles
plt.xlim(-60, 60)
plt.ylim(-30, 30)
plt.xlabel("East/West (Leagues)")
plt.ylabel("North/South (Leagues)")
plt.legend()
plt.grid(True, alpha=0.3)
plt.show()