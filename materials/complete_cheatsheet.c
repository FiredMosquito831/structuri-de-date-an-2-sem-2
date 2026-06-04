/*
 * ======================================================================================
 * COMPREHENSIVE C DATA STRUCTURES EXAM GUIDE & THEORETICAL RECAP
 * ======================================================================================
 * BY PAPIDEUS UWU
 * FILE DESCRIPTION:
 * This source file is the ultimate study companion for the Data Structures (C) exam.
 * It integrates theory with practical implementation patterns extracted from the
 * seminar files (01_DataTypes.c through 06_HashTable_Chain.c).
 *
 * STRUCTURE - 6 PARTS:
 * PART 1: INTRODUCTORY CONCEPTS (Seminars 1-3)
 * PART 2: SIMPLE (SINGLY) LINKED LISTS (Seminar 4)
 * PART 3: DOUBLY LINKED LISTS (Seminar 5)
 * PART 4: HASH TABLES WITH CHAINING (Seminar 6)
 * PART 5: BINARY SEARCH TREES (Seminar 7)
 * PART 6: HEAP STRUCTURE (Seminar 8)
 *
 * HOW TO STUDY THIS FILE:
 * - Read the "THEORY BLOCK" before each section.
 * - Read the inline comments for specific implementation details.
 * - Trace the execution in main() to see concepts in action.
 * - Memorize the patterns: they repeat across all data structures.
 * ======================================================================================
 */

/*
 * STANDARD LIBRARY INCLUDES
 *
 * #include tells the preprocessor to paste the contents of the named header
 * into this file BEFORE compilation begins.
 *
 * < >   = search system include paths (compiler's standard library)
 * " "   = search local directory first, then system paths
 *
 * We use < > for standard libraries.
 */
#include <stdio.h>    // Standard I/O: printf, scanf, fopen, fclose, fgets, etc.
#include <stdlib.h>   // Standard library: atoi, atof, malloc, free, NULL definition
#include <string.h>   // String manipulation: strcpy, strlen, strcmp, strtok
#include <malloc.h>   // Memory allocation: malloc, free (sometimes in stdlib.h, but explicit here)

/*
 * ======================================================================================
 * PART 1: INTRODUCTORY CONCEPTS
 * ======================================================================================
 *
 * SEMINAR 1: DATA TYPES, POINTERS, AND DYNAMIC MEMORY (01_DataTypes.c)
 * SEMINAR 2: MAIN ARGUMENTS (02_MainArgs.c)
 * SEMINAR 3: STRUCTS (03_Structs.c)
 *
 * CORE CONCEPTS COVERED:
 * 1. **Memory Segments**: Stack (automatic) vs Heap (dynamic)
 *    - Stack: managed by compiler, automatic allocation/deallocation, limited size
 *    - Heap: managed by programmer via malloc/free, large, flexible lifetime
 *    - Data segment: stores string literals, static/global variables (read-only)
 *    - Code segment: stores compiled instructions (read-only, executable)
 *
 * 2. **Pointers**: Declaration, assignment, dereferencing, NULL safety
 *    - A pointer STORES A MEMORY ADDRESS
 *    - Pointer size: 4 bytes on x86, 8 bytes on x64 (independent of pointed type)
 *    - & = address-of operator (returns where a variable lives)
 *    - * = dereference operator (accesses value at the stored address)
 *    - NULL = safety sentinel meaning "points to nothing"
 *
 * 3. **Dynamic Allocation**: malloc, free, sizeof, NULL assignment after free
 *    - malloc(size): allocates 'size' bytes on heap, returns void* (uninitialized memory)
 *    - free(ptr): returns heap memory to the system (does NOT set ptr to NULL)
 *    - sizeof: compile-time operator returning size in bytes of a type/variable
 *    - Always set pointer to NULL after free to prevent dangling pointer access
 *
 * 4. **Arrays & Matrices**: Stack arrays vs heap arrays, 2D dynamic matrices
 *    - Stack arrays: size known at compile time, automatic management
 *    - Heap arrays: size determined at runtime, manual management
 *    - 2D dynamic matrices require TWO levels of allocation (row pointers + row data)
 *    - Deallocation must be in REVERSE order of allocation
 *
 * 5. **CLI Arguments**: argc, argv, atoi() conversion
 *    - argc = argument count (always >= 1, argv[0] is program name)
 *    - argv = argument vector (array of strings, all arguments are STRINGS)
 *    - atoi() converts ASCII string to integer (returns 0 for non-numeric input)
 *    - atof() converts ASCII string to double
 *
 * 6. **Structures**: struct definition, typedef, sizeof calculation, memory alignment,
 *    struct pointers, strcpy for char arrays inside structs
 *    - struct groups related variables of DIFFERENT types under one name
 *    - typedef creates an alias (shorter name) for the struct type
 *    - sizeof(struct) >= sum of field sizes (compiler adds padding for alignment)
 *    - char* in struct = pointer (requires malloc before use)
 *    - char[] in struct = fixed array (part of struct's memory, no malloc needed)
 *    - Arrow operator (->): shorthand for dereference + dot access (ptr->field = (*ptr).field)
 *
 * MEMORY MODEL VISUALIZATION:
 *
 * +---------------------+  LOW ADDRESSES
 * | Stack (grows down)  |    Local variables, function parameters, return addresses
 * +---------------------+
 * | Heap (grows up)     |    Dynamically allocated memory (malloc'd)
 * +---------------------+
 * | Data (BSS/Data)     |    Global/static variables, string literals (read-only)
 * +---------------------+
 * | Code (Text)         |    Compiled instructions (read-only, executable)
 * +---------------------+  HIGH ADDRESSES
 *
 * KEY RULES:
 * - Stack addresses CANNOT be reassigned: &a = &b; // ILLEGAL (compile error)
 * - Heap memory MUST be freed: every malloc() needs a matching free()
 * - Pointer size on x86 = 4 bytes, on x64 = 8 bytes
 * - sizeof(array) != sizeof(pointer_to_array) - arrays don't decay in sizeof
 * - String literals are stored in read-only memory - do not free or modify
 * - After free(ptr), always set ptr = NULL to prevent dangling pointer access
 * - Free inner allocations BEFORE outer ones (char* before node)
 */

/*
 * ======================================================================================
 * FORWARD DECLARATIONS FOR FUNCTIONS USED IN MAIN
 * ======================================================================================
 *
 * In C, a function must be DECLARED before it is CALLED.
 * A declaration (prototype) tells the compiler:
 *   - The return type
 *   - The function name
 *   - The parameter types and names
 *
 * This is like a "contract" - the compiler knows the function exists
 * and checks that calls match the expected signature.
 *
 * The actual IMPLEMENTATION (body) can come later in the file.
 *
 * Without a prototype, calling a function defined later would cause:
 *   - Implicit declaration warning (old C)
 *   - Compilation error (modern C standards)
 */

// Forward declarations for linked list functions (defined in detail in Parts 2-4)
struct Node;  // Forward declaration of the Node struct tag
typedef struct Node Node;  // typedef alias - used in function signatures below

// These will be fully defined later - we just declare the signatures here so main() can call them

// Forward declarations for BST functions (defined in detail in Part 5)
struct BSTNode;  // Forward declaration of the BSTNode struct tag
typedef struct BSTNode BSTNode;  // typedef alias - used in function signatures below

// BST functions will be fully defined in Part 5
BSTNode* insertEmployeeBST(BSTNode* node, Employee emp, unsigned char *flag_insert);
void BSTParse(BSTNode* node);
Employee* searchEmployeeBST(BSTNode* node, unsigned short int search_key);
BSTNode* deallocateBST(BSTNode *node);
BSTNode* deleteNodeBST(BSTNode * node, unsigned short int search_key, Employee* empl);
// BST/Heap selective save functions (exam extension)
int countMatchingBST(BSTNode* node, float threshold);
int saveMatchingBSTToArray(BSTNode* node, float threshold, Employee* array, int index);
Node* saveMatchingBSTToList(BSTNode* node, float threshold, Node* list);
int countMatchingHeap(Employee* heap, int heapSize, float threshold);
void saveMatchingHeapToArray(Employee* heap, int heapSize, float threshold, Employee* array, int* arraySize);
Node* saveMatchingHeapToList(Employee* heap, int heapSize, float threshold);

// Heap functions will be fully defined in Part 6
void insertEmployeeHeap(Employee* strHeap, int* strSize, Employee emp);
Employee deleteEmployeeHeap(Employee* strHeap, int* strSize);

/*
 * ======================================================================================
 * 1.1 PRIMITIVE DATA TYPES AND MEMORY ADDRESSES
 * ======================================================================================
 *
 * Every variable in C occupies memory based on its type.
 * The & operator returns the ADDRESS where that variable lives.
 * The %p format specifier prints addresses in hexadecimal.
 */
void demo_dataTypes_and_addresses()
{
    // char: 1 byte, signed by default on most platforms (-128 to 127)
    // The = operator INITIALIZES the variable with a value at declaration time
    // The compiler allocates space on the STACK for this variable
    char a = 11;   // 'a' occupies 1 byte on the stack, holds value 11

    // Negative values work with SIGNED char (the default)
    // If char were unsigned, -11 would wrap around to 245 (256 - 11)
    char b = -11;  // 'b' occupies 1 byte on the stack, holds value -11

    // %d = decimal integer format specifier
    // printf replaces format specifiers with the corresponding argument values
    // \n = newline character (line feed), moves cursor to next line
    printf("a = %d\n", a);  // Prints: a = 11
                            // %d is replaced with value of 'a' (promoted to int)
                            // '\n' flushes the output buffer on many systems

    printf("b = %d\n", b);  // Prints: b = -11

    // &a = "address of a" - the unary & operator
    // Returns the memory address where variable 'a' is stored
    // %p = pointer format specifier, prints address in hex (e.g., 0x0061FF1B)
    // The actual address value depends on the OS, stack layout, and ASLR
    printf("Adress(a) = 0x%p\n", &a);
    // NOTE: Stack variables are typically allocated in HIGH memory addresses
    // and grow DOWNWARD (toward lower addresses) as more variables are declared

    printf("Adress(b) = 0x%p\n", &b);
    // 'a' and 'b' are typically adjacent on the stack (1 byte apart,
    // possibly with padding for alignment depending on compiler)

    // CRITICAL: Stack memory addresses are FIXED and CANNOT be reassigned
    // &a = &b;  // COMPILER ERROR: "lvalue required as left operand of assignment"
    // &a is not a modifiable lvalue - it's a compile-time computed address
    // You can't change WHERE a variable lives, only WHAT VALUE it contains
}

/*
 * ======================================================================================
 * 1.2 POINTERS - DECLARATION, ASSIGNMENT, DEREFERENCING
 * ======================================================================================
 *
 * A pointer is a variable that STORES A MEMORY ADDRESS.
 *
 * SYNTAX:  type* pointerName;
 *   - type = the type of data the pointer POINTS TO (not the pointer's own size)
 *   - The * can be attached to type or name: char* p OR char *p (same meaning)
 *   - Convention: char* p emphasizes "p is a pointer to char"
 *   - Convention: char *p emphasizes "*p is a char"
 *   - Both are equivalent; the compiler doesn't care
 *
 * POINTER SIZE:
 *   - On x86 (32-bit): 4 bytes
 *   - On x64 (64-bit): 8 bytes
 *   - Independent of the pointed-to type!
 *   - char* and int* and double* are all the same size
 */
void demo_pointers()
{
    char a = 11;  // Stack variable, 1 byte

    // POINTER DECLARATION + INITIALIZATION
    // char* = "pointer to char" type
    // NULL = macro defined in stdlib.h as (void*)0 or just 0
    // NULL means "points to nothing" / invalid address
    // ALWAYS initialize pointers to NULL if not immediately assigned a valid address
    // Uninitialized pointers contain GARBAGE (random memory addresses) -> CRASH on dereference
    char* p = NULL;

    // Printing the VALUE stored in the pointer (the address it points to)
    // Since p = NULL, this prints 0x00000000 (or 0x0 on some systems)
    printf("p = 0x%p\n", p);

    // &p = address OF the pointer variable itself (where p lives on the stack)
    // This is DIFFERENT from p (the value stored in p)
    // p = WHERE p points TO
    // &p = WHERE p ITSELF is stored
    printf("Adress(p) = 0x%p\n", &p);
    // p and &p are COMPLETELY DIFFERENT things!
    // p is the CONTENT of variable p (an address)
    // &p is the LOCATION of variable p (an address)

    // ASSIGNMENT: Store the address of 'a' into pointer 'p'
    // After this line, p CONTAINS the address where 'a' is stored
    // We say "p points to a" or "p references a"
    // The = operator here copies an ADDRESS, not a value
    p = &a;

    // Now p contains the same address as &a
    // These two printf calls print the SAME hexadecimal address
    printf("p = 0x%p\n", p);           // Prints: 0x0061FF1B (or similar)
    printf("&a = 0x%p\n", &a);         // Prints: 0x0061FF1B (SAME address!)

    // &p is still the address of the pointer variable itself (unchanged)
    printf("Adress(p) = 0x%p\n", &p);

    // DEREFERENCING: Access the VALUE at the address stored in p
    // *p means "the value at the address p points to"
    // Since p = &a, *p = a
    // The * operator is the DEREFERENCE (or indirection) operator
    // It "follows" the pointer to the actual data
    *p = a - 14;  // Equivalent to: a = 11 - 14 = -3
                  // This MODIFIES 'a' through the pointer!
                  // Read as: "store (a-14) into the location that p points to"

    // Verify: a has been changed through the pointer
    printf("a = %d\n", a);  // Prints: a = -3

    // IMPORTANT CONCEPTS:
    // 1. Pointers INDIRECTLY access data (through addresses)
    // 2. *p = value AT the address (dereference)
    // 3. p = the address ITSELF
    // 4. &p = where the pointer LIVES
    // 5. Changing *p changes the original variable (no copy is made)
}

/*
 * ======================================================================================
 * 1.3 STACK ARRAYS VS DYNAMIC ARRAYS
 * ======================================================================================
 *
 * STACK ARRAY:
 * - Size known at compile time
 * - Automatically managed (created/destroyed with function scope)
 * - Stored in the stack segment
 * - sizeof(array) returns total bytes of the array
 *
 * DYNAMIC ARRAY (HEAP):
 * - Size determined at runtime
 * - Must be manually managed (malloc + free)
 * - Stored in the heap segment
 * - sizeof(pointer) returns pointer size, NOT array size!
 *
 * KEY DISTINCTION: sizeof(v) vs sizeof(p)
 * v is an ARRAY - sizeof gives total bytes (8 bytes for 8 chars)
 * p is a POINTER - sizeof gives pointer size (4 bytes on x86, 8 on x64)
 */
void demo_arrays_and_dynamic()
{
    // STACK ARRAY INITIALIZATION
    // char v[] = {...} - compiler counts elements and allocates exactly that many bytes
    // No size needed in [] because the initializer list determines the size
    // v occupies 8 contiguous bytes on the stack
    char v[] = {9, 8, 7, 9, 9, 5, 3, 6};
    // Memory: [9][8][7][9][9][5][3][6]  (8 consecutive bytes)

    // sizeof(v) = 8 (total bytes of the array)
    // sizeof is a COMPILE-TIME operator, not a function
    // It returns the size in bytes of its operand's type
    // For arrays: sizeof = element_size * number_of_elements
    printf("Size of v = %d bytes\n", sizeof(v));  // Prints: 8

    // Array name 'v' DECAYS to pointer to first element in most contexts
    // v is equivalent to &v[0] (address of first element)
    // BUT sizeof(v) is NOT sizeof(&v[0]) - sizeof is one of the exceptions
    // where the array does NOT decay to pointer
    printf("Adress(v) = 0x%p\n", &v);  // Address of the entire array (same as &v[0])

    // POINTER DECLARATION
    char* p = NULL;

    // POINTING TO STACK ARRAY
    // v decays to &v[0] when assigned to a pointer
    // p now points to the first element of stack array v
    // IMPORTANT: p does NOT own this memory - it's on the stack
    // Do NOT free(p) - stack memory is automatically managed!
    p = v;  // Equivalent to: p = &v[0];
    printf("Size of v = %d bytes\n", sizeof(v));  // Still 8 (array doesn't change)
    printf("Size of p = %d bytes\n", sizeof(p));  // 4 (pointer size on x86)
    // THIS IS A COMMON EXAM TRICK: sizeof(pointer) != sizeof(array)!

    // DYNAMIC ALLOCATION
    // malloc(sizeof(v)) = allocate 8 bytes on the HEAP
    // malloc returns void* - a generic pointer to the allocated memory
    // void* automatically converts to any pointer type (implicit cast)
    // malloc does NOT initialize memory - contents are GARBAGE (random bytes)
    // If malloc fails (out of memory), it returns NULL
    p = malloc(sizeof(v));
    // Now p points to 8 bytes on the HEAP, NOT the stack array v
    // The stack array v is no longer accessible through p (pointer was overwritten)
    // The stack array v still exists on the stack (it's not freed - just p no longer points to it)

    // FILLING THE DYNAMIC ARRAY
    // sizeof(v) is still 8 - we use the stack array's size to know how many bytes to fill
    // unsigned char: 0 to 255 - sufficient for loop counter 0-7, saves 3 bytes vs int
    // p[i] = *(p + i) - array indexing is POINTER ARITHMETIC
    // *(p + i) = "value at address (p + i * sizeof(char))"
    // Since char is 1 byte, p + i moves i bytes forward from p
    for (unsigned char i = 0; i < sizeof(v); i++)
        p[i] = v[i] - 1;  // Copy each element from stack to heap, subtracting 1
                          // p[0] = v[0] - 1 = 9 - 1 = 8
                          // p[1] = v[1] - 1 = 8 - 1 = 7
                          // etc.

    // DEALLOCATION - CRITICAL STEP
    // free(p) returns the heap memory to the system
    // The bytes are marked as available for future malloc() calls
    // free() takes a pointer and releases the memory block it points to
    // free() does NOT know how many bytes were allocated - it uses internal metadata
    free(p);

    // SETTING TO NULL AFTER FREE - SAFETY PATTERN
    // After free(), p still contains the OLD address (DANGLING POINTER)
    // Accessing *p after free is UNDEFINED BEHAVIOR (could crash, could work, could corrupt)
    // Setting to NULL prevents accidental use:
    //   - Dereferencing NULL crashes immediately (easier to debug)
    //   - Double-free of NULL is safe (does nothing)
    p = NULL;
    // Without this: *p = 5; might silently corrupt memory
    // With this:    *p = 5; crashes immediately (segfault) - easier to find the bug
}

