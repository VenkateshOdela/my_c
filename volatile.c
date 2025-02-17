void main()
{
    //int victim =3;

    volatile int victim =4;

    for(int i=0; i<10; i++)
    {
        victim++;
    }
}

/*
void main()
{
    int victim =3;

    //volatile int victim =4;

    for(int i=0; i<10; i++)
    {
        victim++;
    }
}

@VenkateshOdela ➜ /workspaces/my_c (main) $ cat volatile.S
cat: volatile.S: No such file or directory
@VenkateshOdela ➜ /workspaces/my_c (main) $ cat volatile.
volatile.c  volatile.s  
@VenkateshOdela ➜ /workspaces/my_c (main) $ cat volatile.
volatile.c  volatile.s  
@VenkateshOdela ➜ /workspaces/my_c (main) $ cat volatile.s
        .file   "volatile.c"
        .option nopic
        .attribute arch, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
        .attribute unaligned_access, 0
        .attribute stack_align, 16
        .text
        .align  1
        .globl  main
        .type   main, @function
main:
        ret
        .size   main, .-main
        .ident  "GCC: () 9.3.0"

*/

/*
void main()
{
    //int victim =3;

    volatile int victim =4;

    for(int i=0; i<10; i++)
    {
        victim++;
    }
}

@VenkateshOdela ➜ /workspaces/my_c (main) $ cat volatile.s
        .file   "volatile.c"
        .option nopic
        .attribute arch, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
        .attribute unaligned_access, 0
        .attribute stack_align, 16
        .text
        .align  1
        .globl  main
        .type   main, @function
main:
        addi    sp,sp,-16
        li      a5,4
        sw      a5,12(sp)
        li      a4,10
.L2:
        lw      a5,12(sp)
        addiw   a5,a5,1
        sw      a5,12(sp)
        addiw   a4,a4,-1
        bnez    a4,.L2
        addi    sp,sp,16
        jr      ra
        .size   main, .-main
        .ident  "GCC: () 9.3.0"

*/

/*
The volatile keyword in C is used to tell the compiler that a variable can change at any time without any action being taken by the code itself. 
This prevents the compiler from optimizing out reads and writes to that variable. Let’s analyze your two cases in detail.

Case 1: Without volatile

void main()
{
    int victim = 3;

    for(int i = 0; i < 10; i++)
    {
        victim++;
    }
}
Generated Assembly (volatile.s without volatile):
assembly
.text
.align  1
.globl  main
.type   main, @function
main:
    ret
.size   main, .-main
.ident  "GCC: () 9.3.0"

Explanation:
The compiler completely optimized out the victim variable and the loop.
This is because victim is a local variable and is not used after the loop. Since it has no observable effect, the compiler assumes it is useless and removes it entirely.
The generated assembly contains only a ret (return) instruction, meaning the function does nothing.

Case 2: With volatile


void main()
{
    volatile int victim = 4;

    for(int i = 0; i < 10; i++)
    {
        victim++;
    }
}

Generated Assembly (volatile.s with volatile):
assembly

.text
.align  1
.globl  main
.type   main, @function
main:
    addi    sp, sp, -16  # Allocate stack space
    li      a5, 4        # Load immediate value 4 into a5
    sw      a5, 12(sp)   # Store victim at stack location
    li      a4, 10       # Load loop counter (10)

.L2:
    lw      a5, 12(sp)   # Load victim from memory
    addiw   a5, a5, 1    # Increment victim
    sw      a5, 12(sp)   # Store updated victim back to memory
    addiw   a4, a4, -1   # Decrement loop counter
    bnez    a4, .L2      # If not zero, loop again

    addi    sp, sp, 16   # Restore stack
    jr      ra           # Return
.size   main, .-main
.ident  "GCC: () 9.3.0"

Explanation:
The loop and victim updates are preserved.
The compiler does not optimize out victim, because volatile tells the compiler that victim might change unexpectedly (e.g., due to hardware or external factors).
Every read and write to victim actually occurs in memory, rather than being optimized into a register.
The loop is fully implemented in assembly, meaning victim is updated and stored every iteration.

Key Takeaways
Scenario	        Optimized Out?	    Reads/Writes to Memory?	        Why?
Without volatile	✅ (Yes)	            ❌ (No)	                    Compiler removes unused code.
With volatile	    ❌ (No)	            ✅ (Yes)	                    volatile forces memory access.


When to Use volatile?
Memory-mapped hardware registers (e.g., device drivers, embedded systems).
Global variables modified by an interrupt service routine (ISR).
Shared memory in multi-threaded programming where a variable is modified by another thread.
Using volatile prevents unintended optimizations that could lead to unexpected behavior in such cases.
*/
