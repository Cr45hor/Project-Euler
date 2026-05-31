import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import math
import time

def calculate_vertices(a, b, c):
    """Constructs triangle ABC where B=(0,0), C=(a,0)."""
    B = np.array([0.0, 0.0])
    C = np.array([a, 0.0])
    x_A = (a**2 + c**2 - b**2) / (2 * a)
    
    y_A_squared = c**2 - x_A**2
    if y_A_squared < 0:
        return None 
        
    y_A = math.sqrt(y_A_squared)
    A = np.array([x_A, y_A])
    return np.array([A, B, C])

def rotate_points(points, angles):
    """
    Vectorized rotation.
    points: (3, 2) array of vertices
    angles: (N,) array of angles in radians
    Returns: (N, 3, 2) array of rotated coordinates
    """
    cos_t = np.cos(angles)
    sin_t = np.sin(angles)
    
    # Pre-allocate output
    rotated = np.zeros((len(angles), 3, 2))
    
    # Apply rotation matrix manually for broadcasting
    rotated[:, :, 0] = points[:, 0] * cos_t[:, None] - points[:, 1] * sin_t[:, None]
    rotated[:, :, 1] = points[:, 0] * sin_t[:, None] + points[:, 1] * cos_t[:, None]
    
    return rotated

def get_min_bounding_square_bruteforce(a, b, c, num_angles=360000):
    """
    Zero assumptions. Just sweeps 360,000 angles.
    """
    x_A = (a**2 + c**2 - b**2) / (2 * a)
    y_A_sq = c**2 - x_A**2
    if y_A_sq <= 0: return 0.0
        
    y_A = math.sqrt(y_A_sq)
    Vx = np.array([0.0, a, x_A])
    Vy = np.array([0.0, 0.0, y_A])
    
    # Pure dense sweep of the 90-degree quadrant
    angles = np.linspace(0, np.pi/2, num_angles)
    
    cos_t = np.cos(angles)
    sin_t = np.sin(angles)
    
    # Vectorized X and Y projections for all angles simultaneously
    X = np.outer(cos_t, Vx) - np.outer(sin_t, Vy)
    Y = np.outer(sin_t, Vx) + np.outer(cos_t, Vy)
    
    # Find bounding box dimensions across all angles
    W = np.ptp(X, axis=1) # Width for each angle
    H = np.ptp(Y, axis=1) # Height for each angle
    
    # Minimum of the bounding squares
    return np.min(np.maximum(W, H))

def process_candidate(a, b, c, visualize=False, tol=1e-5):
    min_sq = get_min_bounding_square_bruteforce(a, b, c)
    if min_sq == 0.0:
        return False, 0
        
    nearest_int = round(min_sq)
    is_valid = abs(min_sq - nearest_int) < tol

    if is_valid and visualize:
        vertices = calculate_vertices(a, b, c)
        if vertices is not None:
            print(f"  [!] Visualizing confirmed triangle ({a}, {b}, {c}) -> Size: {min_sq:.6f}")
            _run_animation(vertices, a, b, c, min_sq)
            
    return is_valid, nearest_int

def _run_animation(vertices, a, b, c, target_size):
    angles = np.linspace(0, np.pi/2, 360) 
    fig, ax = plt.subplots(figsize=(8, 8))
    ax.set_aspect('equal')
    ax.grid(True, linestyle='--', alpha=0.6)
    
    poly = plt.Polygon(vertices, fill=True, color='lightblue', edgecolor='blue', alpha=0.7)
    ax.add_patch(poly)
    square_poly = plt.Polygon(np.zeros((4,2)), fill=False, color='red', linewidth=2)
    ax.add_patch(square_poly)
    
    max_dim = max(a, b, c) * 1.5
    ax.set_xlim(-max_dim, max_dim)
    ax.set_ylim(-max_dim, max_dim)
    
    # Re-using the single-angle rotation for the animation loop
    def rot_single(points, angle):
        cos_t, sin_t = np.cos(angle), np.sin(angle)
        R = np.array([[cos_t, -sin_t], [sin_t,  cos_t]])
        return points @ R.T

    def update_poly(frame):
        theta = angles[frame]
        rotated = rot_single(vertices, theta)
        
        min_x, max_x = np.min(rotated[:, 0]), np.max(rotated[:, 0])
        min_y, max_y = np.min(rotated[:, 1]), np.max(rotated[:, 1])
        
        size = max(max_x - min_x, max_y - min_y)
        cx, cy = (min_x + max_x) / 2, (min_y + max_y) / 2
        
        bl_x, bl_y = cx - size/2, cy - size/2
        corners = np.array([
            [bl_x, bl_y], [bl_x + size, bl_y], 
            [bl_x + size, bl_y + size], [bl_x, bl_y + size]
        ])
        
        square_poly.set_xy(rot_single(corners, -theta))
        ax.set_title(f"Triangle ({a},{b},{c}) | Angle: {np.degrees(theta):.1f}° | Size: {size:.4f}")
        
        if abs(size - target_size) < 1e-4:
            square_poly.set_color('green')
            square_poly.set_linewidth(3)
        else:
            square_poly.set_color('red')
            square_poly.set_linewidth(2)
        return square_poly,

    ani = FuncAnimation(fig, update_poly, frames=len(angles), interval=20, blit=True)
    plt.show() 

def compute_T(n, visualize=False):
    print(f"--- Computing T({n}) [Total Brute-Force Sweep] ---")
    start_time = time.time()
    total_perimeter = 0
    max_side = int(n * math.sqrt(2)) + 1
    
    print(f"{'Triangle (a, b, c)':<20} | {'Square Size':<12} | {'Perimeter'}")
    print("-" * 55)
    
    for a in range(1, max_side + 1):
        for b in range(1, a + 1):
            for c in range(a - b + 1, b + 1):
                
                is_valid, exact_size = process_candidate(a, b, c, visualize=visualize)
                
                if is_valid and exact_size <= n:
                    p = a + b + c
                    total_perimeter += p
                    print(f"({a:>2}, {b:>2}, {c:>2})          | {exact_size:>12} | {p:>4}")

    elapsed = time.time() - start_time
    print("-" * 55)
    print(f"T({n}) = {total_perimeter}")
    print(f"Time taken: {elapsed:.3f} seconds\n")

if __name__ == "__main__":
    # Test your specific triangle first
    min_size = get_min_bounding_square_bruteforce(19, 19, 17)
    print(f"Absolute pure minimum for (19, 19, 17) is: {min_size:.6f}\n")
    
    compute_T(40, visualize=False)