/*
 * ======================================================================================
 * 1.4 DYNAMIC 2D MATRIX ALLOCATION
 * ======================================================================================
 *
 * A dynamic matrix requires TWO levels of allocation:
 *
 * LEVEL 1: Array of ROW POINTERS (the "spine")
 *   char** pm = malloc(rows * sizeof(char*));
 *   Creates: [ptr0][ptr1]  (each ptr is 4 bytes on x86)
 *
 * LEVEL 2: Each ROW's data
 *   for each i: pm[i] = malloc(cols * sizeof(char));
 *   Creates: [data][data][data] for each row
 *
 * DEALLOCATION must be in REVERSE ORDER:
 *   1. Free each row's data
 *   2. Free the pointer array
 *   Freeing in wrong order = memory leak or crash
 */
void demo_dynamic_matrix()
{
    // char** = "pointer to pointer to char"
    // This is a DOUBLE POINTER - it points to an array of char* (pointers)
    // Initialized to NULL - good practice for safety
    char** pm = NULL;

    // STEP 1: Allocate the ARRAY OF ROW POINTERS
    // 2 rows = 2 char* pointers = 2 * 4 = 8 bytes (on x86)
    // malloc returns void*, auto-converts to char**
    // pm now points to a block of 8 bytes on the heap containing 2 uninitialized pointers
    pm = malloc(2 * sizeof(char*));
    // Memory: pm -> [ptr0 (garbage)][ptr1 (garbage)]

    // STEP 2: Allocate EACH ROW individually
    // Each row needs space for 3 characters PLUS 1 byte for the null terminator (\0)
    // The +1 is CRITICAL for string functions (printf %s, strlen, strcpy, etc.)
    // Without it, printf("%s") would read past the row into random memory!
    for (unsigned char i = 0; i < 2; i++)
    {
        // (3 + 1) * sizeof(char) = 4 bytes per row
        // sizeof(char) is ALWAYS 1 by definition in C
        // Writing sizeof(char) is a habit for clarity (shows you're thinking about types)
        pm[i] = malloc((3 + 1) * sizeof(char));
        // pm[0] -> 4 bytes on heap
        // pm[1] -> 4 bytes on heap (separate allocation, not necessarily adjacent!)
    }

    // FILL THE MATRIX
    // Two nested loops: outer = rows, inner = columns
    for (unsigned char i = 0; i < 2; i++)       // For each row
    {
        for (unsigned char j = 0; j < 3; j++)   // For each column in that row
            // 'G' is ASCII value 71
            // j=0: 'G' - 0 = 71 = 'G'
            // j=1: 'G' - 1 = 70 = 'F'
            // j=2: 'G' - 2 = 69 = 'E'
            // Characters are stored as their ASCII numeric values in memory!
            pm[i][j] = 'G' - j;

        // NULL BYTE (string terminator)
        // In C, strings END with a byte of value 0 (also written as '\0')
        // This byte signals "end of string" to all string functions
        // printf("%s") reads characters until it hits a 0 byte
        // strlen() counts characters until it hits a 0 byte
        // strcpy() copies bytes until it copies a 0 byte
        pm[i][3] = 0;  // Same as: pm[i][3] = '\0'; or pm[i][3] = '\x00';
                       // This makes pm[i] a valid C-string: "GFE\0"
    }

    // PRINTING - INDIVIDUAL CHARACTERS
    // %d = decimal integer (prints the ASCII numeric value)
    // %c = character (prints the character representation of the ASCII value)
    // Same byte in memory, interpreted differently by the format specifier
    for (unsigned char i = 0; i < 2; i++)
    {
        for (unsigned char j = 0; j < 3; j++)
            printf(" %d->%c ", pm[i][j], pm[i][j]);
            // Prints:  71->G  70->F  69->E
            // Shows that characters ARE numbers in memory
        printf("\n");  // Newline after each row
    }

    // PRINTING - AS STRINGS
    // %s expects a char* pointing to a null-terminated string
    // pm[i] is char* pointing to the first character of row i
    // printf reads characters from pm[i] until it hits the 0 byte at pm[i][3]
    // Without the null byte at pm[i][3], this would print garbage past the row!
    for (unsigned char i = 0; i < 2; i++)
        printf("%s\n", pm[i]);  // Prints: GFE (then newline)

    // DEALLOCATION - MUST BE IN REVERSE ORDER!
    //
    // Why reverse order? Because if you free(pm) first, you lose access to pm[0] and pm[1].
    // The addresses stored in pm[0] and pm[1] would be LOST = memory leak.
    //
    // WRONG:
    //   free(pm);          // Frees the pointer array - pm is now invalid
    //   free(pm[0]);       // CRASH! pm[0] can't be accessed - pm was freed!
    //
    // CORRECT:
    //   free(pm[0]);       // Free row 0's data first
    //   free(pm[1]);       // Free row 1's data
    //   free(pm);          // Then free the pointer array

    // Step 1: Free each row's data (the actual character arrays)
    for (unsigned char i = 0; i < 2; i++)
        free(pm[i]);  // Returns row i's 4 bytes to the heap
                      // After this, pm[i] is a DANGLING POINTER
                      // But we're about to free pm anyway, so we don't set to NULL

    // Step 2: Free the pointer array (the "spine" that held the row pointers)
    free(pm);  // Returns the 8 bytes (2 pointers * 4 bytes) to the heap
    pm = NULL; // Safety: prevent dangling pointer access
    // After both frees, ALL heap memory used by the matrix is released.
    // Total allocated: 2*4 (rows) + 2*4 (data) = 16 bytes. All freed.
}

/*
 * ======================================================================================
 * 1.5 COMMAND-LINE ARGUMENTS (argc/argv)
 * ======================================================================================
 *
 * int main(int argc, char* argv[])
 *
 * argc = ARGument Count: number of command-line arguments + 1 (program name)
 * argv = ARGument Vector: array of strings (char pointers)
 *
 * When you run: program.exe 10 hello 5
 *   argc = 4
 *   argv[0] = "program.exe"   (program name/path)
 *   argv[1] = "10"            (first argument as STRING)
 *   argv[2] = "hello"         (second argument)
 *   argv[3] = "5"             (third argument)
 *   argv[4] = NULL            (sentinel value - marks end of array)
 *
 * IMPORTANT: All arguments are STRINGS, even if they look like numbers!
 * You must convert them with atoi(), atof(), etc.
 *
 * char* argv[] is equivalent to char** argv
 * Both mean "pointer to pointer to char" or "array of char*"
 */
void demo_main_args(int argc, char* argv[])
{
    // Print all command-line arguments
    // Loop from 0 to argc-1 (argv[argc] is NULL, the sentinel)
    for (int i = 0; i < argc; i++)
        // argv[i] is a char* (C-string) - %s prints it
        printf("%s\n", argv[i]);
        // If run as "program.exe 10 hello 5", prints:
        //   program.exe
        //   10
        //   hello
        //   5

    printf("\n\n");

    // Sum all arguments as integers
    // atoi converts ASCII string to integer
    // atoi("10") = 10
    // atoi("hello") = 0 (can't convert - returns 0 for non-numeric strings)
    // atoi("5abc") = 5 (converts until non-digit character)
    // atoi is in <stdlib.h>
    int sum = 0;  // Initialize accumulator to 0
    for (int i = 0; i < argc; i++)
        // argv[i] is a string, atoi converts it to int
        // Even argv[0] ("program.exe") gets converted - atoi("program.exe") = 0
        sum += atoi(argv[i]);  // Same as: sum = sum + atoi(argv[i]);

    // Print the total sum
    // If run as "program.exe 10 20 5":
    //   sum = 0 + 0 + 10 + 20 + 5 = 35
    printf("Sum  = %d\n", sum);
}

/*
 * ======================================================================================
 * 1.6 STRUCTURES (struct)
 * ======================================================================================
 *
 * A struct groups related variables of DIFFERENT types under ONE NAME.
 * Each variable inside the struct is called a MEMBER or FIELD.
 *
 * MEMORY LAYOUT:
 * - Members are stored in memory in the order they are declared
 * - The compiler may add PADDING bytes between members for alignment
 * - sizeof(struct) >= sum of individual member sizes
 *
 * WHY PADDING?
 * - CPUs read memory in word-sized chunks (4 bytes on x86)
 * - Accessing a 4-byte int at an address that's a multiple of 4 is FASTER
 * - The compiler adds invisible padding bytes to ensure proper alignment
 *
 * typedef:
 * - Creates a SHORTER NAME (alias) for the struct type
 * - Without typedef: must write "struct Employee" everywhere
 * - With typedef: just write "Employee"
 */
void demo_structs()
{
    // STRUCT DEFINITION
    // The keyword 'struct' introduces a structure type
    // 'Employee' is the STRUCT TAG (name)
    // Everything inside { } are the MEMBERS
    // The semicolon after } is REQUIRED (struct is a statement)
    struct Employee
    {
        // unsigned short int: 2 bytes, range 0 to 65535
        // 'unsigned' = only positive values (no sign bit)
        // 'short' = 2 bytes (vs 'int' which is 4 bytes)
        unsigned short int id;  // 2 bytes - employee ID number

        // char* = pointer to character (4 bytes on x86)
        // This is a POINTER, not an array!
        // It stores an ADDRESS where the string data lives
        // It does NOT allocate space for the string itself!
        // MUST be assigned to malloc'd memory or string literal before use
        char* name;  // 4 bytes (x86 target platform) - pointer to employee name string

        // float: 4 bytes, single-precision floating point
        // Can represent decimal numbers: 7450.99
        // Less precise than double (4 bytes vs 8 bytes)
        float salary;  // 4 bytes - employee salary

        // unsigned char: 1 byte, range 0 to 255
        // Perfect for age (human ages are 0-255 max)
        // Saves 3 bytes vs using int for this field
        unsigned char age;  // 1 byte - employee age

        // char[11]: FIXED array of 11 characters (11 bytes)
        // This IS the storage - no pointer, no malloc needed
        // 10 characters + 1 null terminator = 11 bytes
        // Format: "MM/DD/YYYY" = 10 chars + '\0' = 11 bytes
        // Unlike char*, this array is PART OF the struct's memory
        char hire_date[11];  // 11 bytes (including null byte terminator)
        // TOTAL of 22 bytes as sum of fields, but 24 bytes as allocated memory area
        // 2 padding bytes are added by the compiler for alignment
    };

    // TYPEDEF: Create an alias so we don't have to write "struct Employee"
    // After this: "Employee" = "struct Employee"
    // This is a CONVENIENCE - it doesn't create a new type, just a new name for it
    typedef struct Employee Employee;

    // VARIABLE DECLARATION
    // Creates an Employee variable on the STACK
    // All members are allocated together as one contiguous block
    // Initial values are GARBAGE (uninitialized stack memory)
    Employee emp_1;

    // sizeof(Employee) returns the TOTAL bytes including padding
    // Expected: 24 bytes (22 actual + 2 padding)
    // The exact padding depends on the compiler and target architecture
    printf("Size of structure Employee is: %d bytes\n", sizeof(Employee));

    // MEMBER ACCESS USING DOT OPERATOR (.)
    // The dot operator accesses a member of a struct VARIABLE
    // Syntax: variable.member
    emp_1.id = 1001;  // Store integer 1001 in the 'id' field (2 bytes)

    // STRING LITERALS are stored in READ-ONLY memory (data segment)
    // emp_1.name is assigned the ADDRESS of the string literal "Jack Smith"
    // emp_1.name does NOT own this memory - it just points to a constant string
    // DO NOT free(emp_1.name) here - string literals are not malloc'd
    // DO NOT modify through emp_1.name - it's read-only memory
    emp_1.name = "Jack Smith";

    emp_1.age = 29;  // Store byte value 29 in 'age' field

    // EXPLICIT CAST: (float) converts the double literal 7450.99 to float
    // In C, decimal literals are double by default
    // (float) truncates precision from ~15 digits to ~7 digits
    // Without cast: implicit conversion happens anyway, but explicit is clearer
    emp_1.salary = (float)7450.99;

    // strcpy = STRING COPY
    // strcpy(destination, source) copies characters from source to destination
    // Copies until (and including) the null terminator '\0'
    // hire_date is a char[11] - it HAS storage built into the struct
    // We COPY INTO it, not assign a pointer to it
    // IMPORTANT: strcpy does NOT check if destination is big enough!
    // If source is longer than 10 chars, it overwrites adjacent memory (buffer overflow)
    // The destination must be big enough: strlen(source) + 1 <= destination_size
    strcpy(emp_1.hire_date, "11/07.2023");  // Copies "11/07.2023\0" into hire_date

    // POINTER TO STRUCT
    // Employee* = "pointer to Employee"
    // &emp_1 = address of the emp_1 variable on the stack
    // pEmp stores the address where emp_1 lives
    // pEmp does NOT copy emp_1 - it just POINTS to it
    Employee* pEmp = &emp_1;

    // ACCESSING STRUCT MEMBERS THROUGH A POINTER
    // Two equivalent notations:
    //   pEmp->name  = "arrow notation" (preferred, cleaner)
    //   (*pEmp).name = dereference then dot (more verbose, same meaning)
    // The -> operator combines dereference and member access in one step
    // pEmp->name means: "dereference pEmp, then access the 'name' member"
    // pEmp->age = 12; would change emp_1.age to 12 (through the pointer)

    // %s = string format specifier - prints characters until null terminator
    // %d = decimal integer format specifier
    // pEmp->name is a char* pointing to "Jack Smith"
    // pEmp->age is an unsigned char (promoted to int for printf)
    printf(" Name  = %s, Age = %d\n", pEmp->name, pEmp->age);

    // MEMORY VISUALIZATION:
    // Stack:
    //   emp_1: [id:1001 | name:addr | salary:7450.99 | age:29 | hire_date:"11/07.2023\0"]
    //                              |
    //                              v
    //   Read-only data segment:    "Jack Smith\0"
    //
    // pEmp -> [address of emp_1]
}

/*
 * ======================================================================================
 * COMMON EMPLOYEE STRUCT - REUSED ACROSS PARTS 2, 3, AND 4
 * ======================================================================================
 *
 * This struct is the DATA TYPE used in all three data structures:
 * singly linked lists, doubly linked lists, and hash tables.
 *
 * It's defined here as a GLOBAL definition so all parts can use it.
 */
struct Employee
{
    unsigned short int id;  // 2 bytes - unique employee identifier
    char* name;             // 4 bytes (x86) - pointer to dynamically allocated name string
    float salary;           // 4 bytes - employee salary
    unsigned char age;      // 1 byte - employee age
    char hire_date[11];     // 11 bytes - hire date as "MM/DD/YYYY" string
    // TOTAL: 22 bytes sum, 24 bytes allocated (2 bytes padding for alignment)
};

typedef struct Employee Employee;

/*
 * ======================================================================================
 * PART 2: SIMPLE (SINGLY) LINKED LISTS
 * ======================================================================================
 *
 * SEMINAR 4: 04_SimpleList.c
 *
 * WHAT IS A LINKED LIST?
 * - A LINEAR data structure where elements (nodes) are connected by POINTERS
 * - Unlike arrays, nodes are NOT stored in contiguous memory
 * - Each node is ALLOCATED INDEPENDENTLY (via malloc)
 * - Nodes can be SCATTERED across the heap
 *
 * WHY USE LINKED LISTS?
 * - Dynamic size: grow/shrink without reallocation (unlike arrays)
 * - O(1) insertion at beginning (unlike arrays which need O(n) shifting)
 * - No wasted pre-allocated space (unlike fixed-size arrays)
 *
 * WHY NOT USE LINKED LISTS?
 * - No random access: can't do list[5] - must traverse from beginning
 * - Extra memory per node: each node stores a pointer (4 bytes on x86)
 * - Poor cache locality: scattered memory = more cache misses
 *
 * MEMORY VISUALIZATION:
 *
 * [first] -> [Node1] -> [Node2] -> [Node3] -> NULL
 *             |          |          |
 *             v          v          v
 *           [emp1]     [emp2]     [emp3]
 *
 * Each Node:
 * +------------------+
 * | Employee emp     |  24 bytes (the actual data)
 * | Node* next       |   4 bytes (pointer to next node)
 * +------------------+
 *  Total: 28 bytes per node
 *
 * KEY OPERATIONS AND THEIR COMPLEXITY:
 * - Insert at end:    O(n) - must traverse to find last node
 * - Delete from end:  O(n) - must find second-to-last node
 * - Parse:            O(n) - traverse all nodes once
 * - Search:           O(n) - linear scan from first to end
 *
 * THE EMPTY LIST REPRESENTATION:
 * - An empty list is represented by first == NULL
 * - No sentinel/dummy node is used
 * - All operations must handle the empty list case
 *
 * FILE PARSING PATTERN:
 * - fopen("file", "r") opens a text file for reading
 * - fgets(buffer, size, file) reads one line at a time
 * - strtok(buffer, delimiters) splits a line into tokens
 *   * FIRST call: pass the string
 *   * SUBSEQUENT calls: pass NULL to continue from last position
 *   * strtok MODIFIES the original string (replaces delimiter with \0)
 *   * Separators: ",\n" means both comma AND newline are delimiters
 * - atoi() converts token to integer, atof() converts to double
 * - strcpy(dest, src) copies string content
 *
 * MEMORY MANAGEMENT FOR LINKED LISTS:
 * - Each node is malloc'd -> must be free'd individually
 * - Each employee's name is malloc'd -> must be free'd BEFORE the node
 * - Setting head = NULL is NOT enough (memory leak)
 * - Must traverse and free every node and every name
 *
 * TWO-PASS ALGORITHM PATTERN:
 * - Sometimes you need global information (like average) before making decisions
 * - Pass 1: traverse and collect statistics (sum, count)
 * - Pass 2: traverse and act on collected info (delete, filter, etc.)
 * - Time: O(n) + O(n) = O(n) - still linear
 * - Space: O(1) - no extra allocation needed
 */

