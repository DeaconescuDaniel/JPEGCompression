import numpy as np
import cv2

NUM_TESTS = 1  # Number of test cases

# JPEG Luminance Quantization Table (8x8)
quant_table_Y = np.array([
    3,  2,  2,  2,  2,  2,  3,
    2,  2,  2,  3,  3,  3,  3,  4,
    6,  4,  4,  4,  4,  4,  8,  6,
    6,  5,  6,  9,  8, 10, 10,  9,
    8,  9,  9, 10, 12, 15, 12, 10,
    11, 14, 11,  9,  9, 13, 17, 13,
    14, 15, 16, 16, 17, 16, 10, 12,
    18, 19, 18, 16, 19, 15, 16, 16,16
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
