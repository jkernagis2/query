//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                            d_grep.c                                  //
//  Includes and declarations for the d_grep.c file.                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef D_GREP_H
#define D_GREP_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/************************************************************************/
/*                                                                      */
/*  Name:     d_grep                                                    */
/*  Inputs:   Buffer with a grep command.                               */
/*            Machine ID                                                */
/*  Output:   None                                                      */
/*  Function: Calls the system's gawk function to search machine.i.log. */
/*                                                                      */
/************************************************************************/
void d_grep(char* command_buffer, int machine_num); 

/************************************************************************/
/*                                                                      */
/*  Name:     combine                                                   */
/*  Inputs:   None.                                                     */
/*  Output:   None.                                                     */
/*  Function: Combines the result1.tmp through results4.tmp if present  */
/*              into a single file. Also deletes the temp files when    */
/*              done with them.                                         */
/*                                                                      */
/************************************************************************/
void combine();

#endif // D_GREP_H