// ======================================================================================
// 2.1 NODE STRUCTURE DEFINITION
// ======================================================================================

/*
 * The Node struct represents ONE ELEMENT in a singly linked list.
 * It has TWO fields:
 * 1. emp: The DATA (Employee struct, embedded directly - not a pointer)
 *    - Embedded (not pointer) means the Employee is PART OF the node's memory
 *    - When you malloc a Node, you get space for both emp AND next
 *    - No separate malloc needed for emp
 *    - Copying a node copies the entire Employee struct (shallow copy)
 *    - BUT emp.name is a char* pointer - it gets COPIED, not the string itself
 * 2. next: Pointer to the NEXT node in the chain (or NULL if last)
 *    - Self-referential: Node contains a pointer to another Node
 *    - This is how nodes are "linked" together
 *    - NULL marks the END of the list
 */
struct Node
{
    Employee emp;       // Embedded struct (24 bytes) - NOT a pointer, actual data
    struct Node* next;  // Pointer to next node (4 bytes on x86)
                        // Uses "struct Node*" because typedef isn't complete yet
                        // (the typedef comes AFTER the struct definition)
};

typedef struct Node Node;  // Now we can use "Node" instead of "struct Node"

// ALTERNATIVE SYNTAX (equivalent):
// typedef struct Node {
//     Employee emp;
//     struct Node* next;
// } Node;
// This combines the struct definition and typedef in one statement.

/*
 * ======================================================================================
 * 2.2 INSERT NODE AT END
 * ======================================================================================
 *
 * PURPOSE: Add a new employee to the END of the linked list.
 *
 * ALGORITHM:
 * 1. Create a new node on the heap (malloc)
 * 2. Copy the employee data into the new node
 * 3. Set new_node->next = NULL (it's the new last node)
 * 4. If the list is empty, new node becomes the head
 * 5. If not empty, traverse to the end and link the new node
 * 6. Return the (possibly updated) head pointer
 *
 * TIME COMPLEXITY: O(n) - must traverse the entire list to find the end
 * SPACE COMPLEXITY: O(1) - allocates exactly one new node
 *
 * WHY RETURN THE LIST?
 * - If the list was NULL (empty), the new node BECOMES the head
 * - The caller must capture the return value: first = insertNodeEnd(first, emp)
 * - Without capturing the return value, inserting into an empty list would be lost
 * - For non-empty lists, returning the same head is harmless
 */
Node* insertNodeEnd(Node* list, Employee data)
{
    // malloc(sizeof(Node)) allocates 28 bytes on the heap (24 emp + 4 next)
    // Returns void* which auto-converts to Node*
    // The memory is UNINITIALIZED - contains garbage values
    // If allocation fails (out of memory), malloc returns NULL
    Node* new_node = malloc(sizeof(Node));

    // STRUCT ASSIGNMENT: copies each member of 'data' into new_node->emp
    // This is a SHALLOW COPY:
    //   - id, salary, age, hire_date[] are copied by value (safe)
    //   - name (char*) is copied as a pointer - both structs point to SAME string
    // Since 'data' comes from the caller's stack (parsed from file),
    // the caller's empl.name pointer is COPIED to new_node->emp.name
    // Both point to the same malloc'd string - no problem because caller's
    // stack variable goes out of scope after the call
    new_node->emp = data;

    // NULL marks this as the LAST node in the list
    // Without this, new_node->next would contain garbage -> crash during traversal
    // NULL is the SENTINEL value that tells traversal loops to stop
    new_node->next = NULL;

    // CHECK: Is the list empty?
    // An empty list is represented by a NULL head pointer
    // This is the BASE CASE - simplest scenario
    if (list == NULL)
    {
        // The new node is the ONLY node in the list
        // It is both the first and the last node
        // Return the new node as the new head of the list
        return new_node;
    }
    else
    {
        // LIST HAS AT LEAST ONE NODE
        // We need to find the LAST node to append the new node

        // Create a TRAVERSAL POINTER
        // 't' is a temporary pointer that walks through the list
        // We use 't' instead of modifying 'list' directly
        // 'list' is a parameter (local copy of the pointer) - modifying it
        // wouldn't affect the caller's variable, but we still use 't' for clarity
        Node* t = list;  // Start at the head of the list

        // TRAVERSAL LOOP: walk to the last node
        // t->next != NULL means "t is NOT the last node"
        // Keep moving forward (t = t->next) until we find the last node
        // When the loop exits, t points to the LAST node (t->next == NULL)
        //
        // VISUAL:
        // Before loop: t -> [Node1] -> [Node2] -> [Node3] -> NULL
        // After loop:                        t -> [Node3] -> NULL
        while (t->next != NULL)
            t = t->next;  // Move to the next node
                          // t = t->next means: "t now points to where t->next points"
                          // This advances the pointer one node forward

        // LINK: Connect the last node to the new node
        // t is the last node (t->next was NULL)
        // Set t->next to point to new_node
        // Now: ... -> [last node] -> [new_node] -> NULL
        t->next = new_node;

        // Return the SAME head pointer (unchanged)
        // The head of the list doesn't change when inserting at the end
        return list;
    }
}

/*
 * ======================================================================================
 * 2.3 PARSE (TRAVERSE) THE LIST
 * ======================================================================================
 *
 * PURPOSE: Print all employees in the list from first to last.
 *
 * THE STANDARD TRAVERSAL PATTERN (memorize this!):
 *   Node* t = list;           // Start at head
 *   while (t != NULL) {       // Continue until end of list
 *       // Process t->data    // Do something with current node
 *       t = t->next;          // Move to next node
 *   }
 *
 * KEY RULES:
 * 1. NEVER modify the original 'list' pointer - use a temporary 't'
 * 2. The condition 't != NULL' handles empty lists and end-of-list
 * 3. After the loop, t == NULL (past the last node)
 * 4. This is READ-ONLY - doesn't modify the list structure
 */
void parseList(Node* list)
{
    // Create a temporary traversal pointer
    // Initialized to 'list' (the head of the list)
    // 't' walks through the list; 'list' stays pointing to the head
    Node* t = list;

    // LOOP CONDITION: continue as long as t points to a valid node
    // When t == NULL, we've gone past the last node - stop
    // This correctly handles empty lists: if list == NULL, t starts as NULL, loop doesn't run
    while (t != NULL)
    {
        // t->emp.name = access the 'name' field of the Employee in the current node
        // t->emp.id = access the 'id' field
        // %s prints the name string (until null terminator)
        // %d prints the id as a decimal integer
        // The order here is: name first, then id (different from doubly linked list!)
        printf("%s %d\n", t->emp.name, t->emp.id);

        // ADVANCE: Move the traversal pointer to the next node
        // t = t->next means "follow the 'next' pointer to the next node"
        // This is the ENGINE that drives the loop forward
        // Without this line, infinite loop (t never changes)
        t = t->next;
        // After this line, t points to the next node in the chain
        // If t was pointing to the last node, t->next was NULL, so t becomes NULL
        // The loop then checks t != NULL and exits
    }
    // After the loop: t == NULL, list still points to the head
    // The list is unchanged - this was a read-only operation
}

/*
 * ======================================================================================
 * 2.4 DELETE NODE FROM END
 * ======================================================================================
 *
 * PURPOSE: Remove and free the LAST node in the list.
 *
 * WHY IS THIS COMPLEX?
 * Because in a SINGLY linked list, the last node doesn't know about its predecessor.
 * We must FIND the second-to-last node to update its 'next' pointer to NULL.
 *
 * THREE CASES:
 * CASE 1: EMPTY LIST (list == NULL)
 *   - Nothing to delete, return NULL
 *
 * CASE 2: ONE NODE (list->next == NULL)
 *   - Free the node's name (dynamic string)
 *   - Free the node itself
 *   - Set list = NULL (return empty list indicator)
 *
 * CASE 3: TWO OR MORE NODES
 *   - Traverse to SECOND-TO-LAST node
 *   - Save reference to last node (t->next)
 *   - Free the last node's name and the node itself
 *   - Set second-to-last node's next = NULL
 *
 * CRITICAL: Free the EMPLOYEE'S NAME first (char* was malloc'd during file parsing),
 * then free the node. Otherwise you lose the pointer to the name = MEMORY LEAK.
 *
 * TIME COMPLEXITY: O(n) - must traverse to find second-to-last node
 * SPACE COMPLEXITY: O(1) - only uses temporary pointers
 *
 * MEMORY FLOW (Case 3):
 * Before: [emp1|next] -> [emp2|next] -> [emp3|next] -> NULL
 * Step:   Find second-to-last (emp2)
 * Free:   emp3's name, then emp3's node
 * After:  [emp1|next] -> [emp2|next] -> NULL
 */
Node* deleteNodeEnd(Node* list)
{
    // CHECK: Is the list non-empty?
    // Only attempt deletion if there's at least one node
    if (list != NULL)
    {
        // Start traversal from the head
        Node* t = list;

        // CHECK: Is there only ONE node?
        // If t->next == NULL, then t is the ONLY node (first AND last)
        if (t->next == NULL)
        {
            // ONE NODE CASE: After deletion, the list becomes EMPTY

            // STEP 1: Free the employee's name (dynamic string)
            // t->emp.name is a char* that was malloc'd during file parsing
            // It was allocated with malloc(strlen(token) + 1)
            // MUST free it BEFORE freeing the node, otherwise we lose the pointer!
            free(t->emp.name);

            // STEP 2: Free the node itself
            // Returns the node's memory (28 bytes) to the heap
            free(t);

            // STEP 3: Set list to NULL
            // This signals that the list is now EMPTY
            // Since 'list' is the return value, the caller receives NULL
            // The caller then sets their head pointer to NULL
            list = NULL;
        }
        else
        {
            // TWO+ NODES CASE: Need to find the SECOND-TO-LAST node

            // TRAVERSAL: Find the node BEFORE the last one
            // (t->next)->next != NULL means "the node after t is NOT the last node"
            // We look AHEAD two nodes: if the one after next exists, keep going
            //
            // Why not t->next != NULL?
            // Because that would stop at the LAST node, but we need the one BEFORE it.
            //
            // VISUAL TRAVERSAL:
            // Start: t -> [Node1] -> [Node2] -> [Node3] -> NULL
            // Check: (t->next)->next = Node3->next = NULL -> loop exits
            // t is at Node1, t->next is Node2, (t->next)->next is Node3->next = NULL
            // Wait, that's wrong for 3 nodes. Let me re-trace:
            //
            // For [A] -> [B] -> [C] -> NULL:
            // t = A; (A->next)->next = B->next = C->next = NULL? No! C->next = NULL
            // Actually: (t->next)->next when t=A: A->next=B, B->next=C, C!=NULL -> continue
            // t = B; (B->next)->next = C->next = NULL -> exit loop
            // t is at B (second-to-last). Correct!
            while ((t->next)->next != NULL)
                t = t->next;  // Advance to next node

            // At this point, t is the SECOND-TO-LAST node
            // t->next is the LAST node (the one to delete)

            // Save reference to the last node BEFORE unlinking it
            // We need this pointer to free the node after we unlink it
            Node* q = t->next;  // q points to the last node to be deleted

            // Free the employee's name (dynamic string in the node being deleted)
            free(q->emp.name);

            // Free the node itself
            free(q);

            // UNLINK: Set second-to-last node's next to NULL
            // This makes 't' the new last node in the list
            // The deleted node (q) is now disconnected from the list
            t->next = NULL;
        }
    }

    // Return the (possibly updated) head pointer
    // If the last node was deleted, list was set to NULL
    // Otherwise, list still points to the original head
    return list;
}

/*
 * ======================================================================================
 * 2.5 DELETE NODES BELOW AVERAGE SALARY
 * ======================================================================================
 *
 * PURPOSE: Remove ALL nodes where salary <= average_salary.
 *
 * WHY TWO PASSES?
 * We need the average salary BEFORE we can decide which nodes to remove.
 * But we can't compute the average without visiting ALL nodes first.
 * So: Pass 1 = compute average, Pass 2 = remove nodes.
 *
 * PASS 1: Calculate the average
 *   - Traverse entire list, sum salaries, count nodes
 *   - average = sum / count
 *
 * PASS 2: Remove nodes with salary <= average
 *   This has TWO sub-phases:
 *
 *   Sub-phase A: Remove from the BEGINNING (changes head)
 *     While the FIRST node's salary <= average, delete it.
 *     Each deletion changes the head pointer.
 *
 *   Sub-phase B: Remove from the MIDDLE/END (doesn't change head)
 *     Traverse the list. When a node's NEXT has salary <= average,
 *     remove that NEXT node (bypass it in the chain).
 *
 * WHY TWO SUB-PHASES IN PASS 2?
 * - Removing from the beginning changes WHICH node is the head.
 * - Removing from the middle doesn't change the head.
 * - The logic for each is different, so they're handled separately.
 *
 * IMPORTANT: The comparison uses <= (less than OR EQUAL to average)
 * This means nodes WITH exactly the average salary are ALSO removed.
 *
 * TIME COMPLEXITY: O(n) - two passes, each visits every node at most once
 * SPACE COMPLEXITY: O(1) - only uses temporary pointers, no extra allocation
 */
Node* deleteNodesBelowAverageSalary(Node* list) {
    // EDGE CASE: Empty list - nothing to do
    if (list == NULL) {
        return NULL;
    }

    // ========================
    // PASS 1: Compute average salary
    // ========================

    // Temporary traversal pointer - start at head
    Node* temp = list;

    // Accumulator for the sum of all salaries
    // double is used (not float) to avoid precision loss with many additions
    // Even though salary is float, we accumulate in double for accuracy
    double sum = 0;

    // Counter for the number of nodes
    // unsigned int = 0 to 4 billion, can't be negative (logical for a count)
    unsigned int count = 0;

    // Traverse the entire list, summing salaries and counting nodes
    while (temp != NULL)
    {
        // (double)temp->emp.salary = cast float to double
        // float has ~7 significant digits, double has ~15
        // Casting prevents precision loss during accumulation
        sum += (double)temp->emp.salary;  // Same as: sum = sum + (double)temp->emp.salary;
        count++;                           // Same as: count = count + 1;
        temp = temp->next;                 // Advance to next node
    }

    // Calculate the average
    // (double)sum / count = division in double precision
    // If count is 0, this would be division by zero - but we checked list != NULL above
    double avg = (double)sum / count;

    // ========================
    // PASS 2: Remove nodes with salary <= average
    // ========================

    // Reset traversal pointer to the head
    temp = list;

    // --- SUB-PHASE A: Remove nodes from the BEGINNING ---
    // These nodes, when removed, change the HEAD of the list.
    //
    // CONDITION: temp != NULL && temp->emp.salary <= avg
    //   - temp != NULL: list is not empty (still has nodes)
    //   - temp->emp.salary <= avg: current node's salary is at or below average
    //
    // The && (logical AND) uses SHORT-CIRCUIT evaluation:
    //   If temp == NULL, the second condition is NOT evaluated (prevents crash)
    while (temp != NULL && temp->emp.salary <= avg)
    {
        // STEP 1: Save the new head
        // The next node becomes the new head of the list
        // If temp was the last node, temp->next is NULL -> list becomes empty
        list = temp->next;

        // STEP 2: Free the dynamic string (employee name)
        // Must free BEFORE freeing the node (otherwise we lose the pointer)
        free(temp->emp.name);

        // STEP 3: Free the node itself
        free(temp);

        // STEP 4: Move temp to the new head
        // Continue checking from the new head
        // If list is NULL (was last node), the loop condition temp != NULL fails -> exits
        temp = list;
    }

    // CHECK: Were ALL nodes deleted?
    // If the head is NULL, the list is empty - nothing more to do
    if (list == NULL) {
        return NULL;  // Return empty list indicator
    }

    // --- SUB-PHASE B: Remove nodes from the MIDDLE/END ---
    // These nodes, when removed, do NOT change the head.
    //
    // KEY INSIGHT: We check temp->next (the NEXT node), not temp itself.
    // This is because to remove a node, we need its PREDECESSOR to bypass it.
    // In a singly linked list, we can only go forward, so we must stop BEFORE
    // the node we want to delete.
    //
    // CONDITION: temp->next != NULL
    //   - There is at least one more node after temp
    //   - When temp->next == NULL, temp is the last node - nothing after it to delete

    // NOTE: temp is already positioned at the last node that was KEPT
    // (the one that broke out of sub-phase A's loop)
    while (temp->next != NULL)
    {
        // Check if the NEXT node should be deleted
        if (temp->next->emp.salary <= avg)
        {
            // NEXT NODE NEEDS DELETION
            // We need to BYPASS temp->next in the chain

            // STEP 1: Save the node AFTER the one being deleted
            // temp2 = the node that will become temp's new next
            // If temp->next was the last node, temp2 = NULL
            Node* temp2 = temp->next->next;

            // STEP 2: Free the employee name in the node being deleted
            free(temp->next->emp.name);

            // STEP 3: Free the node being deleted
            free(temp->next);

            // STEP 4: Bypass the deleted node
            // temp->next now points to temp2 (the node after the deleted one)
            // This effectively removes the deleted node from the chain
            //
            // BEFORE: [temp] -> [deleted_node] -> [temp2]
            // AFTER:  [temp] -> [temp2]
            temp->next = temp2;

            // IMPORTANT: Do NOT advance temp here!
            // The NEW temp->next (which is temp2) might also need deletion.
            // We must check it in the next iteration.
        }
        else
        {
            // NEXT NODE IS FINE (salary > average)
            // Advance temp to the next node
            // Only advance when we DIDN'T delete - otherwise we'd skip checking temp2
            temp = temp->next;
        }
    }

    // Return the (possibly updated) head pointer
    return list;
}

