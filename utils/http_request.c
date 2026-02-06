// logic for handling http requests

#include <stdio.h>
#include "pico/stdlib.h"
#include "lwip/apps/http_client.h"
#include "pico/cyw43_arch.h"
#include "pico/sync.h"

#define HTTP_RESPONSE_MAX 4096 // max response buffer

static volatile bool request_complete = false;
static volatile bool request_success = false;


// ----- test get request ------

// callbacks when request completes
err_t headers_done_fn(
	httpc_state_t *connection,
	void *arg,
	struct pbuf *hdr,
	u16_t hdr_len,
	u32_t content_len) {
	printf("in headers_done_fn\n");
	printf("header length: %d\n", hdr_len);
	printf("Content length: %lu\n", content_len);
	return ERR_OK;
}

// when request completes
void result_fn(
	void *arg,
	httpc_result_t httpc_result,
	u32_t rx_content_len, 
	u32_t srv_res,
	err_t err) {
	printf(">>> result_fn >>>\n");
	printf("httpc_result: %s\n",
	     httpc_result == HTTPC_RESULT_OK              ? "HTTPC_RESULT_OK"
           : httpc_result == HTTPC_RESULT_ERR_UNKNOWN     ? "HTTPC_RESULT_ERR_UNKNOWN"
           : httpc_result == HTTPC_RESULT_ERR_CONNECT     ? "HTTPC_RESULT_ERR_CONNECT"
           : httpc_result == HTTPC_RESULT_ERR_HOSTNAME    ? "HTTPC_RESULT_ERR_HOSTNAME"
           : httpc_result == HTTPC_RESULT_ERR_CLOSED      ? "HTTPC_RESULT_ERR_CLOSED"
           : httpc_result == HTTPC_RESULT_ERR_TIMEOUT     ? "HTTPC_RESULT_ERR_TIMEOUT"
           : httpc_result == HTTPC_RESULT_ERR_SVR_RESP    ? "HTTPC_RESULT_ERR_SVR_RESP"
           : httpc_result == HTTPC_RESULT_ERR_MEM         ? "HTTPC_RESULT_ERR_MEM"
           : httpc_result == HTTPC_RESULT_LOCAL_ABORT     ? "HTTPC_RESULT_LOCAL_ABORT"
           : httpc_result == HTTPC_RESULT_ERR_CONTENT_LEN ? "HTTPC_RESULT_ERR_CONTENT_LEN"
           : "*UNKNOWN*");
	printf("received %ld bytes\n", rx_content_len);
	printf("server response: %ld\n", srv_res);
	printf("err: %d\n", err);
	request_complete = true;
	request_success = (httpc_result == HTTPC_RESULT_OK);
	printf("<<< result_fn <<<\n");
}

// when body data is received
err_t recv_fn(
	void *arg,
	struct altcp_pcb *conn,
	struct pbuf *p,
	err_t err) {
	printf("in recv_fn\n");
	printf(">>> recv_fn >>>\n");
	if (p == NULL) {
		printf("p is NULL\n");
		return ERR_OK;
	}

	printf("p: %p\n", p);
	printf("len: %d\n", p->len);
	
	char *data = (char *)p->payload;
	int print_len = p->len <200 ? p->len : 200;
	printf("Data: %.*s\n", print_len, data);

	pbuf_free(p);

	printf("<<< recv_fn <<<\n");
	return ERR_OK;
}

int test_http_request() {
    printf("\n=== Starting HTTP Request ===\n");

	httpc_connection_t settings = {
		.use_proxy = 0,
		.headers_done_fn = headers_done_fn,
		.result_fn = result_fn
	};
    httpc_state_t *connection = NULL;

    err_t err = httpc_get_file_dns(
		"example.com",
		80,
		"/",
		&settings,
		recv_fn,
		NULL,
		&connection
	);

    if (err != ERR_OK) {
		printf("HTTP request failed to start %d\n", err);
		return 1;
	}

   	printf("waiting for request\n");
	sleep_ms(10000);
	printf("HTTP Request Done\n");
	return 0;
}