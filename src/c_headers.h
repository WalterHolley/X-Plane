// Automatically generated by Interoptopus.

#ifndef cwic_plug_lib
#define cwic_plug_lib

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>





void* cwic_init();

void cwic_start(void* cwic);

void cwic_device_id(void* cwic, int8_t* buffer_ptr);

void cwic_sortie_id(void* cwic, int8_t* buffer_ptr);

void cwic_cwic_sock_addr(void* cwic, int8_t* buffer_ptr);

void cwic_xplane_sock_addr(void* cwic, int8_t* buffer_ptr);

void cwic_button_one(void* cwic);

void cwic_button_two(void* cwic);

void cwic_button_three(void* cwic);

void cwic_fail(void* cwic);

void cwic_reset(void* cwic);

void cwic_ping(void* cwic);

void cwic_error(void* cwic);

void cwic_display(void* cwic, int8_t* buffer_ptr);

void cwic_tts(void* cwic, int8_t* buffer_ptr);

void cwic_drop(void* ctx);


#ifdef __cplusplus
}
#endif

#endif /* cwic_plug_lib */