/*
 * ======================================================================================
 * PART 3: DOUBLY LINKED LISTS
 * ======================================================================================
 *
 * SEMINAR 5: 05_DoubleLists.c
 *
 * WHAT IS A DOUBLY LINKED LIST?
 * - Like a singly linked list, but each node has TWO pointers:
 *   1. next: points to the NEXT node (forward direction)
 *   2. prev: points to the PREVIOUS node (backward direction)
 * - Can traverse in BOTH directions (forward AND backward)
 * - Can efficiently delete from both ends
 *
 * ADVANTAGES over Singly Linked:
 * - O(1) insertion at end (have 'last' pointer, no traversal needed)
 * - O(1) deletion at end (have 'prev' pointer, no need to find second-to-last)
 * - Bidirectional traversal (can go first->last AND last->first)
 * - Easier to implement certain operations (reverse iteration, undo/redo)
 *
 * DISADVANTAGES:
 * - More memory per node: 2 pointers (8 bytes on x86) vs 1 pointer (4 bytes)
 * - More complex insertion/deletion: must update BOTH next AND prev pointers
 * - Slightly more overhead in every operation
 *
 * MEMORY VISUALIZATION:
 *
 * DoubleList: {first, last}
 *                  |              |
 *                  v              v
 * NULL <- [prev|emp1|next] <-> [prev|emp2|next] <-> [prev|emp3|next] -> NULL
 *
 * Each DNode (32 bytes on x86):
 * +------------------+
 * | Employee emp     |  24 bytes (data)
 * | DNode* next      |   4 bytes (forward pointer)
 * | DNode* prev      |   4 bytes (backward pointer)
 * +------------------+
 *  Total: 32 bytes per node (vs 28 for singly linked)
 *
 * THE DoubleList STRUCT:
 * - Wraps first AND last pointers into one container
 * - Makes passing the list as a parameter cleaner (one struct vs two pointers)
 * - Passed BY VALUE - the struct is COPIED when passed to functions
 * - Functions return the modified struct (since pass-by-value means modifications
 *   to the struct's fields inside the function don't affect the caller)
 * - ALTERNATIVE: pass DoubleList* (pointer) to avoid copy and allow in-place modification
 *
 * EMPTY LIST REPRESENTATION:
 * - DList.first = NULL AND DList.last = NULL (BOTH must be NULL!)
 * - If only first is NULL but last points somewhere, last is a dangling pointer
 *
 * INSERT AT END PATTERN (O(1)):
 * 1. Create new node, set new_node->next = NULL
 * 2. Set new_node->prev = list.last (backward link)
 * 3. If empty: first = last = new_node
 * 4. If not empty: old_last->next = new_node, last = new_node
 *
 * DELETE FROM END PATTERN (O(1)):
 * 1. t = list.last (node to delete)
 * 2. If t == first (only node): first = last = NULL
 * 3. Else: last = t->prev, last->next = NULL
 * 4. Free t->emp.name, then free t
 *
 * PASS-BY-VALUE vs PASS-BY-POINTER:
 * - DoubleList (pass-by-value): struct is copied, function returns modified copy
 *   * Caller must capture: DList = insertDNodeEnd(DList, emp)
 *   * Simpler, safer, but copies 8 bytes (two pointers)
 * - DoubleList* (pass-by-pointer): original is modified in-place
 *   * Function can change list->first and list->last directly
 *   * No return value needed
 *   * More efficient for large structs
 *
 * COMPARISON TABLE:
 * +----------------------+----------------+------------------+
 * | Operation            | Singly Linked  | Doubly Linked    |
 * +----------------------+----------------+------------------+
 * | Insert at end        | O(n) traverse  | O(1) with last   |
 * | Delete from end      | O(n) find 2nd  | O(1) with prev   |
 * | Parse forward        | O(n)           | O(n)             |
 * | Parse backward       | NOT POSSIBLE   | O(n)             |
 * | Memory per node      | data + 4B      | data + 8B        |
 * +----------------------+----------------+------------------+
 */

// ======================================================================================
// 3.1 DNODE STRUCTURE
// ======================================================================================

/*
 * DNode = Doubly-linked Node
 * Similar to Node but with an additional 'prev' pointer.
 *
 * The 'prev' pointer enables:
 * - Backward traversal (last -> first)
 * - O(1) deletion from the end (no need to find second-to-last)
 * - Easier insertion/deletion at any position (have both neighbors)
 */
struct DNode
{
    Employee emp;           // Embedded Employee data (24 bytes)
    struct DNode* next;     // Forward pointer (4 bytes) - points to the next node
    struct DNode* prev;     // Backward pointer (4 bytes) - points to the previous node
                            // NULL for the first node (no predecessor)
};

typedef struct DNode DNode;  // Alias: "DNode" = "struct DNode"

// ======================================================================================
// 3.2 DOUBLELIST CONTAINER STRUCTURE
// ======================================================================================

/*
 * DoubleList WRAPS the two endpoint pointers into a single struct.
 *
 * WHY WRAP IN A STRUCT?
 * - Instead of passing two separate pointers (first, last) to every function,
 *   we pass one struct that contains both.
 * - Cleaner function signatures: DoubleList instead of (DNode*, DNode*)
 * - The two pointers are logically related - they describe ONE data structure
 *
 * PASS-BY-VALUE IMPLICATION:
 * - When a DoubleList is passed to a function, the struct is COPIED
 * - Modifications to list.first and list.last inside the function
 *   modify the COPY, not the original
 * - The function MUST RETURN the modified struct
 * - The caller must capture the return: DList = insertNodeEnd(DList, emp)
 *
 * ALTERNATIVE (not used here): Pass DoubleList* (pointer to struct)
 * - Then modifications would affect the original directly
 * - No need to return the struct
 * - But this requires the caller to have a persistent struct variable
 */
struct DoubleList  // Pack the two endpoint pointers into one structure
{
    DNode* first;  // Pointer to the FIRST node in the list (head)
                   // NULL when the list is empty
    DNode* last;   // Pointer to the LAST node in the list (tail)
                   // NULL when the list is empty
                   // Having this pointer enables O(1) operations at the end
};

typedef struct DoubleList DoubleList;

// EMPTY LIST REPRESENTATION:
//   DList.first = NULL AND DList.last = NULL
//   BOTH must be NULL! If only first is NULL but last points somewhere,
//   last becomes a DANGLING POINTER (points to invalid/freed memory)

/*
 * ======================================================================================
 * 3.3 INSERT NODE AT END (O(1) with last pointer!)
 * ======================================================================================
 *
 * KEY ADVANTAGE: Because we have a 'last' pointer, we DON'T need to traverse!
 * This is O(1) vs O(n) for singly linked lists.
 *
 * ALGORITHM:
 * 1. Allocate new node on heap
 * 2. Copy employee data into new node
 * 3. Set new_node->next = NULL (it's the new last node)
 * 4. Set new_node->prev = list.last (its predecessor is the current last node)
 * 5. If list is empty: both first and last = new_node
 * 6. If list is non-empty: update old last's next, update last to new_node
 * 7. Return the modified list struct
 *
 * TIME COMPLEXITY: O(1) - constant time, no traversal needed!
 * SPACE COMPLEXITY: O(1) - allocates exactly one new node
 *
 * MEMORY FLOW:
 * Before: NULL <- [A|next|prev] <-> [B|next|prev] -> NULL  (last = B)
 * Insert C:
 *   new_node->next = NULL
 *   new_node->prev = B (list.last)
 *   B->next = new_node
 *   last = new_node
 * After:  NULL <- [A|next|prev] <-> [B|next|prev] <-> [C|next|prev] -> NULL
 */
DoubleList insertDNodeEnd(DoubleList list, Employee empl)
{
    // Allocate new node on heap (32 bytes: 24 emp + 4 next + 4 prev)
    // Memory is uninitialized (garbage values)
    DNode* new_node = malloc(sizeof(DNode));

    // Copy employee data into the new node
    // Shallow copy: empl.name pointer is copied (both point to same string)
    new_node->emp = empl;

    // FORWARD POINTER: NULL means this node has no successor
    // It WILL BE the last node in the list
    new_node->next = NULL;

    // BACKWARD POINTER: Points to the current last node (its predecessor)
    // This establishes the backward link
    // If the list is empty, list.last is NULL -> new_node->prev = NULL (correct!)
    new_node->prev = list.last;

    // CHECK: Is the list empty?
    // An empty list has first == NULL (and last == NULL)
    if (list.first == NULL)
    {
        // Empty list: new_node is the ONLY node
        // It is simultaneously the first node AND the last node
        // Chained assignment: list.first = list.last = new_node
        // This is equivalent to:
        //   list.last = new_node;
        //   list.first = new_node;
        list.first = list.last = new_node;
    }
    else
    {
        // Non-empty list: at least one node exists

        // STEP 1: Link the OLD last node to the new node (forward direction)
        // list.last->next = new_node
        // This connects the previous last node to the new node
        // Before: [old_last|next=NULL]
        // After:  [old_last|next=new_node]
        list.last->next = new_node;

        // STEP 2: Update the last pointer to the new node
        // The new node is now the last node in the list
        list.last = new_node;

        // NOTE: We DON'T need to update new_node->prev here because
        // it was already set above (new_node->prev = list.last)
    }

    // Return the modified struct
    // Since DoubleList is passed by value, the caller must capture this return
    return list;
}

/*
 * ======================================================================================
 * 3.4 BIDIRECTIONAL TRAVERSAL
 * ======================================================================================
 *
 * Doubly linked lists support TWO traversal directions:
 *
 * FORWARD:  first -> last (same as singly linked list)
 * BACKWARD: last -> first (unique to doubly linked!)
 *
 * The backward traversal is possible because each node has a 'prev' pointer.
 * This is useful for:
 * - Reverse-order display
 * - Finding elements from the end (when you know they're near the end)
 * - Stack-like LIFO operations
 * - Undo/redo functionality in applications
 */
void parseDList(DoubleList list)
{
    // === FORWARD TRAVERSAL (first -> last) ===
    // Identical to singly linked list traversal
    printf("Double List first->last:\n");

    // Start at the first node
    DNode* t = list.first;

    // Continue until we go past the last node (t becomes NULL)
    while (t != NULL)
    {
        // Print employee ID and name
        // Note: order is ID first, then name (opposite of singly linked list!)
        printf("%d %s\n", t->emp.id, t->emp.name);

        // Move FORWARD to the next node
        t = t->next;
        // t follows the 'next' pointer chain: first -> second -> third -> ... -> NULL
    }

    // === BACKWARD TRAVERSAL (last -> first) ===
    // Unique to doubly linked lists!
    printf("\nDouble List last->first:\n");

    // Start at the LAST node (we have direct access to it!)
    t = list.last;

    // Continue until we go past the first node (t becomes NULL)
    // 'while (t)' is equivalent to 'while (t != NULL)'
    // In C, NULL (0) is falsy, non-NULL is truthy
    while (t)
    {
        // Print employee ID and name (same format as forward traversal)
        printf("%d %s\n", t->emp.id, t->emp.name);

        // Move BACKWARD to the previous node
        t = t->prev;
        // t follows the 'prev' pointer chain: last -> second-to-last -> ... -> NULL
    }
    // After both loops: t == NULL, list is unchanged (read-only operation)
}

/*
 * ======================================================================================
 * 3.5 DELETE NODE FROM END
 * ======================================================================================
 *
 * PURPOSE: Remove and free the LAST node in the doubly linked list.
 *
 * WHY SIMPLER THAN SINGLY LINKED?
 * Because we have:
 * 1. A 'last' pointer (direct access to the last node)
 * 2. A 'prev' pointer in each node (access to the predecessor)
 *
 * We DON'T need to traverse to find the second-to-last node!
 * We can directly access it via last->prev.
 *
 * THREE CASES:
 *
 * CASE 1: EMPTY LIST (list.first == NULL)
 *   - Nothing to delete, return list unchanged
 *
 * CASE 2: ONE NODE (t == list.first, where t = list.last)
 *   - The last node IS the first node (only node in the list)
 *   - Free the node's name, free the node
 *   - Set BOTH first and last to NULL
 *
 * CASE 3: TWO+ NODES
 *   - t = list.last (node to delete)
 *   - list.last = t->prev  (new last = predecessor)
 *   - list.last->next = NULL (new last has no successor)
 *   - Free t's name and node
 *
 * TIME COMPLEXITY: O(1) - constant time, no traversal!
 * (vs O(n) for singly linked lists)
 * SPACE COMPLEXITY: O(1) - only uses temporary pointers
 *
 * MEMORY FLOW (Case 3):
 * Before: NULL <- [A|next|prev] <-> [B|next|prev] <-> [C|next|prev] -> NULL
 * Delete C:
 *   t = C (last)
 *   last = B (t->prev)
 *   B->next = NULL
 *   free C
 * After:  NULL <- [A|next|prev] <-> [B|next|prev] -> NULL
 */
DoubleList deleteDNodeEnd(DoubleList list)
{
    // CHECK: Is the list non-empty?
    // 'if (list.first)' is equivalent to 'if (list.first != NULL)'
    // Only attempt deletion if there's at least one node
    if (list.first)
    {
        // Identify the node to delete: the last node
        // We have direct access via list.last - no traversal needed!
        DNode* t = list.last;  // t is the last node (the one to be deleted)

        // CHECK: Is t also the first node? (i.e., is it the ONLY node?)
        if (t == list.first)
        {
            // ONE NODE CASE: After deletion, the list becomes EMPTY

            // STEP 1: Free the employee's name (dynamic string)
            // Must free BEFORE freeing the node
            free(t->emp.name);

            // STEP 2: Free the node itself
            free(t);

            // STEP 3: Set BOTH endpoints to NULL
            // The list is now empty - both first and last must be NULL
            // If only one is set to NULL, the other becomes a dangling pointer
            list.first = list.last = NULL;
        }
        else
        {
            // TWO+ NODES CASE

            // STEP 1: Update the last pointer to the predecessor
            // t->prev points to the node BEFORE t (second-to-last node)
            // This node becomes the new last node
            list.last = t->prev;

            // STEP 2: Set the new last node's next pointer to NULL
            // The new last node was previously pointing to t (forward)
            // After deletion, it should point to nothing (it's now the end)
            // This completes the unlinking of t from the forward chain
            list.last->next = NULL;

            // NOTE: We DON'T need to update t->prev->prev because
            // the backward link from the new last to its predecessor is unchanged.
            // We only need to fix the forward link of the new last.

            // STEP 3: Free the employee's name in the deleted node
            free(t->emp.name);

            // STEP 4: Free the deleted node itself
            free(t);

            // The node t is now fully removed:
            // - Unlinked from the forward chain (new_last->next = NULL)
            // - Unlinked from the backward chain (already was last->prev, now irrelevant)
            // - Memory freed (name + node)
        }
    }

    // Return the modified struct (first/last may have changed)
    return list;
}

/*
 * ======================================================================================
 * 3.6 INCOMPLETE FUNCTION: getEmployeesUnderAverage
 * ======================================================================================
 *
 * This is an EMPTY FUNCTION SKELETON from the seminar file.
 * It represents a typical EXAM EXTENSION QUESTION.
 *
 * FUNCTION SIGNATURE ANALYSIS:
 *   Employee* getEmployeesUnderAverage(DoubleList* list, unsigned short int* size_array)
 *
 * Parameters:
 *   - DoubleList* list: POINTER to the DoubleList (not the struct itself!)
 *     * Why pointer? So the function can MODIFY the original list's first/last
     * Without a pointer, the function would work on a copy (pass-by-value)
 *     * The caller's list would be unchanged after the function returns
 *   - unsigned short int* size_array: POINTER to store the output array size
 *     * C functions can only return ONE value directly
 *     * To return both the array AND its size, we use a pointer parameter
 *     * The function writes the count to *size_array
 *
 * Return type: Employee*
 *   - Returns a pointer to a malloc'd array of Employee structs
 *   - The caller must free() this array when done
 *
 * EXPECTED BEHAVIOR (based on comments in the source file):
 * 1. Compute the average salary of all nodes in the doubly linked list
 * 2. Count how many employees have salary below the average
 * 3. Allocate an array of exactly that size
 * 4. Copy qualifying employees from the list into the array
 * 5. Remove those nodes from the doubly linked list
 * 6. Set *size_array to the count
 * 7. Return the array pointer
 *
 * After the function:
 *   - The list contains ONLY employees with salary >= average
 *   - The returned array contains all employees that were removed
 *
 * PSEUDOCODE:
 *   // Step 1: Compute average
 *   traverse list, sum salaries, count nodes
 *   avg = sum / count
 *
 *   // Step 2: Count under-average employees
 *   traverse list, count employees with salary < avg
 *
 *   // Step 3: Allocate output array
 *   result = malloc(under_count * sizeof(Employee))
 *
 *   // Step 4-5: Copy and remove
 *   traverse list with a current pointer
 *   for each node with salary < avg:
 *     copy employee data to result array
 *     remove node from doubly linked list (update prev/next links)
 *     free the node (but NOT the employee data - it was copied)
 *
 *   // Step 6-7: Set size and return
 *   *size_array = under_count
 *   return result
 */
Employee* getEmployeesUnderAverage(DoubleList* list, unsigned short int* size_array)
{
    // INTENTIONALLY LEFT EMPTY - this is an exam exercise
    // The student is expected to implement this during the exam
}

