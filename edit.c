#include <string.h>
#include "common.h"
#include "edit.h"
#include "view.h"

Status check_efile_type(char *argv[], mp3File *contxt)
{
    if (argv[4] && strstr(argv[4], ".mp3") != NULL)
    {
        contxt->file_mp3 = argv[4];
        return success;
    }
    return failure;
}

Status edit_copy_tag(mp3File *eFile, mp3File *dupe, char opt, char *new_text)
{
    char target[5];
    int edit_comm = 0;
    
    switch (opt)
    {
    case 't': strcpy(target, "TIT2"); break;
    case 'a': strcpy(target, "TPE1"); break;
    case 'A': strcpy(target, "TALB"); break;
    case 'y': strcpy(target, "TYER"); break;
    case 'm': strcpy(target, "TCON"); break;
    case 'c': strcpy(target, "COMM"); break;
    default:
        return failure;
    }

    /* copy ID3 header */
    fwrite(eFile->id3_tag, 1, 10, dupe->fptr_mp3);
    while (1)
    {
        mp3Tag tag;
        /* to rewind the file pointer to start of the frame */
        long frame_pos = ftell(eFile->fptr_mp3);
        /* read tag */
        if (read_tag(&tag, eFile) == failure)
            break;

        if (read_tag_size(&tag, eFile) == failure)
            return failure;

        /* read 2 byte of flags */
        unsigned char flags[2];
        if (fread(flags, 1, 2, eFile->fptr_mp3) != 2)
            return failure;

        /* COMM frame edit */
        if (strcmp(tag.tag_type, "COMM") == 0)
        {
            int new_size = 1 + 3 + 1 + strlen(new_text); /* enc + lang + desc + text */

            fwrite("COMM", 1, 4, dupe->fptr_mp3);

            unsigned char b[4];
            b[0] = (new_size >> 24) & 0xFF;
            b[1] = (new_size >> 16) & 0xFF;
            b[2] = (new_size >> 8)  & 0xFF;
            b[3] =  new_size        & 0xFF;
            fwrite(b, 1, 4, dupe->fptr_mp3);

            /* frame flags */
            fputc(0x00, dupe->fptr_mp3);
            fputc(0x00, dupe->fptr_mp3);

            /* encoding */
            fputc(0x00, dupe->fptr_mp3);

            /* language */
            fwrite("eng", 1, 3, dupe->fptr_mp3);

            /* short description */
            fputc(0x00, dupe->fptr_mp3);

            /* comment text */
            fwrite(new_text, 1, strlen(new_text), dupe->fptr_mp3);

            /* skip old COMM data */
            fseek(eFile->fptr_mp3, tag.tag_size, SEEK_CUR);
        }
        
        else if (strcmp(tag.tag_type, target) == 0)
        {
            int new_size = strlen(new_text) + 1; /* +1 for encoding byte */
            /* write tag to dupe.mp3 */
            fwrite(tag.tag_type, 1, 4, dupe->fptr_mp3);

            /* write tag size to dupe.mp3 as big endian */
            unsigned char b[4];
            b[0] = (new_size >> 24) & 0xFF;
            b[1] = (new_size >> 16) & 0xFF;
            b[2] = (new_size >> 8) & 0xFF;
            b[3] = new_size & 0xFF;
            fwrite(b, 1, 4, dupe->fptr_mp3);

            /* write the flag bytes to dupe.mp3 */
            fputc(0x00, dupe->fptr_mp3);
            fputc(0x00, dupe->fptr_mp3);

            /* write encoding bytes */
            fputc(0x00, dupe->fptr_mp3);  
            /* write the new text to dupe.mp3 */
            fwrite(new_text, 1, strlen(new_text), dupe->fptr_mp3);

            /* move file pointer to end of text */
            fseek(eFile->fptr_mp3, tag.tag_size, SEEK_CUR);
        }
        else
        {
            /* rewind and copy */
            fseek(eFile->fptr_mp3, frame_pos, SEEK_SET);

            int total = 10 + tag.tag_size; /* header + data */
            unsigned char buf[1024];
            /* copy by chunks to avoid buffer overflow */
            while (total > 0)
            {
                int chunk = total > sizeof(buf) ? sizeof(buf) : total;
                if (fread(buf, 1, chunk, eFile->fptr_mp3) != chunk)
                    return failure;
                fwrite(buf, 1, chunk, dupe->fptr_mp3);
                total -= chunk;
            }
        }
    }
    /* copy audio */
    unsigned char buf[1024];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), eFile->fptr_mp3)) > 0)
        fwrite(buf, 1, n, dupe->fptr_mp3);

    return success;
}

Status edit_tag_details(mp3File *eFile, char opt, char *new_text)
{
    /* validate the new text */
    if (!new_text || *new_text == '\0')
        return failure;

    /* open original file in read mode */
    if (open_file(eFile) == failure)
        return failure;

    /* validate id3 version */
    if (check_id3_version(eFile) == failure)
    {
        close_file(eFile);
        return failure;
    }

    /* create new duplicate file to store the edit */
    mp3File dupe;
    /* open file in write mode */
    if (open_dupe(&dupe) == failure)
    {
        close_file(eFile);
        return failure;
    }

    if (edit_copy_tag(eFile, &dupe, opt, new_text) == failure)
    {
        close_file(eFile);
        close_file(&dupe);
        return failure;
    }
    /* close files */
    close_file(eFile);
    close_file(&dupe);

    /* delete original file and rename duplicate file with orignial file name */
    remove(eFile->file_mp3);
    rename("dupe.mp3", eFile->file_mp3);

    /* create a new file pointer to edited file and call the view function to print info */
    mp3File viewFile;
    viewFile.file_mp3 = eFile->file_mp3;

    if (view_tag_details(&viewFile) == failure)
    {
        printf("ERROR: Unable to display updated tag details.\n");
    }
    return success;
}