#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
 
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

unsigned process_directory( char *theDir )
{
    DIR *dir = NULL;
    struct dirent entry;
    struct dirent *entryPtr = NULL;
    int retval = 0;
    unsigned count = 0;
    char pathName[PATH_MAX + 1];
 
    dir = opendir( theDir );
    if( dir == NULL ) {
        printf( "Error opening %s: %s", theDir, strerror( errno ) );
        return 0;
    }

    retval = readdir_r( dir, &entry, &entryPtr );
    while( entryPtr != NULL ) {
    struct stat entryInfo;
     
    if( ( strncmp( entry.d_name, ".", PATH_MAX ) == 0 ) ||
        ( strncmp( entry.d_name, "..", PATH_MAX ) == 0 ) ) {
        retval = readdir_r( dir, &entry, &entryPtr );
        continue;
    }
        (void)strncpy( pathName, theDir, PATH_MAX );
        (void)strncat( pathName, "/", PATH_MAX );
        (void)strncat( pathName, entry.d_name, PATH_MAX );
         
        if( lstat( pathName, &entryInfo ) == 0 ) {
            count++;
             
            if( S_ISDIR( entryInfo.st_mode ) ) {
                printf( "%s\n", pathName );
            }
        } else {
            printf( "Error statting %s: %s\n", pathName, strerror( errno ) );
        }
        retval = readdir_r( dir, &entry, &entryPtr );
    }
     
    (void)closedir( dir );
    return count;
}

int main( int argc, char **argv )
{
    int idx = 0;
    unsigned count = 0;
    process_directory("/home/lynx/test_fs");     
    
    return EXIT_SUCCESS;
}


