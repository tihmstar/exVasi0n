

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#include <exVasi0n.h>

#include <libimobiledevice/afc.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/diagnostics_relay.h>


uint64_t gFh = 0;

char *udid;


idevice_t gDevice = NULL;
afc_client_t gAfc = NULL;
diagnostics_relay_client_t gDiag = NULL;
lockdownd_client_t gLockdown = NULL;

lockdownd_error_t lderr = 0;
lockdownd_service_descriptor_t port = NULL;
afc_error_t afcerr = 0;
diagnostics_relay_error_t diagerr = 0;


int afc_send_file(afc_client_t afc, const char* local, const char* remote) {
	FILE* fd = NULL;
	uint64_t fh = 0;
	afc_error_t err = 0;
	unsigned int got = 0;
	unsigned int gave = 0;
	unsigned char buffer[0x800];

	fd = fopen(local, "rb");
	if (fd != NULL ) {
		err = afc_file_open(afc, remote, AFC_FOPEN_WR, &fh);
		if (err == AFC_E_SUCCESS) {

			while (!feof(fd)) {
				memset(buffer, '\0', sizeof(buffer));
				got = fread(buffer, 1, sizeof(buffer), fd);
				if (got > 0) {
					afc_file_write(afc, fh, (const char*) buffer, got, &gave);
					if (gave != got) {
						printf("Error!!\n");
						break;
					}
				}
			}

			afc_file_close(afc, fh);
		}
		fclose(fd);
	} else
		return -1;
	return 0;
}


int deviceConnect() {
	idevice_error_t ideverr = 0;
	ideverr = idevice_new(&gDevice, NULL);
	if (ideverr != IDEVICE_E_SUCCESS) {
		return -1;
	}
	
	// grab UDID to show which device we're connected to.
	ideverr = idevice_get_udid(gDevice, &udid);
	if (ideverr != IDEVICE_E_SUCCESS) {
		printf("%s Unable to communicate with device.%s\n", KRED, KNRM);
	}
	
	printf(" UDID: %s!\n", udid);
	return 0;
}

int startLockdownd() {
	// start lockdownd client.
	printf(" lockdownd...\n");
	lderr = lockdownd_client_new_with_handshake(gDevice, &gLockdown, "exVasi0n");
	if (lderr != LOCKDOWN_E_SUCCESS) {
		printf("%s [*] Unable to connect to lockdownd. Please reboot your device and try again.%s\n", KRED, KNRM);
		return -1;
	}
	
	return 0;
}

int startAFC() {
	// start AFC service on lockdownd.
	printf(" afc service\n");
	lderr = lockdownd_start_service(gLockdown, "com.apple.afc", &port);
	if (lderr != LOCKDOWN_E_SUCCESS) {
		printf("%s afc service error%s\n", KRED, KNRM);
		return -1;
	}
	
	return 0;
}

int connectAFC() {
	// create an AFC client .
	printf(" afc client\n");
	afcerr = afc_client_new(gDevice, port, &gAfc);
	if (afcerr != AFC_E_SUCCESS) {
		printf("%s afc client error%s\n", KRED, KNRM);
		lockdownd_client_free(gLockdown);
		idevice_free(gDevice);
		return -1;
	}
	
	lockdownd_client_free(gLockdown);
	gLockdown = NULL;
	
	printf(" copying files\n");
	return 0;
}



int main(int argc, char *argv[]) {
	
	printf(" exVasi0n, tihmstar will pwn you :O\n");
    
    
	printf(" waiting for device\n");
	while (deviceConnect() != 0) {
		sleep(1);
	}
	printf(" device found!\n");
	
	// start lockdownd client.
	if (startLockdownd() != 0) {
		return -1;
	}
	
	// start AFC service on lockdownd.
	if (startAFC() != 0) {
		return -1;
	}
	
	// create an AFC client and connect to AFC service.
	if (connectAFC() != 0) {
		return -1;
	}
	
	afcerr = afc_make_directory(gAfc, "/evasi0n-install");
	if (afcerr != AFC_E_SUCCESS) {
		printf("%s Error creating dir %s\n\n", KRED, KNRM);
		afc_client_free(gAfc);
		idevice_free(gDevice);
		return -1;
	}
	
	afcerr = afc_send_file(gAfc, "mylittlesecret.tar", "evasi0n-install/Cydia.tar");
	if (afcerr != AFC_E_SUCCESS) {
		printf("%s Error putting file.%s\n\n", KRED, KNRM);
		return -1;
	}
	
	//reboot

	// start lockdownd client.
	printf(" lockdownd...\n");
	lderr = lockdownd_client_new_with_handshake(gDevice, &gLockdown, "exVasi0n");
	if (lderr != LOCKDOWN_E_SUCCESS) {
		printf("%s [*] Unable to connect to lockdownd. Please reboot your device and try again.%s\n", KRED, KNRM);
		return -1;
	}
	
	printf(" gonna reboot\n");
	lderr = lockdownd_start_service(gLockdown, "com.apple.mobile.diagnostics_relay", &port);
	if (lderr != LOCKDOWN_E_SUCCESS) {
		printf("%s diag service error%s\n", KRED, KNRM);
		return -1;
	}
	
	diagerr = diagnostics_relay_client_new(gDevice, port, &gDiag);
	if (diagerr != DIAGNOSTICS_RELAY_E_SUCCESS) {
		printf("%s diag client error %s %d\n", KRED, KNRM,diagerr);
		lockdownd_client_free(gLockdown);
		idevice_free(gDevice);
		return -1;
	}
	
	diagerr = diagnostics_relay_restart(gDiag, DIAGNOSTICS_RELAY_ACTION_FLAG_DISPLAY_PASS);
	if (diagerr != DIAGNOSTICS_RELAY_E_SUCCESS && diagerr != -2) {
        printf("%s reboot error, reboot manually %d %s\n", KNRM,diagerr, KNRM);
		lockdownd_client_free(gLockdown);
		idevice_free(gDevice);
		return -1;
	}
    
	
	printf(" done :P \n");
    // thanks a lot to DarkMalloc allowing me to use parts of his breakout JB code <3
	return 0;
	
	
}