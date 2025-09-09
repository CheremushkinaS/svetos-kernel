#include <kernel/fs.h>
#include <kernel/printk.h>
#include <string.h>

void test_fs(void) {
    printk("Testing filesystem...\n");
    
    // Create test file
    if(fs_create("test.txt", FS_FILE) != 0) {
        printk("Failed to create file\n");
        return;
    }
    
    // Write to file
    fs_node_t* file = fs_open("test.txt");
    if(!file) {
        printk("Failed to open file\n");
        return;
    }
    
    char* data = "Hello World!";
    size_t written = fs_write(file, data, strlen(data));
    if(written != strlen(data)) {
        printk("Failed to write to file\n");
        return;
    }
    
    // Read from file
    char buf[32];
    size_t read = fs_read(file, buf, sizeof(buf));
    if(read != strlen(data)) {
        printk("Failed to read from file\n");
        return;
    }
    buf[read] = '\0';
    printk("File content: %s\n", buf);
    
    fs_close(file);
    printk("Filesystem test passed!\n");
}
