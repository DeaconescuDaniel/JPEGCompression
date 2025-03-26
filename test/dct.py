import numpy as np
import cv2

NUM_TESTS = 1  # Number of test cases

# JPEG Luminance Quantization Table (8x8)
quant_table_Y = np.array([
    6, 4, 4, 6, 10, 16, 20, 24,
    5, 5, 6, 8, 10, 23, 24, 22,  
    6, 5, 6, 10, 16, 23, 28, 22,  
    6, 7, 9, 12, 20, 35, 32, 25,  
    7, 9, 15, 22, 27, 44, 41, 31, 
    10, 14, 22, 26, 32, 42, 45, 37,  
    20, 26, 31, 35, 41, 48, 48, 40,  
    29, 37, 38, 39, 45, 40, 41, 40  
], dtype=np.float32).reshape(8, 8)

def generate_random_matrix():
    return np.random.randint(0, 256, (8, 8), dtype=np.uint8)

def compute_dct(matrix):
    float_matrix = np.float32(matrix) - 128
    dct_matrix = cv2.dct(float_matrix)
    return dct_matrix

def apply_quantization(dct_matrix, quant_table):
    quantized = np.round(dct_matrix / quant_table)
    return quantized.astype(np.int32)

for test_num in range(1, NUM_TESTS + 1):
    input_matrix = generate_random_matrix()
    dct_output = compute_dct(input_matrix)
    quantized_output = apply_quantization(dct_output, quant_table_Y)

    print(f"\n// **Test Case {test_num}**")
    
    print("// Input Matrix (8×8)")
    print("float dctInput[8][8] = {")
    for row in input_matrix:
        print("    {" + ", ".join(f"{val}.f" for val in row) + "},")
    print("};\n")

    print("// DCT Output Matrix (8×8)")
    print("float dctOutput[8][8] = {")
    for row in dct_output:
        print("    {" + ", ".join(f"{val:.6f}f" for val in row) + "},")
    print("};\n")

    print("// Quantized DCT Matrix (8×8)")
    print("int quantizedDCT[8][8] = {")
    for row in quantized_output:
        print("    {" + ", ".join(f"{val}" for val in row) + "},")
    print("};\n")

    print("// --------------------------------------------------")
