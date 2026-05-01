#ifndef EDIT_H
#define EDIT_H

#include "common.h"

/* function to check if the type of file is mp3 */
Status check_efile_type(char *argv[], mp3File *vFile);

/* main function to edit the tag data */
Status edit_tag_details(mp3File *eFile, char opt, char *new_text);

/* function to find the tag, edit and copy */
Status edit_copy_tag(mp3File *eFile, mp3File *dupe, char opt, char *new_text);

#endif