/*
 * ======================================================================================
 * PART 4: HASH TABLES WITH CHAINING
 * ======================================================================================
 *
 * SEMINAR 6: 06_HashTable_Chain.c
 *
 * WHAT IS A HASH TABLE?
 * - A data structure that maps KEYS to VALUES using a HASH FUNCTION
 * - Provides FAST average-case O(1) lookup, insertion, and deletion
 * - The hash function converts a key into an ARRAY INDEX
 * - Combines the best of arrays (O(1) access) and linked lists (dynamic size)
 *
 * WHAT IS A HASH FUNCTION?
 * - A mathematical function that transforms a KEY into an ARRAY INDEX
 * - hash(key) -> index (0 <= index < table_size)
 * - The SAME key MUST always produce the SAME index (deterministic)
 * - Good hash functions distribute keys UNIFORMLY across buckets
 *
 * WHAT IS A COLLISION?
 * - When two DIFFERENT keys produce the SAME hash value (same bucket index)
 * - Collisions are INEVITABLE when number of possible keys > number of buckets
 * - Example: hash(1012) = 12, hash(2012) = 12 -> both map to bucket 12
 *
 * WHAT IS CHAINING?
 * - A COLLISION RESOLUTION strategy
 * - Each bucket (array index) contains a LINKED LIST of items
 * - When multiple keys hash to the same index, they're chained together
 * - Simple to implement: reuse linked list operations (insertNodeEnd, etc.)
 * - The hash table is an ARRAY OF POINTERS, each pointing to a linked list
 *
 * ALTERNATIVE TO CHAINING: OPEN ADDRESSING (not covered in seminars)
 * - Linear probing: if bucket is full, try next bucket
 * - Quadratic probing: try bucket+1, bucket+4, bucket+9, etc.
 * - Double hashing: use a second hash function for probing
 *
 * WHY CHAINING?
 * - Handles any number of collisions (list just grows longer)
 * - No need to resize or rehash
 * - Deletion is straightforward (standard linked list deletion)
 * - Works well when the load factor (n/m) is kept low
 * - Code reuse: bucket chains use the same Node struct and insertNodeEnd
 *
 * MEMORY VISUALIZATION:
 *
 * HashTable (array of Node* with 50 buckets)
 * +-----+
 * | [0] | -> NULL (empty bucket)
 * | [1] | -> NULL (empty bucket)
 * | [2] | -> NULL (empty bucket)
 * ...
 * |[12] | -> [Node: id=1012|next] -> [Node: id=2012|next] -> [Node: id=1512|next] -> NULL
 * ...
 * |[49] | -> NULL (empty bucket)
 * +-----+
 *
 * With 3 employees and 50 buckets:
 *   hash(1012) = 1012 % 50 = 12
 *   hash(2012) = 2012 % 50 = 12
 *   hash(1512) = 1512 % 50 = 12
 *   All 3 collide at bucket 12! (They're chained together)
 *
 * LOAD FACTOR: alpha = n / m (elements / buckets)
 * - n = number of elements stored, m = number of buckets
 * - n = 3, m = 50 -> alpha = 0.06 (very low, few collisions expected)
 * - Higher alpha = longer chains = slower operations
 * - Ideal: alpha < 1 (fewer elements than buckets)
 * - When alpha > 1, some buckets have multiple elements (chains grow)
 *
 * TIME COMPLEXITY:
 *   - Average case: O(1) - direct access to bucket, short chain
 *   - Worst case: O(n) - all elements in one bucket (all keys collide)
 *   - Depends on quality of hash function and load factor
 *
 * HASH TABLE OPERATIONS SUMMARY:
 * +----------------------+------------------+
 * | Operation            | Complexity       |
 * +----------------------+------------------+
 * | Insert               | O(1) avg, O(n) worst |
 * | Search               | O(1) avg, O(n) worst |
 * | Delete               | O(1) avg, O(n) worst |
 * | Parse all elements   | O(m + n)         |
 * +----------------------+------------------+
 * (m = buckets, n = elements)
 *
 * INITIALIZATION PATTERN:
 * 1. Allocate array: HT = malloc(size * sizeof(Node*))
 * 2. Initialize all to NULL: for(i=0; i<size; i++) HT[i] = NULL
 *    - CRITICAL: malloc does NOT zero memory - must explicitly set NULL
 *    - Without initialization, buckets contain garbage -> crash on insert
 *
 * SEARCH PATTERN:
 * 1. Compute bucket: pos = hash(search_key)
 * 2. If HT[pos] == NULL -> not found, return NULL
 * 3. Traverse bucket's linked list, comparing each node's key
 * 4. If match found -> return pointer to data
 * 5. If end of list -> not found, return NULL
 *
 * CODE REUSE ACROSS DATA STRUCTURES:
 * - Hash table buckets use the SAME Node struct as singly linked lists
 * - insertNodeEnd is REUSED for inserting into buckets
 * - This demonstrates good software engineering: build on existing code
 */

// NOTE: The Node struct is REUSED from Part 2.
// Same structure: Employee data + next pointer.
// This demonstrates code reuse - hash table buckets are just linked lists!

/*
 * ======================================================================================
 * 4.1 HASH FUNCTION
 * ======================================================================================
 *
 * PURPOSE: Convert a key (employee ID) into a valid array index.
 *
 * THE MODULO HASH FUNCTION:
 *   hash(key) = key % size_ht
 *
 * PROPERTIES:
 * 1. DETERMINISTIC: Same key always produces the same hash
 *    - hash(1012, 50) always returns 12
 *    - This is essential for search to work
 * 2. FAST: O(1) - single modulo operation
 * 3. RANGE: Returns value in [0, size_ht - 1]
 *    - 1012 % 50 = 12 (valid index, 0 <= 12 < 50)
 * 4. UNIFORMITY: Distributes keys somewhat evenly
 *    - The modulo of consecutive-ish numbers spreads across buckets
 *    - But IDs ending in same digits may cluster (not ideal)
 *
 * COLLISION:
 * When two different keys produce the SAME hash value.
 *   hash(1012) = 12, hash(2012) = 12, hash(1512) = 12
 * All three employees map to bucket 12!
 * Resolved by chaining: they form a linked list at bucket 12.
 *
 * WHY unsigned short int?
 * - Matches the Employee.id field type
 * - unsigned = no negative IDs
 * - short = 2 bytes (IDs fit in 0-65535 range)
 */
unsigned short int hash_function(unsigned short int key, unsigned short int size_ht)
{
    // % = modulo operator (remainder after integer division)
    // 1012 % 50 = 12 (because 1012 = 20 * 50 + 12)
    // 2012 % 50 = 12 (because 2012 = 40 * 50 + 12)
    // 1512 % 50 = 12 (because 1512 = 30 * 50 + 12)
    // The result is ALWAYS in range [0, size_ht - 1]
    return (key % size_ht);
}

/*
 * NOTE: The insertNodeEnd function from Part 2 is REUSED here.
 * Hash table bucket chains are just singly linked lists!
 * No new insertion logic is needed.
 */

/*
 * ======================================================================================
 * 4.2 INSERT INTO HASH TABLE
 * ======================================================================================
 *
 * PURPOSE: Add an employee to the hash table using chaining.
 *
 * ALGORITHM:
 * 1. Compute the bucket index using the hash function
 * 2. Insert the employee into that bucket's linked list
 *    - Reuse insertNodeEnd (the same function from Part 2!)
 *    - HT[pos] is the head of the linked list at bucket 'pos'
 *    - After insertion, HT[pos] is updated to the (possibly new) head
 *
 * WHY HT[pos] = insertNodeEnd(...)?
 * - If the bucket was empty (HT[pos] == NULL), insertNodeEnd returns the new node
 * - We MUST update HT[pos] with this new node (it becomes the new head)
 * - If the bucket was non-empty, insertNodeEnd returns the same head
 *   (harmless reassignment, same value)
 *
 * TIME COMPLEXITY:
 *   - Hash computation: O(1)
 *   - insertNodeEnd: O(k) where k = items in that bucket
 *   - Average: O(1) with low load factor
 *   - Worst: O(n) if all items collide into one bucket
 */
void insertEmployeeHashTable(Node** HT, unsigned short int size_ht, Employee data)
{
    // STEP 1: Compute the bucket index
    // Hash the employee's ID to get the array position
    // data.id is the KEY for this hash table
    unsigned short int pos = hash_function(data.id, size_ht);
    // pos is now a valid index: 0 <= pos < size_ht
    // Example: pos = 12 for employee ID 1012

    // STEP 2: Insert into the bucket's linked list
    // HT[pos] is a Node* - the head of the linked list at bucket 'pos'
    // insertNodeEnd handles both cases:
    //   - If HT[pos] == NULL (empty bucket): creates new node, returns it
    //   - If HT[pos] != NULL (non-empty): appends to end, returns same head
    //
    // The assignment HT[pos] = ... is CRITICAL when the bucket was empty.
    // Without it, the new node would be created but not linked to the hash table!
    HT[pos] = insertNodeEnd(HT[pos], data);
}

/*
 * ======================================================================================
 * 4.3 SEARCH IN HASH TABLE
 * ======================================================================================
 *
 * PURPOSE: Find an employee by their ID (key).
 *
 * ALGORITHM:
 * 1. Compute the bucket index using the hash function
 * 2. If the bucket is NULL -> not found, return NULL
 * 3. Traverse the bucket's linked list, comparing each node's ID to the search key
 * 4. If found -> return pointer to the Employee inside that node
 * 5. If end of list reached without match -> return NULL
 *
 * RETURN TYPE: Employee*
 * - Returns a POINTER to the Employee struct INSIDE the node
 * - This is heap memory (inside a malloc'd Node)
 * - The caller can READ the data directly
 * - The caller can MODIFY the data (it would affect the stored data)
 * - If NULL returned -> employee doesn't exist in the hash table
 *
 * WHY RETURN Employee* instead of Node*?
 * - ENCAPSULATION: The caller only needs the employee data, not the Node structure
 * - The Node struct is an implementation detail of the hash table
 * - Returning Employee* hides the linked list implementation from the caller
 *
 * TIME COMPLEXITY:
 *   - Hash computation: O(1)
 *   - List traversal: O(k) where k = items in that bucket
 *   - Average: O(1) - short chain, found quickly
 *   - Worst: O(n) - all items in one bucket, item is last
 */
Employee* searchEmployee(Node** HT, unsigned short int size_ht, unsigned short int search_key)
{
    // STEP 1: Compute which bucket to search
    // Use the SAME hash function as insertion
    // The key must hash to the same position for search to work!
    unsigned short int pos = hash_function(search_key, size_ht);

    // STEP 2: Check if the bucket is non-empty
    // 'if (HT[pos])' is equivalent to 'if (HT[pos] != NULL)'
    // If NULL, the bucket is empty -> employee can't be there -> return NULL
    if (HT[pos])
    {
        // Bucket has at least one node - traverse its linked list
        // Start at the head of this bucket's list
        Node* t = HT[pos];

        // Traverse the linked list at this bucket
        // 'while (t)' is equivalent to 'while (t != NULL)'
        {
            while (t)
            {
                // COMPARISON: Check if this node's employee ID matches the search key
                // t->emp.id is the ID stored in the current node
                // search_key is the ID we're looking for
                if (t->emp.id == search_key)
                {
                    // FOUND! Return a pointer to the Employee inside this node
                    // &t->emp = address of the emp field within the Node
                    // This is a HEAP memory address (Node was malloc'd)
                    // The caller can use this pointer to read/modify the employee data
                    return &t->emp;
                }

                // Not a match - move to the next node in the chain
                t = t->next;
            }
        }
    }

    // NOT FOUND: Either the bucket was empty, or we traversed the entire
    // linked list without finding a matching ID
    return NULL;  // Return NULL to signal "employee does not exist"
}

/*
 * ======================================================================================
 * MASTER PATTERNS - CROSS-CUTTING CONCEPTS ACROSS ALL DATA STRUCTURES
 * ======================================================================================
 *
 * These patterns repeat in singly linked lists, doubly linked lists, and hash tables.
 * Mastering these patterns means you can handle any variation on the exam.
 *
 * PATTERN 1: NODE CREATION (used in ALL data structures)
 * ========================================================
 *   Node* new_node = malloc(sizeof(Node));  // Allocate on heap
 *   new_node->data = data;                   // Copy data into node
 *   new_node->next = NULL;                   // Mark as last node
 *
 * DETAILS:
 * - malloc(sizeof(Node)) allocates exactly the struct's byte size on heap
 * - sizeof(Node) includes ALL fields: embedded struct + pointers + padding
 * - malloc returns void* which implicitly converts to any pointer type
 * - If malloc fails (out of memory), it returns NULL - check before using!
 * - Memory is UNINITIALIZED - must set all fields explicitly (especially next/prev to NULL)
 * - After creation, the node is "orphan" - not yet linked to any data structure
 * - Linking happens AFTER creation (by insert functions)
 *
 * PATTERN 2: EMPTY LIST CHECK (every operation needs this)
 * ========================================================
 *   if (list == NULL) { // handle empty list }
 *
 * DETAILS:
 * - An empty list is ALWAYS represented by head/first == NULL
 * - Every insert, delete, search, and parse function MUST check this
 * - For doubly linked: BOTH first == NULL AND last == NULL
 * - For hash table: each bucket HT[i] == NULL means empty bucket
 * - Common exam trap: forgetting the empty check causes NULL pointer dereference
 *
 * PATTERN 3: LIST TRAVERSAL (the most repeated pattern)
 * =====================================================
 *   Node* t = list;           // Start at head
 *   while (t != NULL) {       // Continue until end of list
 *       // process t->data    // Do something with current node
 *       t = t->next;          // Advance to next node
 *   }
 *
 * DETAILS:
 * - NEVER modify 'list' directly - use a temporary 't'
 * - 't' is a POINTER that walks through the chain
 * - t = t->next means "follow the next pointer to the next node"
 * - The condition t != NULL handles BOTH empty lists and end-of-list
 * - If list is NULL initially, the loop doesn't execute (correct behavior)
 * - After the loop, t == NULL (past the last node)
 * - For doubly linked: use t = t->prev for backward traversal
 * - For hash table: outer loop over buckets, inner loop traverses each bucket's chain
 *
 * PATTERN 4: FILE PARSING WITH strtok (used in seminars 4, 5, 6)
 * ==============================================================
 *   FILE* f = fopen("file.txt", "r");
 *   while (fgets(buffer, sizeof(buffer), f)) {
 *       char* token = strtok(buffer, ",\n");  // First call: pass buffer
 *       // Process first token...
 *       token = strtok(NULL, seps);           // Subsequent calls: pass NULL
 *       // Process next token...
 *   }
 *   fclose(f);
 *
 * DETAILS:
 * - fopen("filename", "r") opens text file for reading, returns FILE* or NULL
 * - fgets(buffer, size, file) reads one line including the newline character
 * - fgets stops at newline, buffer full, or EOF - returns NULL at EOF
 * - strtok REPLACES delimiters with '\0' (modifies the original buffer)
 * - strtok keeps INTERNAL STATE (static pointer) - that's why subsequent calls use NULL
 * - strtok is NOT thread-safe (but fine for single-threaded exam code)
 * - First call: strtok(string, delimiters) - sets up tokenization
 * - Subsequent calls: strtok(NULL, delimiters) - continues from last position
 * - When no more tokens, strtok returns NULL
 * - Delimiters ",\n" means BOTH comma AND newline are treated as separators
 * - atoi(token) converts string to int (returns 0 for non-numeric strings)
 * - atof(token) converts string to double (returns 0.0 for non-numeric)
 * - Always fclose(f) when done - releases the file handle
 *
 * PATTERN 5: SAFE DEALLOCATION (prevent memory leaks)
 * ====================================================
 *   free(dynamic_string);  // Free inner allocations first (char*)
 *   free(node);            // Then free the container (node)
 *   pointer = NULL;        // Prevent dangling pointer access
 *
 * DETAILS:
 * - Every malloc() MUST have a matching free() - ONE TO ONE correspondence
 * - Order matters: free inner (char*) BEFORE outer (node)
 *   * If you free the node first, you lose the pointer to the name = MEMORY LEAK
 * - free() does NOT set the pointer to NULL - must do it manually
 * - After free, the pointer becomes DANGLING (points to freed memory)
 * - Accessing dangling pointer = UNDEFINED BEHAVIOR
 * - Double-free = CRASH or memory corruption
 * - For linked lists: must free ALL nodes one by one
 * - For hash table: must free all nodes in ALL buckets, then free the table array
 * - Setting head = NULL without freeing = MEMORY LEAK (nodes still on heap)
 *
 * PATTERN 6: HASH TABLE OPERATIONS (seminar 6)
 * =============================================
 *   pos = hash(key);              // Compute bucket index
 *   HT[pos] = insertNodeEnd(...); // Insert (reuse linked list)
 *   t = HT[pos]; while(t) {...}   // Search (traverse chain)
 *
 * DETAILS:
 * - Hash table = array of Node* pointers (each points to a linked list head)
 * - Initialization: malloc the array, then set ALL elements to NULL
 * - Insert: hash(key) gives bucket index, insert into that bucket's chain
 * - Search: hash(key) gives bucket index, traverse only that bucket's chain
 * - The SAME hash function MUST be used for insert and search
 * - If different hash functions are used, search will look in wrong bucket
 * - Code reuse: hash table buckets use the same Node and insertNodeEnd
 *
 * MEMORY MANAGEMENT RULES (MEMORIZE):
 * - Every malloc() MUST have a matching free()
 * - Free inner allocations BEFORE outer ones (char* before node, rows before spine)
 * - Set pointers to NULL after free() - prevents dangling pointer access
 * - Never free stack-allocated data (local variables, arrays declared with [])
 * - Never access memory after free() - undefined behavior
 * - Never double-free - crash or corruption
 * - malloc does NOT zero memory - contents are GARBAGE
 * - calloc(size, count) = malloc + zero-initialization (not used in seminars)
 *
 * STRING HANDLING RULES (MEMORIZE):
 * - char* requires: malloc(strlen(s) + 1) THEN strcpy(dest, s)
 * - char[] requires: just strcpy(dest, s) - already allocated as part of struct
 * - String literals ("text") are READ-ONLY - stored in data segment
 * - strlen() returns length WITHOUT null terminator
 * - The +1 in malloc is for '\0' (the string terminator byte)
 * - printf("%s", ptr) reads until it finds a '\0' byte
 * - strcpy(dest, src) copies bytes until (and including) '\0'
 *
 * STRUCT MEMORY RULES (MEMORIZE):
 * - sizeof(struct) >= sum of member sizes (compiler adds padding for alignment)
 * - Padding ensures each member is at its natural alignment boundary
 * - Struct assignment = member-by-member copy (SHALLOW copy)
 * - If struct has char*, only the POINTER is copied, not the string data
 * - Embedded struct (not pointer) = allocated as part of parent's memory
 * - CPU reads memory in word-sized chunks (4 bytes on x86)
 * - Alignment: 2-byte types on 2-byte boundaries, 4-byte on 4-byte, etc.
 *
 * TIME COMPLEXITY SUMMARY (MEMORIZE):
 * +----------------------+----------------+------------------+------------------+
 * | Operation            | Singly Linked  | Doubly Linked    | Hash Table       |
 * +----------------------+----------------+------------------+------------------+
 * | Insert at end        | O(n)           | O(1)             | O(1) avg         |
 * | Delete from end      | O(n)           | O(1)             | N/A              |
 * | Search               | O(n)           | O(n)             | O(1) avg         |
 * | Parse all            | O(n)           | O(n)             | O(m + n)         |
 * | Memory per node      | data + 4B      | data + 8B        | data + 4B        |
 * +----------------------+----------------+------------------+------------------+
 * (m = buckets, n = elements, B = bytes on x86)
 *
 * COMPLEXITY EXPLANATIONS:
 * - Singly linked insert at end = O(n): must traverse entire list to find last node
 * - Doubly linked insert at end = O(1): have direct 'last' pointer, no traversal
 * - Singly linked delete from end = O(n): must find second-to-last node
 * - Doubly linked delete from end = O(1): have 'prev' pointer on last node
 * - Hash table average = O(1): hash computation O(1) + short chain traversal O(1)
 * - Hash table worst = O(n): all elements in one bucket (all keys collide)
 * - Hash table parse = O(m + n): must check all m buckets + visit all n elements
 *
 * COMMON EXAM QUESTIONS / VARIATIONS:
 * 1. Insert at BEGINNING instead of end (O(1) for singly linked!)
 * 2. Delete a specific node by ID (search + delete)
 * 3. Count nodes / sum values / find max / find min
 * 4. Reverse a linked list (change all next pointers)
 * 5. Insert in sorted order (find position, insert)
 * 6. Implement getEmployeesUnderAverage for doubly linked list
 * 7. Delete an employee from hash table by ID
 * 8. Merge two sorted lists
 * 9. Find the middle element of a list (slow/fast pointer technique)
 * 10. Detect a cycle in a linked list (Floyd's algorithm)
 */

