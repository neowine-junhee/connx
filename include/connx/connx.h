#ifndef __CONNX_CONNX_H__
#define __CONNX_CONNX_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// data structure
// DataType follows ONNX's datatype enumeration number
typedef enum _connx_DataType {
	connx_VOID		= 0,
	connx_FLOAT32	= 1,
	connx_UINT8		= 2,
	connx_INT8		= 3,
	connx_UINT16	= 4,
	connx_INT16		= 5,
	connx_INT32		= 6,
	connx_INT64		= 7,
	connx_STRING	= 8,
	connx_BOOL		= 9,
	connx_FLOAT16	= 10,
	connx_FLOAT64	= 11,
	connx_UINT32	= 12,
	connx_UINT64	= 13,
} connx_DataType;

uint32_t connx_DataType_size(connx_DataType type);
const char* connx_DataType_name(connx_DataType type);

// Tensor
typedef struct _connx_Tensor {
	connx_DataType		type;

	uint32_t 			dimension;
	uint32_t*			lengths;

	uint8_t				base[0] __attribute__((aligned(16)));		// Align 16 bytes for vector operation
} connx_Tensor;

typedef struct _connx_PAL connx_PAL;

connx_Tensor* connx_Tensor_create(connx_PAL* pal, connx_DataType type, uint32_t dimension, uint32_t* lengths);
connx_Tensor* connx_Tensor_create_from_buffer(connx_PAL* pal, void* buf);
void connx_Tensor_delete(connx_PAL* pal, connx_Tensor* tensor);

bool connx_Tensor_is_shape_equals(connx_Tensor* x, connx_Tensor* y);
uint32_t connx_Tensor_total(connx_Tensor* tensor);

/**
 * @return 0 to MAX(int32) - 10^-n precision, -2 - shape is not equals, -1 - value is not equals
 */
int32_t connx_Tensor_accuracy(connx_Tensor* x, connx_Tensor* y);

// Backend
typedef struct _connx_Backend connx_Backend;

connx_Backend* connx_Backend_create(connx_PAL* pal);
void connx_Backend_delete(connx_Backend* backend);

bool connx_Backend_run(connx_Backend* backend, uint32_t* output_count, connx_Tensor** outputs, uint32_t input_count, connx_Tensor** inputs);

connx_Tensor* connx_Backend_load_tensor(connx_Backend* backend, const char* name);

// Hardware Abstraction Layer
typedef void* connx_Thread;

struct _connx_PAL {
	// Memory management
	void* (*alloc)(connx_PAL* pal, size_t size);
	void (*free)(connx_PAL* pal, void* ptr);

	// Model loader
	void* (*load)(connx_PAL* pal, const char* name);
	void (*unload)(connx_PAL* pal, void* buf);

	// Thread pool
	connx_Thread* (*alloc_threads)(connx_PAL* pal, uint32_t count);
	void (*free_thread)(connx_PAL* pal, connx_Thread* thread);
	connx_Thread* (*join)(connx_PAL* pal, connx_Thread* thread);

	// error
	uint32_t debug_tab;
	void (*debug)(connx_PAL* pal, const char* format, ...);
	uint32_t info_tab;
	void (*info)(connx_PAL* pal, const char* format, ...);
	uint32_t error_tab;
	void (*error)(connx_PAL* pal, const char* format, ...);

	uint8_t priv[0];
};

#endif /* __CONNX_CONNX_H__ */
