/**
 * @file eFile.h
 * @brief eFile Header
 * @author ALARY Dorian
 * @version 1.0
 * @date 21/07/2024
 * @copyright GNU Public License.
 *
 */

#ifndef __EFILE_H__
#define __EFILE_H__

#include "eLine.h"
#include "util.h"
#include <stdbool.h>


/**
 * @struct eFile structure to reprensent a file in memory
 */
typedef struct efile_s
{
    /** File permissions */
    PERM permissions;

    /** Number of line */
    unsigned int n_elines;

    /** Current pos in current line */
    unsigned int current_pos;

    /** First line of eLine linked list */
    eLine * first_file_line;

    /** First line of screen */
    eLine * first_screen_line;

    /** Current line */
    eLine * current_line;

    /** Filename */
    char * filename;

    /** Path + '/' + filename */
    char * realpath;

    /** boolean to track status of file */
    bool is_saved;

} eFile;


/**
 * @brief The create_eFile() function allocate an eFile structure but do
 *        not open or allocate lines.
 *
 * @param realpath: Path of the file
 *
 * @return eFile pointer or NULL if it was an error, see logs.
 *
 * @note delete_eFile() must be called before exiting.
 */
eFile * create_eFile(char const * realpath);


/**
 * @brief The delete_eFile() delete and deallocate eFile and set the
 *        pointer to NULL.
 *
 * @param efile: eFile pointer pointer
 */
void delete_eFile(eFile ** efile);


/**
 * @brief The open_eFile() function open the file and initialize lines.
 *
 * @param efile: eFile pointer
 *
 * @note close_eFile() must be called to deallocate lines. delete_eFile()
 *       also call close_eFile(). If the file did not exist when calling
 *       create_eFile(), open_eFile write a new file.
 */
int open_eFile(eFile * efile);


/**
 * @brief The close_eFile() function close the file and deallocate eLines.
 *
 */
void close_eFile(eFile *efile);


/**
 * @brief The write_eFile() function write the content of eFile on the
 *        file designed by filename stored in the filename attribute.
 *
 * @param efile eFile pointer
 *
 * @return 0 on sucess or -1 in failure.
 */
int write_eFile(eFile *efile);


/**
 * @brief The add_empty_line_eFile() add an empty line to the position
 *        pos in the file.
 *
 * @param efile: eFile pointer pointer
 * @param line_number: Position of the new line in the file
 *
 * @return 0 on success or -1 in failure.
 */
int add_empty_line_eFile(eFile * efile,
                         unsigned int line_number);


/**
 * @brief The delete_line_eFile() delete a line to the position pos in
 *        the file.
 *
 * @param efile: eFile pointer pointer
 * @param line_number: Line number of the line that will be deleted
 *
 * @return 0 on sucess or -1 in failure.
 */
int delete_line_eFile(eFile * efile,
                      unsigned int line_number);


/**
 * @brief The insert_char_eFile() insert a character to the current
 *        position in the current_line.
 *
 * @param efile: eFile pointer
 * @param ch: character to insert
 *
 * @return 0 on sucess or -1 in failure.
 */
int insert_char_eFile(eFile * efile,
                      char const ch);


/**
 * @brief The remove_char_eFile() remove a character to the current
 *        position in the current_line.
 *
 * @param efile: eFile pointer
 *
 * @return 0 on sucess or -1 in failure.
 */
int remove_char_eFile(eFile * efile);


/**
 * @brief The insert_string_eFile() insert the string at current_pos in
 *        the current line.
 *
 * @param efile: eFile pointer
 * @param string: string to insert
 * @param length: lenght of string
 *
 * @return 0 on sucess or -1 in failure.
 */
int insert_string_eFile(eFile * efile,
                        char const * string,
                        size_t length);


/**
 * @brief The remove_string_eFile() remove the string that start at
 *        current_pos and end to current_pos+length in the current line.
 *
 * @param efile: eFile pointer
 * @param length: of string to remove
 *
 * @return 0 on sucess or -1 in failure.
 */
int remove_string_eFile(eFile *efile,
                        size_t length);

#endif
