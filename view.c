#include <string.h>
#include "common.h"
#include "view.h"

Status check_vfile_type(char *argv[], mp3File *contxt)
{
    if (argv[2] && strstr(argv[2], ".mp3") != NULL)
    {
        contxt->file_mp3 = argv[2];
        return success;
    }
    return failure;
}

/* read the details under the tag */
Status read_tag_data(mp3Tag *vTags, mp3File *vFile) 
{
    /* safety check */
    if (vTags->tag_size >= sizeof(vTags->tag_data))
    {
        /* skip oversized frame (eg: APIC)*/
        fseek(vFile->fptr_mp3, vTags->tag_size, SEEK_CUR);
        vTags->tag_data[0] = '\0';
        return success;
    }


    if (fread(vTags->tag_data, 1, vTags->tag_size, vFile->fptr_mp3) != vTags->tag_size)
    {
        return failure;
    }
    vTags->tag_data[vTags->tag_size] = '\0';
    return success;
}

/* print the tag details */
Status print_tag_data(mp3Tag *vTags)
{
    if (vTags->tag_no == 0)
    {
        return success;
    }
    // printf("Tag number: %d", vTags->tag_no);
    switch (vTags->tag_no)
    {
        case 1: printf("Title   : "); break;
        case 2: printf("Artist  : "); break;
        case 3: printf("Album   : "); break;
        case 4: printf("Year    : "); break;
        case 5: printf("Genre   : "); break;
        case 6: printf("Comment : "); break;
        default: return success;
    }

    /* for COMM tag */
    if (vTags->tag_no == 6)
    {
        int i = 0;

        /* encoding byte */
        i++;

        /* language (3 bytes) */
        i += 3;

        /* short description (null-terminated) */
        while (i < vTags->tag_size && vTags->tag_data[i] != '\0')
            i++;

        /* skip null terminator */
        i++;

        /* print actual comment */
        while (i < vTags->tag_size)
        {
            printf("%c", vTags->tag_data[i]);
            i++;
        }

        printf("\n");
        return success;
    }

    /* for other tags */
    /* skip encoding byte i = 1*/
    for (int i = 1; i < vTags->tag_size && vTags->tag_data[i] != '\0'; i++)
    {
        printf("%c", vTags->tag_data[i]);
    }
    printf("\n");

    return success;
}

/* function contains functions to get each tags and their data */
Status read_file_tags(mp3File *vFile)
{
    while (1)
    {
        mp3Tag vTags;
        /* read each tags */
        if (read_tag(&vTags, vFile) == failure)
        {
            break;
        }
        // printf("Tag no: %d\n", vTags->tag_no);

        /* read the size of data */
        if (read_tag_size(&vTags, vFile) == failure)
        {
            printf("ERROR: Unable to read the size\n");
            return failure;
        }
        // printf("Tag size = %d\n", vTags->tag_size);

        /* skip 2 flag bytes */
        if (fseek(vFile->fptr_mp3, 2, SEEK_CUR) != 0)
        {
            printf("ERROR: Unable to skip flags\n");
            return failure;
        }

        /* read the tag data */
        if (read_tag_data(&vTags, vFile) == failure)
        {
            printf("ERROR: Unable to read the tag data\n");
            return failure;
        }

        print_tag_data(&vTags);
    }
    return success;
}

Status view_tag_details(mp3File *vFile)
{
    printf("--------------------------------------------------------------------\n");
    printf("\t\tMP3 TAG READER AND EDITOR FOR  ID3V2\n");
    printf("--------------------------------------------------------------------\n");
    /* open the file */
    if (open_file(vFile) == failure)
    {
        printf("ERROR: Unable to open mp3 file\n");
        return failure;
    }

    /* check and skip the header (tag and version) */
    if (check_id3_version(vFile) == failure)
    {
        printf("ERROR: Unable to find id3 tags\n");
        close_file(vFile);
        return failure;
    }

    /* read all the tags and details */
    if (read_file_tags(vFile) == failure)
    {
        printf("ERROR: Unable to read the tag details\n");
        close_file(vFile);
        return failure;
    }

    return success;
}