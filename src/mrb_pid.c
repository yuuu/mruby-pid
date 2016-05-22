/*
** mrb_pid.c - PID class
**
** Copyright (c) Yuhei Okazaki 2016
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/hash.h"
#include "mrb_pid.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
	mrb_float target;
	mrb_float kp;
	mrb_float kd;
	mrb_float ki;
	mrb_float sampling;
	mrb_float max;
	mrb_float min;
	mrb_float last_input;
	mrb_float integral;
	mrb_bool first;
} mrb_pid_data;

static mrb_float mrb_pid_get_param(mrb_state *mrb, mrb_value value)
{
	mrb_float f;

	switch(mrb_type(value)) {
	case MRB_TT_FIXNUM:
		f = (mrb_float)mrb_fixnum(value);
		break;
	case MRB_TT_FLOAT:
		f = mrb_float(value);
		break;
	default:
		f = 0.0;
	}

	return f;
}

static const struct mrb_data_type mrb_pid_data_type = {
  "mrb_pid_data", mrb_free,
};

static mrb_value mrb_pid_init(mrb_state *mrb, mrb_value self)
{
  mrb_pid_data *data;
	mrb_value hash;
	mrb_value target;
	mrb_value kp;
	mrb_value kd;
	mrb_value ki;
	mrb_value sampling;
	mrb_value max;
	mrb_value min;
	mrb_bool is_opt;

  data = (mrb_pid_data *)DATA_PTR(self);
  if (data) {
    mrb_free(mrb, data);
  }
  DATA_TYPE(self) = &mrb_pid_data_type;
  DATA_PTR(self) = NULL;

  data = (mrb_pid_data *)mrb_malloc(mrb, sizeof(mrb_pid_data));
	data->target = data->kp = data->ki = data->kd = data->sampling = \
	data->max = data->min = data->last_input = data->integral = (mrb_float)0.0;
	data->first = TRUE;

  mrb_get_args(mrb, "|H?", &hash, &is_opt);
	if(is_opt) {
	  target = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "target")));
		data->target = mrb_pid_get_param(mrb, target);
	  kp = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "kp")));
		data->kp = mrb_pid_get_param(mrb, kp);
	  ki = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "ki")));
		data->ki = mrb_pid_get_param(mrb, ki);
	  kd = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "kd")));
		data->kd = mrb_pid_get_param(mrb, kd);
	  sampling = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "sampling")));
		data->sampling = mrb_pid_get_param(mrb, sampling);
	  max = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "max")));
		data->max = mrb_pid_get_param(mrb, max);
	  min = mrb_hash_get(mrb, hash, mrb_symbol_value(mrb_intern_lit(mrb, "min")));
		data->min = mrb_pid_get_param(mrb, min);
	}

  DATA_PTR(self) = data;

  return self;
}

static mrb_value mrb_pid_get_target(mrb_state *mrb, mrb_value self)
{
  mrb_pid_data *data = DATA_PTR(self);

  return mrb_float_value(mrb, data->target);
}

static mrb_value mrb_pid_set_target(mrb_state *mrb, mrb_value self)
{
	mrb_float target;
  mrb_pid_data *data = DATA_PTR(self);

  mrb_get_args(mrb, "f", &target);
	data->target = target;

  return self;
}

static mrb_value mrb_pid_get_kp(mrb_state *mrb, mrb_value self)
{
  mrb_pid_data *data = DATA_PTR(self);

  return mrb_float_value(mrb, data->kp);
}

static mrb_value mrb_pid_set_kp(mrb_state *mrb, mrb_value self)
{
	mrb_float kp;
  mrb_pid_data *data = DATA_PTR(self);

  mrb_get_args(mrb, "f", &kp);
	data->kp = kp;

  return self;
}

static mrb_value mrb_pid_get_ki(mrb_state *mrb, mrb_value self)
{
  mrb_pid_data *data = DATA_PTR(self);

  return mrb_float_value(mrb, data->ki);
}

static mrb_value mrb_pid_set_ki(mrb_state *mrb, mrb_value self)
{
	mrb_float ki;
  mrb_pid_data *data = DATA_PTR(self);

  mrb_get_args(mrb, "f", &ki);
	data->ki = ki;

  return self;
}

static mrb_value mrb_pid_get_kd(mrb_state *mrb, mrb_value self)
{
  mrb_pid_data *data = DATA_PTR(self);

  return mrb_float_value(mrb, data->kd);
}

static mrb_value mrb_pid_set_kd(mrb_state *mrb, mrb_value self)
{
	mrb_float kd;
  mrb_pid_data *data = DATA_PTR(self);

  mrb_get_args(mrb, "f", &kd);
	data->kd = kd;

  return self;
}

static mrb_value mrb_pid_get_sampling(mrb_state *mrb, mrb_value self)
{
  mrb_pid_data *data = DATA_PTR(self);

  return mrb_float_value(mrb, data->sampling);
}

static mrb_value mrb_pid_set_sampling(mrb_state *mrb, mrb_value self)
{
	mrb_float sampling;
  mrb_pid_data *data = DATA_PTR(self);

  mrb_get_args(mrb, "f", &sampling);
	data->sampling = sampling;

  return self;
}

static mrb_value mrb_pid_get_max(mrb_state *mrb, mrb_value self)
{
  mrb_pid_data *data = DATA_PTR(self);

  return mrb_float_value(mrb, data->max);
}

static mrb_value mrb_pid_set_max(mrb_state *mrb, mrb_value self)
{
	mrb_float max;
  mrb_pid_data *data = DATA_PTR(self);

  mrb_get_args(mrb, "f", &max);
	data->max = max;

  return self;
}

static mrb_value mrb_pid_get_min(mrb_state *mrb, mrb_value self)
{
  mrb_pid_data *data = DATA_PTR(self);

  return mrb_float_value(mrb, data->min);
}

static mrb_value mrb_pid_set_min(mrb_state *mrb, mrb_value self)
{
	mrb_float min;
  mrb_pid_data *data = DATA_PTR(self);

  mrb_get_args(mrb, "f", &min);
	data->min = min;

  return self;
}

static mrb_float mrb_pid_calculate_p(mrb_state *mrb, mrb_pid_data *data, mrb_float input)
{
	mrb_float result;

	result = data->kp * (input - data->target);

  return result;
}

static mrb_float mrb_pid_calculate_i(mrb_state *mrb, mrb_pid_data *data, mrb_float input)
{
	mrb_float result;

	data->integral += ((input - data->target) * data->sampling);
	result = data->ki * data->integral;

  return result;
}

static mrb_float mrb_pid_calculate_d(mrb_state *mrb, mrb_pid_data *data, mrb_float input)
{
	mrb_float result;

	result = 0.0;
	if(data->first != TRUE) {
		result = data->kd * ((input - data->last_input) / data->sampling);
	}
	data->last_input = input;
	data->first = FALSE;

  return result;
}

static mrb_float mrb_pid_limit(mrb_state *mrb, mrb_pid_data *data, mrb_float output)
{
	if(output > data->max) {
		output = data->max;
	} else if(output < data->min) {
		output = data->min;
	} else {
		/*NOP*/
	}
  return output;
}

