/*
 * MIT License (MIT)
 *
 * Copyright (c) 2019 Kristian Kinderlöv
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!
 * \addtogroup CIRCULARBUFFER
 * \ingroup MISC
 * \{
 *
 * \file        circularbuffer.h
 *
 * \brief       circular_buffer public interface.
 *
 * This circular_buffer implementation uses a single empty cell to detect the
 * "full" case for supporting thread safety, meaning it support one producer and
 * one consumer without a lock. The circular_buffer therefore hold as most
 * ((buf_size / element_size) - 1) elements. For use of efficient algorithms the
 * maximum number of elements in the circular_buffer must be a power of 2.
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

/* **** Includes **** */
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* **** Defines **** */

/* **** Typedefs **** */
    typedef unsigned int size_t;

typedef struct {
    uint8_t *buf;         // Pointer to the buffer
    size_t write_pos;     // Write position
    size_t read_pos;      // Read position
    size_t element_size;  // Size of a element in the buffer
    size_t max_size;      // Max number of elements in the buffer
} CircularBufferContext;

/* **** Extern Variables **** */

/* **** Function Declarations **** */

/*!
 * \breif Initializes the circular_buffer context.
 *
 * The max number of elements in the circular_buffer is
 * (buf_size / element_size) - 1.
 *
 * Asserts:
 *      'ctx' is not NULL.
 *      'buf' is not NULL.
 *      'buf_size' is not 0
 *      'element_size' is not 0
 *      'buf_size / element_size' is not power of 2.
 *
 * \param[out]  *ctx            Pointer to the circular_buffer context.
 * \param[in]   buf             Pointer to a buffer.
 * \param[in]   buf_size        The buffer size.
 * \param[in]   element_size    The size of an element in the buffer.
 */
void CircularBufferInit(CircularBufferContext *ctx, void *buf, size_t buf_size,
                        size_t element_size);

/*!
 * \breif Removes all elements from the circular_buffer.
 *
 * Asserts:
 *      'ctx' is not NULL.
 *
 * \param[out]  *ctx            Pointer to the circular_buffer context.
 */
void CircularBufferClear(CircularBufferContext *ctx);

/*!
 * \breif Adds an new element to the end of the buffer. The "val" content is
 * copied to the element.
 *
 * Asserts:
 *      'ctx' is not NULL.
 *      'val' is not NULL.
 *
 * \param[out]  *ctx            Pointer to the circular_buffer context.
 * \param[in]   val             Pointer to the source to be copied.
 * \return                      0 if success, -1 if the buffer is full.
 */
int8_t CircularBufferPushBack(CircularBufferContext *ctx, void *val);

/*!
 * \breif Removes the first element from the buffer. Copies the element content
 * to the "val" destination.
 *
 * Asserts:
 *      'ctx' is not NULL.
 *      'val' is not NULL.
 *
 * \param[out]  *ctx            Pointer to the circular_buffer context.
 * \param[out]  *val            Pointer to the destination where the data is to
 *                              be stored.
 * \return                      0 if success, -1 if the buffer is
 *                              empty.
 */
int8_t CircularBufferPopFront(CircularBufferContext *ctx, void *val);

/*!
 * \breif Peeks the "num" element from the buffer.
 *
 * The "num" argument shall be less than the number of elements added to the
 * buffer.
 *
 * Asserts:
 *      'ctx' is not NULL.
 *
 * \param[in]   *ctx            Pointer to the circular_buffer context.
 * \param[in]   num             The number of the element to peek.
 * \param[out]  elem            Pointer to the 'num' element.
 * \return                      0 if success, -1 or NULL buffer is empty or the
 *                              'num' is out of bounds.
 */
int8_t CircularBufferPeek(const CircularBufferContext *ctx, size_t num,
                           void **elem);

/*!
 * \breif Gets the number of added elements in the buffer.
 *
 * Asserts:
 *      'ctx' is not NULL.
 *
 * \param[in] *ctx              Pointer to the circular_buffer context.
 * \return                      The number of added elements.
 */
size_t CircularBufferSize(const CircularBufferContext *ctx);

/*!
 * \breif Gets the number of free elements in the buffer.
 *
 * Asserts:
 *      'ctx' is not NULL.
 *
 * \param[in] *ctx              Pointer to the circular_buffer context.
 * \return                      The number of free elements.
 */
size_t CircularBufferSpace(const CircularBufferContext *ctx);

/*!
 * \breif Checks if the buffer is empty.
 *
 * Asserts:
 *      'ctx' is not NULL.
 *
 * \param[in] *ctx              Pointer to the circular_buffer context.
 * \return                      true if the buffer is empty otherwise false.
 */
inline bool CircularBufferEmpty(const CircularBufferContext *ctx);

#ifdef __cplusplus
}
#endif

#endif /* CIRCULARBUFFER_H_ */

/*! \} */