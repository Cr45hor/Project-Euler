from typing import Tuple, List

def get_24_rotations():
    """
    Generates all 24 valid 3D rotations of a standard 6-sided die.
    Faces are ordered as: (Left, Right, Bottom, Top, Back, Front)
    Opposite pairs (L,R), (B,T), (Bk,Fr) must sum to 7.
    """
    base: Tuple[int, int, int, int, int, int] = (1, 6, 2, 5, 3, 4)
    
    # Primitive 90-degree rotation functions
    def rot_x(p): return (p[0], p[1], p[4], p[5], p[3], p[2])
    def rot_y(p): return (p[5], p[4], p[2], p[3], p[0], p[1])
    def rot_z(p): return (p[3], p[2], p[0], p[1], p[4], p[5])
    
    rotations = set()
    queue = [base]
    
    while queue:
        curr = queue.pop(0)
        if curr not in rotations:
            rotations.add(curr)
            queue.extend([rot_x(curr), rot_y(curr), rot_z(curr)])
            
    return list(rotations)

ROTATIONS = get_24_rotations()

# Precompute allowed neighbors for massive speedup during axis generation
MATCH_X = {r: [nxt for nxt in ROTATIONS if r[1] == nxt[0]] for r in ROTATIONS}
MATCH_Y = {r: [nxt for nxt in ROTATIONS if r[3] == nxt[2]] for r in ROTATIONS}
MATCH_Z = {r: [nxt for nxt in ROTATIONS if r[5] == nxt[4]] for r in ROTATIONS}

def solve_dice_box(A, B, C):
    grid = {}
    
    def propagate_and_check():
        """
        Takes the skeletal axes and propagates the constraints to the interior.
        If a contradiction is found (zero valid dice for a position), it's invalid.
        """
        for x in range(A):
            for y in range(B):
                for z in range(C):
                    if (x, y, z) in grid:
                        continue # Already populated (it's the origin or on an axis)
                    
                    valid_rots = ROTATIONS
                    
                    # Apply left neighbor constraint
                    if x > 0:
                        req_left = grid[(x-1, y, z)][1]
                        valid_rots = [r for r in valid_rots if r[0] == req_left]
                        
                    # Apply bottom neighbor constraint
                    if y > 0:
                        req_down = grid[(x, y-1, z)][3]
                        valid_rots = [r for r in valid_rots if r[2] == req_down]
                        
                    # Apply back neighbor constraint
                    if z > 0:
                        req_back = grid[(x, y, z-1)][5]
                        valid_rots = [r for r in valid_rots if r[4] == req_back]
                        
                    # If constraints crush all options, this axis arrangement is invalid
                    if not valid_rots:
                        return False
                        
                    # A valid inner die is strictly forced (always exactly 1 choice)
                    grid[(x, y, z)] = valid_rots[0]
                    
        return True

    def build_axes(x, y, z):
        """
        Recursively builds the X axis, then the Y axis, then the Z axis.
        """
        if x < A:
            count = 0
            prev_die = grid[(x-1, 0, 0)]
            for r in MATCH_X[prev_die]:
                grid[(x, 0, 0)] = r
                count += build_axes(x+1, y, z)
            if (x, 0, 0) in grid: del grid[(x, 0, 0)]
            return count
            
        elif y < B:
            count = 0
            prev_die = grid[(0, y-1, 0)]
            for r in MATCH_Y[prev_die]:
                grid[(0, y, 0)] = r
                count += build_axes(x, y+1, z)
            if (0, y, 0) in grid: del grid[(0, y, 0)]
            return count
            
        elif z < C:
            count = 0
            prev_die = grid[(0, 0, z-1)]
            for r in MATCH_Z[prev_die]:
                grid[(0, 0, z)] = r
                count += build_axes(x, y, z+1)
            if (0, 0, z) in grid: del grid[(0, 0, z)]
            return count
            
        else:
            # All 3 axes are fully built. Validate the rest of the box.
            snapshot = grid.copy()
            is_valid = propagate_and_check()
            
            # Restore the grid back to just the skeletal axes for the next backtracking branch
            grid.clear()
            grid.update(snapshot)
            
            return 1 if is_valid else 0

    total_valid_arrangements = 0
    
    # 1. Choose the single corner die (0,0,0)
    for origin_die in ROTATIONS:
        grid[(0, 0, 0)] = origin_die
        # 2. Begin choosing dice for the axes
        total_valid_arrangements += build_axes(1, 1, 1) 
        grid.clear()
        
    return total_valid_arrangements

if __name__ == "__main__":
    for y in range(6):
        for z in range(6):
            print(solve_dice_box(1, y, z), end=' ')
        print()

    print()
    for y in range(6):
        for z in range(6):
            print(solve_dice_box(2, y, z), end=' ')
        print()
    
    print()
    for y in range(6):
        for z in range(6):
            print(solve_dice_box(3, y, z), end=' ')
        print()