/*
 * ======================================================================================
 * PART 5: BINARY SEARCH TREES (BST)
 * ======================================================================================
 *
 * SEMINAR 7: 07_BST.c
 *
 * WHAT IS A BINARY SEARCH TREE (BST)?
 * - A BINARY TREE where each node has AT MOST TWO children (left and right)
 * - BST PROPERTY: For ANY node:
 *   * ALL nodes in the LEFT subtree have IDs < this node's ID
 *   * ALL nodes in the RIGHT subtree have IDs > this node's ID
 *   * NO duplicate IDs allowed (duplicate insertions are rejected)
 * - This ordering enables FAST search: O(log n) average case
 *
 * WHY USE A BST?
 * - SEARCH: O(log n) average vs O(n) for linked lists (sorted order)
 * - INSERT: O(log n) average - find the right spot and add
 * - DELETE: O(log n) average - find the node, restructure the tree
 * - IN-ORDER TRAVERSAL: Visits nodes in SORTED ORDER (ascending by ID)
 *
 * BST VS HASH TABLE:
 * - Hash table: O(1) average lookup, but NO ordering
 * - BST: O(log n) average lookup, but KEEPS DATA SORTED
 * - Hash table: collisions require chaining (linked lists per bucket)
 * - BST: natural structure, no extra collision handling needed
 *
 * BST VS LINKED LIST:
 * - Linked list: O(n) search - must traverse every node
 * - BST: O(log n) search - eliminate half the tree at each step
 * - Linked list: sequential, no ordering guarantee
 * - BST: in-order traversal gives sorted output
 *
 * MEMORY MODEL OF A BST:
 *
 *                    root (id=1012)
 *                   /              \
 *           (id=849)              (id=2012)
 *           /    \                /     \
 *          NULL  NULL          (id=901)  (id=957) ...
 *
 * Each node has: emp data + left pointer + right pointer
 * NULL pointers mean "no child in this direction"
 *
 * TREE TRAVERSAL TYPES:
 * - IN-ORDER:    left -> node -> right  (gives SORTED order)
 * - PRE-ORDER:   node -> left -> right  (gives tree structure)
 * - POST-ORDER:  left -> right -> node  (useful for deallocation)
 *
 * COMMON MISTAKES:
 * - Forgetting to set left/right = NULL after malloc -> garbage pointers -> crash
 * - Allowing duplicate IDs -> breaks BST invariant
 * - Not handling the flag for insertion success -> can't tell if insert worked
 * - Memory leaks: must free name AND node in post-order (children before parent)
 *
 * DELETE OPERATION - THREE CASES:
 * - Leaf node (no children): simply free it
 * - One child: replace node with its child
 * - Two children: find IN-ORDER SUCCESSOR (leftmost node of right subtree),
 *   move its data to the deleted node, then delete the successor
 */

/*
 * ======================================================================================
 * 5.1 BSTNODE STRUCTURE
 * ======================================================================================
 *
 * BSTNode = Binary Search Tree Node
 * Similar to linked list Node but with TWO child pointers (left, right).
 *
 * The Employee is EMBEDDED (not a pointer) - same pattern as linked lists.
 * When you malloc a BSTNode, you get space for both emp AND the two pointers.
 *
 * MEMORY PER NODE: 24 (emp) + 4 (left) + 4 (right) = 32 bytes on x86
 */
struct BSTNode
{
    Employee emp;               // Embedded Employee data (24 bytes)
    struct BSTNode* left;       // Pointer to LEFT child (4 bytes) - NULL if no left child
    struct BSTNode* right;      // Pointer to RIGHT child (4 bytes) - NULL if no right child
};

typedef struct BSTNode BSTNode;  // Alias for convenience

/*
 * ======================================================================================
 * 5.2 INSERT EMPLOYEE INTO BST
 * ======================================================================================
 *
 * PURPOSE: Insert a new employee into the BST while maintaining BST property.
 *
 * ALGORITHM:
 * 1. If current node is NULL, create new node here (base case)
 * 2. If emp.id < node->emp.id, go LEFT (smaller IDs go left)
 * 3. If emp.id > node->emp.id, go RIGHT (larger IDs go right)
 * 4. If emp.id == node->emp.id, DUPLICATE - reject insertion
 * 5. Return the (possibly updated) node pointer
 *
 * WHY THE flag_insert PARAMETER?
 * - The caller needs to know if insertion succeeded or was rejected
 * - *flag_insert = 1 means successful insertion
 * - *flag_insert = 0 means duplicate ID - employee already exists
 * - Without this flag, the caller can't distinguish between:
 *   * "new node was created" (needs no cleanup)
 *   * "duplicate found" (caller must free emp.name to avoid leak)
 *
 * TIME COMPLEXITY: O(log n) average, O(n) worst case (skewed tree)
 * SPACE COMPLEXITY: O(log n) for recursion stack
 *
 * RECURSIVE PATTERN:
 * - This function calls ITSELF to traverse the tree
 * - Each recursive call goes one level deeper
 * - Base case: node == NULL (found the empty spot)
 * - The return value chains back up: parent's left/right = result of recursive call
 *
 * VISUAL INSERTION:
 * Insert 1012, 2012, 849 in order:
 *
 * Step 1: Insert 1012         Step 2: Insert 2012      Step 3: Insert 849
 *      [1012]                     [1012]                    [1012]
 *       /  \                       /  \                      /  \
 *     NULL NULL                  NULL [2012]             [849] [2012]
 *                                    /  \                /  \   /  \
 *                                  NULL NULL          NULL NULL NULL NULL
 */
BSTNode* insertEmployeeBST(BSTNode* node, Employee emp, unsigned char* flag_insert)
{
    if (node != NULL)
    {
        // Tree is not empty - find the correct position
        if (emp.id < node->emp.id)
        {
            // New ID is SMALLER - go LEFT
            // Recursively insert into the left subtree
            // The return value becomes the NEW left child
            node->left = insertEmployeeBST(node->left, emp, flag_insert);
        }
        else
        {
            if (emp.id > node->emp.id)
            {
                // New ID is LARGER - go RIGHT
                // Recursively insert into the right subtree
                node->right = insertEmployeeBST(node->right, emp, flag_insert);
            }
            else
            {
                // DUPLICATE ID! emp.id == node->emp.id
                // BST does NOT allow duplicates - reject insertion
                *flag_insert = 0;  // Signal: insertion FAILED (duplicate)
                return node;       // Return unchanged node - tree not modified
            }
        }
    }
    else
    {
        // BASE CASE: node is NULL - this is where the new node belongs
        // We've traversed left/right until we hit an empty spot
        *flag_insert = 1;  // Signal: insertion SUCCEEDED

        // Allocate the new node on the heap
        node = malloc(sizeof(BSTNode));
        node->emp = emp;    // Copy employee data into the new node
        node->left = NULL;  // No left child yet
        node->right = NULL; // No right child yet
        // CRITICAL: Must set both pointers to NULL!
        // Uninitialized pointers contain garbage -> crash on traversal
    }

    return node;  // Return the (possibly new) node pointer
}

/*
 * ======================================================================================
 * 5.3 BST IN-ORDER TRAVERSAL (PRINTS IN SORTED ORDER)
 * ======================================================================================
 *
 * PURPOSE: Print all employees in the BST from smallest ID to largest ID.
 *
 * WHY IN-ORDER?
 * - In-order traversal of a BST ALWAYS visits nodes in SORTED ORDER
 * - This is because: left < node < right (BST property)
 * - By processing left first, then node, then right, we get ascending order
 *
 * THE RECURSIVE PATTERN:
 *   traverse(left)  -> process all smaller IDs first
 *   process(node)   -> print current node
 *   traverse(right) -> process all larger IDs last
 *
 * TIME COMPLEXITY: O(n) - visits every node exactly once
 * SPACE COMPLEXITY: O(log n) - recursion stack depth = tree height
 *
 * VISUAL TRAVERSAL:
 *      [1012]
 *       /  \
 *    [849] [2012]
 *            \
 *           [957]
 *
 * In-order: 849 -> 957 -> 1012 -> 2012 (sorted!)
 *
 * WHY NOT PRE-ORDER OR POST-ORDER?
 * - Pre-order (node->left->right): gives [1012, 849, 2012, 957] - NOT sorted
 * - Post-order (left->right->node): gives [849, 957, 2012, 1012] - NOT sorted
 * - Only in-order gives sorted output for a BST
 */
void BSTParse(BSTNode* node)
{
    if (node != NULL)
    {
        BSTParse(node->left);   // STEP 1: Recursively process LEFT subtree first
        // All nodes in left subtree have smaller IDs - they print first

        printf("%d %s\n", node->emp.id, node->emp.name);
        // STEP 2: Process the current node (print ID and name)

        BSTParse(node->right);  // STEP 3: Recursively process RIGHT subtree last
        // All nodes in right subtree have larger IDs - they print last
    }
    // If node == NULL, do nothing (base case for leaf children)
    // This is the STOP condition for the recursion
}

/*
 * ======================================================================================
 * 5.4 SEARCH EMPLOYEE IN BST
 * ======================================================================================
 *
 * PURPOSE: Find an employee by their ID in the BST.
 *
 * ALGORITHM:
 * 1. If tree is empty, return NULL (not found)
 * 2. If search_key < node->emp.id, search LEFT (smaller IDs are on the left)
 * 3. If search_key > node->emp.id, search RIGHT (larger IDs are on the right)
 * 4. If search_key == node->emp.id, FOUND - return address of employee
 *
 * TIME COMPLEXITY: O(log n) average, O(n) worst case
 * SPACE COMPLEXITY: O(log n) for recursion stack
 *
 * WHY THIS IS FASTER THAN LINKED LIST:
 * - Linked list: must check EVERY node (O(n))
 * - BST: eliminate HALF the tree at each step (O(log n))
 * - Example: 1000 employees
 *   * Linked list: up to 1000 comparisons
 *   * BST: about 10 comparisons (log2(1000) ≈ 10)
 *
 * WHY RETURN Employee* (not BSTNode*)?
 * - The caller wants the EMPLOYEE DATA, not the tree node
 * - Returns address of emp field INSIDE the node
 * - Caller can read: pEmp->id, pEmp->name, pEmp->salary, etc.
 * - Returns NULL if not found
 */
Employee* searchEmployeeBST(BSTNode* node, unsigned short int search_key)
{
    Employee* emp_result = NULL;  // Default: not found

    if (node != NULL)
    {
        // Tree is not empty - compare search_key with current node's ID
        if (search_key < node->emp.id)
        {
            // Search key is SMALLER - go LEFT
            emp_result = searchEmployeeBST(node->left, search_key);
        }
        else
        {
            if (search_key > node->emp.id)
            {
                // Search key is LARGER - go RIGHT
                emp_result = searchEmployeeBST(node->right, search_key);
            }
            else
            {
                // FOUND! search_key == node->emp.id
                // Return the address of the Employee inside this node
                return &node->emp;
            }
        }
    }

    return emp_result;  // NULL if not found, or result from recursive call
}

/*
 * ======================================================================================
 * 5.5 DEALLOCATE ENTIRE BST (MEMORY CLEANUP)
 * ======================================================================================
 *
 * PURPOSE: Free ALL memory used by the BST (post-order traversal).
 *
 * WHY POST-ORDER?
 * - We MUST free children BEFORE their parent
 * - If we free the parent first, we LOSE the pointers to children -> memory leak
 * - Post-order (left -> right -> node) guarantees children are freed first
 *
 * ALGORITHM:
 * 1. Recursively free left subtree
 * 2. Recursively free right subtree
 * 3. Free current node's name (dynamic string)
 * 4. Free current node itself
 * 5. Set node to NULL and return
 *
 * TIME COMPLEXITY: O(n) - visits every node exactly once
 * SPACE COMPLEXITY: O(log n) for recursion stack
 *
 * IMPORTANT: After deallocation, the root pointer must be set to NULL
 * to prevent use-after-free bugs.
 */
BSTNode* deallocateBST(BSTNode* node)
{
    if (node != NULL)
    {
        // STEP 1: Recursively free the LEFT subtree first
        // Returns NULL after freeing - assign back to node->left
        node->left = deallocateBST(node->left);

        // STEP 2: Recursively free the RIGHT subtree
        node->right = deallocateBST(node->right);

        // STEP 3: Free the employee's name (dynamic string)
        // MUST free BEFORE freeing the node (otherwise we lose the pointer)
        free(node->emp.name);

        // STEP 4: Free the node itself
        free(node);

        // STEP 5: Set node to NULL to prevent use-after-free
        // This NULL is returned so the caller's pointer also becomes NULL
        node = NULL;
    }

    return node;  // Always returns NULL (or unchanged NULL if input was NULL)
}

/*
 * ======================================================================================
 * 5.6 DELETE A SPECIFIC NODE FROM BST
 * ======================================================================================
 *
 * PURPOSE: Remove a node with the given search_key from the BST.
 *
 * WHY IS BST DELETION COMPLEX?
 * Unlike linked lists where we just "unlink" a node, BST deletion must
 * MAINTAIN THE BST PROPERTY after removal. The replacement node must
 * be in the correct position to keep left < node < right ordering.
 *
 * THREE CASES FOR DELETION:
 *
 * CASE 1: LEAF NODE (no children)
 *   - Simply free the node
 *   - Parent's pointer to it becomes NULL
 *
 * CASE 2: ONE CHILD
 *   - Replace the node with its single child
 *   - The child takes the node's place in the tree
 *
 * CASE 3: TWO CHILDRES (most complex)
 *   - Find the IN-ORDER SUCCESSOR: leftmost node of the RIGHT subtree
 *     (this is the SMALLEST node larger than the deleted node)
 *   - Attach the LEFT subtree to the successor
 *   - The RIGHT subtree becomes the new root of this subtree
 *
 * THE SEMINAR'S APPROACH (for two children):
 * 1. Save left and right subtrees
 * 2. Find leftmost node of right subtree (minimum ID in right subtree)
 * 3. Attach left subtree to that leftmost node's left pointer
 * 4. Free the original node
 * 5. Return right_subtree as the new root of this position
 *
 * VISUAL - Deleting node 1012 (has two children):
 *   BEFORE:                    AFTER:
 *     [1012]                      [2012]
 *      /  \                        /
 *   [849] [2012]                [849]
 *    /  \   /  \                 /  \
 *   N   N  [901] [957]         N   [901]
 *               /  \                  /  \
 *              N   [905]            N   [905]
 *
 * The right subtree (2012) becomes the new root.
 * 849 (left subtree) is attached to 901's left (901 was leftmost in right subtree).
 *
 * TIME COMPLEXITY: O(log n) average, O(n) worst case
 * SPACE COMPLEXITY: O(log n) for recursion stack
 */
BSTNode* deleteNodeBST(BSTNode* node, unsigned short int search_key, Employee* empl)
{
    if (node != NULL)
    {
        if (node->emp.id == search_key)
        {
            // FOUND THE NODE TO DELETE
            // Copy employee data to output parameter BEFORE freeing
            // The caller can use this to know what was deleted
            *empl = node->emp;

            // Save references to the two subtrees
            BSTNode* left_subtree = node->left;
            BSTNode* right_subtree = node->right;

            // Find the IN-ORDER SUCCESSOR:
            // Go to the right subtree, then go LEFT as far as possible
            // This finds the node with the SMALLEST ID in the right subtree
            // This node is the correct replacement for maintaining BST order
            BSTNode* temp = right_subtree;
            while (temp->left != NULL)
                temp = temp->left;
            // After this loop: temp points to the leftmost node of right_subtree
            // temp->left is NULL (it's the leftmost)

            // Attach the LEFT SUBTREE to the successor's left pointer
            // The successor (leftmost of right) now "adopts" the left subtree
            // This maintains BST property: all left subtree nodes < successor < rest of right
            temp->left = left_subtree;

            // Free the node being deleted
            // NOTE: We do NOT free emp.name here because *empl = node->emp copies it
            // The caller owns empl->name now and must free it
            free(node);

            // Return the right subtree as the new root of this position
            // The right subtree now contains all nodes from both original subtrees
            // (left subtree was attached to the leftmost node of right subtree)
            node = right_subtree;
        }
        else
        {
            // Not the node to delete - search for it
            if (node->emp.id > search_key)
            {
                // Search key is SMALLER - go LEFT
                node->left = deleteNodeBST(node->left, search_key, empl);
            }
            else
            {
                // Search key is LARGER - go RIGHT
                node->right = deleteNodeBST(node->right, search_key, empl);
            }
        }
    }

    return node;  // Return the (possibly updated) node pointer
}

