#include <stdio.h>

int main(){
	int swch;
	printf("\n 1. Show root directory.\n 2. Show file system tree\n 3. Show superblock info\n 4. Copy file.\n");
	while(1){
	scanf("%d", &swch);
	switch(swch){
		case 1:
			system("./RD");
		break;
		case 2:
			system("tree /home/lynx/test_fs");
		break;
		case 3:
			system("./SB");
		break;
		case 4:
			system("./copyFile /home/lynx/test_fs/image.jpg /home/lynx/test_fs/new_image.jpg rwx");
		break;
		default:
		break;
	}
	}
	return 0;
}
