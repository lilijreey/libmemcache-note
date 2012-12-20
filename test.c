/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Created:  12/20/2012 01:23:43 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libmemcached/memcached.h>

//more see libmemcached(1)
typedef struct tt_s {
    int age;
    int id;
} tt_t ;

int main() {

    //memcached_t 时client handle
    //memcached_return_t 返回的消息类型
    memcached_st *memcli = NULL;

    //EE create a connect
    memcli = memcached_create(NULL); //NULL 为你分配空间
    if (memcli == NULL) {
        perror("memcached_create failed");
        exit(1);
    }

    memcached_return_t rtmsg ;
    //EE connecting to server
    //memcached_server_st server handle
    //connect memcache server
    //加入serverlist
    //port 
    memcached_server_st * memser = memcached_server_list_append(NULL, "127.0.0.1", 11211, &rtmsg);
    if (memser == NULL || rtmsg != MEMCACHED_SUCCESS) {
        perror("memcached_server_list_append failed");
        exit(1);
    }

    //connect client and server
	rtmsg = memcached_server_push(memcli, memser);
    if (rtmsg != MEMCACHED_SUCCESS) {
        printf(" memcached_server_push faled: %s\n", 
               memcached_strerror(memcli, rtmsg));
        exit(1);
    }

    //Add v-k to server
    char *key = "foll";
    char value[] ="hello";

    //EE 所有的设置操作
    //都是相同的 最后时时间和flags
    //len 不包括'\0'结尾
    rtmsg = memcached_set(memcli,  //client
                          key, strlen(key), //key and keylen
                          value, strlen(value), //value, valuelen
                          (time_t)0, //Experimental time
                          0); //flags

    //EE check rtmsg
    if (rtmsg != MEMCACHED_SUCCESS) {
        printf(" memcached_set faled: %s\n", 
               memcached_strerror(memcli, rtmsg));
        exit(1);
    }

    char buf[1024];
    //EE get v-k
    size_t valuelen=0;
    uint32_t flags=0;
    char *v = memcached_get(memcli,
                             key, strlen(key),
                             &valuelen,
                             &flags,
                             &rtmsg);

    memcpy(buf, v, valuelen);
    buf[valuelen]='\0';
    printf("vlen:%ld, flags:%u, value:%s\n", valuelen, flags, buf);


//	 memcached_return_t //
//	   memcached_mget (memcached_st *ptr,
//			 const char * const *keys,
//			 const size_t *key_length,
//			 size_t number_of_keys);

    //EE free server
	 memcached_server_free(memser);
    //EE free client 
    memcached_free(memcli);


    return 0;
}
