// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from my_interface:msg/Example.idl
// generated code does not contain a copyright notice
#include "my_interface/msg/detail/example__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
my_interface__msg__Example__init(my_interface__msg__Example * msg)
{
  if (!msg) {
    return false;
  }
  // data1
  // data2
  return true;
}

void
my_interface__msg__Example__fini(my_interface__msg__Example * msg)
{
  if (!msg) {
    return;
  }
  // data1
  // data2
}

bool
my_interface__msg__Example__are_equal(const my_interface__msg__Example * lhs, const my_interface__msg__Example * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // data1
  if (lhs->data1 != rhs->data1) {
    return false;
  }
  // data2
  if (lhs->data2 != rhs->data2) {
    return false;
  }
  return true;
}

bool
my_interface__msg__Example__copy(
  const my_interface__msg__Example * input,
  my_interface__msg__Example * output)
{
  if (!input || !output) {
    return false;
  }
  // data1
  output->data1 = input->data1;
  // data2
  output->data2 = input->data2;
  return true;
}

my_interface__msg__Example *
my_interface__msg__Example__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_interface__msg__Example * msg = (my_interface__msg__Example *)allocator.allocate(sizeof(my_interface__msg__Example), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(my_interface__msg__Example));
  bool success = my_interface__msg__Example__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
my_interface__msg__Example__destroy(my_interface__msg__Example * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    my_interface__msg__Example__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
my_interface__msg__Example__Sequence__init(my_interface__msg__Example__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_interface__msg__Example * data = NULL;

  if (size) {
    data = (my_interface__msg__Example *)allocator.zero_allocate(size, sizeof(my_interface__msg__Example), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = my_interface__msg__Example__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        my_interface__msg__Example__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
my_interface__msg__Example__Sequence__fini(my_interface__msg__Example__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      my_interface__msg__Example__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

my_interface__msg__Example__Sequence *
my_interface__msg__Example__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  my_interface__msg__Example__Sequence * array = (my_interface__msg__Example__Sequence *)allocator.allocate(sizeof(my_interface__msg__Example__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = my_interface__msg__Example__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
my_interface__msg__Example__Sequence__destroy(my_interface__msg__Example__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    my_interface__msg__Example__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
my_interface__msg__Example__Sequence__are_equal(const my_interface__msg__Example__Sequence * lhs, const my_interface__msg__Example__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!my_interface__msg__Example__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
my_interface__msg__Example__Sequence__copy(
  const my_interface__msg__Example__Sequence * input,
  my_interface__msg__Example__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(my_interface__msg__Example);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    my_interface__msg__Example * data =
      (my_interface__msg__Example *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!my_interface__msg__Example__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          my_interface__msg__Example__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!my_interface__msg__Example__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
