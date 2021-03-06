#include <connx/operator.h>
#include <connx/backend.h>

bool opset_LeakyRelu(connx_Backend* backend, uint32_t counts, uint32_t* params) {
	connx_Tensor* Y = CONNX_GET_OUTPUT(0);
	connx_Tensor* X = CONNX_GET_INPUT(0);
	connx_AttributeFloat* alpha = CONNX_GET_ATTRIBUTE(0);

	// Create Y if NULL
	if(Y == NULL) {
		Y = connx_Tensor_create(backend->pal, X->type, X->dimension, X->lengths);
		CONNX_SET_OUTPUT(0, Y);
	}

	float alpha_value = alpha->value;

	uint32_t total = connx_Tensor_total(X);

	switch(X->type) {
		case connx_FLOAT16:
			{
				float* x = (float*)X->base;
				float* y = (float*)Y->base;

				for(uint32_t i = 0; i < total; i++) {
					y[i] = x[i] > 0 ? x[i] : x[i] * alpha_value;
				}
			}
			break;
		case connx_FLOAT32:
			{
				float* x = (float*)X->base;
				float* y = (float*)Y->base;

				for(uint32_t i = 0; i < total; i++) {
					y[i] = x[i] > 0 ? x[i] : x[i] * alpha_value;
				}
			}
			break;
		case connx_FLOAT64:
			{
				double* x = (double*)X->base;
				double* y = (double*)Y->base;

				for(uint32_t i = 0; i < total; i++) {
					y[i] = x[i] > 0 ? x[i] : x[i] * alpha_value;
				}
			}
			break;
		default:
			backend->pal->error(backend->pal, "Not supported element type: %s\n", connx_DataType_name(X->type));
			return false;
	}

	return true;
}
