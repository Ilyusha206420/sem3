// Test case 1: Basic octal numbers
int octal1 = 0123;    // Valid octal
int octal2 = 0777;    // Valid octal  
int not_octal1 = 089; // Invalid (contains 9)
int not_octal2 = 0x123; // Hexadecimal
int not_octal3 = 123; // Decimal
int zero = 0;         // Valid (just zero)

// Test case 2: Octal in different contexts
int array[] = {0123, 0456, 001};
char *str = "0123 should not be highlighted"; // String literal
char chr = '\012'; // Octal escape sequence

// Test case 3: Complex expressions with octal
void test_function(int param1, int param2) {
    int result1 = param1 + 0123 + param2;
    int result2 = 0777 & 0xFF;
    float not_octal = 0.123; // Float, not octal
    double scientific = 1.23e-4; // Scientific notation
}

// Test case 4: Edge cases and boundaries
int edge1 = 07;      // Valid octal
int edge2 = 08;      // Invalid (contains 8)
int edge3 = 00;      // Valid octal
int edge4 = 0100;    // Valid octal
long edge5 = 01234567; // Valid octal

// int commented_octal = 0123;
/* 
   int multi_line_octal = 0456;
*/

// Test case 6: Complex mixed content
struct test_struct {
    int field1;
    char field2;
    long field3;
};

void complex_test() {
    struct test_struct s = {0123, 'A', 0456};
    int *ptr = &s.field1;
    
    // Mixed operations
    int calc1 = 0123 * 2 + 056;
    int calc2 = 089 + 123; // 089 is invalid octal
    
    // Function calls with octal
    some_function(012, 034, 056);
}

// Test case 7: String and character edge cases
char* test_strings[] = {
    "normal string",
    "string with 0123 numbers",
    "path/to/file",
    "email@domain.com"
};

char test_chars[] = {'A', '0', '\n', '\0'};

#define OCTAL_CONST 0123
#define NOT_OCTAL 089
#ifdef SOME_FLAG
#define ANOTHER_OCTAL 0777
#endif

// Test case 9: Multiple zeros and edge patterns
int multiple_zeros1 = 000;     // Valid octal
int multiple_zeros2 = 0000;    // Valid octal  
int problematic = 
0012;        // Valid octal
int another = 
0089;            // Invalid (contains 8)

unsigned long large_octal = 0123456701234567;