/*
 * ======================================================================================
 * 5.7 SELECTIVE SAVE: BST NODES TO ARRAY (WITH CONDITION)
 * ======================================================================================
 *
 * PURPOSE: Save only BST employees that meet a condition (e.g., salary > threshold)
 * into an array, sorted by ID (in-order traversal).
 *
 * THIS IS THE EXAM PATTERN: "Save data into a vector" — but NOT all nodes, only
 * those matching a specific criterion.
 *
 * EXAMPLE EXAM QUESTIONS:
 * - "Save all employees with salary > X into an array"
 * - "Save all employees with ID between min and max into a vector"
 *
 * ALGORITHM:
 * 1. Recursively traverse left subtree (in-order)
 * 2. If current node meets the condition, copy to array
 * 3. Recursively traverse right subtree (in-order)
 * 4. Return the count of matching nodes
 *
 * TWO-PASS PATTERN (like deleteNodesBelowAverageSalary):
 * - Pass 1: Count matching nodes to know array size
 * - Pass 2: Allocate array and copy matching nodes
 *
 * WHY TWO PASSES?
 * - malloc requires knowing the exact size upfront
 * - We can't reallocate during traversal (would be O(n²))
 * - Counting first gives us the exact size for one allocation
 *
 * TIME COMPLEXITY: O(n) - two traversals, each visits all nodes once
 * SPACE COMPLEXITY: O(m) for output array where m = matching count
 */

// Helper: Check if an employee meets the condition (customize per exam)
static int condition_salary_above(Employee emp, float threshold)
{
    return emp.salary > threshold;  // Modify condition per exam question
}

// Pass 1: Count matching nodes
int countMatchingBST(BSTNode* node, float threshold)
{
    if (node == NULL)
        return 0;
    // Count = left matches + current match + right matches
    int count = countMatchingBST(node->left, threshold);
    if (condition_salary_above(node->emp, threshold))
        count++;
    count += countMatchingBST(node->right, threshold);
    return count;
}

// Pass 2: Copy matching nodes into pre-allocated array (in-order = sorted by ID)
int saveMatchingBSTToArray(BSTNode* node, float threshold, Employee* array, int index)
{
    if (node != NULL)
    {
        // STEP 1: Process left subtree (smaller IDs)
        index = saveMatchingBSTToArray(node->left, threshold, array, index);

        // STEP 2: If condition met, copy to array
        if (condition_salary_above(node->emp, threshold))
        {
            array[index] = node->emp;
            index++;
        }

        // STEP 3: Process right subtree (larger IDs)
        index = saveMatchingBSTToArray(node->right, threshold, array, index);
    }
    return index;  // Returns the count of copied elements
}

/*
 * Usage pattern in main():
 *   // Pass 1: Count
 *   int count = countMatchingBST(root, 1000.0);
 *   // Pass 2: Allocate and copy
 *   Employee* result = malloc(count * sizeof(Employee));
 *   saveMatchingBSTToArray(root, 1000.0, result, 0);
 *   // result now contains employees with salary > 1000, sorted by ID
 */

/*
 * ======================================================================================
 * 5.8 SELECTIVE SAVE: BST NODES TO SINGLY LINKED LIST (WITH CONDITION)
 * ======================================================================================
 *
 * PURPOSE: Save only BST employees that meet a condition into a linked list.
 *
 * EXAM QUESTION: "Save all employees with salary > X into a singly linked list"
 *
 * ALGORITHM:
 * 1. Recursively traverse left subtree (in-order)
 * 2. If current node meets condition, insert at end of linked list
 * 3. Recursively traverse right subtree (in-order)
 * 4. Return the linked list head pointer
 *
 * NOTE: No two-pass needed! Linked list grows dynamically — we just insert as we go.
 * But this is O(n * n) because insertNodeEnd traverses to find the end each time.
 *
 * TIME COMPLEXITY: O(n * n) - n nodes × O(n) per insertNodeEnd
 * SPACE COMPLEXITY: O(m * 28) for m matching nodes (Node struct allocations)
 */
Node* saveMatchingBSTToList(BSTNode* node, float threshold, Node* list)
{
    if (node != NULL)
    {
        // STEP 1: Process left subtree (smaller IDs)
        list = saveMatchingBSTToList(node->left, threshold, list);

        // STEP 2: If condition met, insert at end of linked list
        if (condition_salary_above(node->emp, threshold))
            list = insertNodeEnd(list, node->emp);

        // STEP 3: Process right subtree (larger IDs)
        list = saveMatchingBSTToList(node->right, threshold, list);
    }
    return list;
}

/*
 * Usage pattern in main():
 *   Node* resultList = NULL;
 *   resultList = saveMatchingBSTToList(root, 1000.0, resultList);
 *   parseList(resultList);  // Prints employees with salary > 1000, sorted by ID
 */

/*
 * ======================================================================================
 * PART 6: HEAP STRUCTURE
 * ======================================================================================
 *
 * SEMINAR 8: 08_HeapStruct.c — Adapted to 1071 practices (Employee structs)
 *
 * WHAT IS A HEAP?
 * - A COMPLETE BINARY TREE stored in an ARRAY
 * - COMPLETE TREE: every level is FULL except possibly the last, which is filled left-to-right
 * - Two types: MAX HEAP (parent >= children) and MIN HEAP (parent <= children)
 * - The ROOT (index 0) is always the MAX (or MIN) element
 *
 * WHY USE A HEAP?
 * - Priority Queue: always extract the highest (or lowest) priority element
 * - Heap Sort: O(n log n) sorting algorithm
 * - Finding K largest/smallest elements efficiently
 *
 * MAX HEAP PROPERTY:
 * - Every parent's value >= both children's values
 * - ROOT (index 0) is the MAXIMUM element
 *
 * ARRAY REPRESENTATION (0-based indexing):
 * For a node at INDEX i:
 *   - Parent index: (i - 1) / 2     (integer division)
 *   - Left child index: 2 * i + 1
 *   - Right child index: 2 * i + 2
 *   - Root is at index 0
 *
 * VISUAL MAX HEAP (using salary as key):
 *
 *         [9654.35]                  Array: [9654.35, 500.00, 154.35, ...]
 *         /      \                   Index:   0          1         2
 *     [500.00]  [154.35]            Node 0 (9654.35): children at 1, 2
 *     /    \     /    \             Node 1 (500.00):  children at 3, 4
 *  [200.50] [40] [54.35] [30]       Node 2 (154.35):  children at 5, 6
 *
 * KEY OPERATIONS:
 * 1. INSERT: Add Employee at end, bubble UP (compare salary with parent, swap if larger)
 * 2. DELETE ROOT: Replace root with last element, bubble DOWN (compare with larger child)
 * 3. DYNAMIC RESIZING: When array is full, allocate bigger array, copy, free old
 *
 * HOW THIS ADAPTS THE 1047 SEMINAR TO 1071 PRACTICES:
 * - Uses Employee structs (NOT plain int keys)
 * - Comparison by salary (NOT by int key value)
 * - File parsing: fgets + strtok (NOT fscanf)
 * - malloc + strcpy for name strings (same as all 1071 seminars)
 * - Dynamic array resizing pattern (malloc new, copy, free old)
 *
 * HEAP VS BST:
 * - BST: ordered traversal, O(log n) search by key (id)
 * - Heap: O(1) access to MAX salary, but NO efficient search by arbitrary field
 * - Heap: always complete tree, BST: any shape
 *
 * COMMON MISTAKES:
 * - Forgetting to free emp.name when deleting from heap (memory leak!)
 * - Using 1-based indexing formulas with 0-based array (off-by-one)
 * - Forgetting to update heapSize after insert/delete
 * - Not checking if children exist before accessing them
 */

/*
 * ======================================================================================
 * 6.1 INSERT EMPLOYEE INTO MAX HEAP (BUBBLE UP)
 * ======================================================================================
 *
 * PURPOSE: Insert a new Employee into the max heap, comparing by salary.
 *
 * ALGORITHM:
 * 1. Increment heap size
 * 2. Place employee at the last position
 * 3. Calculate parent offset: (offs_key - 1) / 2
 * 4. While employee's salary > parent's salary, swap them (bubble up)
 * 5. Update offsets and repeat until heap property is satisfied
 *
 * TIME COMPLEXITY: O(log n) - at most tree height swaps
 * SPACE COMPLEXITY: O(1)
 *
 * VISUAL INSERTION (insert salary 500 into heap [9654, 154, 60, 54, 40]):
 *
 * Step 1: Add 500 at end:          Step 2: Bubble up:
 *    [9654]                           [9654]
 *    /    \                           /    \
 *  [154]  [60]                      [500]  [60]
 *  /  \    /                        /  \    /
 *[54] [40] [500]                  [54] [40] [154]
 *
 * 500 > 154 (parent), so swap. 500 < 9654 (new parent), so stop.
 */
void insertEmployeeHeap(Employee* strHeap, int* strSize, Employee emp)
{
    // STEP 1: Increment the heap size
    *strSize += 1;

    // STEP 2: Place the new employee at the last available position
    int offs_key = *strSize - 1;     // offset (index) of the new employee
    strHeap[offs_key] = emp;          // initial insertion at the end of the array
    // NOTE: struct assignment copies all fields (shallow copy for emp.name pointer)

    // STEP 3: Calculate the parent's offset
    // For a node at index i, parent is at (i - 1) / 2 (integer division)
    int offs_parinte = (offs_key - 1) / 2;

    // STEP 4: BUBBLE UP - while new employee's salary > parent's salary, swap
    // This violates the max heap property (parent salary should be >= child salary)
    // We fix it by swapping upward until the property is restored
    while (strHeap[offs_key].salary > strHeap[offs_parinte].salary)
    {
        // SWAP: exchange entire Employee struct with its parent
        // This copies all fields: id, name pointer, salary, age, hire_date
        Employee aux = strHeap[offs_key];
        strHeap[offs_key] = strHeap[offs_parinte];
        strHeap[offs_parinte] = aux;

        // STEP 5: Update employee's offset to the parent's position
        // The employee has moved up one level in the tree
        offs_key = offs_parinte;

        // STEP 6: Calculate the NEW parent offset for the next comparison
        offs_parinte = (offs_key - 1) / 2;
    }
    // Loop stops when: either at root (offs_key == 0) OR parent salary >= this salary
}

/*
 * ======================================================================================
 * 6.2 DELETE ROOT EMPLOYEE FROM MAX HEAP (BUBBLE DOWN / FILTRARE TOP-DOWN)
 * ======================================================================================
 *
 * PURPOSE: Remove and return the root (highest salary) employee from the max heap.
 *
 * ALGORITHM:
 * 1. Save the root employee (max salary) to return later
 * 2. Replace root with the LAST employee in the array
 * 3. Decrease heap size by 1
 * 4. Find the child with the LARGER salary (max child)
 * 5. While max child's salary > lastEmployee's salary, swap and bubble down
 * 6. Return the saved root employee
 *
 * WHY REPLACE WITH LAST ELEMENT?
 * - The last element is a LEAF node - easiest to remove without breaking tree structure
 * - After placing it at root, we bubble down to restore heap property
 *
 * THE BUBBLE DOWN LOGIC:
 * - First, determine which child has LARGER salary (Left or Right)
 * - Only the larger child can be the swap candidate
 * - Compare lastEmployee's salary with the larger child's salary
 * - If larger child > lastEmployee, swap and continue down
 *
 * HANDLING EDGE CASES FOR CHILDREN:
 * - Both children exist: compare salaries, pick larger
 * - Only Left child exists: Left is the max candidate
 * - No children: loop doesn't run (leaf node, heap property satisfied)
 *
 * TIME COMPLEXITY: O(log n) for bubble down
 * SPACE COMPLEXITY: O(1)
 *
 * IMPORTANT: The returned Employee's name pointer is still valid (shallow copy).
 * The caller owns the returned Employee. If the caller wants to keep the name,
 * they must NOT free it until they're done. The heap's internal pointer now
 * points to a DIFFERENT string (from the last element that replaced root).
 */
Employee deleteEmployeeHeap(Employee* strHeap, int* strSize)
{
    Employee key;              // Will hold the deleted root (return value)
    int offs_key = 0;          // Start at root (index 0)
    int max = 0;               // Index of the larger-salary child

    // STEP 1: Save the last element - it will replace the root
    Employee lastElement = strHeap[*strSize - 1];

    // STEP 2: Save the root employee (maximum salary) to return
    key = strHeap[0];

    // STEP 3: Replace root with the last employee
    // Shallow copy: both strHeap[0] and lastElement.name point to same string
    // But lastElement is a LOCAL copy, so this is fine
    strHeap[0] = lastElement;

    // STEP 4: Decrease heap size
    *strSize -= 1;

    // STEP 5: Calculate initial child offsets
    int Left = 2 * offs_key + 1;   // Left child of root
    int Right = 2 * offs_key + 2;  // Right child of root

    // STEP 6: Determine which child has LARGER salary (the swap candidate)
    if ((Left < *strSize) && (Right < *strSize))
    {
        // BOTH children exist - pick the one with higher salary
        if (strHeap[Left].salary > strHeap[Right].salary)
            max = Left;   // Left child has higher salary
        else
            max = Right;  // Right child has higher salary
    }
    else
    {
        // Only LEFT child might exist (complete tree property)
        if (Left < *strSize)
            max = Left;  // Only left child exists
    }
    // If neither child exists (leaf node), max stays 0 but loop won't run

    // STEP 7: BUBBLE DOWN - while larger child's salary > lastElement's salary, swap
    // lastElement is the value we placed at root - it needs to sink down
    // to its correct position in the heap
    while ((*strSize > 0) && (strHeap[max].salary > lastElement.salary))
    {
        // SWAP: exchange current position with the larger-salary child
        Employee aux = strHeap[offs_key];
        strHeap[offs_key] = strHeap[max];
        strHeap[max] = aux;

        // STEP 8: Move down to the child's position
        offs_key = max;

        // STEP 9: Recalculate child offsets for the new position
        Left = 2 * offs_key + 1;
        Right = 2 * offs_key + 2;

        // STEP 10: Re-determine the larger-salary child
        if ((Left < *strSize) && (Right < *strSize))
        {
            if (strHeap[Left].salary > strHeap[Right].salary)
                max = Left;
            else
                max = Right;
        }
        else
        {
            if (Left < *strSize)
                max = Left;
        }
    }
    // Loop stops when: no children OR larger child's salary <= lastElement's salary

    return key;  // Return the deleted maximum-salary employee
}

/*
 * ======================================================================================
 * 6.3 SELECTIVE SAVE: HEAP NODES TO ARRAY (WITH CONDITION)
 * ======================================================================================
 *
 * PURPOSE: Save only heap employees that meet a condition into an array.
 *
 * EXAM QUESTION: "Save all employees with salary > X into an array"
 *
 * Unlike BST, the heap is stored as a flat array — no traversal needed.
 * We just iterate and check the condition.
 *
 * TWO-PASS PATTERN:
 * - Pass 1: Count matching elements
 * - Pass 2: Allocate array and copy matching elements
 *
 * TIME COMPLEXITY: O(n) - two iterations through the array
 * SPACE COMPLEXITY: O(m) for output array
 */

// Helper: Count matching heap elements
int countMatchingHeap(Employee* heap, int heapSize, float threshold)
{
    int count = 0;
    for (int i = 0; i < heapSize; i++)
        if (condition_salary_above(heap[i], threshold))
            count++;
    return count;
}

// Pass 2: Copy matching elements
void saveMatchingHeapToArray(Employee* heap, int heapSize, float threshold, Employee* array, int* arraySize)
{
    int index = 0;
    for (int i = 0; i < heapSize; i++)
    {
        if (condition_salary_above(heap[i], threshold))
        {
            array[index] = heap[i];
            index++;
        }
    }
    *arraySize = index;  // Output: actual count of copied elements
}

/*
 * Usage pattern in main():
 *   int count = countMatchingHeap(heapEmployees, nKeys, 1000.0);
 *   Employee* result = malloc(count * sizeof(Employee));
 *   int resultSize;
 *   saveMatchingHeapToArray(heapEmployees, nKeys, 1000.0, result, &resultSize);
 */

/*
 * ======================================================================================
 * 6.4 SELECTIVE SAVE: HEAP NODES TO SINGLY LINKED LIST (WITH CONDITION)
 * ======================================================================================
 *
 * PURPOSE: Save only heap employees that meet a condition into a linked list.
 *
 * EXAM QUESTION: "Save all employees with salary > X into a singly linked list"
 *
 * ALGORITHM:
 * 1. Iterate through the heap array
 * 2. If employee meets condition, insert at end of linked list
 * 3. Return the linked list
 *
 * NOTE: Order in the list matches heap array order (NOT sorted by salary!)
 *
 * TIME COMPLEXITY: O(n * n) - n elements × O(n) per insertNodeEnd
 * SPACE COMPLEXITY: O(m * 28) for m matching nodes
 */
Node* saveMatchingHeapToList(Employee* heap, int heapSize, float threshold)
{
    Node* list = NULL;  // Start with empty list

    for (int i = 0; i < heapSize; i++)
    {
        if (condition_salary_above(heap[i], threshold))
            list = insertNodeEnd(list, heap[i]);
    }

    return list;
}

/*
 * Usage pattern in main():
 *   Node* resultList = saveMatchingHeapToList(heapEmployees, nKeys, 1000.0);
 *   parseList(resultList);
 */

