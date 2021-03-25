#ifndef DISM_H
#define DISM_H

#ifdef __cplusplus

#endif

#ifdef __cplusplus
extern "C" {
#endif




#include <Windows.h>
#include "wimlib.h"


#define DISM_SAVE 0x00000000
#define DISM_DISCARD 0x00000001


typedef struct Dism_ Dism;

struct Dism_ {

	int status;

	WIMStruct* wim;

	void (*create_progress)(int);

	void (*mount_progress)(int);

	void (*umount_progress)(int);
};


#define TO_PERCENT(numerator, denominator) \
	((float)(((denominator) == 0) ? 0 : ((numerator) * 100 / (float)(denominator))))


__declspec(dllexport) Dism* dism_new();

__declspec(dllexport) int dism_create_image(Dism* dism, const wchar_t* input_dir, const wchar_t* output_file);

__declspec(dllexport) void dism_close(Dism* dism);

__declspec(dllexport) void dism_cleanup_mounts();

__declspec(dllexport) int dism_mount_image(Dism* dism, const wchar_t* input_file, const wchar_t* mnt_dir);

__declspec(dllexport) int dism_umount_image(Dism* dism, DWORD flag, const wchar_t* mnt_dir);

__declspec(dllexport) int dism_remount_image(Dism* dism, const wchar_t* mnt_dir);



#ifdef __cplusplus
}
#endif

#endif