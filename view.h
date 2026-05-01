#ifndef VIEW_H
#define VIEW_H

#include "common.h"           // to include the user defined data types

/* function to check if the type of file is mp3 */
Status check_vfile_type(char *argv[], mp3File *vFile);

/* function to view the file tag details */
Status view_tag_details(mp3File *vFile);

/* function to read all the tags and details */
Status read_file_tags(mp3File *vFile);

/* function to read the details of each tag */
Status read_tag_data(mp3Tag *vTags, mp3File *vFile);

/* function to print the tag details */
Status print_tag_data(mp3Tag *vTags);

#endif