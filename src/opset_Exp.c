#include <math.h>
#include <connx/operator.h>
#include <connx/backend.h>

bool opset_Exp(connx_Backend* backend, uint32_t counts, uint32_t* params) {
	connx_Tensor* Y = CONNX_GET_OUTPUT(0);
	connx_Tensor* X = CONNX_GET_INPUT(0);

	if(Y == NULL) {
		Y = connx_Tensor_create(backend->pal, X->type, X->dimension, X->lengths);
		CONNX_SET_OUTPUT(0, Y);
	}

	uint32_t total = connx_Tensor_total(X);

	switch(X->type) {
		case connx_FLOAT16:
			{
				float* x = (float*)X->base;
				float* y = (float*)Y->base;

				for(uint32_t i = 0; i < total; i++) {
					y[i] = expf(x[i]);
				}
			}
			break;
		case connx_FLOAT32:
			{
				float* x = (float*)X->base;
				float* y = (float*)Y->base;

				for(uint32_t i = 0; i < total; i++) {
					y[i] = expf(x[i]);
				}
			}
			break;
		case connx_FLOAT64:
			{
				double* x = (double*)X->base;
				double* y = (double*)Y->base;

				for(uint32_t i = 0; i < total; i++) {
					y[i] = exp(x[i]);
				}
			}
			break;
		default:
			backend->pal->error(backend->pal, "Not supported type: %u\n", X->type);
			return false;
	}

	return true;
}
