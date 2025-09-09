#ifndef MODULE_LOADER_H
#define MODULE_LOADER_H

int load_module_from_initrd(const char* name);
int load_kernel_modules_from_initrd(void);

#endif
