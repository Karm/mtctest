/*
 ============================================================================
 Name        : mtctest.c
 Author      : Michal Karm Babacek
 Version     : 1.0
 Copyright   : GNU GPL 3.0
 Description : Simple multicast address binding test using APR
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include "apr.h"
#include "apr_network_io.h"
#include "apr_strings.h"

/**
 * Multicast Time to Live (ttl) for the advertise transmission.
 */
#define MA_ADVERTISE_HOPS       10

int main(void) {
    apr_status_t rv;
    static apr_socket_t *ma_mgroup_socket = NULL;
    static apr_sockaddr_t *ma_listen_sa = NULL;
    static apr_sockaddr_t *ma_mgroup_sa = NULL;
    static apr_sockaddr_t *ma_niface_sa = NULL;

    const char *bindaddr = "224.0.1.105";
    const char *addr = "127.0.0.1";
    apr_port_t bindport = 23364;
    apr_port_t port = 8888;
    apr_pool_t *pool;
    apr_initialize();
    apr_pool_create(&pool, NULL);

    puts("!!! Hello World !!!");

    printf("ma_group_join apr_sockaddr_info_get(%s:%d)", addr, port);
    if ((rv = apr_sockaddr_info_get(&ma_mgroup_sa, addr, APR_UNSPEC, port, APR_UNSPEC, pool)) != APR_SUCCESS) {
        printf(" FAILED\n");
        apr_pool_destroy(pool);
        return rv;
    } else {
        printf(" OK\n");
    }
    printf("ma_group_join apr_sockaddr_info_get(%s:%d)", bindaddr, bindport);
    if ((rv = apr_sockaddr_info_get(&ma_listen_sa, bindaddr, ma_mgroup_sa->family, bindport, APR_UNSPEC, pool)) != APR_SUCCESS) {
        printf(" FAILED\n");
        apr_pool_destroy(pool);
        return rv;
    } else {
        printf(" OK\n");
    }

    printf("ma_group_join apr_socket_create");
    if ((rv = apr_socket_create(&ma_mgroup_socket, ma_mgroup_sa->family, SOCK_DGRAM, APR_PROTO_UDP, pool)) != APR_SUCCESS) {
        printf(" FAILED\n");
        apr_pool_destroy(pool);
        return rv;
    } else {
        printf(" OK\n");
    }

    printf("ma_group_join apr_socket_opt_set");
    if ((rv = apr_socket_opt_set(ma_mgroup_socket, APR_SO_REUSEADDR, 1)) != APR_SUCCESS) {
        printf(" FAILED\n");
        apr_pool_destroy(pool);
        return rv;
    } else {
        printf(" OK\n");
    }

    printf("ma_group_join apr_socket_bind");
    if ((rv = apr_socket_bind(ma_mgroup_socket, ma_listen_sa)) != APR_SUCCESS) {
        printf(" FAILED\n");
        apr_pool_destroy(pool);
        return rv;
    } else {
        printf(" OK\n");
    }

    printf("ma_group_join apr_sockaddr_info_get(0.0.0.0:0)");
    if ((rv = apr_sockaddr_info_get(&ma_niface_sa, NULL, ma_mgroup_sa->family, 0, APR_UNSPEC, pool)) != APR_SUCCESS) {
        printf(" FAILED\n");
        apr_pool_destroy(pool);
        return rv;
    } else {
        printf(" OK\n");
    }

    printf("ma_group_join apr_mcast_join");
    if ((rv = apr_mcast_join(ma_mgroup_socket, ma_mgroup_sa, ma_niface_sa, NULL )) != APR_SUCCESS) {
        printf(" FAILED\n");
        printf("    ma_group_join apr_mcast_loopback");
        if ((rv = apr_mcast_loopback(ma_mgroup_socket, 1)) != APR_SUCCESS) {
            printf(" FAILED\n");
            apr_socket_close(ma_mgroup_socket);
            apr_pool_destroy(pool);
            return rv;
        } else {
            printf(" OK\n");
        }
    } else {
        printf(" OK\n");
    }

    printf("ma_group_join apr_mcast_hops");
    if ((rv = apr_mcast_hops(ma_mgroup_socket, MA_ADVERTISE_HOPS)) != APR_SUCCESS) {
        printf(" FAILED\n");
        apr_mcast_leave(ma_mgroup_socket, ma_mgroup_sa, NULL, NULL );
        apr_socket_close(ma_mgroup_socket);
        apr_pool_destroy(pool);
        return rv;
    } else {
        printf(" OK\n");
    }

    printf("\nCheck your netstat -an for %s:%d\n",  bindaddr, bindport);
    printf("Press ENTER to exit program\n");
    getchar();
    printf("Terminating successfully...\n");
    apr_mcast_leave(ma_mgroup_socket, ma_mgroup_sa, NULL, NULL );
    apr_socket_close(ma_mgroup_socket);
    apr_pool_destroy(pool);
    return EXIT_SUCCESS;
}
