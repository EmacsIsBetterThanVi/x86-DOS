//
//  x86-DOSCC.c
//  x86-DOS
//
//  Created by Raphael Wise on 5/14/24.
//

// obays the C99 standard, when applicable.
// register is gaurenteed to be the named register, and is used to avoid the need for a temporary variable, or for interacting with inline asm(should the register be unavailable as far as the compiler is concerned, then the contents will be pushed to the stack, and popped off when the function returns).
