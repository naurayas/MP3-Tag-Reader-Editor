#include <string.h>
#include "common.h"

Status open_file(mp3File *mp3File)
{
    mp3File->fptr_mp3 = fopen(mp3File->file_mp3, "rb");
    /* check for error */
    if (mp3File->fptr_mp3 == NULL)
    {
        return failure;
    }
    return success;
}

Status open_dupe(mp3File *dupe)
{
    dupe->fptr_mp3 = fopen("dupe.mp3", "wb");
    if (dupe->fptr_mp3 == NULL)
    {
        return failure;
    }
    return success;
}

Status close_file(mp3File *mp3File)
{
    if (mp3File->fptr_mp3)
    {
        fclose(mp3File->fptr_mp3);
        mp3File->fptr_mp3 = NULL;
    }
    return success;
}

Status check_id3_version(mp3File *mp3File)
{
    /* read first 10 bytes of header */
    if (fread(mp3File->id3_tag, 1, 10, mp3File->fptr_mp3) != 10)
    {
        //printf("Unable to read ID3\n");
        return failure;
    }
    if (mp3File->id3_tag[0] != 'I' || mp3File->id3_tag[1] != 'D' || mp3File->id3_tag[2] != 0x33)
    {
        //printf("ERROR: Not an ID3\n");
        return failure;
    }
    if (mp3File->id3_tag[3] != 0x03)
    {
        //printf("ERROR: Not ID3V2.3 version\n");
        return failure;
    }
    return success;
}

Status read_tag(mp3Tag *Tags, mp3File *mp3File)
{
    /* read 4 bytes of tag */
    if (fread(Tags->tag_type, 1, 4, mp3File->fptr_mp3) != 4)
    {
        return failure;
    }
    /* if there is padding, there are no more frames */
    if (Tags->tag_type[0] == 0)
    {
        return failure;
    }
    Tags->tag_type[4] = '\0';
    Tags->tag_no = 0;
    if (strcmp(Tags->tag_type, "TIT2") == 0)
    {
        Tags->tag_no = 1;
    }
    else if (strcmp(Tags->tag_type, "TPE1") == 0)
    {
        Tags->tag_no = 2;
    }
    else if (strcmp(Tags->tag_type, "TALB") == 0)
    {
        Tags->tag_no = 3;
    }
    else if (strcmp(Tags->tag_type, "TYER") == 0)
    {
        Tags->tag_no = 4;
    }
    else if (strcmp(Tags->tag_type, "TCON") == 0)
    {
        Tags->tag_no = 5;
    }
    else if (strcmp(Tags->tag_type, "COMM") == 0)
    {
        Tags->tag_no = 6;
    }
    return success;
}

/* read the 4 bytes of size of the data */
Status read_tag_size(mp3Tag *Tags, mp3File *mp3File)
{
    unsigned char size[4];
    if (fread(size, 1, 4, mp3File->fptr_mp3) != 4)
    {
        return failure;
    }
    /* convert the big endian to little endian */
    Tags->tag_size = (size[0] << 24) | (size[1] << 16) | (size[2] << 8) | size[3];
    return success;
}