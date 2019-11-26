﻿#include <pthread.h>
#include <errno.h>
#include <string.h>
#include "ite/itp.h"
#if CFG_NET_HTTP
#include "iperf_api.h"

#define IPERF_HOST "192.168.190.102"
#define IPERF_SERVER_PORT 5201
extern int i_errno;

int iperf_test_client()
{

    int port;
    struct iperf_test *test;
    char* host;


    port = IPERF_SERVER_PORT;

    test = iperf_new_test();
    if ( test == NULL ) {
	fprintf( stderr, " failed to create test\n" );
	return -1;
    }

    iperf_defaults( test );
    iperf_set_verbose( test, 1 );

    iperf_set_test_role( test, 'c' );
    iperf_set_test_server_hostname( test, IPERF_HOST );
    iperf_set_test_server_port( test, port );
    /* iperf_set_test_reverse( test, 1 ); */
    iperf_set_test_omit( test, 3 );
    iperf_set_test_duration( test, 60*60*16);
    iperf_set_test_reporter_interval( test, 1 );
    iperf_set_test_stats_interval( test, 1 );
    /* iperf_set_test_json_output( test, 1 ); */
	//perf_set_test_socket_bufsize(test,1024);
    iperf_set_test_blksize(test,1024*1024);

    if ( iperf_run_client( test ) < 0 ) {
		fprintf( stderr, " error - %s\n", iperf_strerror( i_errno ) );
		return -1;
    }
	
	printf("network iperf_run_client \n");
    if (iperf_get_test_json_output_string(test)) {
		fprintf(iperf_get_test_outfile(test), "%zd bytes of JSON emitted\n",strlen(iperf_get_test_json_output_string(test)));
    }

    iperf_free_test( test );

}

int iperf_test_server()
{

    int port;
    struct iperf_test *test;
    int consecutive_errors;

    port = 5000;

    test = iperf_new_test();
    if ( test == NULL ) {
	fprintf( stderr, " failed to create test\n" );
        return -1;
    }
    iperf_defaults( test );
    iperf_set_verbose( test, 1 );
    iperf_set_test_role( test, 's' );
    iperf_set_test_server_port( test, port );

    consecutive_errors = 0;
    for (;;) {
	if ( iperf_run_server( test ) < 0 ) {
	    fprintf( stderr, "error - %s\n\n", iperf_strerror( i_errno ) );
	    ++consecutive_errors;
	    if (consecutive_errors >= 5) {
	        fprintf(stderr, "too many errors, exiting\n");
		break;
	    }
	} else
	    consecutive_errors = 0;
	iperf_reset_test( test );
    }

    iperf_free_test( test );
    return -1;

}
#endif