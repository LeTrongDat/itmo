/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: database.proto */

#ifndef PROTOBUF_C_database_2eproto__INCLUDED
#define PROTOBUF_C_database_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1005000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct Database__Request Database__Request;
typedef struct Database__Response Database__Response;


/* --- enums --- */


/* --- messages --- */

struct  Database__Request
{
  ProtobufCMessage base;
  char *query;
};
#define DATABASE__REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&database__request__descriptor) \
, (char *)protobuf_c_empty_string }


struct  Database__Response
{
  ProtobufCMessage base;
  char *message;
  protobuf_c_boolean success;
};
#define DATABASE__RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&database__response__descriptor) \
, (char *)protobuf_c_empty_string, 0 }


/* Database__Request methods */
void   database__request__init
                     (Database__Request         *message);
size_t database__request__get_packed_size
                     (const Database__Request   *message);
size_t database__request__pack
                     (const Database__Request   *message,
                      uint8_t             *out);
size_t database__request__pack_to_buffer
                     (const Database__Request   *message,
                      ProtobufCBuffer     *buffer);
Database__Request *
       database__request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   database__request__free_unpacked
                     (Database__Request *message,
                      ProtobufCAllocator *allocator);
/* Database__Response methods */
void   database__response__init
                     (Database__Response         *message);
size_t database__response__get_packed_size
                     (const Database__Response   *message);
size_t database__response__pack
                     (const Database__Response   *message,
                      uint8_t             *out);
size_t database__response__pack_to_buffer
                     (const Database__Response   *message,
                      ProtobufCBuffer     *buffer);
Database__Response *
       database__response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   database__response__free_unpacked
                     (Database__Response *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Database__Request_Closure)
                 (const Database__Request *message,
                  void *closure_data);
typedef void (*Database__Response_Closure)
                 (const Database__Response *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor database__request__descriptor;
extern const ProtobufCMessageDescriptor database__response__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_database_2eproto__INCLUDED */
