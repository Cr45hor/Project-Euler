def find_11_digit_pandigital_step_numbers():
    # A step number: abs(digit[i] - digit[i+1]) == 1
    # Pandigital: contains 0-9 at least once.
    # 11 digits means 10 digits (0-9) must appear at least once. 
    # One digit will be repeated.
    
    results = []
    
    def backtrack(current_number_str, mask):
        if len(current_number_str) == 11:
            if mask == 1023: # All bits 0-9 set
                results.append(current_number_str)
            return
        
        last_digit = int(current_number_str[-1])
        # Step up
        if last_digit < 9:
            next_digit = last_digit + 1
            backtrack(current_number_str + str(next_digit), mask | (1 << next_digit))
        # Step down
        if last_digit > 0:
            next_digit = last_digit - 1
            backtrack(current_number_str + str(next_digit), mask | (1 << next_digit))

    for start_digit in range(1, 10):
        backtrack(str(start_digit), (1 << start_digit))
        if len(results) >= 4:
            break
            
    return results[:4]

print(find_11_digit_pandigital_step_numbers())