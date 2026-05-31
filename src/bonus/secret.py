import numpy as np
from PIL import Image

def solve_instant(image_path="./bonus_secret_statement.png", target=10**12):
    print(f"Loading image: {image_path}")
    img = Image.open(image_path).convert('L')
    pixels = np.array(img, dtype=np.int32) % 7
    H, W = pixels.shape
    
    # 1. Convert the target number to Base-7 digits (least significant to most significant)
    base7_digits = []
    temp = target
    while temp > 0:
        base7_digits.append(temp % 7)
        temp //= 7
        
    print(f"Target {target} in Base 7 (reversed) is: {base7_digits}")
    
    current_state = pixels.copy()
    shift_distance = 1
    
    # 2. Process each Base-7 digit
    for digit in base7_digits:
        # Apply the cellular automaton 'digit' times using the current shift_distance
        for _ in range(digit):
            # Optimize large shifts by wrapping them around the image dimensions
            dy = shift_distance % H
            dx = shift_distance % W
            
            # Shift the grid in all 4 directions by the jump distance
            up = np.roll(current_state, shift=-dy, axis=0)
            down = np.roll(current_state, shift=dy, axis=0)
            left = np.roll(current_state, shift=-dx, axis=1)
            right = np.roll(current_state, shift=dx, axis=1)
            
            # Sum and modulo 7
            current_state = (up + down + left + right) % 7
            
        # Move to the next power of 7
        shift_distance *= 7
        
    print("Computation complete! Generating final image...")
    
    # 3. Render the output (grayscale)
    # Stretch the 0-6 range to 0-255
    # 0 -> 0, 1 -> 42, 2 -> 85, ..., 6 -> 255
    output_pixels = (current_state * (255.0 / 6)).astype(np.uint8)
    output_image = Image.fromarray(output_pixels, mode='L')
    
    output_path = "secret_revealed_grayscale.png"
    output_image.save(output_path)
    print(f"Success! Saved as '{output_path}'.")

if __name__ == "__main__":
    solve_instant()