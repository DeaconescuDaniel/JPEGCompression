import numpy as np
import cv2

NUM_TESTS = 1

def generate_random_matrix():
    return np.random.randint(0, 256, (8, 8), dtype=np.uint8)

def compute_dct(matrix):
    float_matrix = np.float32(matrix) - 128
    dct_matrix = cv2.dct(float_matrix)
    return dct_matrix

for test_num in range(1, NUM_TESTS + 1):
    input_matrix = generate_random_matrix()
    dct_output = compute_dct(input_matrix)
    
    print(f"\n**Test Case {test_num}**")
    print("Input Matrix (8×8):")
    print(input_matrix)

    print("\nDCT Output Matrix (8×8):")
    print(dct_output)

    print("-" * 50)
