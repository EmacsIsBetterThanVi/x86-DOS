# x86-DOSMAKE
A build system for x86-DOS, uses .x86mk as it's file extention and the command for using it is <br/>
*x86-DOSMAKE.bin SUPERTARGET.SUBTARGET*<br/>
# SYNTAX
Uses shell syntax however build instructions have a @ at the begining, with SUBTARGETS formated as NAME:
# COMMANDS
@default: SUBTARGET; Sets what just passing a SUPERTARGET does, skiping everything before the SUBTARGET, also sets when to branch based on SUBTARGET<br/>
@def NAME VALUE; Defines a variable <br/>
@sysdef NAME VALUE; Defines a system variable (AUTO PASSED)<br/>
@return; Returns from the current context, reqired to exit<br/>
@target SUBTARGET; Calls the SUBTARGET<br/>
@changed: FILENAME; If the given file is unchanged then return<br/>
@ramfile NAME; Creates a RAM File, or in other words, a registered shared memory address instead of writing to the disk. Name is RAM_NAME <br/>
@loaddefs FILENAME; Loads FILENAME as key:value pairs of variables(DOES NOT INCLUDE AUTO PASSED) <br/>
@undef NAME; deletes a variable<br/>
@delram NAME; deletes a RAM file caldded RAM_NAME