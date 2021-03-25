
#include <Windows.h>
#include "wimlib.h"
#include "DismApi.h"
#include "dism.h"

#pragma comment(lib, "dismapi.lib")
#pragma comment(lib, "libwim.lib")







static enum wimlib_progress_status write_progress(enum wimlib_progress_msg msg, union wimlib_progress_info* info, void* progctx) {

	Dism* dism = (Dism*)progctx;

	switch (msg) {
		case WIMLIB_PROGRESS_MSG_WRITE_STREAMS:
			dism->create_progress((int)TO_PERCENT(info->write_streams.completed_bytes, info->write_streams.total_bytes));
			break;
		default:
			break;
	}

	return WIMLIB_PROGRESS_STATUS_CONTINUE;
}


Dism* dism_new() {

	Dism* dism = new Dism();

	dism->status = 0;

	//
	dism->wim = NULL;
	int check = wimlib_create_new_wim(WIMLIB_COMPRESSION_TYPE_LZX, &dism->wim);
	if (check != 0) {
		dism->status = -1;
	}

	//
	HRESULT hr = S_OK; 
	DismLogLevel Level = DismLogErrorsWarningsInfo;
	hr = DismInitialize(Level, NULL, NULL);
	if (FAILED(hr)) {
		dism->status = -1;
	}

	return dism;
}


int dism_create_image(Dism *dism, const wchar_t *input_dir, const wchar_t* output_file) {

	wimlib_register_progress_function(dism->wim, write_progress, (void *)dism);

	int status = 0;
	int check = 0;

	// 
	check = wimlib_add_image(dism->wim, input_dir, NULL, NULL, 0);
	if (check != 0) {

		status = -1;
	}

	// 
	check = wimlib_write(dism->wim, output_file, WIMLIB_ALL_IMAGES, 0, 0);
	if (check != 0) {

		status = -1;
	}

	return status;
}


void dism_close(Dism *dism) {

	// 
	wimlib_free(dism->wim);
	wimlib_global_cleanup();

	//
	DismShutdown();

	// 
	delete dism;
}


void dism_cleanup_mounts() {

	DismCleanupMountpoints();
}


void mountDismProgressCallback(UINT Current, UINT Total, PVOID UserData) {

	Dism* dism = (Dism*)UserData;
	dism->mount_progress(Current);
}


void umountDismProgressCallback(UINT Current, UINT Total, PVOID UserData) {

	Dism* dism = (Dism*)UserData;
	dism->umount_progress(Current);
}


int dism_mount_image(Dism *dism, const wchar_t *input_file, const wchar_t *mnt_dir) {

	int status = 0;
	HRESULT hr = S_OK;

	hr = DismMountImage(input_file, mnt_dir, 1, NULL, DismImageIndex, DISM_MOUNT_READWRITE, NULL, mountDismProgressCallback, (void *)dism);
	if (FAILED(hr)) {

		status = -1;
	}

	return status;
}


int dism_umount_image(Dism* dism, DWORD flag, const wchar_t* mnt_dir) {

	int status = 0;
	HRESULT hr = S_OK;

	hr = DismUnmountImage(mnt_dir, flag, NULL, umountDismProgressCallback, (void*)dism);
	if (FAILED(hr)) {

		status = -1;
	}

	return status;
}


int dism_remount_image(Dism* dism, const wchar_t* mnt_dir) {

	int status = 0;
	HRESULT hr = S_OK;

	hr = DismRemountImage(mnt_dir);
	if (FAILED(hr)) {

		status = -1;
	}

	return status;
}