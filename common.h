#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

/* status to be used as function return type */
typedef enum
{
    success,
    failure
}Status;

typedef enum
{
    view,
    edit,
    help,
    error
}OperationType;

/* contains info of the original file*/
typedef struct _MP3File
{
    /* source mp3 info */
    char *file_mp3;
    FILE *fptr_mp3;

    /* source file header info */
    char id3_tag[11];

}mp3File;

/* contains structures to store the details of the mp3 file tags */

typedef struct _MP3Tag
{
    /* tag details */
    int tag_no;
    char tag_type[5];
    int tag_size;
    char tag_data[500];
}mp3Tag;

/* check operation type */
OperationType check_opr_type(char *argv[]);

/* check the type of tag to edit */
Status check_edit_type(char *argv[]);

/* print error message with usage info */
void usage_msg(void);

/* print help menu */
void help_menu(void);

/* function to open the original file */
Status open_file(mp3File *mp3File);

/* function to close original file */
Status close_file(mp3File *mp3File);

/* function to check for id3 tags */
Status check_id3_version(mp3File *mp3File);

/* function to open the duplicate file */
Status open_dupe(mp3File *dupe);

/* function to read each tag */
Status read_tag(mp3Tag *vTags, mp3File *vFile);

/* function to read the data for each tag */
Status read_tag_size(mp3Tag *vTags, mp3File *vFile);
#endif