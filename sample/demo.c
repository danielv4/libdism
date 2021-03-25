// gcc demo.c -o demo -L . -ldism -I include
#include <windows.h>
#include <dism.h>


void g_create_progress(int item) {

	printf("(create_progress) %d \n", item);
}


void g_mount_progress(int item) {

	printf("(mount_progress) %d \n", item);
}


void g_umount_progress(int item) {

	printf("(umount_progress) %d \n", item);
}


int main(int argc, char** argv) {

	int status;
	Dism* dism = dism_new();

	// callbacks
	dism->create_progress = &g_create_progress;
	dism->mount_progress = &g_mount_progress;
	dism->umount_progress = &g_umount_progress;


	// 
	status = dism_create_image(dism, L"C:\\Users\\testUser\\Desktop\\libmount\\test_dir", L"C:\\Users\\testUser\\Desktop\\libmount\\image9.wim");
	if (status == -1) {
	
		printf("dism_create_image err \n");
	}
	else {
	
		printf("dism_create_image OK \n");
	}


	status = dism_mount_image(dism, L"C:\\Users\\testUser\\Desktop\\libmount\\image9.wim", L"C:\\Users\\testUser\\Desktop\\libmount\\mnt9");
	if (status == -1) {

		printf("dism_mount_image err \n");
	}
	else {

		printf("dism_mount_image OK \n");
	}


	status = dism_umount_image(dism, DISM_SAVE, L"C:\\Users\\testUser\\Desktop\\libmount\\mnt9");
	if (status == -1) {

		printf("dism_umount_image err \n");
	}
	else {

		printf("dism_umount_image OK \n");
	}


	dism_close(dism);

	return 0;
}