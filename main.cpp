#include "mbed.h" //mbed-os 6.10 Nucleo-F429ZI
#include "KVStore.h"
#include "kvstore_global_api.h"
#include "FileSystemStore.h"

#define err_code(res) MBED_GET_ERROR_CODE(res)

char            key[] = {"key"};
size_t          actual_size = 0;
const size_t    buffer_size = 20;
char            buffer[buffer_size] = {};

KVStore::info_t info;

KVStore         *kvstore = nullptr;
FileSystem      *fs = nullptr;
BlockDevice     *bd = nullptr;
int res;

int main()
{
    printf("Example of KVStore with FileSystem\n");

    fs = FileSystem::get_default_instance();

    printf("Mounting...\n");    
    if((res = fs->mount(bd))!= 0) printf("fs_mount() error: %d\n", err_code(res));
    else printf("Mounted\n");
    /*if (res) {
        res = fs->reformat(bd);
        printf("Error: %d (Format)\n ", err_code(res));
    }*/

    kvstore = new FileSystemStore(fs);

    printf("kv_init()\n");
    if((res = kvstore->init())!=0) printf("kv_init error: %d (init)\n ", err_code(res));

    printf("kv_get_info of a key\n");
    if((res = kvstore->get_info(key, &info))!=0) printf("kv_get_info error: %d\n", err_code(res));
    else printf("kv_get_info key: %s\nkv_get_info info - size: %u, flags: %u\n", key, info.size, info.flags);

    // Press and hold user button and press reset with it, that will call reset and change of value
    if(DigitalIn(USER_BUTTON).read())
    {
        printf("kv_reset()\n");
        if ((res = kvstore->reset())!= 0) printf("kv_reset() error: %d\n", err_code(res));
        printf("Please enter a short string!\n");
        char str[10];
        scanf("%10s",str); 
        printf("kv_set() - %s\n", str);
        if ((res = kvstore->set(key, str, strlen(str), 0))!= 0) printf("kv_set() error: %d (set)\n ", err_code(res));
    }

    printf("kv_get()\n");
    if ((res = kvstore->get(key, buffer, buffer_size, &actual_size, 0))!= 0) printf("kv_get() error: %d\n", err_code(res));
    else printf("kv_get() - data: %s\n",  buffer);
    
    printf("Example end!\r\n");
}

