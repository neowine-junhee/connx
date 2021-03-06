#ifndef __CONNX_BACKEND_H__
#define __CONNX_BACKEND_H__

#include <connx/connx.h>
#include <connx/operator.h>

typedef struct _connx_Call {
	connx_Operator	op;
	uint32_t		counts;
	uint32_t*		params;
} connx_Call;

connx_Call* connx_Call_create(connx_PAL* pal, connx_Operator op, uint32_t counts);
void connx_Call_delete(connx_PAL* pal, connx_Call* call);

typedef struct _connx_Path connx_Path;

struct _connx_Path {
	uint32_t		input_path_count;
	uint32_t*		input_paths;

	uint32_t		output_path_count;
	uint32_t*		output_paths;

	uint32_t		call_count;
	connx_Call**	calls;
};

connx_Path* connx_Path_create(connx_PAL* pal);
void connx_Path_delete(connx_PAL* pal, connx_Path* call);

bool connx_Path_run(connx_Path* path, connx_Backend* backend);

struct _connx_Backend {
	connx_PAL*		pal;
	uint32_t		opset;

	uint32_t		path_count;
	connx_Path**	paths;

	uint32_t		initializer_count;
	uint32_t		variable_count;
	connx_Tensor**	variables;

	uint32_t*		attribute_index;
	void*			attributes;

	uint32_t		start_count;
	uint32_t*		starts;

	uint32_t		stop_count;
	uint32_t*		stops;
};

bool connx_Backend_has_variable(connx_Backend* backend, uint32_t id);
connx_Tensor* connx_Backend_get_variable(connx_Backend* backend, uint32_t id);
bool connx_Backend_set_variable(connx_Backend* backend, uint32_t id, connx_Tensor* tensor);
bool connx_Backend_delete_variable(connx_Backend* backend, uint32_t id);

void* connx_Backend_get_attribute(connx_Backend* backend, uint32_t id);

#endif /* __CONNX_BACKEND_H__ */