/*
 * ======================================================================================
 * MAIN FUNCTION - COMPLETE INTEGRATION OF ALL CONCEPTS
 * ======================================================================================
 *
 * This main function demonstrates ALL data structures from all 8 seminar files.
 * It reads the same Employees.txt file into multiple data structures,
 * performs operations on each, and prints the results.
 *
 * EXECUTION FLOW:
 * 1. Data Types & Pointers Demo (Seminar 1)
 * 2. Struct Demo (Seminar 3)
 * 3. Simple Linked List: create, parse, filter, delete (Seminar 4)
 * 4. Doubly Linked List: create, parse, delete (Seminar 5)
 * 5. Hash Table: create, insert, search (Seminar 6)
 * 6. Binary Search Tree: create, insert, search, delete, deallocate (Seminar 7)
 * 7. Max Heap: build, insert, delete, sort (Seminar 8)
 */
int main()
{
    // =====================================================================
    // PART 1: DATA TYPES & POINTERS DEMO
    // =====================================================================
    printf("=== SEMINAR 1: DATA TYPES & POINTERS ===\n\n");

    char a = 11;   // 1 byte on stack, value 11
    char b = -11;  // 1 byte on stack, value -11
    printf("a = %d\n", a);   // Print value of a
    printf("b = %d\n", b);   // Print value of b
    printf("Adress(a) = 0x%p\n", &a);  // Print stack address of a
    printf("Adress(b) = 0x%p\n", &b);  // Print stack address of b

    // Pointer demonstration
    char* p = NULL;              // Declare pointer, initialize to NULL
    printf("p = 0x%p\n", p);     // Prints 0x00000000 (NULL)
    p = &a;                      // p now points to a (stores a's address)
    printf("p = 0x%p\n", p);     // Prints same address as &a
    *p = a - 14;                 // Dereference: modify a through p (a becomes -3)
    printf("a after *p = a-14: %d\n", a);  // Verify: a = -3

    // Dynamic array demonstration
    char v[] = {9, 8, 7, 9, 9, 5, 3, 6};  // Stack array, 8 bytes
    p = malloc(sizeof(v));  // Allocate 8 bytes on heap
    for (unsigned char i = 0; i < sizeof(v); i++)
        p[i] = v[i] - 1;  // Copy from stack to heap, subtracting 1

    // Dynamic matrix demonstration
    char** pm = malloc(2 * sizeof(char*));  // Allocate array of 2 char* pointers
    for (unsigned char i = 0; i < 2; i++)
        pm[i] = malloc((3 + 1) * sizeof(char));  // Allocate 4 bytes per row (+1 for \0)
    for (unsigned char i = 0; i < 2; i++) {
        for (unsigned char j = 0; j < 3; j++)
            pm[i][j] = 'G' - j;  // Fill with 'G', 'F', 'E'
        pm[i][3] = 0;  // Null terminator for string functions
        printf("%s\n", pm[i]);  // Print row as string: "GFE"
    }

    // Cleanup: free in REVERSE order of allocation
    free(p);  // Free dynamic array
    p = NULL; // Prevent dangling pointer
    for (unsigned char i = 0; i < 2; i++)
        free(pm[i]);  // Free each row
    free(pm);  // Free the pointer array
    pm = NULL; // Prevent dangling pointer

    // =====================================================================
    // PART 3: STRUCTS DEMO
    // =====================================================================
    printf("\n=== SEMINAR 3: STRUCTS ===\n\n");

    Employee emp_1;  // Stack-allocated Employee struct
    emp_1.id = 1001;                       // Set ID (2 bytes)
    emp_1.name = "Jack Smith";             // Point to string literal (read-only)
    emp_1.age = 29;                        // Set age (1 byte)
    emp_1.salary = (float)7450.99;         // Set salary with explicit cast
    strcpy(emp_1.hire_date, "11/07.2023"); // Copy date string into fixed array

    printf("Size of Employee: %d bytes\n", sizeof(Employee));  // 24 (with padding)

    Employee* pEmp = &emp_1;  // Pointer to struct
    printf("Name = %s, Age = %d\n", pEmp->name, pEmp->age);  // Access via ->

    // =====================================================================
    // PART 2: SIMPLE LINKED LIST (Seminar 4)
    // =====================================================================
    printf("\n=== SEMINAR 4: SIMPLE LINKED LIST ===\n\n");

    Node* first = NULL;  // Head pointer - NULL means empty list

    // Open the text file for reading
    // "r" = read mode, text file
    // fopen returns FILE* (pointer to file handle struct) or NULL on failure
    FILE* f = fopen("Employees.txt", "r");
    char buffer[256];        // Buffer to hold each line (max 255 chars + \0)
    char seps[] = ",\n";     // Delimiters: comma and newline

    // Read the file line by line
    // fgets reads until: newline, buffer full, or EOF
    // Returns NULL at EOF -> loop exits
    while (fgets(buffer, sizeof(buffer), f))
    {
        Employee empl;  // Temporary stack variable for this employee

        // Token 1: Employee ID
        // strtok splits buffer by delimiters (",\n")
        // FIRST call: pass the string to tokenize
        // strtok replaces the delimiter with '\0' and returns pointer to token
        char* token = strtok(buffer, seps);
        empl.id = atoi(token);  // Convert "1012" to integer 1012

        // Token 2: Employee Name
        // SUBSEQUENT calls: pass NULL to continue from last position
        // strtok remembers which string it was tokenizing (static internal state)
        token = strtok(NULL, seps);
        // MALLOC for the name string!
        // strlen(token) gives the length WITHOUT null terminator
        // +1 is for the '\0' byte that strcpy will copy
        // This is a HEAP allocation - MUST be freed later!
        empl.name = malloc(strlen(token) + 1);
        strcpy(empl.name, token);  // Copy the name string into allocated memory

        // Token 3: Salary
        token = strtok(NULL, seps);
        empl.salary = (float)atof(token);  // Convert "9654.35" to float

        // Token 4: Age
        token = strtok(NULL, seps);
        empl.age = atoi(token);  // Convert "28" to integer

        // Token 5: Hire Date
        token = strtok(NULL, seps);
        // hire_date is char[11] (fixed array in struct) - NO malloc needed
        // Just copy directly into the array
        strcpy(empl.hire_date, token);  // Copy "12/12/2024" into hire_date

        // Insert this employee into the singly linked list
        // Capture the return value (might be new head if list was empty)
        first = insertNodeEnd(first, empl);
    }
    fclose(f);  // Close the file - releases the file handle

    printf("Simple list after creation:\n");
    parseList(first);  // Print all employees (forward traversal)

    printf("\nAfter deleting below-average salary nodes:\n");
    first = deleteNodesBelowAverageSalary(first);  // Filter: keep only above-average
    parseList(first);

    printf("\nAfter deleting one node from end:\n");
    first = deleteNodeEnd(first);  // Remove last node
    parseList(first);

    // Complete deallocation: remove nodes one by one until empty
    while (first != NULL)
        first = deleteNodeEnd(first);
    printf("\nList after full deallocation:\n");
    parseList(first);  // Prints nothing (list is empty)

    // =====================================================================
    // PART 3: DOUBLY LINKED LIST (Seminar 5)
    // =====================================================================
    printf("\n=== SEMINAR 5: DOUBLY LINKED LIST ===\n\n");

    // Initialize empty doubly linked list
    DoubleList DList;
    DList.first = DList.last = NULL;  // Both endpoints NULL = empty list

    f = fopen("Employees.txt", "r");  // Re-open the file for reading

    // Same file parsing pattern as singly linked list
    while (fgets(buffer, sizeof(buffer), f))
    {
        Employee empl;
        char* token = strtok(buffer, seps);
        empl.id = atoi(token);

        token = strtok(NULL, seps);
        empl.name = malloc(strlen(token) + 1);
        strcpy(empl.name, token);

        token = strtok(NULL, seps);
        empl.salary = (float)atof(token);

        token = strtok(NULL, seps);
        empl.age = atoi(token);

        token = strtok(NULL, seps);
        strcpy(empl.hire_date, token);

        // Insert into doubly linked list
        // DList is passed by value (struct copy), return value captures modifications
        DList = insertDNodeEnd(DList, empl);
    }
    fclose(f);

    printf("Double list after creation:\n");
    parseDList(DList);  // Prints both directions: first->last AND last->first

    DList = deleteDNodeEnd(DList);  // Remove last node (O(1) operation!)
    printf("\nAfter deleting last node:\n");
    parseDList(DList);

    // Delete all remaining nodes
    while (DList.last != NULL)
        DList = deleteDNodeEnd(DList);
    printf("\nList after full deallocation:\n");
    parseDList(DList);  // Prints nothing (list is empty)

    // =====================================================================
    // PART 4: HASH TABLE WITH CHAINING (Seminar 6)
    // =====================================================================
    printf("\n=== SEMINAR 6: HASH TABLE WITH CHAINING ===\n\n");

    // Hash table: array of Node* pointers
    Node** HashTable = NULL;  // Double pointer (pointer to pointer to Node)
    unsigned short int size_hashTable = 50;  // 50 buckets

    // STEP 1: Allocate the array of pointers
    // 50 * 4 = 200 bytes on x86 (50 pointers, each 4 bytes)
    HashTable = malloc(size_hashTable * sizeof(Node*));

    // STEP 2: Initialize ALL buckets to NULL (empty lists)
    // malloc does NOT zero memory - must explicitly set each bucket to NULL
    for (unsigned short int i = 0; i < size_hashTable; i++)
        HashTable[i] = NULL;  // Mark bucket i as empty

    f = fopen("Employees.txt", "r");  // Re-open the file

    // Same file parsing pattern
    while (fgets(buffer, sizeof(buffer), f))
    {
        Employee empl;
        char* token = strtok(buffer, seps);
        empl.id = atoi(token);

        token = strtok(NULL, seps);
        empl.name = malloc(strlen(token) + 1);
        strcpy(empl.name, token);

        token = strtok(NULL, seps);
        empl.salary = (float)atof(token);

        token = strtok(NULL, seps);
        empl.age = atoi(token);

        token = strtok(NULL, seps);
        strcpy(empl.hire_date, token);

        // Insert into hash table using chaining
        // Computes hash of empl.id, inserts into the corresponding bucket's linked list
        insertEmployeeHashTable(HashTable, size_hashTable, empl);
    }
    fclose(f);

    // Print entire hash table (skip empty buckets)
    printf("Content of entire Hash Table:\n");
    for (unsigned short int i = 0; i < size_hashTable; i++)
    {
        // Only print non-empty buckets
        if (HashTable[i] != NULL)
        {
            printf("Employees in bucket %d:\n", i);
            Node* t = HashTable[i];  // Head of this bucket's linked list
            while (t)
            {
                printf("  %d %s\n", t->emp.id, t->emp.name);
                t = t->next;  // Traverse the chain
            }
        }
    }

    // Search for a specific employee by ID
    // searchEmployee returns Employee* or NULL
    Employee* found = searchEmployee(HashTable, size_hashTable, 2012);
    printf("\nSearch for id=2012: ");
    if (found != NULL)
        // Found! Print the employee's details
        // found points to the Employee INSIDE the Node in the hash table
        printf("Found: %d %s\n", found->id, found->name);
    else
        // Not found: either ID doesn't exist or hashing put it in an empty bucket
        printf("Not found\n");

    // =====================================================================
    // PART 5: BINARY SEARCH TREE (Seminar 7)
    // =====================================================================
    printf("\n=== SEMINAR 7: BINARY SEARCH TREE ===\n\n");

    BSTNode* root = NULL;  // Root of the BST - NULL means empty tree

    // Re-open the file for reading
    f = fopen("Employees.txt", "r");

    while (fgets(buffer, sizeof(buffer), f))
    {
        Employee empl;
        char* token = strtok(buffer, seps);
        empl.id = atoi(token);

        token = strtok(NULL, seps);
        empl.name = malloc(strlen(token) + 1);
        strcpy(empl.name, token);

        token = strtok(NULL, seps);
        empl.salary = (float)atof(token);

        token = strtok(NULL, seps);
        empl.age = atoi(token);

        token = strtok(NULL, seps);
        strcpy(empl.hire_date, token);

        // Insert into BST - flag tells us if insertion succeeded
        unsigned char insert;
        root = insertEmployeeBST(root, empl, &insert);

        if (insert != 0)
        {
            // Insertion succeeded - a new node was created
            printf("Successful insertion of employee: %d\n", empl.id);
        }
        else
        {
            // Duplicate ID rejected - employee already in BST
            // MUST free empl.name because the node was NOT created
            // Without this free, the malloc'd string is leaked
            printf("Employee with id %d already exists - skipping\n", empl.id);
            free(empl.name);
        }
    }
    fclose(f);

    // Print BST content using IN-ORDER traversal (gives sorted output by ID)
    printf("\nContent of the BST:\n");
    BSTParse(root);

    // SEARCH for a specific employee by ID
    // Returns Employee* or NULL
    Employee* pEmp = searchEmployeeBST(root, 849);
    printf("\n/// SEARCH operation ///\n");
    if (pEmp != NULL)
    {
        printf("Employee data: %d %s\n", pEmp->id, pEmp->name);
    }
    else
    {
        printf("There is no employee having the specified id in search operation\n");
    }

    // DELETE a node from the BST
    // deleteNodeBST extracts the employee data into delete_empl before removing
    Employee delete_empl;
    delete_empl.name = NULL;  // Initialize to detect successful extraction
    root = deleteNodeBST(root, 1012, &delete_empl);

    if (delete_empl.name != NULL)
    {
        // Employee was found and extracted
        printf("\nExtracted employee data: %d %s\n", delete_empl.id, delete_empl.name);
        free(delete_empl.name);  // Caller must free the extracted name
    }
    else
    {
        // Employee with this ID does not exist
        printf("\nThere is no employee having the specified search key.\n");
    }

    // Print BST after deletion
    printf("\nContent of BST after deletion:\n");
    BSTParse(root);

    // DEALLOCATE the entire BST (post-order traversal)
    root = deallocateBST(root);
    printf("\nContent of BST after deallocation:\n");
    BSTParse(root);  // Prints nothing - tree is destroyed

    // =====================================================================
    // PART 6: MAX HEAP (Seminar 8)
    // =====================================================================
    printf("\n=== SEMINAR 8: MAX HEAP STRUCTURE (Employee by salary) ===\n\n");

#define DIM 20  // Initial capacity, increases by DIM when full

    Employee* heapEmployees = NULL;  // Dynamic array of Employee structs
    int capacitate_stocare = DIM;    // Current storage capacity
    int nKeys = 0;                   // Current number of employees in heap

    // STEP 1: Allocate initial heap array
    heapEmployees = (Employee*)malloc(DIM * sizeof(Employee));

    // STEP 2: Read employees from file and insert into heap by salary
    f = fopen("Employees.txt", "r");
    while (fgets(buffer, sizeof(buffer), f))
    {
        Employee empl;
        char* token = strtok(buffer, seps);
        empl.id = atoi(token);

        token = strtok(NULL, seps);
        empl.name = malloc(strlen(token) + 1);
        strcpy(empl.name, token);

        token = strtok(NULL, seps);
        empl.salary = (float)atof(token);

        token = strtok(NULL, seps);
        empl.age = atoi(token);

        token = strtok(NULL, seps);
        strcpy(empl.hire_date, token);

        // CHECK: Is the array full? Need to resize?
        if (nKeys == capacitate_stocare)
        {
            // DYNAMIC RESIZING: allocate larger array
            capacitate_stocare += DIM;

            // Allocate new, bigger array
            Employee* new_heap = (Employee*)malloc(capacitate_stocare * sizeof(Employee));

            // Copy all existing employees to the new array
            for (int i = 0; i < nKeys; i++)
                new_heap[i] = heapEmployees[i];

            // Free the old, smaller array
            free(heapEmployees);

            // Switch pointer to the new, larger array
            heapEmployees = new_heap;
        }

        // Insert the employee into the heap (bubbles up by salary)
        insertEmployeeHeap(heapEmployees, &nKeys, empl);
    }
    fclose(f);

    // STEP 3: Print the initial heap (array representation, level-order)
    printf("Initial Heap (ordered by salary - max at root):\n");
    for (int i = 0; i < nKeys; i++)
        printf("  [%d] %s - $%.2f\n", heapEmployees[i].id, heapEmployees[i].name, heapEmployees[i].salary);

    // STEP 4: Insert a new employee into the heap
    Employee newEmp;
    newEmp.id = 3001;
    newEmp.name = malloc(strlen("Test Employee") + 1);
    strcpy(newEmp.name, "Test Employee");
    newEmp.salary = 5000.00;
    newEmp.age = 35;
    strcpy(newEmp.hire_date, "15/05/2025");
    insertEmployeeHeap(heapEmployees, &nKeys, newEmp);
    printf("\nAfter inserting Test Employee ($5000.00):\n");
    for (int i = 0; i < nKeys; i++)
        printf("  [%d] %s - $%.2f\n", heapEmployees[i].id, heapEmployees[i].name, heapEmployees[i].salary);

    // STEP 5: Delete the root (highest salary) employee
    Employee deleted = deleteEmployeeHeap(heapEmployees, &nKeys);
    printf("\nDeleted highest salary employee: [%d] %s - $%.2f\n", deleted.id, deleted.name, deleted.salary);
    free(deleted.name);  // Caller owns the returned employee's name

    printf("\nHeap after deletion:\n");
    for (int i = 0; i < nKeys; i++)
        printf("  [%d] %s - $%.2f\n", heapEmployees[i].id, heapEmployees[i].name, heapEmployees[i].salary);

    // STEP 6: Delete ALL remaining employees (extract in descending salary order)
    printf("\nAll employees extracted by salary (highest to lowest):\n");
    while (nKeys > 0)
    {
        deleted = deleteEmployeeHeap(heapEmployees, &nKeys);
        printf("  [%d] %s - $%.2f\n", deleted.id, deleted.name, deleted.salary);
        free(deleted.name);  // Free each extracted employee's name
    }

    // STEP 7: Free the heap array
    free(heapEmployees);

    return 0;  // Exit program - OS reclaims all remaining heap memory
               // (but in production code, you should always free everything!)
}