static mrb_value mrb_pid_calculate(mrb_state *mrb, mrb_value self)
{
	mrb_float input, calc_p, calc_i, calc_d, output;
  mrb_pid_data *data = DATA_PTR(self);

  mrb_get_args(mrb, "f", &input);

	calc_p = mrb_pid_calculate_p(mrb, data, input);
	calc_i = mrb_pid_calculate_i(mrb, data, input);
	calc_d = mrb_pid_calculate_d(mrb, data, input);

  output = mrb_pid_limit(mrb, data, calc_p + calc_i + calc_d);

  return mrb_float_value(mrb, output);
}

void mrb_mruby_pid_gem_init(mrb_state *mrb)
{
    struct RClass *pid;
    pid = mrb_define_class(mrb, "PID", mrb->object_class);
    mrb_define_method(mrb, pid, "initialize", mrb_pid_init, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, pid, "target", mrb_pid_get_target, MRB_ARGS_NONE());
    mrb_define_method(mrb, pid, "target=", mrb_pid_set_target, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, pid, "kp", mrb_pid_get_kp, MRB_ARGS_NONE());
    mrb_define_method(mrb, pid, "kp=", mrb_pid_set_kp, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, pid, "ki", mrb_pid_get_ki, MRB_ARGS_NONE());
    mrb_define_method(mrb, pid, "ki=", mrb_pid_set_ki, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, pid, "kd", mrb_pid_get_kd, MRB_ARGS_NONE());
    mrb_define_method(mrb, pid, "kd=", mrb_pid_set_kd, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, pid, "sampling", mrb_pid_get_sampling, MRB_ARGS_NONE());
    mrb_define_method(mrb, pid, "sampling=", mrb_pid_set_sampling, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, pid, "max", mrb_pid_get_max, MRB_ARGS_NONE());
    mrb_define_method(mrb, pid, "max=", mrb_pid_set_max, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, pid, "min", mrb_pid_get_min, MRB_ARGS_NONE());
    mrb_define_method(mrb, pid, "min=", mrb_pid_set_min, MRB_ARGS_REQ(1));
    mrb_define_method(mrb, pid, "calculate", mrb_pid_calculate, MRB_ARGS_REQ(1));

    DONE;
}

void mrb_mruby_pid_gem_final(mrb_state *mrb)
{
}

