#ifndef TCP_ERROR_H
#define TCP_ERROR_H

// Library-internal: store the error message returned by tcp_error()
void tcp_set_error(const char* call, const char* cause);

#endif /* TCP_ERROR_H */
