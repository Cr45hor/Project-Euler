import os
import sys

# Disable the 4300-digit limit for integer-to-string conversion
sys.set_int_max_str_digits(0)

CACHE_FILE = "min_s_cache.txt"
MAX_PRIME = 20000

def calculate_total_product():
    if not os.path.exists(CACHE_FILE):
        print(f"Error: {CACHE_FILE} not found.")
        return

    total_product = 1
    count = 0
    

    print("Reading cache and calculating product...")
    
    with open(CACHE_FILE, "r") as f:
        for line in f:
            if ":" in line:
                # Split the line by the colon to grab the 's' value
                p, s_str = line.split(":", 1)
                s_val = int(s_str.strip())
                if int(p.strip()) > MAX_PRIME:
                    break
                total_product *= s_val
                count += 1

    print(f"\nSuccessfully processed {count} primes.")
    
    # Convert to string to safely build scientific notation without float overflow
    product_str = str(total_product)
    exponent = len(product_str) - 1
    
    if exponent > 5:
        # Get the first 6 digits as an integer
        leading_digits = int(product_str[:6])
        
        # Check the 7th digit to see if we need to round up
        if int(product_str[6]) >= 5:
            leading_digits += 1
            
        # Catch the rare rollover (e.g., 999999 rounding up to 1000000)
        if len(str(leading_digits)) > 6:
            leading_digits //= 10
            exponent += 1
            
        rounded_str = str(leading_digits)
        mantissa = f"{rounded_str[0]}.{rounded_str[1:]}"
        
        print(f"Total Product: {mantissa}e+{exponent}")
    else:
        print(f"Total Product: {total_product}")

if __name__ == "__main__":
    calculate_total_product()